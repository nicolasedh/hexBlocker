/*
Copyright 2012, 2013
Author Nicolas Edh,
Nicolas.Edh@gmail.com,
or user "nsf" at cfd-online.com

This file is part of hexBlocker.

    hexBlocker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hexBlocker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with hexBlocker.  If not, see <http://www.gnu.org/licenses/>.

    The license is included in the file COPYING.
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
