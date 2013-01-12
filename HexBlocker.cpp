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


#include <limits>
#include "HexBlocker.h"
#include "HexBlock.h"
#include "hexPatch.h"
#include "HexEdge.h"
#include "HexBC.h"
#include "HexReader.h"

#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>

#include <vtkCollection.h>
#include <vtkIdList.h> //Ta bort?
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkPlaneSource.h>

#include <vtkCellArray.h>
#include <vtkQuad.h>
#include <vtkIdList.h>

#include <vtkMath.h>

#include <vtkLabeledDataMapper.h>
#include <vtkActor2D.h>
#include <vtkProperty2D.h>


HexBlocker::HexBlocker()
{
    //All vertices in the modell
    vertices = vtkSmartPointer<vtkPoints>::New();
    vertData = vtkSmartPointer<vtkPolyData>::New();
    vertData->SetPoints(vertices);

    //All patches in the model
    patches = vtkSmartPointer<vtkCollection>::New();

    //All edges in the model
    edges = vtkSmartPointer<vtkCollection>::New();

    //All hexblocks in the modell
    hexBlocks = vtkSmartPointer<vtkCollection>::New();

    //Boundary conditions in the modell
    hexBCs = vtkSmartPointer<vtkCollection>::New();

    //Representations
    vertSphere = vtkSmartPointer<vtkSphereSource>::New();
    vertSphere->SetThetaResolution(10);
    vertSphere->SetPhiResolution(10);

    vertGlyph  = vtkSmartPointer<vtkGlyph3D>::New();
    vertGlyph->SetInput(vertData);
    vertGlyph->SetSourceConnection(vertSphere->GetOutputPort());

    vertMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    vertMapper->SetInputConnection(vertGlyph->GetOutputPort());

    vertActor = vtkSmartPointer<vtkActor>::New();
    vertActor->SetMapper(vertMapper);

    vertLabelMapper = vtkSmartPointer<vtkLabeledDataMapper>::New();
    vertLabelMapper->SetInput(vertData);

    vertLabelActor = vtkSmartPointer<vtkActor2D>::New();
    vertLabelActor->SetMapper(vertLabelMapper);
    vertLabelActor->GetProperty()->SetColor(1,0,0);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(vertActor);
    renderer->AddActor(vertLabelActor);
}

HexBlocker::~HexBlocker()
{

}

//obsolete
void HexBlocker::createHexBlock()
{
    int numBlocks= hexBlocks->GetNumberOfItems();

    double c0[3]={0.0+2*numBlocks,0.0,0.0};
    double c1[3]={1.0+2.1*numBlocks,1.0,1.0};

    createHexBlock(c0,c1);

}

void HexBlocker::createHexBlock(double c0[3], double c1[3])
{
    vtkIdType numEdges = edges->GetNumberOfItems();
    vtkIdType numPatches= patches->GetNumberOfItems();
    vtkSmartPointer<HexBlock> hex=
            vtkSmartPointer<HexBlock>::New();

    int numBlocks= hexBlocks->GetNumberOfItems();


    hex->init(c0,c1,vertices,edges, patches);

    hexBlocks->AddItem(hex);

    //add edge actors renderer, but not already added ones.
    for (vtkIdType i =numEdges;i<edges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        renderer->AddActor(e->actor);
    }

    //add patch actors to renderer, but not already added ones.
    for(vtkIdType i=numPatches;i<patches->GetNumberOfItems();i++)
    {
        hexPatch *p = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        renderer->AddActor(p->actor);
    }

    renderer->AddActor(hex->hexActor);
    vertices->Modified();
    resetBounds();

    renderer->Render();
}

void HexBlocker::extrudePatch(vtkIdList *selectedPatches, double dist)
{
    if(selectedPatches->GetNumberOfIds()<1)
    {
        std::cout << "no patches selected!" << std::endl;
        return;
    }

    vtkIdType numEdges = edges->GetNumberOfItems();
    vtkIdType numPatches = patches->GetNumberOfItems();

    hexPatch * p =
            hexPatch::SafeDownCast(
                patches->GetItemAsObject(selectedPatches->GetId(0)));
    HexBlock * hex = p->getPrimaryHexBlock();

    /*
    std::cout << "I'am block " << hexBlocks->IsItemPresent(hex) -1 << " patch: "
              << hex->getPatchInternalId(p)
              << ", verts are ("
              << p->vertIds->GetId(0) << " "
              << p->vertIds->GetId(1) << " "
              << p->vertIds->GetId(2) << " "
              << p->vertIds->GetId(3) << ")"<<std::endl;
    */

    vtkSmartPointer<HexBlock> newHex=
            vtkSmartPointer<HexBlock>::New();
    newHex->init(p,dist,vertices,edges,patches);

    hexBlocks->AddItem(newHex);
    vertices->Modified();

    //add edge actors to renderer, but not already added ones.
    for (vtkIdType i =numEdges;i<edges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        renderer->AddActor(e->actor);
    }

    //add patch actors to renderer, but not already added ones.
    for(vtkIdType i=numPatches;i<patches->GetNumberOfItems();i++)
    {
        hexPatch *p = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        renderer->AddActor(p->actor);
    }

    this->resetBounds();
    renderer->AddActor(newHex->hexActor);
    renderer->Render();


}

void HexBlocker::resetBounds()
{


    double minDiag=1e6;
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * hex =
                HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        double c1[3], c2[3];
        vertices->GetPoint(hex->vertIds->GetId(0),c1);
        vertices->GetPoint(hex->vertIds->GetId(6),c2);
        double diag = pow(vtkMath::Distance2BetweenPoints(c1,c2),0.5);
        if(i==0)
            minDiag=diag;
        fmin(minDiag,diag);

    }

    double vertRadius=minDiag*0.02;
    double edgeRadius=vertRadius*0.4;
    double locAxesRadius=edgeRadius*4;

    vertSphere->SetRadius(vertRadius);
    //set radius on edges
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        e->setRadius(edgeRadius);
        e->resetColor();
    }

    //set radius on local axes
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        hb->setAxesRadius(locAxesRadius);
    }

    double bounds[6];
    renderer->ComputeVisiblePropBounds(bounds);
    renderer->ResetCamera(bounds);
    renderer->Modified();


}
void HexBlocker::resetColors()
{
    //patches
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
        hexPatch::SafeDownCast(patches->GetItemAsObject(i))->resetColor();
    //edges
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
        HexEdge::SafeDownCast(edges->GetItemAsObject(i))->resetColor();
}

void HexBlocker::PrintHexBlocks()
{

    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        std::cout << "Hexblock " << i << ":"
                  <<"\t"<< "Vertices Id and posin global list: " <<std::endl;
        HexBlock * hex= HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        double pos[3];
        for(vtkIdType j=0;j<hex->vertIds->GetNumberOfIds();j++)
        {
            vertices->GetPoint(hex->vertIds->GetId(j),pos);
            std::cout << hex->vertIds->GetId(j) << " ("
                      << pos[0] << " " << pos[1] << " " << pos[2] << " )." << std::endl;
        }
        std::cout<<std::endl;

    }

    std::cout << "There are " << patches->GetNumberOfItems() <<" patches." <<std::endl;


    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        hexPatch * patch = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        std::cout << "Patch" << i <<" ids: ";
        for(vtkIdType j=0;j<4;j++)
            std::cout << patch->vertIds->GetId(j) << " ";
        std::cout << std::endl;
    }

    std::cout <<"Boundary Conditions:" <<std::endl;

    for(vtkIdType i = 0;i<hexBCs->GetNumberOfItems();i++)
    {

        HexBC * bc = HexBC::SafeDownCast(hexBCs->GetItemAsObject(i));
        std::cout << "\t BC " << i << ": " << bc->name << ", "
                  << bc->type << ", ";
        for(vtkIdType j = 0;j<bc->patchIds->GetNumberOfIds();j++)
            std::cout << bc->patchIds->GetId(j) << " ";
        std::cout << std::endl;
    }

    std::cout << "Edges: " << edges->GetNumberOfItems() << std::endl;

    for(vtkIdType i= 0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        std::cout << "\t e" << i <<": ("
                  << e->vertIds->GetId(0) << ", "
                  << e->vertIds->GetId(1) << ") nCells "
                  << e->nCells
                  << std::endl;
    }

}

void HexBlocker::exportVertices(QTextStream &os)
{
    os << endl << "vertices" <<endl << "(" <<endl;
    for(vtkIdType i=0;i<vertices->GetNumberOfPoints();i++)
    {
        double x[3];
        vertices->GetPoint(i,x);
        os << "\t(" << x[0] <<", " << x[1] << ", " << x[2] <<") //"<<i <<endl;
    }
    os << endl <<");" << endl;
}

void HexBlocker::exportBlocks(QTextStream &os)
{
    os << "blocks" << endl << "(" << endl;

    for(vtkIdType i=0; i<hexBlocks->GetNumberOfItems(); i++)
    {
        HexBlock *hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        os << "\t hex (";
        for(vtkIdType j=0; j<hb->vertIds->GetNumberOfIds();j++)
        {
            os << hb->vertIds->GetId(j);
            if(j < hb->vertIds->GetNumberOfIds()-1)
                os << " ";
            else
                os << ") ";
        }

        int nCells[3];
        hb->getNumberOfCells(nCells);
        os << "("
           << nCells[0] <<" "<< nCells[1]<<" " << nCells[2]
           << ") simpleGrading (1 1 1) " << endl;

    }
    os << endl << ");" << endl;
}

void HexBlocker::exportBCs(QTextStream &os)
{
    os << "boundary" << endl << "(" << endl;
    HexBC *bc;
    for(vtkIdType i = 0;i<hexBCs->GetNumberOfItems();i++)
    {
        bc = HexBC::SafeDownCast(hexBCs->GetItemAsObject(i));
        QString name = QString::fromUtf8(bc->name.c_str());
        QString type = QString::fromUtf8(bc->type.c_str());
        os << "\t" << name << endl
           << "\t{" <<endl
           << "\t\ttype\t" << type <<";" << endl
           << "\t\tfaces\t" << endl
           << "\t\t(" << endl;
        vtkSmartPointer<hexPatch> p;
        for(vtkIdType j=0; j<bc->patchIds->GetNumberOfIds();j++)
        {
            p = hexPatch::SafeDownCast(patches->GetItemAsObject(bc->patchIds->GetId(j)));
            os << "\t\t\t";
            p->exportVertIds(os);
        }
        os << "\t\t);" << endl<<"\t}"<<endl;
    }
    os << ");" <<endl;
}


void HexBlocker::moveVertices(vtkSmartPointer<vtkIdList> ids,double dist[3])
{
    double pos[3];
    for(vtkIdType i = 0;i<ids->GetNumberOfIds();i++)
    {
//        std::cout << "moving point " << ids->GetId(i) << " (";
        vertices->GetPoint(ids->GetId(i),pos);
//        std::cout << pos[0] << " " << pos[1] << " " << pos[2] << "), (";
        pos[0]+=dist[0];
        pos[1]+=dist[1];
        pos[2]+=dist[2];
//        std::cout << pos[0] << " " << pos[1] << " " << pos[2] << ")." << std::endl;
        vertices->SetPoint(ids->GetId(i),pos);

        vertices->Modified();
    }
    rescaleActors();
}

void HexBlocker::setVerticesPos(vtkSmartPointer<vtkIdList> ids, double newPos[3], bool setPos[3])
{
    double pos[3];
    for(vtkIdType i = 0;i<ids->GetNumberOfIds();i++)
    {
//        std::cout << "setting point " << ids->GetId(i) << " (";
        vertices->GetPoint(ids->GetId(i),pos);
//        std::cout << pos[0] << " " << pos[1] << " " << pos[2] << "), (";
        if(setPos[0])
            pos[0]=newPos[0];
        if(setPos[1])
            pos[1]=newPos[1];
        if(setPos[2])
            pos[2]=newPos[2];
//        std::cout << pos[0] << " " << pos[1] << " " << pos[2] << ")." << std::endl;
        vertices->SetPoint(ids->GetId(i),pos);

        vertices->Modified();
    }
    rescaleActors();
}




int HexBlocker::showParallelEdges(vtkIdType edgeId)
{
    vtkSmartPointer<vtkIdList> allParallelEdges =
            vtkSmartPointer<vtkIdList>::New();

    addParallelEdges(allParallelEdges,edgeId);

    int nCells=-1;
    //color the selectedEdges
    for(vtkIdType i=0;i<allParallelEdges->GetNumberOfIds();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(
                    edges->GetItemAsObject(allParallelEdges->GetId(i)));
        e->setColor(0.0,8.0,0.0);
        if(i==0)
            nCells=e->nCells;
    }

    return nCells;
}

void HexBlocker::addParallelEdges(vtkSmartPointer<vtkIdList> allParallelEdges,
                                  vtkIdType edgeId)
{
//    std::cout << "running on edge " << edgeId << std::endl;
    vtkIdType numEdges = allParallelEdges->GetNumberOfIds();

    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        vtkSmartPointer<vtkIdList> parallelOnBlock =
                vtkSmartPointer<vtkIdList>::New();
        HexBlock * b = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        parallelOnBlock = b->getParallelEdges(edgeId);

        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(edgeId));
//        std::cout << " block num: " << i << ", numPar: "
//                  << parallelOnBlock->GetNumberOfIds()
//                  <<" to edge " << edgeId
//                 << "(" << e->vertIds->GetId(0) << " "
//                 << e->vertIds->GetId(1) <<")"
//                 << std::endl;
        //insert the new edges if unique
        for(vtkIdType j=0;j<parallelOnBlock->GetNumberOfIds();j++)
        {
//            std::cout << "\tinserting edge " << parallelOnBlock->GetId(j)<<std::endl;
            allParallelEdges->InsertUniqueId(parallelOnBlock->GetId(j));
        }
    }

    //if we added more edges we have to redo for those edges.
    vtkIdType newNumEdges = allParallelEdges->GetNumberOfIds();
    for(vtkIdType i=numEdges;i<newNumEdges;i++)
    {
//        std::cout << "\trerunning on " << i <<", " << allParallelEdges->GetId(i) <<std::endl;
        addParallelEdges(allParallelEdges,allParallelEdges->GetId(i));
    }
}


void HexBlocker::setNumberOnParallelEdges(vtkIdType edgeId, int nCells)
{
    vtkSmartPointer<vtkIdList> allParallelEdges =
            vtkSmartPointer<vtkIdList>::New();

    addParallelEdges(allParallelEdges,edgeId);


    //set number on selected Edges
    for(vtkIdType i=0;i<allParallelEdges->GetNumberOfIds();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(
                    edges->GetItemAsObject(allParallelEdges->GetId(i)));
        e->resetColor();
        e->nCells=nCells;
    }
}


void HexBlocker::readBlockMeshDict(HexReader *reader)
{

    //Clear everything first!
    vertices=reader->readVertices;
    hexBlocks = reader->readBlocks;
    edges = reader->readEdges;
    patches = reader->readPatches;
    hexBCs = reader->readBCs;

    vertData->SetPoints(vertices);
    vertices->Modified();

    //add edge actors renderer
    for (vtkIdType i =0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        renderer->AddActor(e->actor);
    }

    //add patch actors to renderer
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        hexPatch *p = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        renderer->AddActor(p->actor);
    }

    // add local coord axis actor of blocks
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock *b = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        renderer->AddActor(b->hexActor);
    }


    resetBounds();
    renderer->Modified();
    renderer->Render();
}

int HexBlocker::calculateTotalNumberOfCells()
{
    long int totNumCells=0;
    for(vtkIdType i = 0; i< hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * block = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        int bNCells[3];
        block->getNumberOfCells(bNCells);
        totNumCells += bNCells[0]*bNCells[1]*bNCells[2];
    }
    return totNumCells;
}

void HexBlocker::rescaleActors()
{
    //edges presently dont need to be reset
    //patches
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        hexPatch * p = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        p->rescaleActor();
    }

    //blocks (local axes)
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * b = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        b->rescaleActor();
    }

    renderer->Render();
}

bool HexBlocker::findEdge(const vtkIdType a, const vtkIdType b, HexEdge * edge)
{
    bool foundIt=false;
    vtkIdList * list = vtkIdList::New();
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        if(e->equals(list))
        {
            edge = e;
            foundIt = true;
            break;
        }
    }
    list->Delete();
    return foundIt;
}

void HexBlocker::mergePatch(vtkIdType masterPatch, vtkIdType slavePatch)
{
    hexPatch * master = hexPatch::SafeDownCast(patches->GetItemAsObject(masterPatch));
    hexPatch * slave = hexPatch::SafeDownCast(patches->GetItemAsObject(slavePatch));

    //check so that both don't have sec.hex
//    renderer->RemoveActor(slave->actor);
//    HexEdge * e0,e1,e2,e3;
//    vtkIdType a = slave->vertIds->GetId(0);
//    vtkIdType b = slave->vertIds->GetId(1);


}

void HexBlocker::arbitraryTest()
{
    //merge patch 2 och 8

    this->createHexBlock();
    double c1[3], c2[3];
    c1[0]=1.1;c1[1]=0.0;c1[2]=0.0;
    c2[0]=2.1;c2[1]=1.0;c2[2]=1.0;
    this->createHexBlock(c1,c2);

    mergePatch(2,8);
    //      vertices->SetNumberOfPoints(2);
//      vertices->InsertPoint(0,0.0,0.0,0.0);
//      vertices->InsertPoint(0,1.0,1.0,1.0);
//      vertData->SetPoints(vertices);
//      int numE=100;
//      std::cout << " creating" << std::endl;
//      for(vtkIdType i=0;i<numE;i++)
//      {
//          vtkSmartPointer<HexEdge> e =
//                  vtkSmartPointer<HexEdge>::New();
//          e->init(0,1,vertices);
//          edges->AddItem(e);
//          renderer->AddActor(e->actor);
////          renderer->Modified();
//          renderer->Render();
//          renderer->GetRenderWindow()->Render();
//          e.~vtkSmartPointerBase();
//      }

//      sleep(2);
//      std::cout << "deleting" << std::endl;
//      for(vtkIdType i=0;i<numE;i++)
//      {
//          HexEdge * de = HexEdge::SafeDownCast(edges->GetItemAsObject(0));
//          renderer->RemoveActor(de->actor);
//          edges->RemoveItem(0);
//      }
//      std::cout << "deleted all ok" <<  std::endl;




//    renderer->RemoveActor(p->actor);

//    patches->RemoveItem(p);
//    p->Delete();

//    std::cout << "creating blocks" << std::endl;
//    int maxbs=10;
//    for(vtkIdType i=0;i<maxbs;i++)
//    {
//        createHexBlock();
//        renderer->Render();

//    }
//    std::cout << "blocks created" << std::endl;
//    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
//    {
//        HexBlock *bl = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
//        renderer->RemoveActor(bl->hexActor);
//        hexBlocks->RemoveItem(i);
//    }
//    std::cout << "removedblocks?" << std::endl;

//    long int a = 6; //Max value 9223372036854775807
}


