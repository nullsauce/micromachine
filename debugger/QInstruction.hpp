/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_QINSTRUCTION_HPP
#define MICROMACHINE_EMU_QINSTRUCTION_HPP


#include <QtCore>
#include <QObject>
#include <QQmlListProperty>
#include "structured_disasm.hpp"

#include "memory.hpp"


class Instruction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString code READ code NOTIFY changed)
    Q_PROPERTY(QString hexAddress READ hexAddress NOTIFY changed)
    Q_PROPERTY(quint32 address READ address NOTIFY changed)
    Q_PROPERTY(quint32 size READ size NOTIFY changed)
    Q_PROPERTY(QString data READ data NOTIFY changed)
    Q_PROPERTY(bool isBreakPoint READ isBreakPoint NOTIFY changed)
    Q_PROPERTY(InstructionDetails* details READ intructionDetails NOTIFY changed)

public:

  Instruction(QObject* parent = nullptr)
		: QObject(parent)
        , _address(0)
        , _size(2)
        , _data("")
        , _is_breakpoint(false) {

	}

    QString data() const {
        return _data;
    }

    void setData(uint32_t data) {
        _data.clear();
        uint8_t* bytes = (uint8_t*)&data;
        for(int i = 0; i < size(); i++) {
            _data += QString("%1").arg(bytes[i], 2, 16, QChar('0'));
        }
        _data += " ";
        for(int i = 0; i < size(); i++) {
            _data += QString("%1").arg(bytes[i], 8, 2, QChar('0')) + " ";
        }
    }

    uint32_t size() const {
        return _size;
    }

    void setSize(uint32_t size) {
        _size = size;
    }

	QString hexAddress() const {
		return QString("%1").arg(_address, 8, 16, QChar('0'));
	}

	QString code() const {
		return _code;
	}

	InstructionDetails* intructionDetails() {
		return &_details;
	}

    InstructionDetails& details() {
        return _details;
    }

    quint32 address() const {
        return _address;
    }

	void setCode(const QString& code) {
		_code = code;
		emit changed();
	}

	void setAddress(uint32_t address) {
		_address = address;
		emit changed();
	}

    bool isBreakPoint() const {
        return _is_breakpoint;
    }

    void setIsBreakPoint(bool isBreakPoint) {
        if(isBreakPoint != _is_breakpoint) {
            _is_breakpoint = isBreakPoint;
            emit changed();
        }
    }

signals:

	void changed();

private:

	uint32_t _address;
	QString _code;
	InstructionDetails _details;
    uint32_t _size;
    QString _data;
    bool _is_breakpoint;

};

#endif //MICROMACHINE_EMU_QINSTRUCTION_HPP
