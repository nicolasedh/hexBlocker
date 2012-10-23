#include "ToolBoxWidget.h"
#include "ui_ToolBoxWidget.h"

ToolBoxWidget::ToolBoxWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ToolBoxWidget)
{
    ui->setupUi(this);
    this->ui->stackedWidget->setCurrentIndex(0);
}

ToolBoxWidget::~ToolBoxWidget()
{
    delete ui;
}
