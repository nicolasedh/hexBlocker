#ifndef HEXEDGE_H
#define HEXEDGE_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <QTextStream>

class vtkIdList;
class vtkPoints;
class vtkPolyDataMapper;
class vtkActor;
class vtkLine;
class vtkCellArray;
class vtkPolyData;


class HexEdge : public vtkObject
{
    vtkTypeMacro(HexEdge,vtkObject)

protected:
    HexEdge();
    // Remember to implement
    ~HexEdge();
    HexEdge(const HexEdge&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexEdge&);  // Not implemented in order to comply with vtkObject.

public:
    //FUNCTIONS
    static HexEdge *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkIdType p0,vtkIdType p1, vtkSmartPointer<vtkPoints> verts);

    bool equals(const HexEdge * other);


    void setColor(double,double,double);
    void resetColor();
    void exportVertIds(QTextStream &os);


    //DATA
    vtkSmartPointer<vtkIdList> vertIds;
    vtkSmartPointer<vtkPoints> globalVertices;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;

private:
    vtkSmartPointer<vtkLine> line;
    vtkSmartPointer<vtkCellArray> lines;
};

#endif // EDGE_H
