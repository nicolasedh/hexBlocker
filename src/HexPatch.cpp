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

#include "HexPatch.h"
#include <vtkObjectFactory.h>

#include <HexBlock.h>
#include <vtkIdList.h>
#include <vtkPoints.h>
#include <vtkQuad.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkMath.h>


vtkStandardNewMacro(HexPatch);

HexPatch::HexPatch()
{

    vertIds = vtkSmartPointer<vtkIdList>::New();
    vertIds->SetNumberOfIds(4);

    quad = vtkSmartPointer<vtkQuad>::New();
    quads = vtkSmartPointer<vtkCellArray>::New();
    data = vtkSmartPointer<vtkPolyData>::New();
    mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor  = vtkSmartPointer<vtkActor>::New();

    hasPrimaryHex = false;
    hasSecondaryHex = false;
}

HexPatch::~HexPatch()
{

}

void HexPatch::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "verts:" ;
    for(vtkIdType i=0;i<4;i++)
        os << indent << vertIds->GetId(i);
    os << "." << std::endl;
}

void HexPatch::orderVertices(){
    //use vtkMath to calc normal and crossproducts
}

bool HexPatch::equals(vtkSmartPointer<HexPatch> other)
{

    return equals(other->vertIds);

}

bool HexPatch::equals(vtkSmartPointer<vtkIdList> otherIds)
{
    bool IHaveOtherVert[4];

//    std::cout << "Testing (true is"<< true <<"): ";
    for(vtkIdType i=0;i<4;i++)
    {
     IHaveOtherVert[i]=false;
     for(vtkIdType j=0;j<4;j++)
     {
//         std::cout << "(" << this->vertIds->GetId(i) <<","<< otherIds->GetId(j)<<")";
      if(this->vertIds->GetId(i) == otherIds->GetId(j))
          IHaveOtherVert[i]=true;
     }
//     std::cout <<"\n\t";
    }

//    std::cout << "\n\tresult: "<< (IHaveOtherVert[0] && IHaveOtherVert[1] && IHaveOtherVert[2] && IHaveOtherVert[3] )<< std::endl;
    return bool(IHaveOtherVert[0] && IHaveOtherVert[1] && IHaveOtherVert[2] && IHaveOtherVert[3]);
}

void HexPatch::init(vtkSmartPointer<vtkIdList> vIds,
                    vtkSmartPointer<vtkPoints> verts, vtkSmartPointer<HexBlock> hex)
{
    hasPrimaryHex=true;
    primaryHex=hex;
    globalVertices = verts;
    vertIds=vIds;
    for(vtkIdType i=0; i<4 ;i++)
        quad->GetPointIds()->SetId(i,vIds->GetId(i));
    quads->Allocate(1,1);
    quads->InsertNextCell(quad);

    data->SetPoints(globalVertices);
    data->SetPolys(quads);

    mapper->SetInput(data);

    actor->SetMapper(mapper);
    actor->SetOrigin(actor->GetCenter());
    actor->SetScale(0.6);
    actor->GetProperty()->EdgeVisibilityOn();
    resetColor();
}

void HexPatch::setColor(double r, double g, double b)
{
    actor->GetProperty()->SetColor(r,b,g);

}

void HexPatch::resetColor()
{
    if(!hasSecondaryHex)
    {
        actor->GetProperty()->SetColor(0.2,0.9,0.2);
        actor->GetProperty()->SetOpacity(1.0);
    }
    else
    {
        actor->GetProperty()->SetColor(0.2,0.2,0.9);
        actor->GetProperty()->SetOpacity(0.5);
    }
}

void HexPatch::exportVertIds(QTextStream &os)
{
    os << "(" << vertIds->GetId(0) << " "
       << vertIds->GetId(1) << " "
       << vertIds->GetId(2) << " "
       << vertIds->GetId(3) << ")" << endl;
}

void HexPatch::getNormal(double n[3])
{
    double v0[3],v1[3],v3[3],x[3],y[3];
    globalVertices->GetPoint(vertIds->GetId(0),v0);
    globalVertices->GetPoint(vertIds->GetId(1),v1);
    globalVertices->GetPoint(vertIds->GetId(3),v3);

    vtkMath::Subtract(v1,v0,x);
    vtkMath::Subtract(v3,v0,y);

    vtkMath::Cross(x,y,n);

    if(hasPrimaryHex)
    {
        //The normal is returned as pointing away from primary block center
        double boxc[3],patchc[3],boxcTopatchc[3];
        primaryHex->getCenter(boxc);
        HexPatch::getCenter(patchc);

        vtkMath::Subtract(patchc,boxc,boxcTopatchc);
//        std::cout << "boxcenter: " << boxc[0] << " "<<  boxc[1] << " " << boxc[2] << "\n"
//                  << ", normal1: " << n[0] << " " << n[1] << " " << n[2]
//                  << ", dot: " << vtkMath::Dot(n,boxcTopatchc) << std::endl;

        if(vtkMath::Dot(n,boxcTopatchc)<0)
            vtkMath::MultiplyScalar(n,-1);

//        std::cout << ", normal2: " << n[0] << " " << n[1] << " " << n[2] << std::endl
//                  << ", ctoc: " << boxcTopatchc[0] <<" "<< boxcTopatchc[1] << " "<< boxcTopatchc[2];

    }
    vtkMath::Normalize(n);
/*
    std::cout << "n =(" << n[0]  <<" " << n[1]  << " " << n[2]  << "), " << std::endl
              << "v0=(" << v0[0] <<" " << v0[1] << " " << v0[2] << "), " << std::endl
              << "v1=(" << v1[0] <<" " << v1[1] << " " << v1[2] << "), " << std::endl
              << "v3=(" << v3[0] <<" " << v3[1] << " " << v3[2] << "), " << std::endl
              << "x =(" << x[0]  <<" " << x[1] << " "  << x[2] << "), "  << std::endl
              << "y =(" << y[0]  <<" " << y[1] << " "  << y[2] << "), "  << std::endl;
*/
}
void HexPatch::getCenter(double c[3])
{
    double pos[3];
    c[0]=0.0;c[1]=0.0;c[2]=0.0;
    for(vtkIdType i=0;i<4;i++)
    {
        globalVertices->GetPoint(vertIds->GetId(i),pos);
        vtkMath::Add(c,pos,c);

//        center[0]+=pos[0];
//        center[1]+=pos[1];
//        center[2]+=pos[2];
//        std::cout << "boxcenter0: " << center[0] << " "<<  center[1] << " " << center[2] << std::endl;
    }
    vtkMath::MultiplyScalar(c,0.25);
}

void HexPatch::setHex(HexBlock *hex)
{
    //error check?
    if (!hasPrimaryHex)
        primaryHex=hex;
    else //
    {
        //std::cout << "setting secondary" <<  std::endl;
        secondaryHex=hex;
        actor->SetOrigin(actor->GetCenter());
        actor->SetScale(0.4);
        hasSecondaryHex=true;
        resetColor();
    }

}

HexBlock* HexPatch::getPrimaryHexBlock()
{
    if(hasPrimaryHex)
        return primaryHex;
    else
    {
         std::cout << "Error: patch (" << vertIds->GetId(0) << " "
                  << vertIds->GetId(1) << " "
                  << vertIds->GetId(2) << " "
                  << vertIds->GetId(3) << ")"
                  << "was asked for a hexblock witch it doesn't have" << std::endl;
        return primaryHex;
    }
}

HexBlock* HexPatch::getSecondaryHexBlock()
{
    if(hasSecondaryHex)
        return secondaryHex;
    else
    {
        std::cout << "Error: patch (" << vertIds->GetId(0) << " "
                  << vertIds->GetId(1) << " "
                  << vertIds->GetId(2) << " "
                  << vertIds->GetId(3) << ")"
                  << "was asked for a hexblock witch it doesn't have" << std::endl;
        return secondaryHex;
    }
}


void HexPatch::rescaleActor()
{
    double cog[3];
    cog[0]=0.0;cog[1]=0.0;cog[2]=0.0;
    for(vtkIdType i=0;i<vertIds->GetNumberOfIds();i++)
    {
        double pos[3];
        globalVertices->GetPoint(vertIds->GetId(i),pos);
        vtkMath::Add(cog,pos,cog);
    }
    vtkMath::MultiplyScalar(cog,0.25);
//    std::cout << "centers is (" << cog[0] <<"," << cog[1] <<"," << cog[2] << ")" << std::endl;
    actor->SetOrigin(cog);

    if(hasSecondaryHex)
        actor->SetScale(0.4);
    else
        actor->SetScale(0.6);
}

void HexPatch::changeVertId(vtkIdType from, vtkIdType to)
{
    vtkIdType pos = vertIds->IsId(from);
    if(pos >= 0)
    {
//        std::cout<<" before("
//                 << vertIds->GetId(0) << "," << vertIds->GetId(1) << ","
//                 << vertIds->GetId(2) << "," << vertIds->GetId(3) << ") after (";
        vertIds->SetId(pos,to);
        quad->GetPointIds()->SetId(pos,to);
        vtkIdType pts[4];
        pts[0]=vertIds->GetId(0);
        pts[1]=vertIds->GetId(1);
        pts[2]=vertIds->GetId(2);
        pts[3]=vertIds->GetId(3);
        quads->ReplaceCell(0,4,pts);
//        std::cout << vertIds->GetId(0) << "," << vertIds->GetId(1) << ","
//        << vertIds->GetId(2) << "," << vertIds->GetId(3) << ")" << std::endl;
//        data->Modified();
//        rescaleActor();

    }
}

void HexPatch::reduceVertId(vtkIdType vId)
{
    for(vtkIdType i=0;i<vertIds->GetNumberOfIds();i++)
    {
        vtkIdType oldId = vertIds->GetId(i);
        if(oldId > vId)
            vertIds->SetId(i,oldId-1);
    }
    vtkIdType pts[4];
    pts[0]=vertIds->GetId(0);
    pts[1]=vertIds->GetId(1);
    pts[2]=vertIds->GetId(2);
    pts[3]=vertIds->GetId(3);
    quads->ReplaceCell(0,4,pts);
}

void HexPatch::removeSafely(HexBlock * hex)
{
//    std::cout << "calld on patch (" << vertIds->GetId(0) << " "
//              << vertIds->GetId(1) << " "
//              << vertIds->GetId(2) << " "
//              << vertIds->GetId(3) << ")" << std::endl;
    if(hasSecondaryHex)
    {
        if(primaryHex->equals(hex) || secondaryHex->equals(hex))
        {
            if(primaryHex->equals(hex))
            {
                   primaryHex=secondaryHex;
            }
            hasSecondaryHex=false;
            secondaryHex=0;
            resetColor();
            rescaleActor();
        }
        else
        {
            //don't do anything
            return;
        }
    }
          //but not secondary
    else if(hasPrimaryHex && primaryHex->equals(hex))
    {
        //This patch will get deleted. The smartpointers should clean up.
        hasPrimaryHex=false;
        primaryHex=0;
    }
    else //if we havn't been initialized
        return;
}

bool HexPatch::hasVertice(vtkIdType vId)
{
    return vertIds->IsId(vId) > -1;
}

bool HexPatch::hasBlock(HexBlock *hb)
{
    if(hasSecondaryHex)
        return (primaryHex->equals(hb) || secondaryHex->equals(hb));
    else if(hasPrimaryHex)
        return primaryHex->equals(hb);
    else
        return false;
}

bool HexPatch::hasBlocks()
{
    return hasPrimaryHex;
}
