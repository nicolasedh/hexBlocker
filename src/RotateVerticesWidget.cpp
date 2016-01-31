/*
Copyright 2016
Author Leonardo Rosa
https://github.com/leorosa

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

#include "RotateVerticesWidget.h"
#include "ui_RotateVerticesWidget.h"
#include "InteractorStyleVertPick.h"
#include <iostream>
#include <QtGui>
#include <QValidator>
#include <MainWindow.h>


RotateVerticesWidget::RotateVerticesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RotateVerticesWidget)
{
    ui->setupUi(this);
    QDoubleValidator *valid = new QDoubleValidator(this);
    ui->centerX->setValidator(valid);
    ui->centerY->setValidator(valid);
    ui->centerZ->setValidator(valid);
    angle   = 0.0;
    center[0] = 0.0;
    center[1] = 0.0;
    center[2] = 0.0;
    axis[0] = 0.0;
    axis[1] = 0.0;
    axis[2] = 0.0;

    //connections
    connect(ui->pushButtonApply,  SIGNAL(clicked()), this, SLOT(slotApply()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()), this, SLOT(slotDone()));
    connect(ui->pushButtonSelectV,SIGNAL(clicked()), this, SIGNAL(startSelect()));

    connect(ui->buttonX, SIGNAL(clicked()), this, SLOT(setAxisX()));
    connect(ui->buttonY, SIGNAL(clicked()), this, SLOT(setAxisY()));
    connect(ui->buttonZ, SIGNAL(clicked()), this, SLOT(setAxisZ()));
}

RotateVerticesWidget::~RotateVerticesWidget()
{
    delete ui;
}

void RotateVerticesWidget::slotApply()
{
    QString AN = ui->angle->text();
    QString CX = ui->centerX->text();
    QString CY = ui->centerY->text();
    QString CZ = ui->centerZ->text();
    QString AX = ui->axisX->text();
    QString AY = ui->axisY->text();
    QString AZ = ui->axisZ->text();

    angle   = AN.toDouble();
    center[0] = CX.toDouble();
    center[1] = CY.toDouble();
    center[2] = CZ.toDouble();
    axis[0] = AX.toDouble();
    axis[1] = AY.toDouble();
    axis[2] = AZ.toDouble();

    emit rotateVertices();

    //MainWindow *mypar = static_cast<MainWindow*>(this->parent());
    //mypar->style->;
    //this->close();
}

void RotateVerticesWidget::slotDone()
{
    emit rotateDone();
}

void RotateVerticesWidget::setAxisX()
{
    ui->axisX->setText("1");
    ui->axisY->setText("0");
    ui->axisZ->setText("0");
}

void RotateVerticesWidget::setAxisY()
{
    ui->axisX->setText("0");
    ui->axisY->setText("1");
    ui->axisZ->setText("0");
}

void RotateVerticesWidget::setAxisZ()
{
    ui->axisX->setText("0");
    ui->axisY->setText("0");
    ui->axisZ->setText("1");
}

