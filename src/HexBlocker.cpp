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


#include <limits>
#include "HexBlocker.h"
#include "HexBlock.h"
#include "HexPatch.h"
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

#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>


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


//    widget->SetInteractor( renderer->GetRenderWindow()->GetInteractor() );

//    widget->SetEnabled( 1 );


}

HexBlocker::~HexBlocker()
{

}

void HexBlocker::initOrientationAxes(vtkRenderWindow *renwin)
{
    // Axes interactor and widget
    orientationAxes = vtkSmartPointer<vtkAxesActor>::New();
    orientationAxesWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    orientationAxesWidget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
    orientationAxesWidget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    orientationAxesWidget->SetOrientationMarker( orientationAxes );
    orientationAxesWidget->SetInteractor(renwin->GetInteractor());
    orientationAxesWidget->SetEnabled(1);
    orientationAxesWidget->InteractiveOff();


}

void HexBlocker::removeOrientationAxes()
{
    orientationAxesWidget->Off();

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
        HexPatch *p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
        renderer->AddActor(p->actor);
    }

    renderer->AddActor(hex->hexAxisActor);
    renderer->AddActor(hex->hexBlockActor);
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

    HexPatch * p =
            HexPatch::SafeDownCast(
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
        HexPatch *p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
        renderer->AddActor(p->actor);
    }

    this->resetBounds();
    renderer->AddActor(newHex->hexAxisActor);
    renderer->AddActor(newHex->hexBlockActor);
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
        HexPatch::SafeDownCast(patches->GetItemAsObject(i))->resetColor();
    //edges
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
        HexEdge::SafeDownCast(edges->GetItemAsObject(i))->resetColor();

    renderer->GetRenderWindow()->Render();
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
        HexPatch * patch = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
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
        os << "\t(" << x[0] <<" " << x[1] << " " << x[2] <<") //"<<i <<endl;
    }
    os << endl <<");" << endl;
}

void HexBlocker::exportBlocks(QTextStream &os)
{
    os << "blocks" << endl << "(" << endl;

    for(vtkIdType i=0; i<hexBlocks->GetNumberOfItems(); i++)
    {
        HexBlock *hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        hb->exportDict(os);

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
        vtkSmartPointer<HexPatch> p;
        for(vtkIdType j=0; j<bc->patchIds->GetNumberOfIds();j++)
        {
            p = HexPatch::SafeDownCast(patches->GetItemAsObject(bc->patchIds->GetId(j)));
            os << "\t\t\t";
            p->exportVertIds(os);
        }
        os << "\t\t);" << endl<<"\t}"<<endl;
    }
    os << ");" <<endl;
}

void HexBlocker::exportEdges(QTextStream &os)
{
    if(edgesDict.isEmpty())
    {
        os << "edges();" << endl;
    }
    else
    {
        os << edgesDict.toAscii().data() << endl;
    }
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




HexEdge *HexBlocker::showParallelEdges(vtkIdType edgeId)
{
    vtkSmartPointer<vtkIdList> allParallelEdges =
            vtkSmartPointer<vtkIdList>::New();

    allParallelEdges->InsertNextId(edgeId);
    addParallelEdges(allParallelEdges,edgeId);

    int nCells=-1;
    //color the selectedEdges
    for(vtkIdType i=0;i<allParallelEdges->GetNumberOfIds();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(
                    edges->GetItemAsObject(allParallelEdges->GetId(i)));
        if(i==0)
        {
            nCells=e->nCells;
            e->setColor(1.0,0.0,0.0);
        }
        else
        {
            e->setColor(0.0,8.0,0.0);
        }
    }

    return HexEdge::SafeDownCast(edges->GetItemAsObject(edgeId));
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


void HexBlocker::setEdgePropsOnParallelEdges(HexEdge * props,vtkIdType edgeId,int mode)
{
    vtkSmartPointer<vtkIdList> allParallelEdges =
            vtkSmartPointer<vtkIdList>::New();

    allParallelEdges->InsertUniqueId(edgeId);
    addParallelEdges(allParallelEdges,edgeId);


    //set number on selected Edges
    for(vtkIdType i=0;i<allParallelEdges->GetNumberOfIds();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(
                    edges->GetItemAsObject(allParallelEdges->GetId(i)));
        e->nCells=props->nCells;
        //set grading if mode is propagate or selected edge
        if(mode==0 || allParallelEdges->GetId(i)==edgeId)
            e->grading=props->grading;
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
        HexPatch *p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
        renderer->AddActor(p->actor);
    }

    // add local coord axis actor of blocks
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock *b = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        renderer->AddActor(b->hexAxisActor);
        renderer->AddActor(b->hexBlockActor);
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
        HexPatch * p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
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

vtkIdType HexBlocker::findEdge(const vtkIdType a, const vtkIdType b)
{
    vtkIdList * list = vtkIdList::New();
    list->InsertNextId(a);
    list->InsertNextId(b);
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        if(e->equals(list))
        {
            return i;
        }
    }
    //    list->Delete();
    return vtkIdType(-1);
}

void HexBlocker::mergePatch(vtkIdType masterId, vtkIdType slaveId)
{
    HexPatch * master = HexPatch::SafeDownCast(patches->GetItemAsObject(masterId));
    HexPatch * slave  = HexPatch::SafeDownCast(patches->GetItemAsObject(slaveId));

    //    vtkSmartPointer<HexPatch>  slave =
    //            vtkSmartPointer<HexPatch>::Take(HexPatch::SafeDownCast(patches->GetItemAsObject(slaveId)));
    //check so that both don't have sec.hex

    vtkIdList * slaveIds = vtkIdList::New();
    slaveIds->DeepCopy(slave->vertIds);

    //find the slave patch corresponing points
    //in master patch
    vtkSmartPointer<vtkPoints> masterVerts =
            vtkSmartPointer<vtkPoints>::New();

    // vtkPoints for master, needed for findPoint
    for(vtkIdType i=0;i<4;i++)
    {
        double vert[3];
        vertices->GetPoint(master->vertIds->GetId(i),vert);
        masterVerts->InsertNextPoint(vert);
    }
    vtkSmartPointer<vtkPolyData> masterData =
            vtkSmartPointer<vtkPolyData>::New();
    masterData->SetPoints(masterVerts);

    vtkIdList * slaveIdsInMaster = vtkIdList::New();
    for(vtkIdType i=0;i<4;i++)
    {
        double vert[3];
        vertices->GetPoint(slave->vertIds->GetId(i),vert);
        vtkIdType idInMasterData = masterData->FindPoint(vert);
        slaveIdsInMaster->InsertUniqueId(master->vertIds->GetId(idInMasterData));
//        std::cout << "m" << slaveIdsInMaster->GetId(i)
//                  << " s" << slaveIds->GetId(i) << std::endl;
    }

    if(slaveIdsInMaster->GetNumberOfIds() < 4)
    {
        std::cout << "error in merge patch,\n"
                  <<" atleast one vertice in slave patch is closest to\n"
                 << " more than one vertice in master patch\n" << std::endl;
        return;
    }

    //Unregister edges
    for (vtkIdType i=0;i<4;i++)
    {
        vtkIdType eId = findEdge(slaveIds->GetId(i),slaveIds->GetId( (i+1) % 4));
        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(eId));
        renderer->RemoveActor(e->actor);
        edges->RemoveItem(eId);
    }

    // CHANGING IDS of slave patch
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        for(vtkIdType k=0;k<4;k++)
        {
            e->changeVertId(
                        slaveIds->GetId(k),
                        slaveIdsInMaster->GetId(k));
        }
    }

    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        HexPatch * p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
        for(vtkIdType k=0;k<4;k++)
        {
            p->changeVertId(
                        slaveIds->GetId(k),
                        slaveIdsInMaster->GetId(k));
        }
        p->rescaleActor(); //Might have to bee removed
    }

    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        for(vtkIdType k=0;k<4;k++)
        {
            hb->changeVertId(
                        slaveIds->GetId(k),
                        slaveIdsInMaster->GetId(k));
        }
        hb->initEdges();
    }

    //Set hex in master patch
    master->setHex(slave->getPrimaryHexBlock());

    // Unregister slave patch
    //SLAVE PATCH
    renderer->RemoveActor(slave->actor);
    for(vtkIdType i=0;i<hexBCs->GetNumberOfItems();i++)
    {
        HexBC *bc = HexBC::SafeDownCast(hexBCs->GetItemAsObject(i));
        bc->removePatchFromList(master);
        bc->notifyRemovedPatch(slave);

    }
    patches->RemoveItem(slave);


    //re point patch ids in blocks
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock *hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        for(vtkIdType k=0;k<hb->patchIds->GetNumberOfIds();k++)
        {
            vtkIdType pId = hb->patchIds->GetId(k);
            if(pId > slaveId)
                hb->patchIds->SetId(k,pId-1);
        }
    }
    removeVerticesSafely(slaveIds);
    vertices->Modified();
    renderer->Render();
    renderer->GetRenderWindow()->Render();
}

void HexBlocker::decreaseList(vtkIdList *list, vtkIdList * toRemove)
{
    for(vtkIdType k=0;k<toRemove->GetNumberOfIds();k++)
    {
        vtkIdType toRem = toRemove->GetId(k);
        decreaseList(list,toRem);
        decreaseList(toRemove,toRem);

    }
}

void HexBlocker::decreaseList(vtkIdList *list, vtkIdType toRem)
{
    for(vtkIdType i=0;i<list->GetNumberOfIds();i++)
    {
        vtkIdType listId = list->GetId(i);
        if(listId>toRem)
            list->SetId(i,listId-1);
    }
}


bool HexBlocker::removeVerticeSafely(vtkIdType toRem )
{
    bool isItsafe = true;
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++ )
    {
        HexBlock *hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        vtkIdType hasId = hb->vertIds->IsId(toRem);
        //toRem is not in list if hasId == -1
        isItsafe = (isItsafe && hasId==-1);

    }
    if(isItsafe)
        removeVertice(toRem);

    return isItsafe;
}

void HexBlocker::removeVerticesSafely(vtkIdList *toRemove)
{
    for(vtkIdType i=0;i< toRemove->GetNumberOfIds();i++)
    {
        bool removed = removeVerticeSafely(toRemove->GetId(i));
        if(removed)
            decreaseList(toRemove,toRemove->GetId(i));
    }
}

void HexBlocker::removeVertices(vtkIdList *toRemove)
{
    //list cannot be empty
    vtkIdType prevId = toRemove->GetId(0);
    removeVertice(toRemove->GetId(0));
    for(vtkIdType i=1;i<toRemove->GetNumberOfIds();i++)
    {
        decreaseList(toRemove,prevId);
        prevId = toRemove->GetId(i);
        removeVertice(prevId);
    }

}

void HexBlocker::removeVertice(vtkIdType toRem)
{
    //Change Ids for existing Hex*
    for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
    {
        HexEdge * e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        e->reduceVertId(toRem);
    }
    //    vertData->Modified();
    //    renderer->Render();
    //    renderer->GetRenderWindow()->Render();

    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        HexPatch * p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
        p->reduceVertId(toRem);
    }

    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        hb->reduceVertId(toRem);
    }

    //DELETING VERTICE
    vtkSmartPointer<vtkPoints> newPs =
            vtkSmartPointer<vtkPoints>::New();
    newPs->SetNumberOfPoints(vertices->GetNumberOfPoints()-1);
    double pos[3];
    for(vtkIdType i=0;i<vertices->GetNumberOfPoints();i++)
    {
        vertices->GetPoint(i,pos);
        if(i < toRem)
        {
            newPs->InsertPoint(i,pos);
        }
        if(i > toRem)
        {
            newPs->InsertPoint(i-1,pos);
        }
    }

    vertices->Initialize();

    vertices->DeepCopy(newPs);

    //    vertices->Modified();
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        HexBlock * hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        hb->rescaleActor();
    }

}
void HexBlocker::removeHexBlock(vtkIdType toRem)
{
    vtkSmartPointer<vtkIdList> vertsToRem = vtkSmartPointer<vtkIdList>::New();
    removeHexBlock(toRem,vertsToRem);
}

void HexBlocker::removeHexBlock(vtkIdType toRem,vtkIdList * vertsToRem)
{
    HexBlock *b2rem = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(toRem));

    //Create a list of vertices that can be removed.
    //start with all verts of b2rem then remove those that are used by other blocks
    vertsToRem->Initialize();
    vertsToRem->DeepCopy(b2rem->vertIds);

    //remove vertices used by other blocks from vertsToRem
    for(vtkIdType v=0;v<b2rem->vertIds->GetNumberOfIds();v++)
    {
        vtkIdType vId = b2rem->vertIds->GetId(v);
        //loop over all blocks but not the one to be deleted
        for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
        {
            if(i!=toRem)
            {
                HexBlock *hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
                if(hb->hasVertice(vId))
                {
                    vertsToRem->DeleteId(vId);
                }
            }
        }
    }
//    std::cout << "verts to remove: " ;
//    for(vtkIdType i=0;i<vertsToRem->GetNumberOfIds();i++)
//    {
//        std::cout << vertsToRem->GetId(i) << " ";
//    }
//    std::cout << std::endl;

    //Find all patches and edges that has vertices in vertsToRem
    vtkSmartPointer<vtkIdList> patches2berem = vtkSmartPointer<vtkIdList>::New();
    vtkSmartPointer<vtkIdList> edges2berem = vtkSmartPointer<vtkIdList>::New();
    for(vtkIdType dI=0;dI<vertsToRem->GetNumberOfIds();dI++)
    {
        vtkIdType vert2rem = vertsToRem->GetId(dI);
        //find patches to remove (don't change the list while we are searching)
        for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
        {
            HexPatch *p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
            if(p->hasVertice(vert2rem))
            {
                patches2berem->InsertUniqueId(i);
                //remove from bc's
                for(vtkIdType bI;bI<hexBCs->GetNumberOfItems();bI++)
                {
                    HexBC * bc = HexBC::SafeDownCast(hexBCs->GetItemAsObject(bI));
                    bc->patchIds->DeleteId(i);
                }
            }
        }

        //edges
        for(vtkIdType i=0;i<edges->GetNumberOfItems();i++)
        {
            HexEdge * e= HexEdge::SafeDownCast(edges->GetItemAsObject(i));
            if(e->hasVertice(vert2rem))
                edges2berem->InsertUniqueId(i);
        }

    }

    //remove the patches
    for(vtkIdType i=0;i<patches2berem->GetNumberOfIds();i++)
    {
        HexPatch *p = HexPatch::SafeDownCast(patches->GetItemAsObject(patches2berem->GetId(i)));
        renderer->RemoveActor(p->actor);
        patches->RemoveItem(p);
        decreaseList(patches2berem,patches2berem->GetId(i));
    }


    //fix patches that have two hexes
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        HexPatch *p = HexPatch::SafeDownCast(patches->GetItemAsObject(i));
        p->removeSafely(b2rem);

    }

    //remove the edges
    for(vtkIdType i=0;i<edges2berem->GetNumberOfIds();i++)
    {
        HexEdge * e= HexEdge::SafeDownCast(edges->GetItemAsObject(edges2berem->GetId(i)));
        renderer->RemoveActor(e->actor);
        edges->RemoveItem(e);
        decreaseList(edges2berem,edges2berem->GetId(i));
    }

    //also degrease patchIds and edgeIds in all blocks not to be removed
    for(vtkIdType bi=0;bi<hexBlocks->GetNumberOfItems();bi++)
    {
        if(bi != toRem)
        {
            HexBlock *hb = HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(bi));
            decreaseList(hb->edgeIds,edges2berem);
            decreaseList(hb->patchIds,patches2berem);
        }
    }

    //delete block
    renderer->RemoveActor(b2rem->hexAxisActor);
    renderer->RemoveActor(b2rem->hexBlockActor);
    hexBlocks->RemoveItem(b2rem);
    //delete vertices

}

void HexBlocker::arbitraryTest()
{

    createHexBlock();
    vtkIdList *extrudelist= vtkIdList::New();
    extrudelist->InsertId(0,0);
    extrudePatch(extrudelist,1.0);
    extrudelist->SetId(0,6);
    extrudePatch(extrudelist,2.0);
    HexPatch * p = HexPatch::SafeDownCast(patches->GetItemAsObject(0));

    renderer->GetRenderWindow()->Render();
    std::cout << "removing" << std::endl;
//    removeHexBlock(0);
    vtkSmartPointer<vtkIdList> vertsToRemove =
            vtkSmartPointer<vtkIdList>::New();
    removeHexBlock(0,vertsToRemove);
    removeVerticesSafely(vertsToRemove);

    std::cout << "num blocks " << hexBlocks->GetNumberOfItems()
              << " num patches " << patches->GetNumberOfItems()
              << " num edges " << edges->GetNumberOfItems() << std::endl;
//    p->removeSafely(HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(0)));


    //    long int a = 6; //Max value 9223372036854775807
}


