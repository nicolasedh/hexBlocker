#ifndef HEXBLOCKER_H
#define HEXBLOCKER_H
/* This is the class that is in charge of storing all
   hexblocks, vertices, edges and faces

   */
#include <vtkSmartPointer.h>


//Predeclarations
class HexBlock;
class vtkPoints;
class vtkPolyData;
class vtkGlyph3D;
class vtkSphereSource;
class vtkPolyDataMapper;
class vtkActor;
class vtkRenderer;
class vtkCollection;
class vtkCellArray;
class vtkQuad;
class vtkIdList;


class HexBlocker
{
public:
    HexBlocker();
    ~HexBlocker();
    void createHexBlock();
    void createHexBlock(double [3],double [3]);
    void extrudePatch();
    void resetBounds();
    void PrintHexBlocks();
    void initPatches(vtkSmartPointer<HexBlock> hex);
    void initPatch(vtkSmartPointer<HexBlock> hex, int ids[4]);
    void moveVertices(vtkSmartPointer<vtkIdList> ids, double dist[]);

    vtkSmartPointer<vtkPoints> vertices;
    vtkSmartPointer<vtkPolyData> vertData;

    vtkSmartPointer<vtkCollection> patches;

    vtkSmartPointer<vtkCollection> hexBlocks;

    vtkSmartPointer<vtkCollection> hexBCs;

    //Representations
    vtkSmartPointer<vtkSphereSource> vertSphere;
    vtkSmartPointer<vtkGlyph3D> vertGlyph;
    vtkSmartPointer<vtkPolyDataMapper> vertMapper;
    vtkSmartPointer<vtkActor> vertActor;

    vtkSmartPointer<vtkRenderer> renderer;




};

#endif // HEXBLOCKER_H
