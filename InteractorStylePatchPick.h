#ifndef INTERACTORSTYLEPATCHPICK_H
#define INTERACTORSTYLEPATCHPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <QObject>

class vtkCollection;
class vtkIdList;
class HexBC;

class InteractorStylePatchPick : public QObject, public vtkInteractorStyleTrackballActor
{
    Q_OBJECT

    vtkTypeMacro(InteractorStylePatchPick,vtkInteractorStyleTrackballActor);
protected:
    InteractorStylePatchPick();
    ~InteractorStylePatchPick();
    InteractorStylePatchPick(InteractorStylePatchPick&);// Not implemented in order to comply with vtkObject.
    void operator=(const InteractorStylePatchPick&);// Not implemented in order to comply with vtkObject.

    vtkSmartPointer<vtkCollection> patches;

public:
    static InteractorStylePatchPick* New();
    void PrintSelf(ostream &os, vtkIndent indent);

    void SetPatches(vtkSmartPointer<vtkCollection> pts);
    virtual vtkIdType findClickedPatch(int x, int y);
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp(){}
    virtual void OnMiddleButtonDown(){}
    virtual void OnMiddleButtonUp();
    virtual void OnRightButtonDown(){}
    virtual void OnRightButtonUp();
    virtual void OnMouseMove(){}

    void OnChar(); //override from superclass

    vtkIdList *selectedPatches;

signals:
    void selectionDone(vtkIdList *selectedPatches);

private:

};

#endif // INTERACTORSTYLEPATCHPICK_H
