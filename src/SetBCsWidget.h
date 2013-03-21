/*
Copyright 2012, 2013
Author Nicolas Edh,
Nicolas.Edh@gmail.com,
or user "nsf" at cfd-online.com

License
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

Description
    This class handles selection of boundary conditions. The BCs
    are esentially two word, (name and type) and a idList of patches.
*/

#ifndef SETBCSWIDGET_H
#define SETBCSWIDGET_H

#include <QWidget>
#include <vtkSmartPointer.h>

class vtkCollection;
class QTreeWidgetItem;
class vtkIdList;
class HexReader;
class HexBlocker;
namespace Ui {
class SetBCsWidget;
}

class SetBCsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetBCsWidget(QWidget *parent = 0);
    ~SetBCsWidget();


    void changeBCs(HexReader * reader);
    void clearBCs();

    HexBlocker *hexBlocker;

public slots:
    void slotCreateBC();
    void slotBCchanged(QTreeWidgetItem *,int);
    void slotSelectPatches();
    void slotSelectionDone(vtkIdList *selectedPatches);
    void slotShowPatchesOnSelection(QTreeWidgetItem* item,int col);
    void slotDeleteBC();

signals:
    void startSelectPatches(vtkIdType bcId);
    void resetInteractor();
    void render();
    void done();
    //below dont work
    void setStatusText(QString);

private:
    Ui::SetBCsWidget *ui;
};

#endif // SETBCSWIDGET_H
