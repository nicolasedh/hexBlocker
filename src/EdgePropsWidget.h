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
    This class creates the dialog that's used to create hexblocks.
    It sends signal to MainWindow which handles hexBlocker.

*/
#ifndef EDGEPROPSWIDGET_H
#define EDGEPROPSWIDGET_H

#include <vtkSmartPointer.h>
#include <QWidget>


class HexBlocker;


namespace Ui {
class EdgePropsWidget;
}

class EdgePropsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EdgePropsWidget(QWidget *parent = 0);
    ~EdgePropsWidget();

    //FUNCTIONS
    void setSelectedEdge(vtkIdType selEdgeId);
    //DATA
    HexBlocker * hexBlocker;

    
public slots:
    void slotApply();
    void slotCancel();

signals:
    void apply();
    void cancel();
    void setStatusText(QString);
    void startSelectEdges();

private:
    vtkIdType selectedEdgeId;
    Ui::EdgePropsWidget *ui;
};

#endif // EDGEPROPSWIDGET_H
