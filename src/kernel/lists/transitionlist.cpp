/*
    Copyright (c) Arne Bersch
    This file is part of Zöglfrex.
    Zöglfrex is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    Zöglfrex is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Zöglfrex. If not, see <https://www.gnu.org/licenses/>.
*/

#include "transitionlist.h"

TransitionList::TransitionList(Kernel *core) : ItemList("Transition", "Transitions") {
    kernel = core;
}

void TransitionList::recordTransitionFade(QList<QString> ids, float fade) {
    if (fade > 60 || fade < 0) {
        kernel->terminal->error("Didn't record Transitions because Record Transition Fade only allows fade times from 0s to 60s.");
        return;
    }
    for (QString id : ids) {
        Transition* transition = getItem(id);
        if (transition == nullptr) {
            transition = recordItem(id);
        }
        transition->fade = fade;
        emit dataChanged(index(getItemRow(transition->id), 0), index(getItemRow(transition->id), 0), {Qt::DisplayRole, Qt::EditRole});
    }
    kernel->terminal->success("Recorded " + QString::number(ids.length()) + " Transitions with fade time " + QString::number(fade) + "s.");
}
