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
    This is a holder dock widget. Each page holds a tool widget. Some
    signals are passed through this class from tool widgets to MainWindow.
    The HexBlocker object must be set before usage.
*/

#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include <QDockWidget>

class CreateBlockWidget;
class MoveVerticesWidget;
class SetBCsWidget;
class QWidget;
class HexBlocker;

namespace Ui {
class ToolBoxWidget;
}

class ToolBoxWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit ToolBoxWidget(QWidget *parent = 0);
    ~ToolBoxWidget();
    QWidget *emptyPage;

    //Tools views
    CreateBlockWidget *createBlockW;
    MoveVerticesWidget *moveVerticesW;
    SetBCsWidget    *setBCsW;



    void setCurrentIndex(int);
    void setHexBlockerPointer(HexBlocker *hexBker);
public slots:
    void slotCancel();
    void slotSetStatusText(QString text);

signals:
    void cancel();
    void setStatusText(QString);

private:
    //Pointer to the hexBlocker object
    HexBlocker *hexBlocker;
    Ui::ToolBoxWidget *ui;
};

#endif // TOOLBOXWIDGET_H
