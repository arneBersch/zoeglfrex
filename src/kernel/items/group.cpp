#include "group.h"
#include "kernel/kernel.h"

Group::Group(Kernel* core) : Item(core) {}

Group::Group(const Group* item) : Item(item->kernel) {
    label = item->label;
    fixtures = item->fixtures;
}

Group::~Group() {
    if (kernel->cuelistView->currentGroup == this) {
        kernel->cuelistView->currentGroup = nullptr;
        kernel->cuelistView->loadCue();
    }
    for (Cue *cue : kernel->cues->items) {
        cue->intensities.remove(this);
        cue->colors.remove(this);
        cue->raws.remove(this);
    }
}

QString Group::name() {
    if (label.isEmpty()) {
        QString response = Item::name();
        for (Fixture* fixture : fixtures) {
            response += fixture->name() + "; ";
        }
        return response;
    }
    return Item::name();
}

QString Group::info() {
    QString info = Item::info();
    QString fixtureNames;
    for (Fixture* fixture : fixtures) {
        fixtureNames += fixture->name() + ", ";
    }
    fixtureNames.chop(2);
    info += "\n2 Fixtures: " + fixtureNames;
    return info;
}

