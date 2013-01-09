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

#ifndef HEXPATCH_H
#define HEXPATCH_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <QTextStream>

class vtkIdList;
class vtkPoints;
class vtkPolyDataMapper;
class vtkActor;
class vtkQuad;
class vtkCellArray;
class vtkPolyData;
class HexBlock;

class hexPatch : public vtkObject
{
    vtkTypeMacro(hexPatch,vtkObject)

protected:
    hexPatch();
    // Remember to implement
    ~hexPatch();
    hexPatch(const hexPatch&);  // Not implemented in order to comply with vtkObject.
    void operator=(const hexPatch&);  // Not implemented in order to comply with vtkObject.

public:
    //FUNCTIONS
    static hexPatch *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkSmartPointer<vtkIdList> vIds,vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<HexBlock> hex);

    bool equals(vtkSmartPointer<hexPatch> other);
    bool equals(vtkSmartPointer<vtkIdList> otherIds);
    void orderVertices(); //not yet implemented,

    void setColor(double,double,double);
    void resetColor();
    void exportVertIds(QTextStream &os);

    //returns the normal outward from primary hexblock
    void getNormal(double n[3]);
    void setHex(vtkSmartPointer<HexBlock> hex);
    vtkSmartPointer<HexBlock> getPrimaryHexBlock();
    vtkSmartPointer<HexBlock> getSecondaryHexBlock();
    void getCenter(double cog[3]);
    void rescaleActor();

    //DATA
    vtkSmartPointer<vtkIdList> vertIds;
    vtkSmartPointer<vtkPoints> globalVertices;
    //representations
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;


private:
    vtkSmartPointer<vtkQuad> quad;
    vtkSmartPointer<vtkCellArray> quads;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<HexBlock> primaryHex;
    vtkSmartPointer<HexBlock> secondaryHex;
    bool hasPrimaryHex;
    bool hasSecondaryHex;
};

#endif // PATCH_H
