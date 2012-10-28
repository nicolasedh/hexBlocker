#include "MoveVerticesWidget.h"
#include "ui_MoveVerticesWidget.h"
#include "InteractorStyleVertPick.h"
#include <iostream>
#include <QtGui>
#include <QValidator>
#include <MainWindow.h>


MoveVerticesWidget::MoveVerticesWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::MoveVerticesWidget)
{
    ui->setupUi(this);
    QDoubleValidator *valid = new QDoubleValidator(this);
    ui->lineEditX->setValidator(valid);
    ui->lineEditY->setValidator(valid);
    ui->lineEditZ->setValidator(valid);
    dist[0]=0.0;
    dist[1]=0.0;
    dist[2]=0.0;
    //connections
    connect(ui->pushButtonApply, SIGNAL(clicked()), this, SLOT(slotApply()));
    connect(ui->pushButtonCancel, SIGNAL(clicked()),this,SLOT(slotDone()));
}

MoveVerticesWidget::~MoveVerticesWidget()
{
    delete ui;
}

void MoveVerticesWidget::slotApply()
{
    QString X = ui->lineEditX->text();
    QString Y = ui->lineEditY->text();
    QString Z = ui->lineEditZ->text();
    delta=ui->checkBoxDelta->isChecked();

    if(delta)
    {
        checkedX=true;
        checkedY=true;
        checkedZ=true;
    }
    else
    {
        checkedX=ui->checkBoxX;
        checkedY=ui->checkBoxY;
        checkedZ=ui->checkBoxZ;
    }
    dist[0] = X.toDouble();
    dist[1] = Y.toDouble();
    dist[2] = Z.toDouble();


    emit moveVertices();

    //MainWindow *mypar = static_cast<MainWindow*>(this->parent());
    //mypar->style->;
    //this->close();
}

void MoveVerticesWidget::slotDone()
{
    emit moveDone();
    this->close();
}
