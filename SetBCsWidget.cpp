#include "SetBCsWidget.h"
#include "ui_SetBCsWidget.h"
#include <QStringList>
#include <QTreeWidget>
#include "SetBCsItem.h"
#include "HexBC.h"

#include <vtkCollection.h>
#include <vtkSmartPointer.h>
#include <QTreeWidgetItem>

SetBCsWidget::SetBCsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetBCsWidget)
{
    ui->setupUi(this);
    QStringList headers;
    headers << tr("Name") << tr("Type");
    ui->treeWidget->setHeaderLabels(headers);

    connect(ui->pushButtonNew,SIGNAL(clicked()),this,SLOT(slotCreateBC()));
    connect(ui->treeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(slotBCchanged(QTreeWidgetItem *,int)));
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

    hexBCs->AddItem(bc->hexBC);

}

void SetBCsWidget::slotBCchanged(QTreeWidgetItem *item, int col)
{
    SetBCsItem *bc = static_cast<SetBCsItem*>(item);
    bc->hexBC->name=bc->text(0).toStdString();
    bc->hexBC->type=bc->text(1).toStdString();

}
