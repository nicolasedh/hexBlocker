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
    This is the main model class. As much as possible that is not GUI or user interaction
    is handled from this class.It's meant to be controlled by MainWindow class but could in principle be used
    alone. It's where all hexBlocks, hexPatches, hexEdges and hexBCs are stored.
*/


#ifndef HEXBLOCKER_H
#define HEXBLOCKER_H

#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <QTextStream>
#include <QString>

//Predeclarations
class HexBlock;
class HexPatch;
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
class vtkAxesActor;
class vtkOrientationMarkerWidget;
class vtkRenderWindow;


class HexBlocker
{
public:
    HexBlocker();
    ~HexBlocker();

    //FUNCTIONS
    //inits the global axes
    void initOrientationAxes(vtkRenderWindow * renwin);
    //removes the same
    void removeOrientationAxes();

    //create a block from (0,0,0) to (1,1,1)
    void createHexBlock();

    //create a block from to corner points.
    void createHexBlock(double [3],double [3]);

    //create a block by extrusion of a patch.
    //only one the first patch in the list will
    //be used
    void extrudePatch(vtkIdList *selectedPatches, double dist);

    //Sets vertices radius and so on the sensible values depending on
    //total domain.
    void resetBounds();

    //Prints blocks, and patches to std::cout
    void PrintHexBlocks();

    //moves vertices by adding dist to the position
    void moveVertices(vtkSmartPointer<vtkIdList> ids, double dist[]);

    //set the position of vertices. if setPos[0] is true then the
    //x coordinate is set.
    void setVerticesPos(vtkSmartPointer<vtkIdList> ids, double pos[],bool setPos[]);

    //rotate vertices by a given angle around a custom axis
    void rotateVertices(vtkSmartPointer<vtkIdList> ids, double angle, double center[], double axis[]);

    //snap vertices to the closest point on a geometry (read STL surface)
    void snapVertices(vtkSmartPointer<vtkIdList> ids);

    //resets colors for patches and edges.
    void resetColors();

    //Exports in blockMeshDict format.
    //Should probably be moved
    //to hexExporter.
    void exportVertices(QTextStream &os);
    void exportBlocks(QTextStream &os);
    void exportBCs(QTextStream &os);
    void exportEdges(QTextStream &os);

    //highlight all parallel edges
    HexEdge * showParallelEdges(vtkIdType edgeId);

    //Set the number of cells and grading
    // mode=0 prop grading to all parallel
    // mode=1 dont propagate grading
    // mode=2 only to block that owns the edge (not yet implemented)
    void setEdgePropsOnParallelEdges(HexEdge *props,vtkIdType edgeId, int mode=0);

    //nx*ny*nz is sumed for all blocks
    int calculateTotalNumberOfCells();

    //Clears previous data and copies pointers
    //from reader
    void readBlockMeshDict(HexReader * reader);

    //Reads a STL geometry
    void readGeometry(char *openFileName);

    //Merges two patches but only if they match
    //i.e. each vertice in master must have
    //at most one vertice which is closet in slave
    void mergePatch(vtkIdType masterId, vtkIdType slaveId);

    // Return true if vertice was removed. Only remove of no block
    // has vertices id toRem
    bool removeVerticeSafely(vtkIdType toRem);
    void removeVerticesSafely(vtkIdList * toRemove);

    // remove a block and any patch or edge that has vertices
    // that are no longer needed by any block.
    // it does not remove vertices
    void removeHexBlock(vtkIdType toRem);

    // redraw all actors and render
    void rescaleActors();

    //As above but fill vertsToRem with vertices that are no longer needed.
    void removeHexBlock(const vtkIdType toRem, vtkIdList * vertsToRem);
    //this is a testfunktion for quick and dirty testing
    //and devloping

    // calls removeHexBlock(toRem,vert2rem) for every item in list
    // ie removes all blocks in hte list and their vertices
    void removeHexBlocks(vtkIdList * toRems);
    void arbitraryTest();

    //show/hide objects
    void showBlocks();
    void hideBlocks();
    void visibilityBlocks(bool mode);
    void showPatches();
    void hidePatches();
    void visibilityPatches(bool mode);
    void showEdges();
    void hideEdges();
    void visibilityEdges(bool mode);
    void showVertIDs();
    void hideVertIDs();
    void visibilityVertIDs(bool mode);
    void showGeometry();
    void hideGeometry();
    void visibilityGeometry(bool mode);

    //set model scale; related to convertToMeters in blockMeshDict
    void setModelScale(double scale);
    void scaleGeometry(double scale);
    double convertToMeters;
    double geoScale;
    bool hasGeometry;

    //force a render
    void render();

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
    vtkSmartPointer<vtkActor> GeoActor;
    vtkSmartPointer<vtkSTLReader> GeoReader;
    vtkSmartPointer<vtkAxesActor> orientationAxes;
    vtkSmartPointer<vtkOrientationMarkerWidget> orientationAxesWidget;
    vtkSmartPointer<vtkLabeledDataMapper> vertLabelMapper;
    vtkSmartPointer<vtkActor2D> vertLabelActor;
    vtkSmartPointer<vtkRenderer> renderer;

    //to be removed, has info of arcs and such
    QString edgesDict;

private:
    //Functions
    void addParallelEdges(vtkSmartPointer<vtkIdList> allParallelEdges,vtkIdType edgeId);
    // returns id if found else returns -1
    vtkIdType findEdge(const vtkIdType a, const vtkIdType b);

    void removeVertice(vtkIdType toRem);
    void removeVertices(vtkIdList * toRemove);

    /* decreases Ids > toRem (not equal to toRem)
     these are used when deleting while traversing a list
     For instance lets say we have the list org(10 11 12 13 14) and
     want to remove the items in position toRem(1 2) i.e ids 11 and 12
     first we remove the item in toRem[0] i.e id 11. We get the list
     (10 11 13 14). Then we remove toRem[1] which is id 13.
     Not what was originally intended. the answer is to run
     decreaseList(toRem,toRem[0]) between delete operations.
     this will decrease the 2 in toRem by one. Thus pointing to
     the second item in (10 11 13 14), id 11. The result is
     the list (10 13 14 ) which was the intention*/

     //this algorithm could be improved. E.g. traverse the
    //the list backwards
    void decreaseList(vtkIdList * list, vtkIdList * toRemove);
    void decreaseList(vtkIdList * list, vtkIdType toRem);

    //DATA
    bool isRendering;
};


#endif // HEXBLOCKER_H
