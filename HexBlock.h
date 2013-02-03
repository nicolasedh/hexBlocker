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

#ifndef HEXBLOCK_H
#define HEXBLOCK_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>

//Pre declarations
class HexPatch;
class HexEdge;
class vtkIdList;
class vtkPoints;
class vtkPolyData;
class vtkQuads;
class vtkCollection;
class vtkLine;
class vtkMapper;
class vtkActor;
class vtkTubeFilter;
class vtkCellArray;




class HexBlock : public vtkObject
{
    vtkTypeMacro(HexBlock,vtkObject)

    protected:
        HexBlock();
    // Remember to implement
    ~HexBlock();
    HexBlock(const HexBlock&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexBlock&);  // Not implemented in order to comply with vtkObject.

public:
    //FUNCTIONS
    // inorder to comply with vtkObect
    static HexBlock *New();
    void PrintSelf(ostream &os, vtkIndent indent);

    // Construct from two corner vertices
    void init(double corner0[3], double corner1[3],
              vtkSmartPointer<vtkPoints> verts,
              vtkSmartPointer<vtkCollection> edges,
              vtkSmartPointer<vtkCollection> patches);

    // Extrude from a patch, distance and global list of vertices, edges and patches
    void init(vtkSmartPointer<HexPatch> p, double dist,
              vtkSmartPointer<vtkPoints> verts,
              vtkSmartPointer<vtkCollection> edges,
              vtkSmartPointer<vtkCollection> patches);

    //Construct from a list of vertices that exists in globalVertices,
    // edges and patches are added if not already existing
    void init(vtkSmartPointer<vtkIdList> myVertIds,
              vtkSmartPointer<vtkPoints> verts,
              vtkSmartPointer<vtkCollection> edges,
              vtkSmartPointer<vtkCollection> patches);

    vtkIdType getPatchInternalId(vtkSmartPointer<HexPatch> otherP);
    vtkSmartPointer<vtkIdList> getParallelEdges(vtkIdType edgeId);
    void getNumberOfCells(int nCells[3]);
    void setAxesRadius(double rad);
    void rescaleActor();
    void getCenter(double center[3]);
    void changeVertId(vtkIdType from, vtkIdType to);
    void reduceVertId(vtkIdType vId);
    void initEdges();

    //DATA
    vtkSmartPointer<vtkPoints> globalVertices; //Global list of vertices
    vtkSmartPointer<vtkCollection> globalEdges;
    vtkSmartPointer<vtkCollection> globalPatches;
    vtkSmartPointer<vtkPolyData> hexData;
    vtkSmartPointer<vtkIdList> vertIds; // own vertices in globalVertices
    vtkSmartPointer<vtkIdList> edgeIds;
    vtkSmartPointer<vtkIdList> patchIds; //own patches in globalPatches
    vtkSmartPointer<vtkActor> hexActor; //for axes.

private:
    //FUNCTIONS
    //
    void initAll();
    void drawLocalaxes();

    void initEdge(vtkIdType p0,vtkIdType p1);
    void initPatches();
    void initPatch(int ids[4]);
    vtkIdType patchIdInGlobalList(vtkSmartPointer<HexPatch> p);

    //DATA
    vtkSmartPointer<vtkPolyData> axesData;


    vtkSmartPointer<vtkTubeFilter> axesTubes;

};

#endif // HEXBLOCK_H
