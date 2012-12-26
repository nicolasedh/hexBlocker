#ifndef HEXBLOCK_H
#define HEXBLOCK_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>

//Pre declarations
class hexPatch;
class HexEdge;
class vtkIdList;
class vtkPoints;
class vtkPolyData;
class vtkQuads;
class vtkCollection;
class vtkLine;
class vtkMapper;
class vtkActor;
class vtkTubeFilter;




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
    void init(double corner0[3], double corner1[3],
              vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<vtkCollection> edges);

    // Construct from a list of vertices and the global list of vertices
    void init(vtkSmartPointer<hexPatch> p, double dist, vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<vtkCollection> edges);

    vtkIdType getPatchInternalId(vtkSmartPointer<hexPatch> otherP);
    vtkSmartPointer<vtkIdList> getParallelEdges(vtkIdType edgeId);
    void getNumberOfCells(int nCells[3]);

    void setAxesRadius(double rad);

    //DATA
    vtkSmartPointer<vtkCollection> globalPatches;
    vtkSmartPointer<vtkPoints> globalVertices; //Global list of vertices
    vtkSmartPointer<vtkCollection> globalEdges;
    vtkSmartPointer<vtkPolyData> hexData;
    vtkSmartPointer<vtkIdList> vertIds; // own list of Ids
    vtkSmartPointer<vtkActor> hexActor; //for axes.
    vtkSmartPointer<vtkIdList> edgeIds;
private:
    //FUNCTIONS
    void drawLocalaxes();
    void initEdges();
    void initEdge(vtkIdType p0,vtkIdType p1);

    //DATA
    vtkSmartPointer<vtkMapper> hexMapper;
    vtkSmartPointer<vtkPoints> hexVertices;
    vtkSmartPointer<vtkTubeFilter> axesTubes;

};

#endif // HEXBLOCK_H
