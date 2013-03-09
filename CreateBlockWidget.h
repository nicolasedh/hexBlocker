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
#ifndef CREATEBLOCKWIDGET_H
#define CREATEBLOCKWIDGET_H

#include <QWidget>

namespace Ui {
class CreateBlockWidget;
}

class CreateBlockWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CreateBlockWidget(QWidget *parent = 0);
    ~CreateBlockWidget();
    
    double c0[3];
    double c1[3];
public slots:
    void slotApply();
    void slotCancel();

signals:
    void apply();
    void cancel();
    void setStatusText(QString);
private:
    Ui::CreateBlockWidget *ui;
};

#endif // CREATEBLOCKWIDGET_H
