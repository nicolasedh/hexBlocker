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
#include "GradingCalculatorDialog.h"
#include <vtkIdList.h>

EdgePropsWidget::EdgePropsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EdgePropsWidget)
{
    ui->setupUi(this);

    selectedEdgeId=-1;
    QDoubleValidator *gradingValidator = new QDoubleValidator(1e-6,1e6,3);
    ui->gradingLineEdit->setValidator(gradingValidator);

    gradCalcDialog = new GradingCalculatorDialog();

    connect(ui->pushButtonApply,SIGNAL(clicked()),this,SLOT(slotApply()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
    connect(ui->selectEdgePushButton,SIGNAL(clicked()),this,SIGNAL(startSelectEdges()));

    connect(ui->calcPushButton,SIGNAL(clicked()),this,SLOT(slotOpenCalc()));
    connect(gradCalcDialog,SIGNAL(apply()),this,SLOT(slotCalcApplied()));
}

EdgePropsWidget::~EdgePropsWidget()
{
    delete ui;
}

void EdgePropsWidget::slotApply()
{
    if(selectedEdgeId<0)
    {
        emit setStatusText("Select edge first!");
        return;
    }

    HexEdge * props = HexEdge::New();

    props->nCells = ui->numCellsSpinBox->value();
    props->grading = ui->gradingLineEdit->text().toDouble();
    int mode=1;
    if(ui->PropagateGradingcheckBox->isChecked())
        mode=0;
    hexBlocker->setEdgePropsOnParallelEdges(props,selectedEdgeId,mode);

    props->Delete();

    //clear selection
    selectedEdgeId=-1;
    hexBlocker->resetColors();

    //Output number of cells
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
    selectedEdgeId=-1;
    emit cancel();
}

void EdgePropsWidget::setSelectedEdge(vtkIdType selEdgeId)
{
    selectedEdgeId=selEdgeId;
    if(selectedEdgeId > -1)
    {
        selectedEdge =hexBlocker->showParallelEdges(selectedEdgeId);
        long int prevNCells = selectedEdge->nCells;
        this->ui->numCellsSpinBox->setValue(prevNCells);
        double grading = selectedEdge->grading;
        QString gradS =QString();
        gradS.sprintf("%.2f",grading);
        this->ui->gradingLineEdit->setText(gradS);
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

void EdgePropsWidget::slotOpenCalc()
{
    if(selectedEdgeId > -1)
    {
        double grading = ui->gradingLineEdit->text().toDouble();
        double length = selectedEdge->getLength();
        long int nCells = ui->numCellsSpinBox->value();

        gradCalcDialog->setLengthGradingNcells(length,grading,nCells);
        //set length, grading and numcells
        gradCalcDialog->show();

    }
    else
    {
        setStatusText("Select an edge first");
    }
}

void EdgePropsWidget::slotCalcApplied()
{
    //if Im visible else nothing
    if(this->isVisible())
    {
        QString str;
        str.sprintf("%.3g",gradCalcDialog->getGrading());
        ui->gradingLineEdit->setText(str);
        ui->numCellsSpinBox->setValue(gradCalcDialog->getNCells());
    }
    //get ncells and grading
}
