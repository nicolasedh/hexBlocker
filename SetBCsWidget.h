#ifndef SETBCSWIDGET_H
#define SETBCSWIDGET_H

#include <QWidget>
#include <vtkSmartPointer.h>

class vtkCollection;
class QTreeWidgetItem;
class vtkIdList;
class HexReader;

namespace Ui {
class SetBCsWidget;
}

class SetBCsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SetBCsWidget(QWidget *parent = 0);
    ~SetBCsWidget();


    void changeBCs(HexReader * reader);

    vtkSmartPointer<vtkCollection> hexBCs;
    vtkSmartPointer<vtkCollection> allPatches;

public slots:
    void slotCreateBC();
    void slotBCchanged(QTreeWidgetItem *,int);
    void slotSelectPatches();
    void slotSelectionDone(vtkIdList *selectedPatches);
    void slotShowPatchesOnSelection(QTreeWidgetItem* item,int col);
    
signals:
    void startSelectPatches(vtkIdType bcId);
    void resetInteractor();
    void render();
    void done();
    //below dont work
    void setStatusText(QString);

private:
    Ui::SetBCsWidget *ui;
};

#endif // SETBCSWIDGET_H
