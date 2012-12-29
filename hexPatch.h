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
class HexBlock;

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
    //FUNCTIONS
    static hexPatch *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkSmartPointer<vtkIdList> vIds,vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<HexBlock> hex);

    bool equals(vtkSmartPointer<hexPatch> other);
    bool equals(vtkSmartPointer<vtkIdList> otherIds);
    void orderVertices(); //not yet implemented,

    void setColor(double,double,double);
    void resetColor();
    void exportVertIds(QTextStream &os);
    void getNormal(double n[3]);
    void setHex(vtkSmartPointer<HexBlock> hex);
    vtkSmartPointer<HexBlock> getPrimaryHexBlock();
    vtkSmartPointer<HexBlock> getSecondaryHexBlock();

    //DATA
    vtkSmartPointer<vtkIdList> vertIds;
    vtkSmartPointer<vtkPoints> globalVertices;
    //representations
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;


private:
    vtkSmartPointer<vtkQuad> quad;
    vtkSmartPointer<vtkCellArray> quads;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<HexBlock> primaryHex;
    vtkSmartPointer<HexBlock> secondaryHex;
    bool hasPrimaryHex;
    bool hasSecondaryHex;
};

#endif // PATCH_H
