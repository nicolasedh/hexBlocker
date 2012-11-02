#ifndef SETBCSWIDGET_H
#define SETBCSWIDGET_H

#include <QWidget>
#include <vtkSmartPointer.h>

class vtkCollection;
class QTreeWidgetItem;
class vtkIdList;


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
    void slotSelectionDone(vtkIdList *selectedPatches);
    
signals:
    void startSelectPatches(vtkIdType bcId);
    void resetInteractor();

private:
    Ui::SetBCsWidget *ui;
};

#endif // SETBCSWIDGET_H
