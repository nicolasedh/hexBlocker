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

#ifndef HEXEDGE_H
#define HEXEDGE_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <QTextStream>

class vtkIdList;
class vtkPoints;
class vtkPolyDataMapper;
class vtkActor;
class vtkLine;
class vtkCellArray;
class vtkPolyData;
class vtkTubeFilter;


class HexEdge : public vtkObject
{
    vtkTypeMacro(HexEdge,vtkObject)

protected:
    HexEdge();
    // Remember to implement
    ~HexEdge();
    HexEdge(const HexEdge&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexEdge&);  // Not implemented in order to comply with vtkObject.

public:
    //FUNCTIONS
    static HexEdge *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkIdType p0,vtkIdType p1, vtkSmartPointer<vtkPoints> verts);

    bool equals(const HexEdge * other);


    void setColor(double,double,double);
    void resetColor();
    void setRadius(double rad);
    void exportVertIds(QTextStream &os);


    //DATA
    vtkSmartPointer<vtkIdList> vertIds;
    vtkSmartPointer<vtkPoints> globalVertices;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<vtkTubeFilter> tube;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    int nCells; // number of cells on edge

private:
    vtkSmartPointer<vtkLine> line;
    vtkSmartPointer<vtkCellArray> lines;
};

#endif // EDGE_H
