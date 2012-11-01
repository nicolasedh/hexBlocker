#include "SetBCsWidget.h"
#include "ui_SetBCsWidget.h"
#include <QStringList>
#include <QTreeWidget>
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
}

SetBCsWidget::~SetBCsWidget()
{
    delete ui;
}

void SetBCsWidget::slotCreateBC()
{
    QTreeWidgetItem *bc = new QTreeWidgetItem(ui->treeWidget);
    bc->setText(0,tr("name"));
    bc->setText(1,tr("patch"));
    bc->setFlags(bc->flags() | Qt::ItemIsEditable);
}
