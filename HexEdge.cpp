/*
Copyright 2012, 2013
Author Nicolas Edh,
Nicolas.Edh@gmail.com,
or user "nsf" at cfd-online.com

This file is part of hexBlocker.

    hexBlocker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hexBlocker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with hexBlocker.  If not, see <http://www.gnu.org/licenses/>.

    The license is included in the file COPYING.
*/

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
#include <vtkTubeFilter.h>

vtkStandardNewMacro(HexEdge);

HexEdge::HexEdge()
{
    vertIds = vtkSmartPointer<vtkIdList>::New();
    vertIds->SetNumberOfIds(2);

    line = vtkSmartPointer<vtkLine>::New();
    lines = vtkSmartPointer<vtkCellArray>::New();
    data = vtkSmartPointer<vtkPolyData>::New();
    tube = vtkSmartPointer<vtkTubeFilter>::New();
    tube->SetNumberOfSides(24);
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

bool HexEdge::equals(const vtkSmartPointer<vtkIdList> otherIds)
{
    if(otherIds->GetNumberOfIds() < 2)
        return false;

//    std::cout << "comparing other ("
//              << otherIds->GetId(0) << ","
//              << otherIds->GetId(1) << ") my ("
//              << vertIds->GetId(0) << ","
//              << vertIds->GetId(1) << ")" << std::endl;
    return (
                otherIds->GetId(0) == this->vertIds->GetId(0) &&
                otherIds->GetId(1) == this->vertIds->GetId(1)
            ) ||
            (
                otherIds->GetId(0) == this->vertIds->GetId(1) &&
                otherIds->GetId(1) == this->vertIds->GetId(0)
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

    tube->SetInput(data);
    tube->SetRadius(0.05);
    mapper->SetInputConnection(tube->GetOutputPort());

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

void HexEdge::setRadius(double rad)
{
    tube->SetRadius(rad);
}

void HexEdge::exportVertIds(QTextStream &os)
{
    os << "(" << vertIds->GetId(0) << " "
         << vertIds->GetId(1) << ")" << endl;
}

void HexEdge::changeVertId(vtkIdType from, vtkIdType to)
{
    vtkIdType pos = vertIds->IsId(from);
    if(pos >= 0)
    {
        vertIds->SetId(pos,to);
        line->GetPointIds()->SetId(pos,to);
        vtkIdType pts[2];
        pts[0]=vertIds->GetId(0);
        pts[1]=vertIds->GetId(1);
        lines->ReplaceCell(0,2,pts);

//        line->GetPoints()
//        line->Modified();
//        lines->Modified();
//        data->Modified(); //funkar
//        tube->Modified(); //funkar
//        actor->Modified();
//        std::cout << "found i" << std::endl;
    }

}

void HexEdge::reduceVertId(vtkIdType vId)
{
    for(vtkIdType i=0;i<vertIds->GetNumberOfIds();i++)
    {
        vtkIdType oldId = vertIds->GetId(i);
        if(oldId > vId)
        {
            vertIds->SetId(i,oldId-1);
        }
    }
    vtkIdType pts[2];
    pts[0]=vertIds->GetId(0);
    pts[1]=vertIds->GetId(1);
    lines->ReplaceCell(0,2,pts);
}
