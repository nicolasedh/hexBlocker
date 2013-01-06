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

#include "MoveVerticesWidget.h"
#include "ui_MoveVerticesWidget.h"
#include "InteractorStyleVertPick.h"
#include <iostream>
#include <QtGui>
#include <QValidator>
#include <MainWindow.h>


MoveVerticesWidget::MoveVerticesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MoveVerticesWidget)
{
    ui->setupUi(this);
    QDoubleValidator *valid = new QDoubleValidator(this);
    ui->lineEditX->setValidator(valid);
    ui->lineEditY->setValidator(valid);
    ui->lineEditZ->setValidator(valid);
    dist[0]=0.0;
    dist[1]=0.0;
    dist[2]=0.0;
    delta=true;
    ui->radioMove->setChecked(true);
    ui->radioSet->setChecked(false);
    //connections
    connect(ui->pushButtonApply, SIGNAL(clicked()), this, SLOT(slotApply()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()),this,SLOT(slotDone()));
    connect(ui->pushButtonSelectV,SIGNAL(clicked()),this,SIGNAL(startSelect()));
}

MoveVerticesWidget::~MoveVerticesWidget()
{
    delete ui;
}

void MoveVerticesWidget::slotApply()
{
    QString X = ui->lineEditX->text();
    QString Y = ui->lineEditY->text();
    QString Z = ui->lineEditZ->text();
    delta=ui->radioMove->isChecked();

    if(delta)
    {
        checkedX=true;
        checkedY=true;
        checkedZ=true;
    }
    else
    {
        checkedX=ui->checkBoxX->isChecked();
        checkedY=ui->checkBoxY->isChecked();
        checkedZ=ui->checkBoxZ->isChecked();
    }
    dist[0] = X.toDouble();
    dist[1] = Y.toDouble();
    dist[2] = Z.toDouble();


    emit moveVertices();

    //MainWindow *mypar = static_cast<MainWindow*>(this->parent());
    //mypar->style->;
    //this->close();
}

void MoveVerticesWidget::slotDone()
{
    emit moveDone();
}

void MoveVerticesWidget::slotToggleRadio()
{
    if(ui->radioMove->isChecked())
    {
        ui->radioSet->setChecked(false);
    }
    if(ui->radioSet->isChecked())
    {
        ui->radioMove->setChecked(false);
    }
}
