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
class vtkCellArray;


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
    static HexBlock *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // Construct from two corner vertices
    void init(double corner0[3], double corner1[3], vtkSmartPointer<vtkPoints> verts);
    //Construct from a list of vertices and the list of vertices data
    void init(vtkSmartPointer<vtkIdList> vIds, double dist[3], vtkSmartPointer<vtkPoints> verts);

    //implementera void Update eller modified()?

    vtkSmartPointer<vtkPoints> globalVertices; //Global list of vertices
    vtkSmartPointer<vtkPolyData> hexData;
    vtkSmartPointer<vtkCellArray> patches;
    vtkSmartPointer<vtkIdList> vertIds; // own list of Ids
    vtkSmartPointer<vtkActor> hexActor;

private:
    // Creates the hex representation
    void initPatches();
    vtkSmartPointer<vtkMapper> hexMapper;

    vtkSmartPointer<vtkPoints> hexVertices;


};

#endif // HEXBLOCK_H
