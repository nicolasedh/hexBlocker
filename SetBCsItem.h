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

#ifndef SETBCSITEM_H
#define SETBCSITEM_H

//#include <QList>
//#include <QVariant>
#include <QTreeWidgetItem>
//#include <QtGui>
#include <vtkSmartPointer.h>

class HexBC;

class SetBCsItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    SetBCsItem(QTreeWidget *tree);
    SetBCsItem(QTreeWidget *tree, const QStringList &strings);
    ~SetBCsItem();

public slots:


public:
    vtkSmartPointer<HexBC> hexBC;
};


#endif
