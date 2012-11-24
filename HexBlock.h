#ifndef HEXBLOCK_H
#define HEXBLOCK_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>

//Pre declarations
class vtkIdList;
class vtkPoints;
class vtkPolyData;
class vtkQuads;
class hexPatch;
class vtkCollection;

class vtkMapper;
class vtkActor;




class HexBlock : public vtkObject
{
    vtkTypeMacro(HexBlock,vtkObject)

    protected:
        HexBlock();
    // Remember to implement
    ~HexBlock();
    HexBlock(const HexBlock&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexBlock&);  // Not implemented in order to comply with vtkObject.

public:
    //FUNCTIONS
    // inorder to comply with vtkObect
    static HexBlock *New();
    void PrintSelf(ostream &os, vtkIndent indent);

    // Construct from two corner vertices
    void init(double corner0[3], double corner1[3], vtkSmartPointer<vtkPoints> verts);

    // Construct from a list of vertices and the global list of vertices
    void init(vtkSmartPointer<hexPatch> p, double dist, vtkSmartPointer<vtkPoints> verts);

    vtkIdType getPatchInternalId(vtkSmartPointer<hexPatch> otherP);

    //DATA
    vtkSmartPointer<vtkCollection> patches;
    vtkSmartPointer<vtkPoints> globalVertices; //Global list of vertices
    vtkSmartPointer<vtkPolyData> hexData;
    vtkSmartPointer<vtkIdList> vertIds; // own list of Ids
    vtkSmartPointer<vtkActor> hexActor;

private:
    //FUNCTIONS
    void initPatch();

    //DATA
    vtkSmartPointer<vtkMapper> hexMapper;
    vtkSmartPointer<vtkPoints> hexVertices;


};

#endif // HEXBLOCK_H
