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
    This class creates the dialog sets the type of edge, i.e. line,
    arc, polyLine, polySpline

*/
#ifndef EDGESETTYPEWIDGET_H
#define EDGESETTYPEWIDGET_H

#include <vtkSmartPointer.h>
#include <QWidget>
#include <QItemSelection>
//Predeclarations
class HexBlocker;
class HexEdge;
class PointsTableModel;

namespace Ui {
class EdgeSetTypeWidget;
}

class EdgeSetTypeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EdgeSetTypeWidget(QWidget *parent = 0);
    ~EdgeSetTypeWidget();

    //FUNCTIONS
    void setSelectedEdge(vtkIdType selEdgeId);
    //Called when the user selected a patch
    //for arc creation
    void setSelectedPatch(vtkIdType selPatchId);
    //DATA
    HexBlocker * hexBlocker;

    
public slots:
    void slotApply();
    void slotTypeChanged();
    void slotDataChanged();
    void slotUseRadiusToggled();


signals:
    void apply();
    void setStatusText(QString);
    void startSelectPatch();
    void selectionChanged (const QItemSelection &selected);

private:
    vtkIdType selectedEdgeId;
    HexEdge *selectedEdge;
    Ui::EdgeSetTypeWidget *ui;
    PointsTableModel * table;
};

#endif // EDGESETTYPEWIDGET_H
