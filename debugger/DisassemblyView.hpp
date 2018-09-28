/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_DISASMVIEW_HPP
#define MICROMACHINE_EMU_DISASMVIEW_HPP



#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QFontDatabase>

#include "QMemRegion.hpp"
#include "QInstruction.hpp"
#include "BreakpointRegistry.hpp"
#include "structured_disasm.hpp"
#include "AddressTrackerList.hpp"

class DisassemblyView : public QQuickPaintedItem {
	Q_OBJECT
	Q_PROPERTY(QMemRegion* memoryRegion READ memoryRegion WRITE setMemoryRegion NOTIFY regionChanged)
	Q_PROPERTY(quint32 baseAddress READ baseAddress WRITE setBaseAddress NOTIFY baseAddressChanged)
	Q_PROPERTY(BreakpointRegistry* breakpointRegistry READ breakpointRegistry WRITE setBreakpointRegistry NOTIFY breakpointRegistryChanged)
	Q_PROPERTY(int maxVisibleInstructionCount READ maxDisplayableIntructions NOTIFY dimensionsChanged)
	Q_PROPERTY(QQmlListProperty<AddressTracker> addressTrackers READ addressTrackers)
private:

	class InstructionPainter : public structured_disasm {
	public:
		InstructionPainter(int lineHeight)
			: _painter(nullptr)
			, _line_height(lineHeight)
			, _name_font("Hack")
			, _register_font(_name_font)
			, _address_font(_name_font) {
			_name_font.setPixelSize(18);
			_register_font.setPixelSize(18);
			_register_font.setBold(false);
			_address_font.setPixelSize(18);
		}
		virtual ~InstructionPainter() = default;

		void drawHighlight(const AddressTracker* addressTracker) {
			_painter->save();
			_painter->fillRect(0,0, 500, _line_height, addressTracker->color());
			_painter->restore();
		}

		void drawVoid(uint32_t instruction_address) {
			setAddress(instruction_address);
			_painter->save();
			drawBreakpointFlag(false);
			draw_invalid_address(_addr);
			_painter->restore();
		}

		void drawInstruction(uint32_t instruction_address, instruction_pair instruction, bool isBreakpoint) {
			//_painter->fillRect(0,0, 500, _line_height, Qt::gray);
			//_painter->drawRect(0,0, 500, _line_height);
			// make up for baseline, so that text is vertically centered
			setAddress(instruction_address);
			_painter->save();
			drawBreakpointFlag(isBreakpoint);
			_painter->translate(0, -3);
			draw_address(_addr);
			dispatch_instruction(instruction);
			_painter->restore();
		}

		void next_line() {
			_painter->translate(0, _line_height);
		}

		void prev_line() {
			_painter->translate(0, -_line_height);
		}

		void setPainter(QPainter* painter) {
			_painter = painter;
		}

	private:
		QPainter* _painter;
		int _line_height;
		QFont _name_font;
		QFont _register_font;
		QFont _address_font;

		void drawBreakpointFlag(bool enabled) {
			if(enabled) {
				_painter->setBrush(Qt::red);
				_painter->drawEllipse(4,4,_line_height-8, _line_height-8);
			}
			_painter->translate(22, 0);
		}

		void draw_invalid_address(uint32_t address) {
			_painter->setPen(QColor("#393939"));
			_painter->setFont(_address_font);
			_painter->drawText(5, _line_height, QString("%1").arg(address, 8, 16, QChar('0')));
			_painter->translate(100, 0);
		}

		void draw_address(uint32_t address) {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->setFont(_address_font);
			_painter->drawText(5, _line_height, QString("%1").arg(address, 8, 16, QChar('0')));
			_painter->translate(100, 0);
		}

		void emit_name(const std::string& name) override {
			_painter->setPen(QColor("#d3d3d3"));
			_painter->setFont(_name_font);
			QString text = QString::fromStdString(name);
			_painter->drawText(2, _line_height, text);
			QFontMetrics metrics(_name_font);
			int width = metrics.size(Qt::TextSingleLine, text).width();
			_painter->translate(width + 2, 0);
		}

		void emit_reg(uint8_t reg_id) override {
			_painter->setPen(QColor("#ff8700"));
			_painter->setFont(_register_font);
			_painter->drawText(5, _line_height, R(reg_id));
			_painter->translate(40, 0);
		}

		void emit_special_reg(const std::string& reg_name) override {
			_painter->setPen(QColor("#d7005f"));
			_painter->setFont(_register_font);
			_painter->drawText(5, _line_height, QString::fromStdString(reg_name));
			_painter->translate(40, 0);
		}

		void emit_immediate(uint32_t immediate) override {
			_painter->setPen(QColor("#5fd7ff"));
			_painter->setFont(_register_font);
			QString text = QString("#%1").arg(QString::number(immediate, 16));
			_painter->drawText(5, _line_height, text);
			QFontMetrics metrics(_register_font);
			int width = metrics.size(Qt::TextSingleLine, text).width();
			_painter->translate(width + 5, 0);
		}

		void emit_regs(const std::vector<uint8_t>& regs) override {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->drawText(2, _line_height, "{");
			_painter->translate(20, 0);

			_painter->setPen(QColor("#ad5c00"));
			_painter->setFont(_register_font);
			for(uint8_t reg_id : regs) {
				_painter->drawText(0, _line_height, R(reg_id));
				_painter->translate(30, 0);
			}
			_painter->setPen(QColor("#6c6c6c"));
			_painter->drawText(10, _line_height, "}");
		}

		void emit_condition_flag(const std::string& name) override {
			_painter->setPen(QColor("#9e9e9e"));
			_painter->setFont(_register_font);
			_painter->drawText(0, _line_height, QString::fromStdString(name));
			_painter->translate(20, 0);
		}

		void emit_label_address(uint32_t label_address) override {
			_painter->setPen(QColor("#87d787"));
			_painter->setFont(_register_font);
			_painter->drawText(5, _line_height, QString("%1").arg(label_address, 8, 16, QChar('0')));
			_painter->translate(40, 0);
		}

		void emit_begin_deref() override {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->setFont(_address_font);
			_painter->drawText(2, _line_height, "[");
			_painter->translate(10, 0);
		}

		void emit_end_deref() override {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->setFont(_address_font);
			_painter->drawText(2, _line_height, "]");
			_painter->translate(10, 0);
		}

		const char* R(reg_idx i) {
			return reg_names_std[i];
		}

		const char* const reg_names_std[16] = {
			"r0","r1","r2","r3",
			"r4","r5","r6","r7",
			"r8","r9","r10","r11",
			"r12","sp","lr","pc"
		};
	};

public:
	DisassemblyView(QObject* parent = nullptr)
		: mMem(nullptr)
		, mBaseAddress(0)
		, mBreakpointRegistry(nullptr)
		, mLineHeight(20)
	{
		connect(this, &DisassemblyView::baseAddressChanged, this, &DisassemblyView::requestUpdate);
		connect(this, &DisassemblyView::dimensionsChanged, this, &DisassemblyView::requestUpdate);
	}

	QQmlListProperty<AddressTracker> addressTrackers();

	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) override {
		QQuickItem::geometryChanged(newGeometry, oldGeometry);
		emit dimensionsChanged();
	}

	void setBreakpointRegistry(BreakpointRegistry* registry) {
		if(registry != mBreakpointRegistry) {
			mBreakpointRegistry = registry;
			// force a refresh whenever a breakpoint changes
			connect(mBreakpointRegistry, &BreakpointRegistry::breakpointsChanged, this, &DisassemblyView::requestUpdate);
			emit breakpointRegistryChanged();
		}
	}

	BreakpointRegistry* breakpointRegistry() {
		return mBreakpointRegistry;
	}

	void setBaseAddress(uint32_t address) {
		if(mBaseAddress != address) {
			mBaseAddress = address;
			emit baseAddressChanged();
		}
	}

	uint32_t baseAddress() const {
		return mBaseAddress;
	}

	void setMemoryRegion(QMemRegion* region) {
		if(mMem != region) {
			if(mMem) {
				disconnect(mMem, &QMemRegion::memoryChanged, this, &DisassemblyView::requestUpdate);
			}
			mMem = region;
			connect(mMem, &QMemRegion::memoryChanged, this, &DisassemblyView::requestUpdate);
			emit regionChanged();
		}
	}

	QMemRegion* memoryRegion() {
		return mMem;
	}

	int maxDisplayableIntructions() const {
		return static_cast<int>(qFloor(boundingRect().height() / mLineHeight));
	}

	void paint(QPainter* painter) override {
		paintPaged(painter);
	}

	void paintPaged(QPainter* painter) {
		painter->resetTransform();
		InstructionPainter instructionPainter(mLineHeight);
		instructionPainter.setPainter(painter);
		mAddressIndexCache.clear();
		const int numDiplaybaleInstructions = maxDisplayableIntructions();
		uint32_t address = mBaseAddress;
		for(int i = 0; i < numDiplaybaleInstructions; i++) {
			mAddressIndexCache.push_back(address);
			instruction_pair instruction;
			if(!instructionAtAddress(address, instruction)) {
				instructionPainter.drawVoid(address);
				instructionPainter.next_line();
				address += 2;
				continue;
			}

			const AddressTracker* foundTracker = nullptr;
			for(const AddressTracker* addressTracker : mAddressTrackers) {
				if(address == addressTracker->address()) {
					foundTracker = addressTracker;
					break;
				}
			}


			bool isBreakpoint = mBreakpointRegistry ? mBreakpointRegistry->shouldBreakAt(address) : false;
			if(foundTracker) {
				instructionPainter.drawHighlight(foundTracker);
			}
			instructionPainter.drawInstruction(address, instruction, isBreakpoint);
			instructionPainter.next_line();
			address += instruction.size();
		}
	}

	void paintCentered(QPainter* painter) {

		const int maxNumDiplaybalePrevInstructions = (maxDisplayableIntructions() / 2);
		const int maxNumDiplaybaleNextInstructions = (maxDisplayableIntructions() / 2);
		const int numDiplaybaleInstructions = 1 +
				maxNumDiplaybalePrevInstructions +
				maxNumDiplaybaleNextInstructions;
		painter->resetTransform();

		// move at the vertical center offset
		painter->translate(0, mLineHeight * maxNumDiplaybalePrevInstructions);
		painter->save();

		InstructionPainter instructionPainter(mLineHeight);
		instructionPainter.setPainter(painter);

		uint32_t address = mBaseAddress;
		mAddressIndexCache.clear();
		mAddressIndexCache.resize(numDiplaybaleInstructions);

		// First walk back the instruction stream and
		// each previosu instructions upward
		int displayedPrevInstructons = 0;
		while(displayedPrevInstructons < maxNumDiplaybalePrevInstructions) {
			instruction_pair instruction;
			instructionPainter.prev_line();
			if(prevInstruction(address, instruction)) {
				address -= instruction.size();
				bool isBreakpoint = mBreakpointRegistry ? mBreakpointRegistry->shouldBreakAt(address) : false;
				instructionPainter.drawInstruction(address, instruction,isBreakpoint);
			} else {
				instructionPainter.drawVoid(address);
				address -= 2;
			}
			int index = maxNumDiplaybalePrevInstructions - displayedPrevInstructons - 1;
			mAddressIndexCache[index] = address;
			displayedPrevInstructons++;
		}

		painter->restore();
		address = mBaseAddress;
		int displayedNextInstructons = 0;
		while(displayedNextInstructons < maxNumDiplaybaleNextInstructions) {
			int index = maxNumDiplaybalePrevInstructions + displayedNextInstructons;;
			mAddressIndexCache[index] = address;
			if(address == mBaseAddress) {
				painter->fillRect(0,0, 500, mLineHeight, Qt::gray);
			}
			instruction_pair instruction;
			if(instructionAtAddress(address, instruction)) {
				bool isBreakpoint = mBreakpointRegistry ? mBreakpointRegistry->shouldBreakAt(address) : false;
				instructionPainter.drawInstruction(address, instruction,isBreakpoint);
				address += instruction.size();
			} else {
				instructionPainter.drawVoid(address);
				address += 2;
			}
			instructionPainter.next_line();
			displayedNextInstructons++;
		}
	}

signals:
	void regionChanged();
	void baseAddressChanged();
	void breakpointRegistryChanged();
	void dimensionsChanged();
	void adressTrackersChanged();

public slots:

	quint32 addressAtYPos(int yPos) {
		qreal ratio = yPos / boundingRect().height();
		size_t instructionIndex = qFloor(maxDisplayableIntructions() * ratio);
		if(instructionIndex < mAddressIndexCache.size()) {
			return mAddressIndexCache.at(instructionIndex);
		} else {
			qWarning() << "Trying to read out of bounds index from AddressIndexCache";
			return 0;
		}
	}

	void requestUpdate() {
		update();
	}

	bool prevInstruction(uint32_t address, instruction_pair& previous_instruction) const {
		// address MUST be the address of a valid instruction
		uint32_t regularInstructionAddress = address - 2;
		uint32_t wideInstructionAddress = address - 4;
		if(mMem->isValidVirtualAddress(wideInstructionAddress)) {
			instruction_pair candidate;
			if(instructionAtAddress(wideInstructionAddress, candidate) && candidate.is_wide()) {
				// Bingo: instruction at -4 is a valid 4-byte instruction.
				previous_instruction = candidate;
				return true;
			} else {
				// try to see if theres is a valid standard (2-bytes) intruction
				// at - 2
				instruction_pair candidate;
				if(instructionAtAddress(regularInstructionAddress, candidate)) {
					if(candidate.is_wide()) {
						// can happend if address is NOT a valid instruction address
						fprintf(stderr, "Trying to find a 4 byts instruction from a wrong address\n");
					}
					// Bingo: instruction at -2 is a valid 2-byte instruction.
					previous_instruction = candidate;
					return true;
				}
			}
		} else {
			return false;
		}
	}

	bool prevInstructionAddress(uint32_t address, uint32_t& previous_address) const {
		instruction_pair previous_instruction;
		if(prevInstruction(address, previous_instruction)) {
			previous_address = address - previous_instruction.size();
			return true;
		}
		return false;
	}

	bool nextInstruction(uint32_t address, instruction_pair& next_instruction) const {
		// get instruction at address and add its size
		instruction_pair base;
		if(instructionAtAddress(address, base)) {
			uint32_t candidate = address + base.size();
			if(!mMem->isValidVirtualAddress(candidate)) {
				return false;
			}
			instruction_pair next;
			if(instructionAtAddress(address + base.size(), next)) {
				next_instruction = next;
				return true;
			}
			return true;
		} else {
			return false;
		}
	}

	bool nextInstructionAddress(uint32_t address, uint32_t& next_address) const {
		instruction_pair instr;
		if(nextInstruction(address, instr)) {
			next_address = address + instr.size();
			return true;
		}
		return false;
	}

	void scrollUp() {
		uint32_t newAddress;
		if(prevInstructionAddress(mBaseAddress, newAddress)) {
			setBaseAddress(newAddress);
		} else {
			setBaseAddress(mBaseAddress-2);
		}
	}

	void scrollDown() {
		uint32_t newAddress;
		if(nextInstructionAddress(mBaseAddress, newAddress)) {
			setBaseAddress(newAddress);
		} else {
			setBaseAddress(mBaseAddress+2);
		}
	}

private:

	bool instructionAtAddress(uint32_t address, instruction_pair& result) const {
		halfword first_instr = 0;
		halfword second_instr = 0;
		// first look at the first halfowrd and
		// decide if we must fetch another halfword
		// because the instruction is 4 bytes
		halfword* maybe_first_instr = mMem->virtualToHost<halfword*>(address);
		if(nullptr != maybe_first_instr) {
			first_instr = *maybe_first_instr;
			if(instruction_pair::is_wide_intruction(first_instr)) {
				if(mMem->isValidVirtualAddress(address+sizeof(halfword))) {
					second_instr = maybe_first_instr[1];
				} else {
					// the instruction can't be 4 bytes because that would
					// be off limits.
					return false;
				}
			}
		} else {
			// invalid address
			return false;
		}
		result = instruction_pair(first_instr, second_instr);
		return true;
	}

	QMemRegion* mMem;
	uint32_t mBaseAddress;
	BreakpointRegistry* mBreakpointRegistry;
	int mLineHeight;
	std::vector<uint32_t> mAddressIndexCache;
	std::vector<AddressTracker*> mAddressTrackers;
};

#endif //MICROMACHINE_EMU_DISASMVIEW_HPP
