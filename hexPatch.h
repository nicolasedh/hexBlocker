#ifndef HEXPATCH_H
#define HEXPATCH_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <QTextStream>

class vtkIdList;
class vtkPoints;
class vtkPolyDataMapper;
class vtkActor;
class vtkQuad;
class vtkCellArray;
class vtkPolyData;

class hexPatch : public vtkObject
{
    vtkTypeMacro(hexPatch,vtkObject)

protected:
    hexPatch();
    // Remember to implement
    ~hexPatch();
    hexPatch(const hexPatch&);  // Not implemented in order to comply with vtkObject.
    void operator=(const hexPatch&);  // Not implemented in order to comply with vtkObject.

public:
    static hexPatch *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkSmartPointer<vtkIdList> vIds,vtkSmartPointer<vtkPoints> verts);

    bool operator ==(vtkSmartPointer<hexPatch> other);
    void orderVertices(); //not yet implemented,
    vtkSmartPointer<vtkIdList> vertIds;
    vtkSmartPointer<vtkPoints> globalVertices;

    vtkIdType primaryHexId;
    vtkIdType secondaryHexId;

    //representation

    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

    void setColor(double,double,double);
    void resetColor();
    void exportVertIds(QTextStream &os);

private:
    vtkSmartPointer<vtkQuad> quad;
    vtkSmartPointer<vtkCellArray> quads;
    vtkSmartPointer<vtkPolyData> data;

};

#endif // PATCH_H
