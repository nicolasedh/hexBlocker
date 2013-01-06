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
/* This is the class that is in charge of storing all
   hexblocks, vertices, edges and faces

   */

#ifndef HEXBLOCKER_H
#define HEXBLOCKER_H

#include <vtkSmartPointer.h>
#include <QTextStream>

//Predeclarations
class HexBlock;
class hexPatch;
class HexEdge;
class HexReader;
class vtkPoints;
class vtkPolyData;
class vtkGlyph3D;
class vtkSphereSource;
class vtkPolyDataMapper;
class vtkActor;
class vtkRenderer;
class vtkCollection;
class vtkCellArray;
class vtkQuad;
class vtkIdList;
class vtkLabeledDataMapper;
class vtkActor2D;


class HexBlocker
{
public:
    HexBlocker();
    ~HexBlocker();

    //FUNCTIONS
    void createHexBlock();
    void createHexBlock(double [3],double [3]);
    void extrudePatch(vtkIdList *selectedPatches, double dist);
    void resetBounds();
    void PrintHexBlocks();
    void moveVertices(vtkSmartPointer<vtkIdList> ids, double dist[]);
    void setVerticesPos(vtkSmartPointer<vtkIdList> ids, double pos[],bool setPos[]);

    void resetColors();
    void exportVertices(QTextStream &os);
    void exportBlocks(QTextStream &os);
    void exportBCs(QTextStream &os);
    int showParallelEdges(vtkIdType edgeId); // shows all parallel edges
    void setNumberOnParallelEdges(vtkIdType edgeId,int nCells);

    void arbitraryTest();

    int calculateTotalNumberOfCells();
    void readBlockMeshDict(HexReader * reader);

    //DATA
    vtkSmartPointer<vtkPoints> vertices;
    vtkSmartPointer<vtkPolyData> vertData;
    vtkSmartPointer<vtkCollection> patches; //global patch list
    vtkSmartPointer<vtkCollection> hexBlocks;
    vtkSmartPointer<vtkCollection> hexBCs; //BCs
    vtkSmartPointer<vtkCollection> edges; //global list of edges


    //Representations
    vtkSmartPointer<vtkSphereSource> vertSphere;
    vtkSmartPointer<vtkGlyph3D> vertGlyph;
    vtkSmartPointer<vtkPolyDataMapper> vertMapper;
    vtkSmartPointer<vtkActor> vertActor;

    vtkSmartPointer<vtkLabeledDataMapper> vertLabelMapper;
    vtkSmartPointer<vtkActor2D> vertLabelActor;
    vtkSmartPointer<vtkRenderer> renderer;

private:
    //Functions
    void addParallelEdges(vtkSmartPointer<vtkIdList> allParallelEdges,vtkIdType edgeId);
    void rescaleActors();
};


#endif // HEXBLOCKER_H
