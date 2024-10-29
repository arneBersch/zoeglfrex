/*
    Copyright (c) Arne Bersch
    This file is part of Zöglfrex.
    Zöglfrex is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    Zöglfrex is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Zöglfrex. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef KERNEL_H
#define KERNEL_H

#include <QtWidgets>

#include "kernel/lists/modellist.h"
#include "kernel/lists/fixturelist.h"
#include "kernel/lists/grouplist.h"
#include "kernel/lists/intensitylist.h"
#include "kernel/lists/colorlist.h"
#include "kernel/lists/transitionlist.h"
#include "kernel/lists/cuelist.h"
#include "terminal/terminal.h"
#include "inspector/inspector.h"
#include "cuelistview/cuelistview.h"

namespace Keys {
enum {
    Zero, // 0
    One, // 1
    Two, // 2
    Three, // 3
    Four, // 4
    Five, // 5
    Six, // 6
    Seven, // 7
    Eight, // 8
    Nine, // 9
    Plus, // +
    Minus, // -
    Period, // .
    Asterisk, // *
    Thru, // /
    Model, // M
    Fixture, // F
    Group, // G
    Intensity, // I
    Color, // C
    Transition, // T
    Cue, // Q
    Copy, // SHIFT C
    Delete, // SHIFT D
    Label, // SHIFT L
    Move, // SHIFT M
    Record, // SHIFT R
};
}

class Kernel {
public:
    Kernel();
    QString execute(QList<int> command, QString text = QString());
    ModelList *models;
    FixtureList *fixtures;
    GroupList *groups;
    IntensityList *intensities;
    ColorList *colors;
    TransitionList *transitions;
    CueList *cues;
    Terminal *terminal;
    Inspector *inspector;
    CuelistView *cuelistView;
    QMutex *mutex;
private:
    bool isItem(int key);
    bool isOperator(int key);
    bool isNumber(int key);
    int keyToNumber(int key);
    QString keysToId(QList<int> keys);
    QList<float> keysToValue(QList<int> keys);
    QList<QString> keysToSelection(QList<int> keys);
};

#endif // KERNEL_H
