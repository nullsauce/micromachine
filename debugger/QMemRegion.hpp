/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_QMEMREGION_HPP
#define MICROMACHINE_EMU_QMEMREGION_HPP

#include <QObject>
#include "memory.hpp"

class QMemRegion : public QObject {
    Q_OBJECT
    Q_PROPERTY(quint32 base READ base NOTIFY changed)
    Q_PROPERTY(QString name READ name NOTIFY changed)

public:

    QMemRegion(QObject* parent = nullptr)
        : QObject(parent)
        , _mapping(nullptr)
    {}

    void setMapping(const memory::mem_mapping* mapping) {
        if(_mapping != mapping) {
            _mapping = mapping;
            emit changed();
        }
    }

    uint32_t base() const {
        return virtualBase();
    }

    QString name() const {
        return QString::fromStdString(_mapping->name());
    }

    const uint8_t* hostMemoryBase() const {
        return _mapping->host_mem();
    }

    const uint32_t virtualBase() const {
        return _mapping->start();
    }

    const uint32_t virtualRange() const {
        return _mapping->end() - _mapping->start();
    }

    bool isValidVirtualAddress(uint32_t address) const {
        return _mapping->start() <= address &&
            _mapping->end() > address;
    }

    bool isValidOffset(uint32_t offset) const {
        return offset < virtualRange();
    }

    uint32_t virtualAddressOffset(uint32_t address) const {
        return address - _mapping->start();
    }

    void markMemoryChanged() {
        emit memoryChanged();
    }


signals:

    void changed();
    void memoryChanged();

private:

    const memory::mem_mapping* _mapping;

};
#endif //MICROMACHINE_EMU_QMEMREGION_HPP
