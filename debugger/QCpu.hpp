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
#include "BreakpointRegistry.hpp"
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


class QCpu : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<Register> regs READ regs CONSTANT)
	Q_PROPERTY(quint32 currentPC READ currentPC NOTIFY changed)
	Q_PROPERTY(QQmlListProperty<QMemRegion> memoryRegions READ memoryRegions CONSTANT)
	Q_PROPERTY(BreakpointRegistry* breakpointRegistry READ breakpointRegistry CONSTANT)
public:
	QCpu(QObject* parent = nullptr)
		: QObject(parent)
		, _breakpoint_registry(nullptr) {

		const char* const reg_names_std[16] = {
			"r00","r01","r02","r03",
			"r04","r05","r06","r07",
			"r08","r09","r10","r11",
			"r12","sp","lr","pc"
		};

		for(int i = 0; i < 16; i++) {
			_registers.append(new Register(this,reg_names_std[i]));
		}

		_cpu.set_io_callback([this](uint8_t op, uint8_t data) {
			//fprintf(stderr, "IO: op:%02x data:%02x char:%c\n", op, data, data);
			emit ioData(op, data);
		});
	}

	BreakpointRegistry* breakpointRegistry() {
		return _breakpoint_registry;
	}

	void setBreakpointRegistry(BreakpointRegistry* registry) {
		_breakpoint_registry = registry;
	}

	QQmlListProperty<QMemRegion> memoryRegions() {
		return QQmlListProperty<QMemRegion>(this, _memory_regions);
	}

	QQmlListProperty<Register> regs() {
		return QQmlListProperty<Register>(this, _registers);
	}

	quint32 currentPC() const {
		return _cpu.regs().get_pc();
	}

public slots:

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

	bool step(int steps) {
		_breakpoint_registry->markAllAsUnreached();
		bool iterationCompleted = true;
		for(int i = 0; i < steps; i++) {
			_cpu.step();
			uint32_t addr = _cpu.regs().get_pc();
			if(_breakpoint_registry->shouldBreakAt(addr)) {
				_breakpoint_registry->breakpointAt(addr)->setReached(true);
				iterationCompleted = false;
				break;
			}
		}
		updateViewModels();
		return iterationCompleted;
	}

	void reset() {
		_cpu.reset();
		updateViewModels();
	}


signals:
	void regsChanged();
	void nameChanged();
	void instructionsChanged();
	void changed();
	void desiredInstructionCountChanged();
	void memViewChanged();
	void ioData(quint8 op, quint8 data);

private:
	void updateViewModels() {
		for(int i = 0; i < 16; i++) {
			_registers.at(i)->setValue(_cpu.regs().get(i));
		}

		for(int i = 0; i < _memory_regions.size(); i++) {
			_memory_regions.at(i)->markMemoryChanged();
		}
		emit instructionsChanged();
		emit changed();
	}

	QList<Register*> _registers;
	QList<QMemRegion*> _memory_regions;
	cpu _cpu;
	BreakpointRegistry* _breakpoint_registry;


};


#endif //MICROMACHINE_CPU_HPP
