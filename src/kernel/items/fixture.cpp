/*
    Copyright (c) Arne Bersch
    This file is part of Zöglfrex.
    Zöglfrex is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    Zöglfrex is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Zöglfrex. If not, see <https://www.gnu.org/licenses/>.
*/

#include "fixture.h"
#include "kernel/kernel.h"

Fixture::Fixture(Kernel* core) : Item(core) {}

Fixture::Fixture(const Fixture* item) : Item(item->kernel) {
    label = item->label;
    model = item->model;
    address = 0; // using the same address as the other Fixture could result in an address conflict
}

Fixture::~Fixture() {
    for (Group* group : kernel->groups->items) {
        group->fixtures.removeAll(this);
    }
    for (Intensity* intensity : kernel->intensities->items) {
        intensity->fixtureDimmers.remove(this);
    }
}

QString Fixture::name() {
    QString channels = "Channel";
    if (model != nullptr) {
        channels = model->channels;
    }
    if (label.isEmpty()) {
        return Item::name() + channels + " (" + QString::number(address) + ")";
    }
    return Item::name();
}

QString Fixture::info() {
    QString info = Item::info();
    info += "\n" + kernel->fixtures->MODELATTRIBUTEID + " Model: ";
    if (model == nullptr) {
        info += "None (Dimmer)";
    } else {
        info += model->name();
    }
    info += "\n" + kernel->fixtures->ADDRESSATTRIBUTEID + " Address: " + QString::number(address);
    return info;
}
