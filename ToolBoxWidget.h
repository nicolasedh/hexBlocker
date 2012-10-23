#ifndef TOOLBOXWIDGET_H
#define TOOLBOXWIDGET_H

#include <QDockWidget>

namespace Ui {
class ToolBoxWidget;
}

class ToolBoxWidget : public QDockWidget
{
    Q_OBJECT
    
public:
    explicit ToolBoxWidget(QWidget *parent = 0);
    ~ToolBoxWidget();
    
private:
    Ui::ToolBoxWidget *ui;
};

#endif // TOOLBOXWIDGET_H
