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

#include "EdgeSetTypeWidget.h"
#include "ui_EdgeSetTypeWidget.h"
#include <QtGui>
#include <QDoubleValidator>
#include <vtkIdList.h>
#include <iostream>
#include "HexBlocker.h"
#include "HexEdge.h"
#include "HexPatch.h"
#include "PointsTableModel.h"
#include "vtkCollection.h"
#include "vtkIdList.h"
#include "vtkPoints.h"

EdgeSetTypeWidget::EdgeSetTypeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EdgeSetTypeWidget)
{
    ui->setupUi(this);

    table = new PointsTableModel();
//    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
//    proxyModel->setSourceModel(table);
//    ui->tableView->setModel(proxyModel);

    ui->tableView->setModel(table);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSortingEnabled(false);
//    ui->tableView->set
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->verticalHeader()->hide();
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    table->insertRows(0,2,QModelIndex());
//    selectedEdgeId=-1;

    QDoubleValidator * dvalidator = new QDoubleValidator();
    ui->radiusLineEdit->setValidator(dvalidator);
    ui->radiusLineEdit->setText("1.0");
    connect(ui->tableView->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
        this, SIGNAL(selectionChanged(QItemSelection)));

    connect(table,SIGNAL(pointEdited()),
            this,SLOT(slotDataChanged()));
    connect(ui->pushButtonApply,SIGNAL(clicked()),this,SLOT(slotApply()));
    connect(ui->selectPatchPushButton,SIGNAL(clicked()),
            this,SIGNAL(startSelectPatch()));
    connect(ui->typeComboBox,SIGNAL(currentIndexChanged(int)),
            this,SLOT(slotTypeChanged()));
    connect(ui->useRadiusCheckBox,SIGNAL(toggled(bool)),
            this,SLOT(slotUseRadiusToggled()));
}

EdgeSetTypeWidget::~EdgeSetTypeWidget()
{
    delete ui;
}

void EdgeSetTypeWidget::slotApply()
{
    hexBlocker->resetColors();
    emit apply();
}


void EdgeSetTypeWidget::setSelectedEdge(vtkIdType selEdgeId)
{
    selectedEdgeId=selEdgeId;
    selectedEdge = HexEdge::SafeDownCast(
                hexBlocker->edges->GetItemAsObject(selectedEdgeId));

    vtkIdList * cntrlPoints = selectedEdge->cntrlPointsIds.GetPointer();
    vtkPoints * points = selectedEdge->myPoints;
    table->setPoints(points,cntrlPoints);

    this->ui->typeComboBox->setCurrentIndex(selectedEdge->getType());
    this->slotTypeChanged(); //so buttons are updated.
}


void EdgeSetTypeWidget::slotTypeChanged()
{
    int typeInd = ui->typeComboBox->currentIndex();
    HexEdge::edgeTypes type;
    //set up the GUI
    switch(typeInd)
    {
    case 0:
        type=HexEdge::LINE;
        ui->tableTitle->setText(tr("Not used for type Line"));
        ui->tableView->setEnabled(false);
        ui->radiusLineEdit->setEnabled(false);
        ui->useRadiusCheckBox->setEnabled(false);
        ui->selectPatchPushButton->setEnabled(false);
        break;
    case 1:
        ui->tableTitle->setText(tr("Arc control point in blockMeshDict"));
        type=HexEdge::ARC;
        ui->selectPatchPushButton->setEnabled(true);
        ui->tableView->setEnabled(true);
        ui->radiusLineEdit->setEnabled(ui->useRadiusCheckBox->isChecked());
        ui->useRadiusCheckBox->setEnabled(true);
        break;
    }
    selectedEdge->setType(type);
    table->update();
//    this->setSelectedEdge(selectedEdgeId);
}

void EdgeSetTypeWidget::slotDataChanged()
{
    selectedEdge->redrawedge();
    hexBlocker->render();
}

void EdgeSetTypeWidget::setSelectedPatch(vtkIdType selPatchId)
{
    double c[3];
    HexPatch * p = HexPatch::SafeDownCast(
        hexBlocker->patches->GetItemAsObject(selPatchId)
                );
    p->getCenter(c);
    p->resetColor();
    if(ui->useRadiusCheckBox->isChecked())
    {
        double R=ui->radiusLineEdit->text().toDouble();

        //check for sensible radius.
        if(R==0.0)
        {
            emit setStatusText(tr("Cannot use zero radius"));
            return;
        }
        double edgeLength = selectedEdge->getLength();
        if( -edgeLength/2.0 < R &&  R < edgeLength/2.0)
        {
            QString msg=QString(
  "The radius has to be larger than or equal to  half the edge length, i.e >= ");
            msg.append(QString::number(edgeLength/2.0));
            emit setStatusText(tr(msg));
            return;
        }
        selectedEdge->calcArcControlPointFromCenter(c,R);
    }
    else
    {
        selectedEdge->calcArcControlPointFromCenter(c);
    }
//    selectedEdge->getControlPoint(0,c);
    table->update();
    slotDataChanged();
}

void EdgeSetTypeWidget::slotUseRadiusToggled()
{
    //enable or disable radius field
    ui->radiusLineEdit->setEnabled(ui->useRadiusCheckBox->isChecked());

}
