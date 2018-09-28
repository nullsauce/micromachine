/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_QCPU_HPP
#define MICROMACHINE_QCPU_HPP

#include <QtCore>
#include <QObject>
#include <QQmlListProperty>

#include "cpu.hpp"
#include "MemView.hpp"
#include "QInstruction.hpp"
#include <unordered_map>

class Register : public QObject {
	Q_OBJECT
	Q_PROPERTY(QString name READ name CONSTANT)
	Q_PROPERTY(quint32 value READ value NOTIFY valueChanged)
	Q_PROPERTY(QString hexValue READ hexValue NOTIFY valueChanged)
public:
	Register(QObject* parent = nullptr, const QString& name = "")
		: QObject(parent)
		, _val(0)
		, _name(name) {

	}

	QString hexValue() const {
		 return QString("%1").arg(_val, 8, 16, QChar('0'));
	}

	uint32_t value() const {
		return _val;
	}

	void setValue(uint32_t v) {
		if(v != _val) {
			_val = v;
			emit valueChanged();
		}
	}

	QString name() const {
		return _name;
	}

signals:
	void valueChanged();

private:

	uint32_t _val;
	const QString _name;
};

class Breakpoint : public QObject {
	Q_OBJECT
	Q_PROPERTY(quint32 address MEMBER _address CONSTANT)
	Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
	void setEnabled(bool val) {
		if(val != _enabled) {
			_enabled = val;
			emit enabledChanged();
		}
	}

	bool enabled() const {
		return _enabled;
	}

	Breakpoint(uint32_t address = 0, QObject* parent = nullptr)
		: QObject(parent)
		, _address(address)
		, _enabled(true) {

	}

signals:
	void enabledChanged();

private:
	bool _enabled;
	const uint32_t _address;

};

class BreakpointRegistry : public QObject {
	Q_OBJECT
public:
	using breakpoint_list = std::unordered_map<uint32_t, std::unique_ptr<Breakpoint>>;

public slots:

	void setBreakPoint(quint32 address, bool enabled) {
		Breakpoint& bp = findOrCreateAtAddress(address);
		bp.setEnabled(enabled);
	}

	bool toggleBreakpoint(quint32 address) {
		bool newState = !isBreakPoint(address);
		setBreakPoint(address, newState);
		return newState;
	}

	bool isBreakPoint(quint32 instructionAddr) {
		auto bkp = _breakpoints.find(instructionAddr);
		if(bkp != _breakpoints.end()) {
			return bkp->second->enabled();
		} else {
			return false;
		}
	}


private:

	Breakpoint& findOrCreateAtAddress(uint32_t address) {
		auto it = _breakpoints.find(address);
		if(_breakpoints.end() != it) {
			return *it->second;
		} else {
			auto ptr = std::make_unique<Breakpoint>(address);
			auto newit = _breakpoints.insert(std::make_pair(address, std::move(ptr)));
			return *ptr;
		}
	}

	breakpoint_list _breakpoints;
};

class QCpu : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<Register> regs READ regs CONSTANT)
	Q_PROPERTY(QQmlListProperty<Instruction> instructions READ instructions NOTIFY instructionsChanged)
	Q_PROPERTY(quint32 currentPC READ currentPC NOTIFY changed)
	Q_PROPERTY(int desiredInstructionCount READ desiredInstructionCount WRITE setDesiredInstructionCount NOTIFY desiredInstructionCountChanged)
	Q_PROPERTY(QQmlListProperty<QMemRegion> memoryRegions READ memoryRegions CONSTANT)
public:
	QCpu(QObject* parent = nullptr)
		: QObject(parent) {

		const char* const reg_names_std[16] = {
			"r00","r01","r02","r03",
			"r04","r05","r06","r07",
			"r08","r09","r10","r11",
			"r12","sp","lr","pc"
		};

		for(int i = 0; i < 16; i++) {
			_registers.append(new Register(this,reg_names_std[i]));
		}

		setDesiredInstructionCount(20);
	}

	bool loadElf(const QString& file) {
		qDebug() << "Loading ELF file" << file;
		_cpu.load_elf(file.toStdString());

		_memory_regions.clear();
		for(const auto& region: _cpu.mem().regions()) {
			auto r = new QMemRegion(this);
			r->setMapping(&region);
			_memory_regions.append(r);
		}

		emit regsChanged();
		emit nameChanged();
		reset();
	}

	QQmlListProperty<QMemRegion> memoryRegions() {
		return QQmlListProperty<QMemRegion>(this, _memory_regions);
	}

	QQmlListProperty<Register> regs() {
		return QQmlListProperty<Register>(this, _registers);
	}

	QQmlListProperty<Instruction> instructions() {
		return QQmlListProperty<Instruction>(this, this,
			&QCpu::_instructionCount,
			&QCpu::_instruction
		);
	}

	int desiredInstructionCount() const {
		return _desired_instruction_count;
	}

	void setDesiredInstructionCount(int count) {
		if(count != _desired_instruction_count) {
			_desired_instruction_count = count;

			while(_instructions.size() < _desired_instruction_count) {
				_instructions.append(
					QSharedPointer<Instruction>(new Instruction, &QObject::deleteLater)
				);
			}
			while(_instructions.size() > _desired_instruction_count) {
				_instructions.pop_back();
			}

			emit desiredInstructionCountChanged();
			updateViewModels();
		}
	}

	int instructionCount() {
		return _instructions.size();
	}

	static int _instructionCount(QQmlListProperty<Instruction>* list) {
		return reinterpret_cast<QCpu*>((list->data))->instructionCount();
	}

	Instruction* instruction(int index) {
		if(index < instructionCount()) {
			return _instructions.at(index).data();
		}
		return &_dummy_instr;
	}

	static Instruction* _instruction(QQmlListProperty<Instruction>* list, int index) {
		return reinterpret_cast<QCpu*>((list->data))->instruction(index);
	}

	quint32 currentPC() const {
		return _cpu.regs().get_pc();
	}

	Q_INVOKABLE bool step(int steps) {
		bool iterationCompleted = true;
		for(int i = 0; i < steps; i++) {
			_cpu.step();
			word addr = _cpu.regs().get_pc();
			if(false /*isBreakPoint(addr)*/) {
				iterationCompleted = false;
				break;
			}
		}
		updateViewModels();
		return iterationCompleted;
	}

	Q_INVOKABLE void reset() {
		_cpu.reset();
	   // _cpu.regs().set_sp(0x10000u);
		updateViewModels();
	}

signals:
	void regsChanged();
	void nameChanged();
	void instructionsChanged();
	void changed();
	void desiredInstructionCountChanged();
	void memViewChanged();

private:


	/*
	void dissasembleIntructionsAround(uint32_t addr) {
		word offset = (desiredInstructionCount()/2)*2;
		for(int i = 0; i < _instructions.size(); i++) {
			uint32_t displayed_addr = addr-offset;
			instruction_pair instr = _cpu.fetch_instruction_debug(displayed_addr);
			auto instruction = _instructions.at(i);
			instruction->setCode(QString::fromStdString(disasm::disassemble_instruction(instr, displayed_addr)));
			instruction->setAddress(displayed_addr);
			auto bkp = _breakpoint_instructions.find(displayed_addr);
			if(bkp != _breakpoint_instructions.end()) {
				instruction->setIsBreakPoint(bkp->second);
			} else {
				instruction->setIsBreakPoint(false);
			}
			addr = addr + instr.size();
		}

	}*/

	void updateViewModels() {
		for(int i = 0; i < 16; i++) {
			_registers.at(i)->setValue(_cpu.regs().get(i));
		}

		//word addr = _cpu.regs().get_pc();
		//dissasembleIntructionsAround(addr);

		for(size_t i = 0; i < _memory_regions.size(); i++) {
			_memory_regions.at(i)->markMemoryChanged();
		}
		emit instructionsChanged();
		emit changed();
	}

   QList<Register*> _registers;
   QList<QSharedPointer<Instruction>> _instructions;
   QList<QMemRegion*> _memory_regions;
   cpu _cpu;
   std::vector<uint8_t> _stack_mem;
   std::vector<uint8_t> _heap_mem;
   std::vector<uint8_t> _initialized_data;
   Instruction _dummy_instr;
   int _desired_instruction_count;


};


#endif //MICROMACHINE_CPU_HPP
