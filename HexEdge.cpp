#include "HexEdge.h"
#include <vtkObjectFactory.h>


#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkMath.h>


vtkStandardNewMacro(HexEdge);

HexEdge::HexEdge()
{
    vertIds = vtkSmartPointer<vtkIdList>::New();
    vertIds->SetNumberOfIds(2);

    line = vtkSmartPointer<vtkLine>::New();
    lines = vtkSmartPointer<vtkCellArray>::New();
    data = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor  = vtkSmartPointer<vtkActor>::New();

    nCells = 10; //default value
}

HexEdge::~HexEdge()
{

}

void HexEdge::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "verts:" ;
    for(vtkIdType i=0;i<2;i++)
        os << indent << vertIds->GetId(i);
    os << "." << std::endl;
}


bool HexEdge::equals(const HexEdge  *other)
{
    return (
                other->vertIds->GetId(0) == this->vertIds->GetId(0) &&
                other->vertIds->GetId(1) == this->vertIds->GetId(1)
            ) ||
            (
                other->vertIds->GetId(0) == this->vertIds->GetId(1) &&
                other->vertIds->GetId(1) == this->vertIds->GetId(0)
            );

}

void HexEdge::init(vtkIdType p0,
                   vtkIdType p1,
                    vtkSmartPointer<vtkPoints> verts)
{
    globalVertices = verts;
    vertIds->InsertId(0,p0);
    vertIds->InsertId(1,p1);
    line->GetPointIds()->SetId(0,p0);
    line->GetPointIds()->SetId(1,p1);
    lines->InsertNextCell(line);

    data->SetPoints(globalVertices);
    data->SetLines(lines);
    mapper->SetInput(data);

    actor->SetMapper(mapper);

    resetColor();
}

void HexEdge::setColor(double r, double g, double b)
{
    actor->GetProperty()->SetColor(r,b,g);
}

void HexEdge::resetColor()
{
    actor->GetProperty()->SetColor(0.8,0.9,0.8);
}

void HexEdge::setLineWidth(float width)
{
    actor->GetProperty()->SetLineWidth(width);

}

void HexEdge::resetLineWidth()
{
    actor->GetProperty()->SetLineWidth(1.0);
}

void HexEdge::exportVertIds(QTextStream &os)
{
    os << "(" << vertIds->GetId(0) << " "
         << vertIds->GetId(1) << ")" << endl;
}


