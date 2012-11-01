/*

  */

#include <QStringList>
#include <QTreeWidget>
#include "SetBCsItem.h"


SetBCsItem::SetBCsItem(QTreeWidget *tree) :
    QTreeWidgetItem(tree)
{

}

SetBCsItem::SetBCsItem(QTreeWidget *tree, const QStringList &strings) :
    QTreeWidgetItem(tree,strings)
{

}

SetBCsItem::~SetBCsItem()
{
    //QTreeWidgetItem::~QTreeWidgetItem();
}
