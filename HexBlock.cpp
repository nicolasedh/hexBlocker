#include "HexBlock.h"

#include <vtkObjectFactory.h>
#include <vtkMath.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>
#include <vtkQuad.h>
#include <hexPatch.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCollection.h>

vtkStandardNewMacro(HexBlock);

//Default constructor
HexBlock::HexBlock()
{
    vertIds = vtkSmartPointer<vtkIdList>::New();
    hexVertices = vtkSmartPointer<vtkPoints>::New();
    hexData = vtkSmartPointer<vtkPolyData>::New();
    patches = vtkSmartPointer<vtkCollection>::New();
}

HexBlock::~HexBlock()
{

}

void HexBlock::init(double corner0[3], double corner1[3], vtkSmartPointer<vtkPoints> verts)
{
    double x0=corner0[0];
    double y0=corner0[1];
    double z0=corner0[2];
    double x1=corner1[0];
    double y1=corner1[1];
    double z1=corner1[2];

    globalVertices = verts;

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

}


//Extrude from patch and distance
void HexBlock::init(vtkSmartPointer<hexPatch> p,
                    double dist,
                    vtkSmartPointer<vtkPoints> verts)
{
    vtkSmartPointer<HexBlock> fromHex = p->getPrimaryHexBlock();
    globalVertices=verts;

    // number of vertices before extrude
    vtkIdType nGv=globalVertices->GetNumberOfPoints();

    vtkSmartPointer<vtkIdList> oldIds =
            p->vertIds;

    //We want to keep extruded blocks aligned
    //so there's one order for each patch
    switch (fromHex->getPatchInternalId(p))
    {

    case 0:
        vertIds->InsertNextId(nGv);
        vertIds->InsertNextId(nGv+1);
        vertIds->InsertNextId(nGv+2);
        vertIds->InsertNextId(nGv+3);
        vertIds->InsertNextId(oldIds->GetId(0));
        vertIds->InsertNextId(oldIds->GetId(1));
        vertIds->InsertNextId(oldIds->GetId(2));
        vertIds->InsertNextId(oldIds->GetId(3));
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
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        vtkSmartPointer<hexPatch> p = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        if(p==otherP)
            patchId=i;
    }
    return patchId;
}

void HexBlock::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is the hexblock! " << hexVertices->GetPoint(0)[0] << std::endl;
}

void HexBlock::initPatch()
{

}
