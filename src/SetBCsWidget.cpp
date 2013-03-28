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

#include "SetBCsWidget.h"
#include "ui_SetBCsWidget.h"
#include <QStringList>
#include <QTreeWidget>
#include "SetBCsItem.h"
#include "HexBC.h"
#include "HexReader.h"
#include <vtkCollection.h>
#include <vtkSmartPointer.h>
#include <vtkIdList.h>
#include "HexBlocker.h"
#include <QTreeWidgetItem>
#include <QList>

SetBCsWidget::SetBCsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetBCsWidget)
{
    ui->setupUi(this);
    QStringList headers;
    headers << tr("Name") << tr("Type");
    ui->treeWidget->setHeaderLabels(headers);

    connect(ui->pushButtonNew,SIGNAL(clicked()),this,SLOT(slotCreateBC()));
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),
            this,SLOT(slotBCchanged(QTreeWidgetItem *,int)));
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this,SLOT(slotShowPatchesOnSelection(QTreeWidgetItem*,int)));
    connect(ui->pushButtonSelect,SIGNAL(clicked()),this,SLOT(slotSelectPatches()));
    connect(ui->pushButtonDone,SIGNAL(clicked()),this,SIGNAL(done()));
    connect(ui->pushButtonDel,SIGNAL(clicked()),this,SLOT(slotDeleteBC()));
}

SetBCsWidget::~SetBCsWidget()
{
    delete ui;
}

void SetBCsWidget::slotCreateBC()
{
    SetBCsItem *bc = new SetBCsItem(ui->treeWidget);
    bc->hexBC = vtkSmartPointer<HexBC>::New();

    bc->setFlags(bc->flags() | Qt::ItemIsEditable);
    bc->setText(0,tr("name"));
    bc->setText(1,tr("patch"));
    bc->hexBC->globalPatches=hexBlocker->patches;
    hexBlocker->hexBCs->AddItem(bc->hexBC);
}

void SetBCsWidget::slotBCchanged(QTreeWidgetItem *item, int col)
{
    SetBCsItem *bc = static_cast<SetBCsItem*>(item);
    bc->hexBC->name=bc->text(0).toStdString();
    bc->hexBC->type=bc->text(1).toStdString();
}

void SetBCsWidget::slotSelectPatches()
{
    //If non selected
    QList<QTreeWidgetItem*> selectedTreeItem = ui->treeWidget->selectedItems();
    if(selectedTreeItem.count()>0)
    {
        SetBCsItem *bcItem = static_cast<SetBCsItem*>(selectedTreeItem[0]);
        vtkIdType hexBCId = hexBlocker->hexBCs->IsItemPresent(bcItem->hexBC)-1;
        HexBC *hexBC = HexBC::SafeDownCast(hexBlocker->hexBCs->GetItemAsObject(hexBCId));
        setStatusText("Select patches with left mouse button, deselect with right, middle when done.");
        emit startSelectPatches(hexBC->patchIds);
    }
    else
    {
        setStatusText("Select a boundary condition in the list first");
    }
}

void SetBCsWidget::slotSelectionDone(vtkIdList *selectedPatches)
{
    QList<QTreeWidgetItem*> selectedTreeItem = ui->treeWidget->selectedItems();
    SetBCsItem *bcItem = static_cast<SetBCsItem*>(selectedTreeItem[0]);
    vtkIdType hexBCId = hexBlocker->hexBCs->IsItemPresent(bcItem->hexBC)-1;
    HexBC *hexBC = HexBC::SafeDownCast(hexBlocker->hexBCs->GetItemAsObject(hexBCId));
    hexBC->patchIds->DeepCopy(selectedPatches);
//    std::cout <<" patchIds: " << hexBC->patchIds->GetNumberOfIds()<<std::endl;

    emit resetInteractor();
}

void SetBCsWidget::slotShowPatchesOnSelection(QTreeWidgetItem *item, int col)
{
    //Reset all colors first!
    emit resetInteractor();
    SetBCsItem *bcitem = static_cast<SetBCsItem*>(item);
    bcitem->hexBC->setPatchColors(0.0,1.0,0.0);
    emit render();

}

void SetBCsWidget::changeBCs(HexReader * reader)
{


    ui->treeWidget->clear();
    hexBlocker->hexBCs = reader->readBCs;
    hexBlocker->patches= reader->readPatches;
    for(vtkIdType i=0;i<hexBlocker->hexBCs->GetNumberOfItems();i++)
    {
        vtkSmartPointer<HexBC> bc = HexBC::SafeDownCast(hexBlocker->hexBCs->GetItemAsObject(i));
        SetBCsItem *bcitem = new SetBCsItem(ui->treeWidget);
        bcitem->hexBC = bc;

        QString name = QString::fromUtf8(bc->name.c_str());
        QString type = QString::fromUtf8(bc->type.c_str());

        bcitem->setFlags(bcitem->flags() | Qt::ItemIsEditable);
        bcitem->setText(0,name.simplified());
        bcitem->setText(1,type.simplified());
        bcitem->hexBC->globalPatches = hexBlocker->patches;
        ui->treeWidget->addTopLevelItem(bcitem);

    }
}

void SetBCsWidget::clearBCs()
{

    ui->treeWidget->clear();
}

void SetBCsWidget::slotDeleteBC()
{
    QList<QTreeWidgetItem*> selectedTreeItem = ui->treeWidget->selectedItems();
    if(selectedTreeItem.count() < 1)
    {
        emit setStatusText(QString("No BC selected"));
        return;
    }
    SetBCsItem *bcItem = static_cast<SetBCsItem*>(selectedTreeItem[0]);
    int i = ui->treeWidget->indexOfTopLevelItem(bcItem);
    ui->treeWidget->takeTopLevelItem(i);


    vtkIdType hexBCId = hexBlocker->hexBCs->IsItemPresent(bcItem->hexBC)-1;
    if(hexBCId < 0)
    {
        return;
    }
    HexBC *hexBC = HexBC::SafeDownCast(hexBlocker->hexBCs->GetItemAsObject(hexBCId));

    hexBC->patchIds->Delete();
    hexBlocker->hexBCs->RemoveItem(hexBC);

}
