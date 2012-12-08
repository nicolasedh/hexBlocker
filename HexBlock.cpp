#include "HexBlock.h"
#include <vtkObjectFactory.h>

#include "hexPatch.h"
#include "HexEdge.h"


#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCollection.h>
#include <vtkCellArray.h>
#include <vtkCellData.h>
#include <vtkLine.h>
#include <vtkProperty.h>

vtkStandardNewMacro(HexBlock);

//Default constructor
HexBlock::HexBlock()
{
    vertIds = vtkSmartPointer<vtkIdList>::New();
    hexVertices = vtkSmartPointer<vtkPoints>::New();
    hexData = vtkSmartPointer<vtkPolyData>::New();
    globalPatches = vtkSmartPointer<vtkCollection>::New();
    globalEdges = vtkSmartPointer<vtkCollection>::New();
    hexActor = vtkSmartPointer<vtkActor>::New();


}

HexBlock::~HexBlock()
{

}

void HexBlock::init(double corner0[3], double corner1[3],
                    vtkSmartPointer<vtkPoints> verts,
                    vtkSmartPointer<vtkCollection> edges)
{


    double x0=corner0[0];
    double y0=corner0[1];
    double z0=corner0[2];
    double x1=corner1[0];
    double y1=corner1[1];
    double z1=corner1[2];

    globalVertices = verts;
    globalEdges = edges;

    vtkIdType j = globalVertices->GetNumberOfPoints();

    globalVertices->InsertNextPoint(x0,y0,z0);
    globalVertices->InsertNextPoint(x1,y0,z0);
    globalVertices->InsertNextPoint(x1,y1,z0);
    globalVertices->InsertNextPoint(x0,y1,z0);
    globalVertices->InsertNextPoint(x0,y0,z1);
    globalVertices->InsertNextPoint(x1,y0,z1);
    globalVertices->InsertNextPoint(x1,y1,z1);
    globalVertices->InsertNextPoint(x0,y1,z1);


    //set global Ids
    vertIds->SetNumberOfIds(8);
    for(vtkIdType i=0;i<8;i++)
        vertIds->SetId(i,i+j);

    initEdges();
    drawLocalaxes();
}


//Extrude from patch and distance
void HexBlock::init(vtkSmartPointer<hexPatch> p,
                    double dist,
                    vtkSmartPointer<vtkPoints> verts,
                    vtkSmartPointer<vtkCollection> edges)
{
    vtkSmartPointer<HexBlock> fromHex = p->getPrimaryHexBlock();
    globalVertices=verts;
    globalEdges = edges;

    // number of vertices before extrude
    vtkIdType nGv=globalVertices->GetNumberOfPoints();

    vtkSmartPointer<vtkIdList> oldIds =
            p->vertIds;

    //We want to keep extruded blocks aligned
    //so the extrude operation is diffierent depending
    //on which patch was selected
    switch (fromHex->getPatchInternalId(p))
    {

    case 0:
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(nGv+3);
        vertIds->InsertNextId(nGv+2);
        vertIds->InsertNextId(nGv+1);
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(oldIds->GetId(3));
        vertIds->InsertNextId(oldIds->GetId(2));
        vertIds->InsertNextId(oldIds->GetId(1));
        break;
    case 1:
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(nGv+1);
        vertIds->InsertNextId(oldIds->GetId(1));
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(nGv+3);
        vertIds->InsertNextId(nGv+2);
        vertIds->InsertNextId(oldIds->GetId(2));
        vertIds->InsertNextId(oldIds->GetId(3));
        break;
    case 2:
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(oldIds->GetId(3));
        vertIds->InsertNextId(nGv+3);
        vertIds->InsertNextId(nGv+1);
        vertIds->InsertNextId(oldIds->GetId(1));
        vertIds->InsertNextId(oldIds->GetId(2));
        vertIds->InsertNextId(nGv+2);
        break;
    case 3:
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(nGv+1);
        vertIds->InsertNextId(oldIds->GetId(1));
        vertIds->InsertNextId(oldIds->GetId(3));
        vertIds->InsertNextId(nGv+3);
        vertIds->InsertNextId(nGv+2);
        vertIds->InsertNextId(oldIds->GetId(2));
        break;
    case 4:
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(oldIds->GetId(3));
        vertIds->InsertNextId(nGv+3);
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(oldIds->GetId(1));
        vertIds->InsertNextId(oldIds->GetId(2));
        vertIds->InsertNextId(nGv+2);
        vertIds->InsertNextId(nGv+1);
        break;
    case 5:
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(oldIds->GetId(1));
        vertIds->InsertNextId(oldIds->GetId(2));
        vertIds->InsertNextId(oldIds->GetId(3));
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(nGv+1);
        vertIds->InsertNextId(nGv+2);
        vertIds->InsertNextId(nGv+3);
        break;
    default:
        std::cout << "unexpected patch number" << std::endl;
        return;
    }

    double n_dist[3];
    p->getNormal(n_dist);
    vtkMath::MultiplyScalar(n_dist,dist);
    //Insert the new 4 points into globalVertices
    for(vtkIdType i=0;i<4;i++)
    {
        double oldCoords[3];
        double newCoords[3];
        globalVertices->GetPoint(p->vertIds->GetId(i),oldCoords);
        vtkMath::Add(oldCoords,n_dist,newCoords);
        globalVertices->InsertNextPoint(newCoords);
    }

    initEdges();

    drawLocalaxes();
/*
    //vIds are old ids.
    //insert old ids
    for (vtkIdType i =0;i<4;i++)
    {
        //3-i; 3,2,1,0
        vertIds->InsertNextId(vIds->GetId(i));
    }

    double coords[3];
    for (vtkIdType i=4;i<8;i++)
    {
        globalVertices->GetPoint(vertIds->GetId(i-4),coords);
        vertIds->InsertNextId(globalVertices->GetNumberOfPoints());
        globalVertices->InsertNextPoint(coords[0]+dist[0],coords[1]+dist[1],coords[2]+dist[2]);
    }
*/
}

vtkIdType HexBlock::getPatchInternalId(vtkSmartPointer<hexPatch> otherP)
{
    vtkIdType patchId=-1;
    for(vtkIdType i=0;i<globalPatches->GetNumberOfItems();i++)
    {
        vtkSmartPointer<hexPatch> p = hexPatch::SafeDownCast(globalPatches->GetItemAsObject(i));
        if(p==otherP)
            patchId=i;
    }
    return patchId;
}

void HexBlock::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is the hexblock! " << hexVertices->GetPoint(0)[0] << std::endl;
}

void HexBlock::drawLocalaxes()
{
    unsigned char red[3] = {255, 0, 0};
    unsigned char green[3] = {0, 255, 0};
    unsigned char blue[3] = {0, 0, 255};
    vtkSmartPointer<vtkUnsignedCharArray> colors =
            vtkSmartPointer<vtkUnsignedCharArray>::New();
    colors->SetNumberOfComponents(3);
    colors->SetName("Colors");
    colors->InsertNextTupleValue(red);
    colors->InsertNextTupleValue(green);
    colors->InsertNextTupleValue(blue);

    vtkSmartPointer<vtkCellArray> axes =
        vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkLine> xaxes =
           vtkSmartPointer<vtkLine>::New();
    xaxes->GetPointIds()->SetId(0,vertIds->GetId(0));
    xaxes->GetPointIds()->SetId(1,vertIds->GetId(1));
    axes->InsertNextCell(xaxes);
    vtkSmartPointer<vtkLine> yaxes =
            vtkSmartPointer<vtkLine>::New();
    yaxes->GetPointIds()->SetId(0,vertIds->GetId(0));
    yaxes->GetPointIds()->SetId(1,vertIds->GetId(3));
    axes->InsertNextCell(yaxes);
    vtkSmartPointer<vtkLine> zaxes =
           vtkSmartPointer<vtkLine>::New();
    zaxes->GetPointIds()->SetId(0,vertIds->GetId(0));
    zaxes->GetPointIds()->SetId(1,vertIds->GetId(4));
    axes->InsertNextCell(zaxes);

    vtkSmartPointer<vtkPolyData> axesData =
        vtkSmartPointer<vtkPolyData>::New();
    axesData->SetPoints(globalVertices);
    axesData->SetLines(axes);

    axesData->GetCellData()->SetScalars(colors);

    // Visualize
    vtkSmartPointer<vtkPolyDataMapper> axesMapper =
      vtkSmartPointer<vtkPolyDataMapper>::New();
  #if VTK_MAJOR_VERSION <= 5
    axesMapper->SetInput(axesData);
  #else
    axesMapper->SetInputData(axesData);
  #endif

    hexActor->SetMapper(axesMapper);
    double x[3];
    globalVertices->GetPoint(vertIds->GetId(0),x);
    hexActor->SetOrigin(x);
    hexActor->SetScale(0.4);
    hexActor->GetProperty()->SetLineWidth(3);
    hexActor->SetPickable(false);
}

void HexBlock::initEdges()
{
    //Keep the same order of edges as on
    //docs on blockMesh
    initEdge(vertIds->GetId(1),vertIds->GetId(0)); //0
    initEdge(vertIds->GetId(3),vertIds->GetId(2)); //1
    initEdge(vertIds->GetId(7),vertIds->GetId(6)); //2
    initEdge(vertIds->GetId(4),vertIds->GetId(5)); //3

    initEdge(vertIds->GetId(0),vertIds->GetId(3)); //4
    initEdge(vertIds->GetId(1),vertIds->GetId(2)); //5
    initEdge(vertIds->GetId(5),vertIds->GetId(6)); //6
    initEdge(vertIds->GetId(4),vertIds->GetId(7)); //7

    initEdge(vertIds->GetId(0),vertIds->GetId(4)); //8
    initEdge(vertIds->GetId(1),vertIds->GetId(5)); //9
    initEdge(vertIds->GetId(2),vertIds->GetId(6)); //10
    initEdge(vertIds->GetId(3),vertIds->GetId(7)); //11
}

void HexBlock::initEdge(vtkIdType p0, vtkIdType p1)
{

    vtkSmartPointer<HexEdge> e =
            vtkSmartPointer<HexEdge>::New();
    e->init(p0,p1,globalVertices);
//Check if unique first.
    globalEdges->AddItem(e);
}
