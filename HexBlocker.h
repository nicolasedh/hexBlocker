#ifndef HEXBLOCKER_H
#define HEXBLOCKER_H
/* This is the class that is in charge of storing all
   hexblocks, vertices, edges and faces

   */
#include <vtkSmartPointer.h>
#include <QTextStream>

//Predeclarations
class HexBlock;
class hexPatch;
class HexEdge;
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
class vtkLabeledDataMapper;
class vtkActor2D;


class HexBlocker
{
public:
    HexBlocker();
    ~HexBlocker();

    //FUNCTIONS
    void createHexBlock();
    void createHexBlock(double [3],double [3]);
    void extrudePatch(vtkIdList *selectedPatches, double dist);
    void resetBounds();
    void PrintHexBlocks();
    void initPatches(vtkSmartPointer<HexBlock> hex);
    void initPatch(vtkSmartPointer<HexBlock> hex, int ids[4]);
    void moveVertices(vtkSmartPointer<vtkIdList> ids, double dist[]);
    vtkIdType isPatchInGlobalList(vtkSmartPointer<hexPatch> p);

    void resetPatchesColor();
    void exportVertices(QTextStream &os);
    void exportBlocks(QTextStream &os);
    void exportBCs(QTextStream &os);
    int showParallelEdges(vtkIdType edgeId); // shows all parallel edges
    void setNumberOnParallelEdges(vtkIdType edgeId,int nCells);

    //DATA
    vtkSmartPointer<vtkPoints> vertices;
    vtkSmartPointer<vtkPolyData> vertData;
    vtkSmartPointer<vtkCollection> patches; //global patch list
    vtkSmartPointer<vtkCollection> hexBlocks;
    vtkSmartPointer<vtkCollection> hexBCs; //BCs
    vtkSmartPointer<vtkCollection> edges; //global list of edges


    //Representations
    vtkSmartPointer<vtkSphereSource> vertSphere;
    vtkSmartPointer<vtkGlyph3D> vertGlyph;
    vtkSmartPointer<vtkPolyDataMapper> vertMapper;
    vtkSmartPointer<vtkActor> vertActor;

    vtkSmartPointer<vtkLabeledDataMapper> vertLabelMapper;
    vtkSmartPointer<vtkActor2D> vertLabelActor;
    vtkSmartPointer<vtkRenderer> renderer;

private:

    //Functions
    void addParallelEdges(vtkSmartPointer<vtkIdList> allParallelEdges,vtkIdType edgeId);
};


#endif // HEXBLOCKER_H
