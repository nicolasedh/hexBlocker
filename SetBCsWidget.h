#ifndef SETBCSWIDGET_H
#define SETBCSWIDGET_H

#include <QWidget>
#include <vtkSmartPointer.h>

class vtkCollection;
class QTreeWidgetItem;


namespace Ui {
class SetBCsWidget;
}

class SetBCsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetBCsWidget(QWidget *parent = 0);
    ~SetBCsWidget();

    vtkSmartPointer<vtkCollection> hexBCs;


public slots:
    void slotCreateBC();
    void slotBCchanged(QTreeWidgetItem *,int);
    void slotSelectPatches();
    
signals:
    void startSelectPatches(vtkIdType bcId);
    void resetInteractor();

private:
    Ui::SetBCsWidget *ui;
};

#endif // SETBCSWIDGET_H
