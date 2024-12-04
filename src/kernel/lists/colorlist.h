/*
    Copyright (c) Arne Bersch
    This file is part of Zöglfrex.
    Zöglfrex is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    Zöglfrex is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Zöglfrex. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef COLORLIST_H
#define COLORLIST_H

#include <QtWidgets>

#include "itemlist.h"
#include "../items/color.h"

class Kernel;

template class ItemList<Color>;
class ColorList : public ItemList<Color> {
    Q_OBJECT
public:
    ColorList(Kernel *core);
    void setOtherAttribute(QList<QString> ids, QMap<int, QString> attribute, QList<int> value, QString text);
};

#include "kernel/kernel.h"

#endif // COLORLIST_H
