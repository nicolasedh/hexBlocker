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

#include "VerticeEditorWidget.h"
#include "ui_VerticeEditorWidget.h"
#include "HexBlocker.h"
#include "PointsTableModel.h"
#include <iostream>
#include <vtkPoints.h>

VerticeEditorWidget::VerticeEditorWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::VerticeEditorWidget)
{
    ui->setupUi(this);
    table = new PointsTableModel();
    ui->tableView->setModel(table);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSortingEnabled(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->verticalHeader()->hide();
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(table,SIGNAL(pointEdited()),this,SLOT(slotPointChanged()));

}

VerticeEditorWidget::~VerticeEditorWidget()
{
    delete ui;
}

void VerticeEditorWidget::setHexBlocker(HexBlocker *hexBlk)
{
    hexBlocker=hexBlk;
    vtkPoints *pnts = hexBlocker->vertices;
    table->setPoints(pnts);
    table->update();
    //update here
}

void VerticeEditorWidget::updateVertices()
{
    table->update();
}

void VerticeEditorWidget::slotPointChanged()
{
    hexBlocker->rescaleActors();
}
