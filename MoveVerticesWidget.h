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

#ifndef MOVEVERTICESWIDGET_H
#define MOVEVERTICESWIDGET_H

#include <QWidget>

namespace Ui {
class MoveVerticesWidget;
}

class MoveVerticesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MoveVerticesWidget(QWidget *parent = 0);
    ~MoveVerticesWidget();

    double dist[3];
    bool delta,checkedX,checkedY,checkedZ;

public slots:
    void slotApply();
    void slotDone();

signals:
    void moveVertices();
    void moveDone();
    void setStatusText(QString);
private:
    Ui::MoveVerticesWidget *ui;



};

#endif // MOVEVERTICESWIDGET_H
