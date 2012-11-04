#ifndef MOVEVERTICESWIDGET_H
#define MOVEVERTICESWIDGET_H

#include <QWidget>

namespace Ui {
class MoveVerticesWidget;
}

class MoveVerticesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MoveVerticesWidget(QWidget *parent = 0);
    ~MoveVerticesWidget();

    double dist[3];
    bool delta,checkedX,checkedY,checkedZ;

public slots:
    virtual void slotApply();
    virtual void slotDone();

signals:
    void moveVertices();
    void moveDone();
    void setStatusText(QString);
private:
    Ui::MoveVerticesWidget *ui;



};

#endif // MOVEVERTICESWIDGET_H
