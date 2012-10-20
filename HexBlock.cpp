#include "HexBlock.h"

#include <vtkObjectFactory.h>
#include <vtkPoints.h>
#include <vtkIdList.h>
#include <vtkPolyData.h>
#include <vtkQuad.h>
#include <vtkCellArray.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>


vtkStandardNewMacro(HexBlock);

//Default constructor
HexBlock::HexBlock()
{
    vertIds = vtkSmartPointer<vtkIdList>::New();
    hexVertices = vtkSmartPointer<vtkPoints>::New();
    hexData = vtkSmartPointer<vtkPolyData>::New();
    patches = vtkSmartPointer<vtkCellArray>::New();
    patches->SetNumberOfCells(6);
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


//Construct from list of 4 points, and ofset
//2do borde ta face som argument dist ska vara skalar*normal, eller?
void HexBlock::init(vtkSmartPointer<vtkIdList> vIds, double dist[3], vtkSmartPointer<vtkPoints> verts)
{

    globalVertices=verts;

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

}



void HexBlock::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is the hexblock! " << hexVertices->GetPoint(0)[0] << std::endl;
}
