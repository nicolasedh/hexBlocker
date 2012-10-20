#ifndef INTERACTORSTYLEPATCHPICK_H
#define INTERACTORSTYLEPATCHPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballActor.h>

class vtkCollection;


class InteractorStylePatchPick : public vtkInteractorStyleTrackballActor
{
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
    virtual void OnLeftButtonDown();
    void OnChar(); //override from superclass

private:





};

#endif // INTERACTORSTYLEPATCHPICK_H
