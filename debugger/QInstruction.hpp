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

#include "memory.hpp"

class Instruction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString code READ code NOTIFY changed)
    Q_PROPERTY(QString hexAddress READ hexAddress NOTIFY changed)
    Q_PROPERTY(quint32 address READ address NOTIFY changed)
    Q_PROPERTY(quint32 size READ size  NOTIFY changed)
    Q_PROPERTY(bool isBreakPoint READ isBreakPoint NOTIFY changed)

public:

  Instruction(QObject* parent = nullptr)
		: QObject(parent)
        , _address(0)
        , _is_breakpoint(false)
        , _size(2){

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

    QString& mutableCode() {
        return _code;
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
    bool _is_breakpoint;
    uint32_t _size;

};

#endif //MICROMACHINE_EMU_QINSTRUCTION_HPP
