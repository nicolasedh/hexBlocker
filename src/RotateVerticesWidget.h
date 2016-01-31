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

Description
    This is dockable widget that handles moving of vertices.
*/

#ifndef ROTATEVERTICESWIDGET_H
#define ROTATEVERTICESWIDGET_H

#include <QWidget>

class HexBlocker;

namespace Ui {
class RotateVerticesWidget;
}

class RotateVerticesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RotateVerticesWidget(QWidget *parent = 0);
    ~RotateVerticesWidget();

    double angle;
    double center[3];
    double axis[3];
    HexBlocker *hexBlocker;

public slots:
    void slotApply();
    void slotDone();
    void setAxisX();
    void setAxisY();
    void setAxisZ();

signals:
    void startSelect();
    void rotateVertices();
    void rotateDone();
    void setStatusText(QString);
private:
    Ui::RotateVerticesWidget *ui;

};

#endif // ROTATEVERTICESWIDGET_H
