#include "hexPatch.h"
#include <vtkObjectFactory.h>

#include <HexBlock.h>
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkQuad.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkMath.h>

//include vtkVector för att beräkna grejor.
vtkStandardNewMacro(hexPatch);

hexPatch::hexPatch()
{

    vertIds = vtkSmartPointer<vtkIdList>::New();
    vertIds->SetNumberOfIds(4);

    quad = vtkSmartPointer<vtkQuad>::New();
    quads = vtkSmartPointer<vtkCellArray>::New();
    data = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor  = vtkSmartPointer<vtkActor>::New();

    hasPrimaryHex = false;
    hasSecondaryHex = false;
}

hexPatch::~hexPatch()
{

}

void hexPatch::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "verts:" ;
    for(vtkIdType i=0;i<4;i++)
        os << indent << vertIds->GetId(i);
    os << "." << std::endl;
}

void hexPatch::orderVertices(){
    //use vtkMath to calc normal and crossproducts
}

bool hexPatch::operator==(vtkSmartPointer<hexPatch> other)
{
    bool same=true;
    bool existInOther=false;
    for(vtkIdType i=0;i<4;i++)
    {
     existInOther=false;
     for(vtkIdType j=0;j<4;j++)
      if(vertIds->GetId(i) == other->vertIds->GetId(j))
          existInOther=true;
     same = same && existInOther;
    }
    return same;
}

void hexPatch::init(vtkSmartPointer<vtkIdList> vIds,
                    vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<HexBlock> hex)
{
    hasPrimaryHex=true;
    primaryHex=hex;
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
    resetColor();
}

void hexPatch::setColor(double r, double g, double b)
{
    actor->GetProperty()->SetColor(r,b,g);

}

void hexPatch::resetColor()
{
    if(!hasSecondaryHex)
        actor->GetProperty()->SetColor(0.2,0.9,0.2);
    else
    {
        actor->GetProperty()->SetColor(0.2,0.2,0.9);
        actor->GetProperty()->SetOpacity(0.5);
    }
}

void hexPatch::exportVertIds(QTextStream &os)
{
    os << "(" << vertIds->GetId(0) << " "
       << vertIds->GetId(1) << " "
       << vertIds->GetId(2) << " "
       << vertIds->GetId(3) << ")" << endl;
}

void hexPatch::getNormal(double n[3])
{
    double v0[3],v1[3],v3[3],x[3],y[3];
    globalVertices->GetPoint(vertIds->GetId(0),v0);
    globalVertices->GetPoint(vertIds->GetId(1),v1);
    globalVertices->GetPoint(vertIds->GetId(3),v3);

    vtkMath::Subtract(v1,v0,x);
    vtkMath::Subtract(v3,v0,y);

    vtkMath::Cross(x,y,n);
    vtkMath::Normalize(n);
/*
    std::cout << "n =(" << n[0]  <<" " << n[1]  << " " << n[2]  << "), " << std::endl
              << "v0=(" << v0[0] <<" " << v0[1] << " " << v0[2] << "), " << std::endl
              << "v1=(" << v1[0] <<" " << v1[1] << " " << v1[2] << "), " << std::endl
              << "v3=(" << v3[0] <<" " << v3[1] << " " << v3[2] << "), " << std::endl
              << "x =(" << x[0]  <<" " << x[1] << " "  << x[2] << "), "  << std::endl
              << "y =(" << y[0]  <<" " << y[1] << " "  << y[2] << "), "  << std::endl;
*/
}

void hexPatch::setHex(vtkSmartPointer<HexBlock> hex)
{
    //error check?
    if (!hasPrimaryHex)
        primaryHex=hex;
    else //
    {
        //std::cout << "setting secondary" <<  std::endl;
        secondaryHex=hex;
        actor->SetOrigin(actor->GetCenter());
        actor->SetScale(0.4);
        hasSecondaryHex=true;
        resetColor();
    }

}

vtkSmartPointer<HexBlock> hexPatch::getPrimaryHexBlock()
{
    return primaryHex;
}

vtkSmartPointer<HexBlock> hexPatch::getSecondaryHexBlock()
{
    return secondaryHex;
}
