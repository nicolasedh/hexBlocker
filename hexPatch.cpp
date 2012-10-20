#include "hexPatch.h"
#include <vtkObjectFactory.h>

#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkQuad.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
//include vtkVector för att beräkna grejor.
vtkStandardNewMacro(hexPatch);

hexPatch::hexPatch()
{
    primaryHexId=-1;
    secondaryHexId=-1;
    vertIds = vtkSmartPointer<vtkIdList>::New();
    vertIds->SetNumberOfIds(4);

    quad = vtkSmartPointer<vtkQuad>::New();
    quads = vtkSmartPointer<vtkCellArray>::New();
    data = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor  = vtkSmartPointer<vtkActor>::New();
}

hexPatch::~hexPatch()
{

}

void hexPatch::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "verts:" ;
    for(vtkIdType i=0;i<4;i++)
        os << indent << vertIds->GetId(i);
    os << "," <<indent
       <<"hexIds:" << indent <<primaryHexId
      <<indent << secondaryHexId <<std::endl;
}

void hexPatch::orderVertices(){
    //use vtkMath to calc normal and crossproducts
}

bool hexPatch::operator ==(vtkSmartPointer<hexPatch> other)
{
    bool same=true;
    bool existInOther=false;
    for(vtkIdType i=0;i<4;i++)
    {
     existInOther=false;
     for(vtkIdType j=0;j<4;j++)
      if(vertIds->GetId(0) == other->vertIds->GetId(j))
          existInOther=true;
     same = same && existInOther;

    }
}

void hexPatch::init(vtkSmartPointer<vtkIdList> vIds, vtkSmartPointer<vtkPoints> verts)
{
    globalVertices = verts;
    vertIds=vIds;

    for(vtkIdType i=0; i<4 ;i++)
        quad->GetPointIds()->SetId(i,vIds->GetId(i));

    quads->Allocate(1,1);
    quads->InsertNextCell(quad);

    data->SetPoints(globalVertices);
    data->SetPolys(quads);

    mapper->SetInput(data);

    actor->SetMapper(mapper);
    actor->SetOrigin(actor->GetCenter());
    actor->SetScale(0.6);
    actor->GetProperty()->SetColor(0.2,0.9,0.2);
}
