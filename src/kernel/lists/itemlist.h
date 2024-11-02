#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QtWidgets>

#include "kernel/items/item.h"

class ItemList : public QAbstractTableModel {
    Q_OBJECT
public:
    ItemList();
    virtual Item* getItem(QString id) = 0;
    virtual int getItemRow(QString id) = 0;
    virtual bool copyItems(QList<QString> ids, QString targetId) = 0;
    virtual bool deleteItems(QList<QString> ids) = 0;
    bool labelItems(QList<QString> ids, QString label);
    virtual bool moveItems(QList<QString> ids, QString targetId) = 0;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool greaterId(QString firstId, QString secondId);
};

#endif // ITEMLIST_H
