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
    This class is the representation of a hex block. It has all the
    data to create and export blocks. Since this class is ment to be
    created as a pointer it has init functions that actually create
    the block. Blocks can be created from 8 vertices, two points or
    from an extruded patch.
*/

#ifndef HEXBLOCK_H
#define HEXBLOCK_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>
#include <QTextStream>

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
class vtkHexahedron;
class vtkUnstructuredGrid;
class vtkDataSetMapper;



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

    //if otherP is found vtkIdType 1-5 is returned, else -1.
    vtkIdType getPatchInternalId(vtkSmartPointer<HexPatch> otherP);

    //returns all dependent edges of an edgeId in the global list
    vtkSmartPointer<vtkIdList> getParallelEdges(vtkIdType edgeId);

    //returns the total number of cells in the block
    void getNumberOfCells(int nCells[3]);

    //set the representaton radius of the red,blue and green axes.
    void setAxesRadius(double rad);

    //resets the center and rescales the axis
    void rescaleActor();

    //reset the color of the box
    void resetColor();

    //returns the center of the block, calculated as the
    //average of all vertices.
    void getCenter(double center[3]);

    //changes the id in the internal lis. from and to
    //are ids in the global lsit
    void changeVertId(vtkIdType from, vtkIdType to);

    //reduces the ids if they are greater than vId
    void reduceVertId(vtkIdType vId);

    //creates new edges if needed
    void initEdges();

    //puts hex ( 1 2 3.. ) (10 10 10) (grading) to os
    void exportDict(QTextStream &os);

    // returns true if simpleGrading is possible
    bool getGradings(double gradings[12] );

    // returns a list of common vertices. NOT YET TESTED
    vtkIdList* commonVertices(HexBlock *hb);

    // returns true if vId is in vertIds
    bool hasVertice(vtkIdType vId);

    //eId is global id
    //if local or global is needed use
    //localEdges->isItemPresent(e)-1
    bool hasEdge(vtkIdType eId);
    bool hasEdge(HexEdge *e);

    //returns true if they have the same vertices
    //in the same order
    bool equals(HexBlock *other);

    //DATA
    vtkSmartPointer<vtkPoints> globalVertices; //Global list of vertices
    vtkSmartPointer<vtkCollection> globalEdges;
    vtkSmartPointer<vtkCollection> globalPatches;
    vtkSmartPointer<vtkPolyData> hexData;
    vtkSmartPointer<vtkIdList> vertIds; // own vertices in globalVertices
    vtkSmartPointer<vtkCollection> localPatches; //own patches
    vtkSmartPointer<vtkCollection> localEdges; //own edges
    vtkSmartPointer<vtkActor> hexAxisActor; //for axes.
    vtkSmartPointer<vtkActor> hexBlockActor; //for axes.

private:
    //FUNCTIONS
    //creates edges and patches
    void initAll();

    //the litle red,green and blue representations
    //of the local coordinates
    void drawLocalaxes();

    // draws a blue block.
    void drawBlock();

    void initEdge(vtkIdType p0,vtkIdType p1);
    void initPatches();
    void initPatch(int id0,int id1,int id2,int id3);
    vtkIdType patchIdInGlobalList(vtkSmartPointer<HexPatch> p);

    //DATA
    vtkSmartPointer<vtkPolyData> axesData;
    vtkSmartPointer<vtkTubeFilter> axesTubes;

//    vtkSmartPointer<vtkHexahedron> hexahedron;
//    vtkSmartPointer<vtkCellArray> hexes;
//    vtkSmartPointer<vtkUnstructuredGrid> uGrid;
//    vtkSmartPointer<vtkDataSetMapper> hexaMapper;
};

#endif // HEXBLOCK_H
