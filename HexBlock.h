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
              vtkSmartPointer<vtkPoints> verts,
              vtkSmartPointer<vtkCollection> edges,
              vtkSmartPointer<vtkCollection> patches);

    // Extrude from a patch, distance and global list of vertices, edges and patches
    void init(vtkSmartPointer<hexPatch> p, double dist,
              vtkSmartPointer<vtkPoints> verts,
              vtkSmartPointer<vtkCollection> edges,
              vtkSmartPointer<vtkCollection> patches);

    //Construct from a list of vertices that exists in globalVertices,
    // edges and patches are added if not already existing
    void init(vtkSmartPointer<vtkIdList> myVertIds,
              vtkSmartPointer<vtkPoints> verts,
              vtkSmartPointer<vtkCollection> edges,
              vtkSmartPointer<vtkCollection> patches);

    vtkIdType getPatchInternalId(vtkSmartPointer<hexPatch> otherP);
    vtkSmartPointer<vtkIdList> getParallelEdges(vtkIdType edgeId);
    void getNumberOfCells(int nCells[3]);

    void setAxesRadius(double rad);

    //DATA
    vtkSmartPointer<vtkPoints> globalVertices; //Global list of vertices
    vtkSmartPointer<vtkCollection> globalEdges;
    vtkSmartPointer<vtkCollection> globalPatches;
    vtkSmartPointer<vtkPolyData> hexData;
    vtkSmartPointer<vtkIdList> vertIds; // own vertices in globalVertices
    vtkSmartPointer<vtkIdList> edgeIds;
    vtkSmartPointer<vtkIdList> patchIds; //own patches in globalPatches
    vtkSmartPointer<vtkActor> hexActor; //for axes.

private:
    //FUNCTIONS
    //
    void initAll();
    void drawLocalaxes();
    void initEdges();
    void initEdge(vtkIdType p0,vtkIdType p1);
    void initPatches();
    void initPatch(int ids[4]);
    vtkIdType patchIdInGlobalList(vtkSmartPointer<hexPatch> p);

    //DATA
    vtkSmartPointer<vtkMapper> hexMapper;
    vtkSmartPointer<vtkPoints> hexVertices;
    vtkSmartPointer<vtkTubeFilter> axesTubes;

};

#endif // HEXBLOCK_H
