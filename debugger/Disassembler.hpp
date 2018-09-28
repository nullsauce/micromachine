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
//#include "structured_disasm.hpp"

class Disassembler : public QQuickPaintedItem {
	Q_OBJECT
	Q_PROPERTY(QMemRegion* memoryRegion READ memoryRegion WRITE setMemoryRegion NOTIFY regionChanged)
	Q_PROPERTY(quint32 centerAddress READ centerAddress WRITE setCenterAddress NOTIFY centerAddressChanged)

private:

	class InstructionPainter : public structured_disasm {
	public:
		InstructionPainter()
			: _painter(nullptr)
			, _name_font("Hack")
			, _register_font(_name_font)
			, _address_font(_name_font) {
			_name_font.setPixelSize(18);
			_register_font.setPixelSize(18);
			_register_font.setBold(false);
			_address_font.setPixelSize(18);
		}
		virtual ~InstructionPainter() = default;

		void drawVoid(QPainter* painter, uint32_t instruction_address) {
			_painter = painter;
			setAddress(instruction_address);
			_painter->save();
			draw_address(_addr);
			_painter->restore();
			_painter->translate(0, 20);
		}

		void drawInstruction(QPainter* painter, uint32_t instruction_address, instruction_pair instruction) {
			_painter = painter;
			setAddress(instruction_address);
			_painter->save();
			draw_address(_addr);
			dispatch_instruction(instruction);
			_painter->restore();
			_painter->translate(0, 20);
		}

	private:
		QPainter* _painter;
		QFont _name_font;
		QFont _register_font;
		QFont _address_font;


		void draw_address(uint32_t address) {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->setFont(_address_font);
			_painter->drawText(5, 20, QString("%1").arg(address, 8, 16, QChar('0')));
			_painter->translate(100, 0);
		}

		void emit_name(const std::string& name) override {
			_painter->setPen(QColor("#d3d3d3"));
			_painter->setFont(_name_font);
			QString text = QString::fromStdString(name);
			_painter->drawText(2, 20, text);
			QFontMetrics metrics(_name_font);
			int width = metrics.size(Qt::TextSingleLine, text).width();
			_painter->translate(width + 2, 0);
		}

		void emit_reg(uint8_t reg_id) override {
			_painter->setPen(QColor("#ff8700"));
			_painter->setFont(_register_font);
			_painter->drawText(5, 20, R(reg_id));
			_painter->translate(40, 0);
		}

		void emit_special_reg(const std::string& reg_name) override {
			_painter->setPen(QColor("#d7005f"));
			_painter->setFont(_register_font);
			_painter->drawText(5, 20, QString::fromStdString(reg_name));
			_painter->translate(40, 0);
		}

		void emit_immediate(uint32_t immediate) override {
			_painter->setPen(QColor("#5fd7ff"));
			_painter->setFont(_register_font);
			QString text = QString("#%1").arg(QString::number(immediate, 16));
			_painter->drawText(5, 20, text);
			QFontMetrics metrics(_register_font);
			int width = metrics.size(Qt::TextSingleLine, text).width();
			_painter->translate(width + 5, 0);
		}

		void emit_regs(const std::vector<uint8_t>& regs) override {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->drawText(2, 20, "{");
			_painter->translate(20, 0);

			_painter->setPen(QColor("#ad5c00"));
			_painter->setFont(_register_font);
			for(uint8_t reg_id : regs) {
				_painter->drawText(0, 20, R(reg_id));
				_painter->translate(30, 0);
			}
			_painter->setPen(QColor("#6c6c6c"));
			_painter->drawText(10, 20, "}");
		}

		void emit_condition_flag(const std::string& name) override {
			_painter->setPen(QColor("#9e9e9e"));
			_painter->setFont(_register_font);
			_painter->drawText(0, 20, QString::fromStdString(name));
			_painter->translate(20, 0);
		}

		void emit_label_address(uint32_t label_address) override {
			_painter->setPen(QColor("#87d787"));
			_painter->setFont(_register_font);
			_painter->drawText(5, 20, QString("%1").arg(label_address, 8, 16, QChar('0')));
			_painter->translate(40, 0);
		}

		void emit_begin_deref() override {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->setFont(_address_font);
			_painter->drawText(2, 20, "[");
			_painter->translate(10, 0);
		}

		void emit_end_deref() override {
			_painter->setPen(QColor("#6c6c6c"));
			_painter->setFont(_address_font);
			_painter->drawText(2, 20, "]");
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


	Disassembler(QObject* parent = nullptr)
		: mMem(nullptr)
		, mCenterAddress(0)
	{
		//connect(this, &Disassembler::paddingInstructionCountChanged, this, &Disassembler::update);
		connect(this, &Disassembler::centerAddressChanged, this, &Disassembler::requestUpdate);
		connect(this, &Disassembler::geometryChanged, this, &Disassembler::requestUpdate);
	}

	void setCenterAddress(uint32_t address) {
		if(mCenterAddress != address) {
			mCenterAddress = address;
			emit centerAddressChanged();
		}
	}

	uint32_t centerAddress() const {
		return mCenterAddress;
	}

	void setMemoryRegion(QMemRegion* region) {
		if(mMem != region) {
			if(mMem) {
				disconnect(mMem, &QMemRegion::memoryChanged, this, &Disassembler::requestUpdate);
			}
			mMem = region;
			connect(mMem, &QMemRegion::memoryChanged, this, &Disassembler::requestUpdate);
			emit regionChanged();
		}
	}

	QMemRegion* memoryRegion() {
		return mMem;
	}

	void paint(QPainter* painter) override {

		painter->resetTransform();
		InstructionPainter instructionPainter;
		const uint32_t base = mCenterAddress;
		const float lineHeight = 20;
		const uint32_t numDiplaybaleInstructions = (uint32_t)qFloor(boundingRect().height()/lineHeight);
		uint32_t displayedInstructons = 0;
		uint32_t address = base;
		while(displayedInstructons < numDiplaybaleInstructions) {
			if(!mMem->isValidVirtualAddress(address)) {
				instructionPainter.drawVoid(painter, address);
				address += 2;
				displayedInstructons++;
				continue;
			};
			instruction_pair instr = instructionAtAddress(address);
			instructionPainter.drawInstruction(painter, address, instr);
			address += instr.size();
			displayedInstructons++;
		}
	}

signals:

	void regionChanged();
	void instructionsChanged();
	void paddingInstructionCountChanged();
	void centerAddressChanged();

public slots:

	void requestUpdate() {
		update();
	}

	void scrollUp() {
		//if(mPaddingInstructionCount < 1) return;
		//if(mInstructions.size() < 3) return;
		uint32_t prevAddress = centerAddress() - instructionAtAddress(mCenterAddress-2).size();
		if(mMem->isValidVirtualAddress(prevAddress)) {
			setCenterAddress(prevAddress);
		}
	}

	void scrollDown() {
		//if(mPaddingInstructionCount < 1) return;
		//if(mInstructions.size() < 3) return;
		uint32_t nextAddress = centerAddress() + instructionAtAddress(mCenterAddress).size();
		if(mMem->isValidVirtualAddress(nextAddress)) {
			setCenterAddress(nextAddress);
		}
	}

private:
	/*
	uint32_t disassembleAddress(uint32_t address, InstructionDetails& disasm, uint32_t& data) {
		uint32_t offset = mMem->virtualAddressOffset(address);
		const uint8_t* buffer = mMem->hostMemoryBase();
		halfword first_instr = *(uint16_t*)(buffer + offset); // always prefetch
		halfword second_instr = *(uint16_t*)(buffer + offset + sizeof(halfword)); // always prefetch
		instruction_pair instr = instruction_pair(first_instr, second_instr);
		data = instr.second << 16 | instr.first;
		structured_disasm d(address, DISASM_FMT_HEX, disasm);
		d.dispatch_instruction(instr);
		return instr.size();
	}*/

	instruction_pair instructionAtAddress(uint32_t address) {
		halfword first_instr = 0;
		halfword second_instr = 0;
		// first look at the first halfowrd and
		// decide if we must fetch another halfword
		// because the instruction is 4 bytes
		halfword* maybe_first_instr = mMem->virtualToHost<halfword*>(address);
		if(nullptr != maybe_first_instr) {
			first_instr = *maybe_first_instr;
			if(instruction_pair::is_wide_intruction(first_instr)) {
				if(mMem->isValidOffset(address+sizeof(halfword))) {
					second_instr = maybe_first_instr[1];
				} else {
					// the instruction can't be 4 bytes because that would
					// be off limits.
					fprintf(stderr, "Attempt to fetch a 4 bytes instruction outside memory region limits.");
				}
			}
		} else {
			// invalid address
		}
		return instruction_pair(first_instr, second_instr);
	}

	QMemRegion* mMem;
	uint32_t mCenterAddress;
};

#endif //MICROMACHINE_EMU_DISASMVIEW_HPP
