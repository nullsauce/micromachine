/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_ADDRESSTRACKER_HPP
#define MICROMACHINE_EMU_ADDRESSTRACKER_HPP

#include <QObject>
#include <QQmlListProperty>
#include <QColor>

class AddressTracker : public QObject {
    Q_OBJECT
    Q_PROPERTY(quint32 address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString label READ label WRITE setLabel)
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:

    quint32 address() const {
        return _address;
    }

    void setAddress(quint32 address) {
        if(address != _address) {
            _address = address;
            emit addressChanged();
        }
    }

    void setLabel(const QString& val) {
        _label = val;
    }

    QString label() const {
        return _label;
    }

    void setColor(const QColor& val) {
        _color = val;
    }

    QColor color() const {
        return _color;
    }

    AddressTracker(
            const QString& label = "",
            QColor color = Qt::gray,
            QObject* parent = nullptr)
        : QObject(parent)
        , _address(0)
        , _label(label)
        , _color(color) {

    }

signals:
    void addressChanged();

private:
    quint32 _address;
    QString _label;
    QColor _color;
};




#endif //MICROMACHINE_EMU_ADDRESSTRACKER_HPP
