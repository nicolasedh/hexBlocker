/*

  */

#include <QStringList>
#include <QTreeWidget>
#include "SetBCsItem.h"
#include "HexBC.h"


SetBCsItem::SetBCsItem(QTreeWidget *tree) :
    QTreeWidgetItem(tree)
{
    hexBC = vtkSmartPointer<HexBC>::New();
}

SetBCsItem::SetBCsItem(QTreeWidget *tree, const QStringList &strings) :
    QTreeWidgetItem(tree,strings)
{
    hexBC = vtkSmartPointer<HexBC>::New();
}

SetBCsItem::~SetBCsItem()
{
    //QTreeWidgetItem::~QTreeWidgetItem();
}
