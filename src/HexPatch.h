/*
Copyright 2012, 2013
Author Nicolas Edh,
Nicolas.Edh@gmail.com,
or user "nsf" at cfd-online.com

License
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

Description
    This class contains vtk objects for representation of a patch. It also
    has pointers to primary and secondary hexBlock. If its an internal patch
    then it has two connecteds blocks.
*/

#ifndef HEXPATCH_H
#define HEXPATCH_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <QTextStream>

//pre declarations
class vtkIdList;
class vtkPoints;
class vtkPolyDataMapper;
class vtkActor;
class vtkQuad;
class vtkCellArray;
class vtkPolyData;
class HexBlock;

class HexPatch : public vtkObject
{
    vtkTypeMacro(HexPatch,vtkObject)

protected:
    HexPatch();
    // Remember to implement
    ~HexPatch();
    HexPatch(const HexPatch&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexPatch&);  // Not implemented in order to comply with vtkObject.

public:
    //FUNCTIONS
    static HexPatch *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkSmartPointer<vtkIdList> vIds,vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<HexBlock> hex);

    //returns true if other has the same ids.
    //the order is not important.
    bool equals(vtkSmartPointer<HexPatch> other);

    //returns true if otherIds contains the same
    //vertices as this.
    bool equals(vtkSmartPointer<vtkIdList> otherIds);
    void orderVertices(); //not yet implemented,

    void setColor(double,double,double);
    void resetColor();

    //export vertices as ( 1 2 3 4 )
    void exportVertIds(QTextStream &os);

    //returns the normal outward from primary hexblock
    void getNormal(double n[3]);

    //set primary if we have no block else secondary
    void setHex(HexBlock * hex);
    HexBlock * getPrimaryHexBlock();
    HexBlock * getSecondaryHexBlock();

    //average of vertices position
    void getCenter(double c[3]);

    //if it has two block patch is scaled by 0.4 else 0.6
    void rescaleActor();

    //from and two are vertices ids in global list
    void changeVertId(vtkIdType from, vtkIdType to);
    void reduceVertId(vtkIdType vId);

    //if we only have one block then
    void removeSafely(HexBlock *hex);

    bool hasBlock(HexBlock * hb);
    bool hasVertice(vtkIdType vId);
    bool hasBlocks();

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
    HexBlock * primaryHex;
    HexBlock * secondaryHex;
    bool hasPrimaryHex;
    bool hasSecondaryHex;
};

#endif // PATCH_H
