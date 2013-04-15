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

#include "GradingCalculatorDialog.h"
#include "ui_GradingCalculatorDialog.h"
#include "TEdgeSpace.h"

#include <iostream>

GradingCalculatorDialog::GradingCalculatorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradingCalculatorDialog)
{
    ui->setupUi(this);

    QDoubleValidator * posDoubleValidator = new QDoubleValidator(1e-200,1e200,3,this);
    posDoubleValidator->setNotation(QDoubleValidator::ScientificNotation);
    dsdeDoubleValidator = new QDoubleValidator(1e-200,1,3,this);

// QRegExp reg=QRegExp("\\d{0,5}\\.{0,1}\\d{1,5}([eE][+-]{0,1}\\d{1,5}){0,1}");
//    QRegExpValidator * gradValidator = new QRegExpValidator(reg,ui->gradingLineEdit);
    ui->gradingLineEdit->setValidator(posDoubleValidator);
    ui->c2cLineEdit->setValidator(posDoubleValidator);
    ui->dsLineEdit->setValidator(dsdeDoubleValidator);
    ui->deLineEdit->setValidator(dsdeDoubleValidator);

    tes = new TEdgeSpace();//Markus calculator

    numCheckedBoxes=2;
    //CONNECTIONS
    connect(ui->applyPushButton,SIGNAL(clicked()),this,SIGNAL(apply()));
    connect(ui->closePushButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(ui->applyPushButton,SIGNAL(clicked()),this,SLOT(slotCalculate()));

    //connect edits changed to calculate
    connect(ui->nCellsSpinBox,SIGNAL(valueChanged(int)),this,SLOT(slotCalculate()));
    connect(ui->gradingLineEdit,SIGNAL(editingFinished()),this,SLOT(slotCalculate()));
    connect(ui->c2cLineEdit,SIGNAL(editingFinished()),this,SLOT(slotCalculate()));
    connect(ui->dsLineEdit,SIGNAL(editingFinished()),this,SLOT(slotCalculate()));
    connect(ui->deLineEdit,SIGNAL(editingFinished()),this,SLOT(slotCalculate()));

    //connect checkboxes to slotCeck
    connect(ui->nCellsCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotCheckboxesChanged()));
    connect(ui->gradingCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotCheckboxesChanged()));
    connect(ui->c2cCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotCheckboxesChanged()));
    connect(ui->dsCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotCheckboxesChanged()));
    connect(ui->deCheckBox,SIGNAL(toggled(bool)),this,SLOT(slotCheckboxesChanged()));
}

GradingCalculatorDialog::~GradingCalculatorDialog()
{
    delete ui;
}

void GradingCalculatorDialog::setLengthGradingNcells(double length, double grading, long int ncells)
{
    QString str;
    str.sprintf("%.3g",length);
    ui->lengthLineEdit->setText(str);
    str.sprintf("%.3g",length);
    ui->gradingLineEdit->setText(str);
    ui->nCellsSpinBox->setValue(ncells);

    //change ds and de validator so they
    //cant be bigger than the length
    dsdeDoubleValidator->setTop(length);
}

void GradingCalculatorDialog::slotCheckboxesChanged()
{
    QCheckBox * chkdBox = qobject_cast<QCheckBox*>(QObject::sender());
    if(chkdBox->isChecked())
    {
        numCheckedBoxes++;
        if(numCheckedBoxes >1)
        {
            //only two can be used so disable all others
            if(!ui->nCellsCheckBox->isChecked())
            {
                ui->nCellsCheckBox->setEnabled(false);
                ui->nCellsSpinBox->setEnabled(false);
            }
            if(!ui->gradingCheckBox->isChecked())
            {
                ui->gradingCheckBox->setEnabled(false);
                ui->gradingLineEdit->setEnabled(false);
            }
            if(!ui->c2cCheckBox->isChecked())
            {
                ui->c2cCheckBox->setEnabled(false);
                ui->c2cLineEdit->setEnabled(false);
            }
            if(!ui->dsCheckBox->isChecked())
            {
                ui->dsCheckBox->setEnabled(false);
                ui->dsLineEdit->setEnabled(false);
            }
            if(!ui->deCheckBox->isChecked())
            {
                ui->deCheckBox->setEnabled(false);
                ui->deLineEdit->setEnabled(false);
            }
        }

    }
    else
    {
        if(numCheckedBoxes>0)
            numCheckedBoxes--;
         //user unchecked so enable all boxes.
        ui->nCellsSpinBox->setEnabled(true);
        ui->nCellsCheckBox->setEnabled(true);
        ui->gradingLineEdit->setEnabled(true);
        ui->gradingCheckBox->setEnabled(true);
        ui->c2cLineEdit->setEnabled(true);
        ui->c2cCheckBox->setEnabled(true);
        ui->dsLineEdit->setEnabled(true);
        ui->dsCheckBox->setEnabled(true);
        ui->deLineEdit->setEnabled(true);
        ui->deCheckBox->setEnabled(true);
    }
    //QObject::sender() or signal mapper
}

void GradingCalculatorDialog::slotCalculate()
{
    if(numCheckedBoxes==2)
    {
        //Find which two arguments to use
        QStringList arglist = QStringList();
        double value[2]; int valueInd=0;

        if(ui->nCellsCheckBox->isChecked())
        {
            value[valueInd] = (double) ui->nCellsSpinBox->value();
            arglist.append("n");
            valueInd++;
        }
        if(ui->gradingCheckBox->isChecked())
        {
            value[valueInd] = ui->gradingLineEdit->text().toDouble();
            arglist.append("r");
            valueInd++;
        }
        if(ui->c2cCheckBox->isChecked())
        {
            value[valueInd] = ui->c2cLineEdit->text().toDouble();
            arglist.append("k");
            valueInd++;
        }
        if(ui->dsCheckBox->isChecked())
        {
            value[valueInd] = ui->dsLineEdit->text().toDouble();
            arglist.append("ds");
            valueInd++;
        }
        if(ui->deCheckBox->isChecked())
        {
            value[valueInd] = ui->deLineEdit->text().toDouble();
            arglist.append("de");
            valueInd++;
        }
        double length = ui->lengthLineEdit->text().toDouble();

        try
        {
            tes->calc(arglist.at(0).toAscii().data(),
                  arglist.at(1).toAscii().data(),
                  length,
                  value[0],
                  value[1]
             );
        }
        catch(...)
        {
            std::cout << "Error calculating grading" << std::endl;
        }
        //Set value to gui
        ui->nCellsSpinBox->setValue(tes->getn());
        QString str;
        str.sprintf("%.3g",tes->getr());
        ui->gradingLineEdit->setText(str);
        str.sprintf("%.3g",tes->getk());
        ui->c2cLineEdit->setText(str);
        str.sprintf("%.3g",tes->getds());
        ui->dsLineEdit->setText(str);
        str.sprintf("%.3g",tes->getde());
        ui->deLineEdit->setText(str);
    }
}

double GradingCalculatorDialog::getGrading()
{
    return ui->gradingLineEdit->text().toDouble();
}

long int GradingCalculatorDialog::getNCells()
{
    return ui->nCellsSpinBox->value();
}
