/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "DisassemblyView.hpp"

namespace {
    std::vector<AddressTracker*>& getAddressTrackers(QQmlListProperty<AddressTracker>* list) {
        using tracker_list = std::vector<AddressTracker*>;
        tracker_list* trackers = reinterpret_cast<tracker_list*>(list->data);
        return *trackers;
    }

    void appendAddressTracker(QQmlListProperty<AddressTracker>* list, AddressTracker* val) {
        getAddressTrackers(list).push_back(val);
    }

    int addressTrackerCount(QQmlListProperty<AddressTracker>* list) {
        return getAddressTrackers(list).size();
    }

    AddressTracker* addressTracker(QQmlListProperty<AddressTracker>* list, int index) {
        return getAddressTrackers(list).at(index);
    }

    void clearAddressTrackers(QQmlListProperty<AddressTracker>* list) {
        getAddressTrackers(list).clear();
    }

}

QQmlListProperty<AddressTracker> DisassemblyView::addressTrackers() {
    return QQmlListProperty<AddressTracker>(this, &mAddressTrackers,
                                            &appendAddressTracker,
                                            &addressTrackerCount,
                                            &addressTracker,
                                            &clearAddressTrackers);
}

