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
    enum edgeTypes{LINE=0,ARC=1};

    //FUNCTIONS
    static HexEdge *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    // initializes the ids and points
    void init(vtkIdType p0,vtkIdType p1, vtkSmartPointer<vtkPoints> verts);

    //true edge has same ids as in other. Doesnt have to be
    //in the same order.
    bool equals(const HexEdge * other);
    bool equals(const vtkSmartPointer<vtkIdList> otherIds);

    void setColor(double,double,double);
    void resetColor();

    //radius of tube representation
    void setRadius(double rad);

    //for debug output, outpus edgeIds
    void exportVertIds(QTextStream &os);
    //export the edge info to screen
    void exportEdgeDict(QTextStream &os);

    //changes Id of a vertice. redrawEdge should probably
    //be called after this.
    void changeVertId(vtkIdType from, vtkIdType to);
    void reduceVertId(vtkIdType vId);

    double getLength();

    bool hasVertice(vtkIdType vId);

    //redraws and edge and updates internal points
    void redrawedge();

    //sets the type and prepares internal structures
    void setType(edgeTypes newType);
    int getType();

    // calculates a point on the line between the two vertices
    // of the edge
    void calcParametricPointOnLine(const double t,double pt[3]);

    // calculates a point on the arc between the two edges
    // given the center of the arc
    void calcParametricPointOnArc(const double t, const double c[3],double arcp[3]);

    //not used
    void getControlPoint(const vtkIdType cId,double cntrp[3]);
    void setControlPoint(const vtkIdType cId,const double cntrp[3]);

    //calculates and sets a control point from patch center and optionally radius
    void calcArcControlPointFromCenter(const double pac[3], double radius=0.0);
//    void getControlPoints(vtkPoints *pnts,vtkIdList *ctrlPsIds);
    int getNumberOfControlPoints();

    //DATA
    vtkSmartPointer<vtkIdList> vertIds;
    vtkSmartPointer<vtkPoints> globalVertices;
    vtkSmartPointer<vtkPolyData> data;
    vtkSmartPointer<vtkTubeFilter> tube;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkActor> actor;
    int nCells; // number of cells on edge
    double grading;

    //cntrlPoints are to be printed in blockMeshDict
    //they are ids of myPoints. Empty if type is line.
    vtkSmartPointer<vtkIdList> cntrlPointsIds;
    vtkSmartPointer<vtkPoints> myPoints;
private:
    //DATA
    edgeTypes edgeType;
    vtkSmartPointer<vtkCellArray> lines;
    int arcNpoints;



    //FUNCTIONS
    void drawArc(double c[3]);
    void drawLine();
    //calcs f=|r_n| - R, used by calcArcControlPointFromCenter
    //please see images/HexEdge_calcArcPoint_prescribed_radius*
    double secF(const double R,const double pac[3], const double xn);
};

#endif // EDGE_H
