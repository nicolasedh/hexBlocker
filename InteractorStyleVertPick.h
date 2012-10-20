#ifndef INTERACTORVERTPICK_H
#define INTERACTORVERTPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleRubberBandPick.h>

class vtkPolyData;
class vtkGlyph3D;
class vtkSphereSource;
class vtkDataSetMapper;
class vtkActor;
class vtkIdList;

class InteractorStyleVertPick : public vtkInteractorStyleRubberBandPick
{
    vtkTypeMacro(InteractorStyleVertPick,vtkInteractorStyleRubberBandPick);
protected:
    InteractorStyleVertPick();
    ~InteractorStyleVertPick();
    InteractorStyleVertPick(InteractorStyleVertPick&);// Not implemented in order to comply with vtkObject.
    void operator=(const InteractorStyleVertPick&);// Not implemented in order to comply with vtkObject.

public:
    static InteractorStyleVertPick* New();
    void PrintSelf(ostream &os, vtkIndent indent);
    virtual void OnLeftButtonUp();
    virtual void OnMouseMove();
    virtual void OnLeftButtonDown();
    void SetPoints(vtkSmartPointer<vtkPolyData> points) {this->Points = points;}
    void clearSelection();
    void OnChar(); //override from superclass
    vtkSmartPointer<vtkSphereSource> SelectedSphere;
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkIdList> SelectedList;
    vtkSmartPointer<vtkPolyData> Points;

    vtkSmartPointer<vtkGlyph3D> SelectedGlyph;
private:



    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;


};

#endif // INTERACTORVERTPICK_H
