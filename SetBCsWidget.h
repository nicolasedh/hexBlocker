#ifndef MOVEVERTICESWIDGET_H
#define MOVEVERTICESWIDGET_H

#include <QDockWidget>

namespace Ui {
class MoveVerticesWidget;
}

class MoveVerticesWidget : public QDockWidget
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
private:
    Ui::MoveVerticesWidget *ui;



};

#endif // MOVEVERTICESWIDGET_H
