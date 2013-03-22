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
*/

#include "EdgePropsWidget.h"
#include "ui_EdgePropsWidget.h"
#include <QtGui>
#include <QValidator>
#include <iostream>
#include "HexBlocker.h"
#include "HexEdge.h"
#include <vtkIdList.h>

EdgePropsWidget::EdgePropsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EdgePropsWidget)
{
    ui->setupUi(this);

    selectedEdge=-1;

    connect(ui->pushButtonApply,SIGNAL(clicked()),this,SLOT(slotApply()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(ui->selectEdgePushButton,SIGNAL(clicked()),this,SIGNAL(startSelectEdges()));
}

EdgePropsWidget::~EdgePropsWidget()
{
    delete ui;
}

void EdgePropsWidget::slotApply()
{



    if(selectedEdge<0)
    {
        emit setStatusText("Select edge first!");
        return;
    }

    int nCells = ui->numCellsSpinBox->value();
    hexBlocker->setNumberOnParallelEdges(selectedEdge,nCells);
    selectedEdge=-1;
    hexBlocker->resetColors();

    QString msg("Total number of cells: ");
    long int NCells = hexBlocker->calculateTotalNumberOfCells();
    msg=msg.append(QString::number(NCells));
    msg.append("\n which is approximately ");
    double aNCells = (double)NCells;
    msg.append(QString::number(aNCells,'g',3));
    msg.append(".");
    emit setStatusText(msg);
    emit apply();
}

void EdgePropsWidget::slotCancel()
{
    hexBlocker->resetColors();
    selectedEdge=-1;
    emit cancel();
}

void EdgePropsWidget::setSelectedEdge(vtkIdType selEdge)
{
    selectedEdge=selEdge;
    if(selectedEdge > -1)
    {
        int prevNCells=hexBlocker->showParallelEdges(selectedEdge);
        this->ui->numCellsSpinBox->setValue(prevNCells);

        QString msg("Total number of cells: ");
        long int NCells = hexBlocker->calculateTotalNumberOfCells();
        msg=msg.append(QString::number(NCells));
        msg.append("\n which is approximately ");
        double aNCells = (double)NCells;
        msg.append(QString::number(aNCells,'g',3));
        msg.append(".");
        emit setStatusText(msg);
    }
}
