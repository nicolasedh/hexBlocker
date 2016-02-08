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
    edgeType=LINE;

    data = vtkSmartPointer<vtkPolyData>::New();
    lines = vtkSmartPointer<vtkCellArray>::New();
    tube = vtkSmartPointer<vtkTubeFilter>::New();
    tube->SetNumberOfSides(24);
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor  = vtkSmartPointer<vtkActor>::New();

    nCells = 10; //default value
    grading = 1.0; //default value

    arcNpoints=50;

    myPoints = vtkSmartPointer<vtkPoints>::New();
    cntrlPointsIds = vtkSmartPointer<vtkIdList>::New();

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
    myPoints->SetNumberOfPoints(2);
    setType(LINE);
    drawLine();

    data->SetPoints(myPoints);
    data->SetLines(lines);

#if VTK_MAJOR_VERSION >= 6
    tube->SetInputData(data);
#else
    tube->SetInput(data);
#endif    
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

void HexEdge::exportEdgeDict(QTextStream &os)
{
    if(edgeType == LINE )
        return;
    os << "\t";
    switch(edgeType)
    {
    case ARC:
        os << "arc ";
        break;

    }
    os << vertIds->GetId(0) << " " <<vertIds->GetId(1) << " ";
    int numPoints = cntrlPointsIds->GetNumberOfIds();
    if(numPoints == 1)
    {
        double pos[3];
        myPoints->GetPoint(cntrlPointsIds->GetId(0),pos);
        os << "(" << pos[0] << " " << pos[1] << " " << pos[2] << ") " <<endl;
    }
    else
    {
        os << endl <<"\t(" << endl;
        for(vtkIdType i=0;i<numPoints;i++)
        {
            double pos[3];
            myPoints->GetPoint(cntrlPointsIds->GetId(0),pos);
            os << "\t\t(" << pos[0] << " " << pos[1] << " " << pos[2] << ")" <<endl;
        }
        os << "\t)"<<endl;
    }
}

void HexEdge::changeVertId(vtkIdType from, vtkIdType to)
{
    vtkIdType pos = vertIds->IsId(from);
    if(pos >= 0)
    {
        vertIds->SetId(pos,to);
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
}

double HexEdge::getLength()
{
    double pos0[3],pos1[3],diff[3];
    globalVertices->GetPoint(vertIds->GetId(0),pos0);
    globalVertices->GetPoint(vertIds->GetId(1),pos1);
    vtkMath::Subtract(pos0,pos1,diff);
    return vtkMath::Norm(diff);
}


bool HexEdge::hasVertice(vtkIdType vId)
{
    return vertIds->IsId(vId) > -1;
}

void HexEdge::setType(edgeTypes newType)
{
    if(edgeType == newType)
    {
        redrawedge();
        return;
    }
    double pc0[3], pc1[3];
    globalVertices->GetPoint(vertIds->GetId(0),pc0);
    globalVertices->GetPoint(vertIds->GetId(1),pc1);

    myPoints->Initialize();
    lines->Initialize();
    cntrlPointsIds->Initialize();

    edgeType=newType;
    switch(edgeType)
    {
    case LINE:
    {
        myPoints->SetNumberOfPoints(2);
        myPoints->SetPoint(0,pc0);
        myPoints->SetPoint(1,pc1);
        cntrlPointsIds->SetNumberOfIds(0);
    }
        break;
    case ARC:
    {
        myPoints->SetNumberOfPoints(arcNpoints);
        myPoints->SetPoint(0,pc0);
        cntrlPointsIds->SetNumberOfIds(1);
        cntrlPointsIds->InsertId(0,arcNpoints/2);
        myPoints->SetPoint(arcNpoints-1,pc1);
        //create an initial random arcppoint, so the three points are not inline
        double p[3];
        calcParametricPointOnLine(0.5,p);
        double rand[3]={vtkMath::Random(),
                        vtkMath::Random(),
                        vtkMath::Random()};
        vtkMath::Normalize(rand);
        vtkMath::MultiplyScalar(rand,1e-5);
        double arcp[3];
        vtkMath::Add(rand,p,arcp);
        myPoints->SetPoint(cntrlPointsIds->GetId(0),arcp);
    }
        break;
    default:
        break;
    }
    redrawedge();
}

int HexEdge::getType()
{
    switch(edgeType)
    {
    case LINE:
        return 0;
    case ARC:
        return 1;
    }
}

void HexEdge::redrawedge()
{
    double gp0[3], gp1[3];
    globalVertices->GetPoint(vertIds->GetId(0),gp0);
    globalVertices->GetPoint(vertIds->GetId(1),gp1);

    //have the points changed?
    double op0[3],op1[3],delta0[3],delta1[3];
    myPoints->GetPoint(0,op0);
    myPoints->GetPoint(myPoints->GetNumberOfPoints()-1,op1);

    vtkMath::Subtract(gp0,op0,delta0);
    vtkMath::Subtract(gp1,op1,delta1);


    switch(edgeType)
    {
    case LINE:
        myPoints->SetPoint(0,gp0);
        myPoints->SetPoint(1,gp1);
        drawLine();
        break;
    case ARC:
        double c[3],arcpnt[3];
        myPoints->GetPoint(cntrlPointsIds->GetId(0),arcpnt);
        //new arcp = oldarcp + t*delta0 +(1-t)*delta1
        double t = calcParameterFromId(0);
        vtkMath::MultiplyScalar(delta0,t);
        vtkMath::MultiplyScalar(delta1,1-t);
        vtkMath::Add(delta0,arcpnt,arcpnt);
        vtkMath::Add(delta1,arcpnt,arcpnt);

        //Store the new points
        myPoints->SetPoint(0,gp0);
        myPoints->SetPoint(cntrlPointsIds->GetId(0),arcpnt);
        myPoints->SetPoint(arcNpoints-1,gp1);
        vtkMath::Solve3PointCircle(gp0,arcpnt,gp1,c);
        drawArc(c);
        break;
    }
    data->Modified();
    tube->Modified();
}

void HexEdge::drawLine()
{
    //incase vertices moved.
    double pc0[3],pc1[3];
    //Copy "end" vertices from global to local
    globalVertices->GetPoint(vertIds->GetId(0),pc0);
    globalVertices->GetPoint(vertIds->GetId(1),pc1);

    myPoints->SetPoint(0,pc0);
    myPoints->SetPoint(1,pc1);

    vtkSmartPointer<vtkLine> line =
            vtkSmartPointer<vtkLine>::New();
    line->GetPointIds()->SetId(0,0);
    line->GetPointIds()->SetId(1,1);
    lines->Initialize();
    lines->InsertNextCell(line);
}

void HexEdge::drawArc(double c[3])
{
    edgeType=ARC;

    double pc0[3], pc1[3];
    globalVertices->GetPoint(vertIds->GetId(0),pc0);
    globalVertices->GetPoint(vertIds->GetId(1),pc1);

    lines->Initialize();

    //insert first vertice
    myPoints->SetPoint(0,pc0);

    for(vtkIdType i=1;i<arcNpoints-1;i++)
    {
        double t=i /((double)(arcNpoints-1));
        double arcp[3];
        calcParametricPointOnArc(t,c,arcp);
        myPoints->SetPoint(i,arcp);
        vtkSmartPointer<vtkLine> line =
                vtkSmartPointer<vtkLine>::New();
        line->GetPointIds()->SetId(0,i-1);
        line->GetPointIds()->SetId(1,i);
        lines->InsertNextCell(line);
    }
//    myPoints->SetPoint(arcNpoints-1,pc1); //redundant
    vtkSmartPointer<vtkLine> line =
            vtkSmartPointer<vtkLine>::New();
    line->GetPointIds()->SetId(0,arcNpoints-2);
    line->GetPointIds()->SetId(1,arcNpoints-1);
    lines->InsertNextCell(line);

    lines->Modified();
    tube->Modified();
}

void HexEdge::calcParametricPoint(const double t, double pt[])
{
    switch (edgeType)
    {
    case LINE:
        calcParametricPointOnLine(t,pt);
        break;
    case ARC:
        calcParametricPointOnArc(t,pt);
        break;
    }
}

void HexEdge::calcParametricPointOnLine(const double t, double pt[3])
{
    double pc0[3], pc1[3];
    globalVertices->GetPoint(vertIds->GetId(0),pc0); //tail
    globalVertices->GetPoint(vertIds->GetId(1),pc1); //head

    //pt = t(pc1-pc0) + pc0 = t*pc1 + (1-t) pc0
    pt[0]=t*pc1[0] + (1-t)*pc0[0];
    pt[1]=t*pc1[1] + (1-t)*pc0[1];
    pt[2]=t*pc1[2] + (1-t)*pc0[2];
}

void HexEdge::calcParametricPointOnArc(const double t, double pt[])
{
    double center[3],arcp[3],pc0[3],pc1[3];
    globalVertices->GetPoint(vertIds->GetId(0),pc0); //tail
    globalVertices->GetPoint(vertIds->GetId(1),pc1); //head
    myPoints->GetPoint(cntrlPointsIds->GetId(0),arcp);
    vtkMath::Solve3PointCircle(pc1,arcp,pc0,center);
    calcParametricPointOnArc(t,center,pt);
}

double HexEdge::calcParameterFromId(vtkIdType cntlId)
{
    switch(edgeType)
    {
    case LINE:
        return 0.0;
    case ARC:
        double center[3],arcp[3],pc0[3],pc1[3];
        globalVertices->GetPoint(vertIds->GetId(0),pc0); //tail
        globalVertices->GetPoint(vertIds->GetId(1),pc1); //head
        myPoints->GetPoint(cntrlPointsIds->GetId(0),arcp);
        vtkMath::Solve3PointCircle(pc1,arcp,pc0,center);

        double u[3],v[3],w[3];
        vtkMath::Subtract(pc0,center,u);
        vtkMath::Subtract(pc1,center,v);
        vtkMath::Subtract(arcp,center,w);

        //cos(a) = u*w / |u| |w|
        vtkMath::Normalize(u);
        vtkMath::Normalize(v);
        vtkMath::Normalize(w);

        double dotp=vtkMath::Dot(w,u);
        double alpha = std::acos(dotp);
        dotp=vtkMath::Dot(v,u);
        double alphaMax = std::acos(dotp);//+std::asin(1);
        //t = alpha/alphaMax
        return alpha/alphaMax;

    }

}

void HexEdge::calcParametricPointOnArc(const double t, const double c[3], double arcp[3])
{
    double pc0[3], pc1[3];
    globalVertices->GetPoint(vertIds->GetId(0),pc0); //tail
    globalVertices->GetPoint(vertIds->GetId(1),pc1); //head
    double u[3],v[3];
    vtkMath::Subtract(pc0,c,u); //u=p0-c
    vtkMath::Subtract(pc1,c,v); //v=p1-c

    //R = (|u| + |v|)/2 average value
    double R = 0.5 * (vtkMath::Norm(u) + vtkMath::Norm(v) );

    double pt[3]; //parametric point on straight line
    calcParametricPointOnLine(t,pt);

    double z[3];
    vtkMath::Subtract(pt,c,z); //z=pt-c
    double nz=vtkMath::Norm(z);

    arcp[0] = c[0] + R*z[0]/nz;
    arcp[1] = c[1] + R*z[1]/nz;
    arcp[2] = c[2] + R*z[2]/nz;
}

void HexEdge::getControlPoint(const vtkIdType cId, double ctrlp[])
{
    if(edgeType==ARC) //theres only one controlpoint cId is ignored
        myPoints->GetPoint(myPoints->GetNumberOfPoints()/2,ctrlp);
}

void HexEdge::setControlPoint(const vtkIdType cId, const double cntrp[])
{
    switch (edgeType)
    {
    case LINE:
        break; //a line has no controlPoints
    case ARC:
        myPoints->SetPoint(cntrlPointsIds->GetId(0),cntrp);
        break;
    }
}

void HexEdge::calcArcControlPointFromCenter(const double pac[3], double radius)
{
    if(edgeType!=ARC)
        return;
    double pc0[3], pc1[3];
    double R=radius;
    globalVertices->GetPoint(vertIds->GetId(0),pc0); //tail
    globalVertices->GetPoint(vertIds->GetId(1),pc1); //head
    double arcp[3];

    //calc radius as average of |c -p0| and |c -p1|
    if(radius==0.0)
    {

        calcParametricPointOnArc(0.5,pac,arcp);
    }
    else
    {
        //use the secant method to calculate
        //initial guesses
        double xn2,xn1,xn; //x_{n-2}=x0,x_{n-1}=x1,x_n
        if(R >0)
        {
            xn2=1.005;
            xn1=1.01;
        }
        else
        {
            xn2=0.995;
            xn1=0.99;
            R=-R;
        }
        //secant method at most 1000 iterations
        double tol = 1e-12;
        double f=1.0;
        for(int i =0;i<1000 && f/R>tol;i++)
        {
            xn = xn1 - secF(R,pac,xn1) *
                    (xn1-xn2) /
                    (secF(R,pac,xn1)-secF(R,pac,xn2));
            f=secF(R,pac,xn);
            xn2=xn1;
            xn1=xn;
            //std::cout << "iter:" << i <<", f=" << f << ", xn=" << xn << std::endl;

        }
        double w[3],midp[3];
        //midp
        vtkMath::Add(pc0,pc1,midp);
        vtkMath::MultiplyScalar(midp,0.5);

        //w
        vtkMath::Subtract(midp,pac,w);
        vtkMath::MultiplyScalar(w,xn);

        //arcp_n
        vtkMath::Add(pac,w,arcp);
        //std::cout << "radius is: " << secF(R,pac,xn)+R<<std::endl;
    }

    //std::cout << "arc (" << arcp[0] <<" " << arcp[1] <<" " << arcp[2] <<")" << std::endl;
    setControlPoint(0,arcp);
}

double HexEdge::secF(const double R, const double pac[3],const double xn )
{
    double f,pc0[3], pc1[3],midp[3],w[3],arcpn[3],rn[3],cn[3];
    globalVertices->GetPoint(vertIds->GetId(0),pc0); //tail
    globalVertices->GetPoint(vertIds->GetId(1),pc1); //head

    //midp
    vtkMath::Add(pc0,pc1,midp);
    vtkMath::MultiplyScalar(midp,0.5);

    //w
    vtkMath::Subtract(midp,pac,w);
    vtkMath::MultiplyScalar(w,xn);

    //arcp_n
    vtkMath::Add(pac,w,arcpn);

    //cn
    vtkMath::Solve3PointCircle(pc0,arcpn,pc1,cn);

    //r_n
    vtkMath::Subtract(arcpn,cn,rn);

    //f
    f=vtkMath::Norm(rn)-R;

    return f;
}

//void HexEdge::getControlPoints(vtkPoints* pnts,
//                               vtkIdList* ctrlPsIds)
//{
////    pnts = vtkPoints::SafeDownrCast(myPoints);
////    ctrlPsIds = vtkIdList::SafeDownCast(cntrlPointsIds);
//    pnts->ShallowCopy(myPoints.GetPointer());
//    std::cout << "from edge numids" << cntrlPointsIds->GetNumberOfIds() << std::endl;
//    ctrlPsIds = cntrlPointsIds;
//}


int HexEdge::getNumberOfControlPoints()
{
    if(edgeType==LINE)
        return 0;
    else//i.e. arc
        return 1;
}
