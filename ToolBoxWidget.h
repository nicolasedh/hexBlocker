#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include <QDockWidget>

class CreateBlockWidget;
class MoveVerticesWidget;
class SetBCsWidget;
class QWidget;

namespace Ui {
class ToolBoxWidget;
}

class ToolBoxWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit ToolBoxWidget(QWidget *parent = 0);
    ~ToolBoxWidget();
    QWidget *emptyPage;

    //Tools views
    CreateBlockWidget *createBlockW;
    MoveVerticesWidget *moveVerticesW;
    SetBCsWidget    *setBCsW;

    void setCurrentIndex(int);

public slots:
    virtual void slotCancel();

signals:
    void cancel();

private:
    Ui::ToolBoxWidget *ui;
};

#endif // TOOLBOXWIDGET_H
