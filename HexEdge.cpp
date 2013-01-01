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


