#ifndef SETBCSWIDGET_H
#define SETBCSWIDGET_H

#include <QWidget>

namespace Ui {
class SetBCsWidget;
}

class SetBCsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetBCsWidget(QWidget *parent = 0);
    ~SetBCsWidget();

public slots:
    void slotCreateBC();
    
private:
    Ui::SetBCsWidget *ui;
};

#endif // SETBCSWIDGET_H
