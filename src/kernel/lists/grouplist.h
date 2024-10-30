/*
    Copyright (c) Arne Bersch
    This file is part of Zöglfrex.
    Zöglfrex is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
    Zöglfrex is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
    You should have received a copy of the GNU General Public License along with Zöglfrex. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GROUPLIST_H
#define GROUPLIST_H

#include <QtWidgets>

#include "itemlist.h"

class Kernel;
struct Fixture;

struct Group {
    QString id;
    QString label;
    QSet<Fixture*> fixtures;
};

class GroupList : public ItemList {
    Q_OBJECT
public:
    GroupList(Kernel *core);
    Group* getGroup(QString id);
    int getGroupRow(QString id);
    QString copyGroup(QList<QString> ids, QString targetId);
    QString deleteGroup(QList<QString> ids);
    void deleteFixture(Fixture *fixture);
    QString labelGroup(QList<QString> ids, QString label);
    QString moveGroup(QList<QString> ids, QString targetId);
    QString recordGroupFixtures(QList<QString> ids, QList<QString> fixtureIds);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, const int role) const override;
    QList<QString> getIds();
private:
    QList<Group*> groups;
    Kernel *kernel;
    Group* recordGroup(QString id);
};

#include "kernel/kernel.h"

#endif // GROUPLIST_H
