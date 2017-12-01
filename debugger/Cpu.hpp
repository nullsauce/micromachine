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

class Register : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(int value READ value NOTIFY valueChanged)
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


class Instruction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString code READ code NOTIFY changed)
    Q_PROPERTY(QString hexAddress READ hexAddress NOTIFY changed)

public:

  Instruction(QObject* parent = nullptr, uint32_t address = 0, const QString& code = "<>")
		: QObject(parent)
		, _address(address)
		, _code(code) {

	}

	QString hexAddress() const {
		return QString("%1").arg(_address, 8, 16, QChar('0'));
	}

	QString code() const {
		return _code;
	}

	void setCode(const QString& code) {
		_code = code;
		emit changed();
	}

	void setAddress(uint32_t address) {
		_address = address;
		emit changed();
	}

signals:

	void changed();

private:

	uint32_t _address;
	QString _code;

};

static
unsigned char program[] = {
  0x90, 0xb5, 0x83, 0xb0, 0x00, 0xaf, 0x00, 0x23, 0x7b, 0x60, 0x7b, 0x68,
  0x01, 0x3b, 0x7a, 0x68, 0x10, 0x1c, 0x19, 0x1c, 0x00, 0xf0, 0x10, 0xf8,
  0x04, 0x1c, 0x7b, 0x68, 0x5b, 0x00, 0x7a, 0x68, 0x10, 0x1c, 0x19, 0x1c,
  0x00, 0xf0, 0x08, 0xf8, 0x03, 0x1c, 0x63, 0x43, 0x7a, 0x68, 0xd3, 0x18,
  0x7b, 0x60, 0x61, 0xdf, 0xe9, 0xe7, 0xc0, 0x46, 0x80, 0xb5, 0x82, 0xb0,
  0x00, 0xaf, 0x78, 0x60, 0x39, 0x60, 0x7b, 0x68, 0x7a, 0x68, 0x5a, 0x43,
  0x3b, 0x68, 0xd3, 0x18, 0x18, 0x1c, 0xbd, 0x46, 0x02, 0xb0, 0x80, 0xbd
};

class QCpu : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Register> regs READ regs)
    Q_PROPERTY(QQmlListProperty<Instruction> instructions READ instructions NOTIFY instructionsChanged)
    Q_PROPERTY(int currentPC READ currentPC NOTIFY changed)

public:
    QCpu(QObject* parent = nullptr)
        : QObject(parent)
        , _instructions(20)
        , _dummy_instr(nullptr, 0, "<nothing>"){

        const char* const reg_names_std[16] = {
            "r00","r01","r02","r03",
            "r04","r05","r06","r07",
            "r08","r09","r10","r11",
            "r12","sp","lr","pc"
        };
        for(int i = 0; i < 16; i++) {
            _registers.append(new Register(this,reg_names_std[i]));
        }

        _stack_mem.assign(1024, 0);
        _cpu.mem().map(program, 0, sizeof(program)+1000);
        _cpu.mem().map(_stack_mem.data(), 0x10000u-_stack_mem.size(), _stack_mem.size());

        emit regsChanged();
        emit nameChanged();

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

	int instructionCount() {
		return _instructions.size();
	}

	static int _instructionCount(QQmlListProperty<Instruction>* list) {
		return reinterpret_cast<QCpu*>((list->data))->instructionCount();
	}

	Instruction* instruction(int index) {
		if(index < instructionCount()) {
			return &_instructions[index];
		}
		return &_dummy_instr;
	}

	static Instruction* _instruction(QQmlListProperty<Instruction>* list, int index) {
		return reinterpret_cast<QCpu*>((list->data))->instruction(index);
	}

    int currentPC() const {
        return _cpu.regs().get_pc();
    }

    Q_INVOKABLE void step() {
        _cpu.step();
        updateViewModels();
        word addr = _cpu.regs().get_pc();
        instruction_pair instr = _cpu.fetch_instruction(addr);
    }

    Q_INVOKABLE void reset() {
        _cpu.reset();
        _cpu.regs().set_sp(0x10000u);
        updateViewModels();
    }

signals:
    void regsChanged();
    void nameChanged();
    void instructionsChanged();
    void changed();

private:

    void updateViewModels() {
        for(int i = 0; i < 16; i++) {
            _registers.at(i)->setValue(_cpu.regs().get(i));
        }

	 	word addr = _cpu.regs().get_pc();
        for(int i = 0; i < _instructions.size(); i++) {
        	instruction_pair instr = _cpu.fetch_instruction(addr);
        	_instructions.at(i).setCode(
        		QString::fromStdString(disasm::disassemble_instruction(instr, addr))
        	);
        	_instructions.at(i).setAddress(addr);
        	addr = addr + instr.size();
        }
        emit instructionsChanged();
        emit changed();
    }

   QList<Register*> _registers;
   std::vector<Instruction> _instructions;
   cpu _cpu;
   std::vector<uint8_t> _stack_mem;
   Instruction _dummy_instr;

};


#endif //MICROMACHINE_CPU_HPP
