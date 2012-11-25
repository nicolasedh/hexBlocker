#include "CreateBlockWidget.h"
#include "ui_CreateBlockWidget.h"
#include <QtGui>
#include <QValidator>
#include <iostream>

CreateBlockWidget::CreateBlockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateBlockWidget)
{
    ui->setupUi(this);
    QDoubleValidator *valid = new QDoubleValidator(this);
    ui->lineEditC0X->setValidator(valid);
    ui->lineEditC0X->setText("0.0");
    ui->lineEditC0Y->setValidator(valid);
    ui->lineEditC0Y->setText("0.0");
    ui->lineEditC0Z->setValidator(valid);
    ui->lineEditC0Z->setText("0.0");
    ui->lineEditC1X->setValidator(valid);
    ui->lineEditC1X->setText("1.0");
    ui->lineEditC1Y->setValidator(valid);
    ui->lineEditC1Y->setText("1.0");
    ui->lineEditC1Z->setValidator(valid);
    ui->lineEditC1Z->setText("1.0");

    c0[0]=c0[1]=c0[2]=0.0;
    c1[0]=c1[1]=c1[2]=1.0;

    connect(ui->pushButtonApply,SIGNAL(clicked()),this,SLOT(slotApply()));
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(slotCancel()));
}

CreateBlockWidget::~CreateBlockWidget()
{
    delete ui;
}

void CreateBlockWidget::slotApply()
{
    QString content = ui->lineEditC0X->text();
    c0[0]=content.toDouble();
    content = ui->lineEditC0Y->text();
    c0[1]=content.toDouble();
    content = ui->lineEditC0Z->text();
    c0[2]=content.toDouble();

    content = ui->lineEditC1X->text();
    c1[0]=content.toDouble();
    content = ui->lineEditC1Y->text();
    c1[1]=content.toDouble();
    content = ui->lineEditC1Z->text();
    c1[2]=content.toDouble();
/*
    std::cout << "creating block at c0=("<<c0[0]<<"," <<c0[1]<<","<<c0[2]<<"), ("
              << c1[0]<<"," <<c1[1]<<","<<c1[2]<<")"<<std::endl;
              */
    emit apply();
}

void CreateBlockWidget::slotCancel()
{
    emit cancel();
}
