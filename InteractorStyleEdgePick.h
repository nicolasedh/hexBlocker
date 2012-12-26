#ifndef INTERACTORSTYLEEDGEPICK_H
#define INTERACTORSTYLEEDGEPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <QObject>

class vtkCollection;
//class vtkIdList;
class HexBC;

class InteractorStyleEdgePick : public QObject, public vtkInteractorStyleTrackballActor
{
    Q_OBJECT

    vtkTypeMacro(InteractorStyleEdgePick,vtkInteractorStyleTrackballActor);
protected:
    InteractorStyleEdgePick();
    ~InteractorStyleEdgePick();
    InteractorStyleEdgePick(InteractorStyleEdgePick&);// Not implemented in order to comply with vtkObject.
    void operator=(const InteractorStyleEdgePick&);// Not implemented in order to comply with vtkObject.


public:
    static InteractorStyleEdgePick* New();
    void PrintSelf(ostream &os, vtkIndent indent);

    void SetEdges(vtkSmartPointer<vtkCollection> edgs);
    virtual vtkIdType findClickedEdge(int x, int y);
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp(){}
    virtual void OnMiddleButtonDown(){}
    virtual void OnMiddleButtonUp();
    virtual void OnRightButtonDown(){}
    virtual void OnRightButtonUp();
    virtual void OnMouseMove(){}

    void OnChar(); //override from superclass

    vtkSmartPointer<vtkCollection> globalEdges;

signals:
    void selectionDone(vtkIdType selectedEdge);

private:
};

#endif // INTERACTORSTYLEEDGEPICK_H
