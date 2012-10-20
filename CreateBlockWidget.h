#ifndef CREATEBLOCKWIDGET_H
#define CREATEBLOCKWIDGET_H

#include <QDockWidget>

namespace Ui {
class CreateBlockWidget;
}

class CreateBlockWidget : public QDockWidget
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
private:
    Ui::CreateBlockWidget *ui;
};

#endif // CREATEBLOCKWIDGET_H
