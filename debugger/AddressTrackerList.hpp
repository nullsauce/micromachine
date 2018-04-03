/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_ADDRESSTRACKERLIST_HPP
#define MICROMACHINE_EMU_ADDRESSTRACKERLIST_HPP

#include <QObject>
#include <QQmlListProperty>

#include "AddressTracker.hpp"

#include <unordered_map>
#include <string>

class AddressTrackerList : public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AddressTracker> trackers READ trackers NOTIFY trackerListChanged)

public:

    AddressTrackerList(QObject* parent = nullptr)
        : QObject(parent) {

    }

    QQmlListProperty<AddressTracker> trackers() {
        return QQmlListProperty<AddressTracker>(this, _trackers);
    }

public slots:

    AddressTracker* addTracker(const QString& label, const QColor& color) {
        if(_labels.end() != _labels.find(label.toStdString())) {
            return nullptr;
        }

        AddressTracker* tracker = new AddressTracker(label, color, this);
        _trackers.append(tracker);
        _labels.insert({label.toStdString(), tracker});

        emit trackerListChanged();
        return tracker;
    }

    bool removeTracker(const QString& label) {
        auto it = _labels.find(label.toStdString());
        if(_labels.end() == it) {
            return false;
        }
        _labels.erase(it);
        int index = _trackers.indexOf(it->second);
        if(-1 != index) {
            _trackers.removeAt(index);
        }

        emit trackerListChanged();
        return true;
    }

signals:
    void trackerListChanged();

private:
    quint32 _address;
    QList<AddressTracker*> _trackers;
    std::unordered_map<std::string, AddressTracker*> _labels;
};



#endif //MICROMACHINE_EMU_ADDRESSTRACKERLIST_HPP
