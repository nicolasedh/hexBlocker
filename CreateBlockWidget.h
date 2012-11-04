#ifndef CREATEBLOCKWIDGET_H
#define CREATEBLOCKWIDGET_H

#include <QWidget>

namespace Ui {
class CreateBlockWidget;
}

class CreateBlockWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CreateBlockWidget(QWidget *parent = 0);
    ~CreateBlockWidget();
    
    double c0[3];
    double c1[3];
public slots:
    virtual void slotApply();
    virtual void slotCancel();

signals:
    void apply();
    void cancel();
    void setStatusText(QString);
private:
    Ui::CreateBlockWidget *ui;
};

#endif // CREATEBLOCKWIDGET_H
