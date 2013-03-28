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
    This a class that provides an GUI for  Markus Hartinger's
    mesh space calculator. The calculator is available at
    http://www.cfd-online.com/Forums/openfoam-meshing-blockmesh/61785-blockmesh-grading-calculator.html

*/
#ifndef GRADINGCALCULATORDIALOG_H
#define GRADINGCALCULATORDIALOG_H

#include <QDialog>

//pre declarations
class TEdgeSpace;
class QDoubleValidator;

namespace Ui {
class GradingCalculatorDialog;
}

class GradingCalculatorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit GradingCalculatorDialog(QWidget *parent = 0);
    ~GradingCalculatorDialog();

    void setLengthGradingNcells(double length,double grading,long int ncells);
    long int getNCells();
    double getGrading();
private:
    Ui::GradingCalculatorDialog *ui;
    TEdgeSpace *tes; //edge space calculator by Markus
    int numCheckedBoxes;
    QDoubleValidator *dsdeDoubleValidator;
private slots:
    void slotCheckboxesChanged();
    void slotCalculate();



signals:
    void apply();
};

#endif // GRADINGCALCULATORDIALOG_H
