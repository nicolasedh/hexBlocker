    /* This is the class that is in charge of storing all
   hexblocks, vertices, edges and faces

   */

#include "HexBlocker.h"
#include "HexBlock.h"
#include "hexPatch.h"
#include "HexEdge.h"
#include "HexBC.h"

#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>

#include <vtkCollection.h>
#include <vtkIdList.h> //Ta bort?
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

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
    vtkSmartPointer<HexBlock> hex=
            vtkSmartPointer<HexBlock>::New();

    int numBlocks= hexBlocks->GetNumberOfItems();


    hex->init(c0,c1,vertices,edges);

    initPatches(hex);
    hexBlocks->AddItem(hex);
    renderer->AddActor(hex->hexActor);
    vertices->Modified();
    //renderer->AddActor(hex->hexActor);
    HexBlocker::resetBounds();

    //add edge actors renderer, but not already added ones.
    for (vtkIdType i =numEdges;i<edges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        renderer->AddActor(e->actor);
    }


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

    vtkSmartPointer<hexPatch> p =
            hexPatch::SafeDownCast(
                patches->GetItemAsObject(selectedPatches->GetId(0)));
    vtkSmartPointer<HexBlock> hex = p->getPrimaryHexBlock();

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
    newHex->init(p,dist,vertices,edges);

    initPatches(newHex);
    hexBlocks->AddItem(newHex);
    vertices->Modified();

    //add edge actors renderer, but not already added ones.
    for (vtkIdType i =numEdges;i<edges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(edges->GetItemAsObject(i));
        renderer->AddActor(e->actor);
    }

    renderer->AddActor(newHex->hexActor);
    renderer->Render();


}

void HexBlocker::resetBounds()
{
    double minDiag=1e6;
    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        vtkSmartPointer<HexBlock> hex =
                HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
        double c1[3], c2[3];
        vertices->GetPoint(hex->vertIds->GetId(0),c1);
        vertices->GetPoint(hex->vertIds->GetId(6),c2);
        double diag = pow(vtkMath::Distance2BetweenPoints(c1,c2),0.5);
        if(i==0)
            minDiag=diag;
        fmin(minDiag,diag);

    }
    //Remember to calculate radius
    double bounds[6];
    renderer->ComputeVisiblePropBounds(bounds);

    vertSphere->SetRadius(minDiag*0.02);
    renderer->ResetCamera(bounds);
    renderer->Modified();

}
void HexBlocker::resetPatchesColor()
{
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
        hexPatch::SafeDownCast(patches->GetItemAsObject(i))->resetColor();
}

void HexBlocker::PrintHexBlocks()
{

    for(vtkIdType i=0;i<hexBlocks->GetNumberOfItems();i++)
    {
        std::cout << "Hexblock " << i << ":"
                  <<"\t"<< "Vertices Id and posin global list: " <<std::endl;
        vtkSmartPointer<HexBlock> hex= HexBlock::SafeDownCast(hexBlocks->GetItemAsObject(i));
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
        vtkSmartPointer<hexPatch> patch = hexPatch::SafeDownCast(patches->GetItemAsObject(i));
        std::cout << "Patch" << i <<" ids: ";
        for(vtkIdType j=0;j<4;j++)
            std::cout << patch->vertIds->GetId(j) << " ";
        std::cout << std::endl;
    }

    std::cout <<"Boundary Conditions:" <<std::endl;

    for(vtkIdType i = 0;i<hexBCs->GetNumberOfItems();i++)
    {

        vtkSmartPointer<HexBC> bc = HexBC::SafeDownCast(hexBCs->GetItemAsObject(i));
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

        os << "(10 10 10) simpleGrading (1 1 1) " << endl;

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

void HexBlocker::initPatches(vtkSmartPointer<HexBlock> hex)
{
    //insert patches like a dice
    //first and last patches define the hexblock.
    //order of points according to OF:
    //Normal is out of domain and according to right hand rule
    //when cycling vertices in patch
    //(dice #1)
    initPatch(hex,(int[4]){0,3,2,1});
    //(dice #2)
    initPatch(hex,(int[4]){0,1,5,4});
    //(dice #3)
    initPatch(hex,(int[4]){0,4,7,3});
    //(dice #4)
    initPatch(hex,(int[4]){1,2,6,5});
    //(dice #5)
    initPatch(hex,(int[4]){3,7,6,2});
    //(dice #6)
    initPatch(hex,(int[4]){4,5,6,7});

    patches->Modified();
}

void HexBlocker::initPatch(vtkSmartPointer<HexBlock> hex,int ids[4])
{
    vtkSmartPointer<hexPatch> patch = vtkSmartPointer<hexPatch>::New();
    vtkSmartPointer<vtkIdList> vlist = vtkSmartPointer<vtkIdList>::New();

    vlist->InsertId(0,hex->vertIds->GetId(ids[0]));
    vlist->InsertId(1,hex->vertIds->GetId(ids[1]));
    vlist->InsertId(2,hex->vertIds->GetId(ids[2]));
    vlist->InsertId(3,hex->vertIds->GetId(ids[3]));

    patch->init(vlist,vertices,hex);

    vtkIdType pId=isPatchInGlobalList(patch);
    if(pId < 0) //i.e. it doesn't exist in global list
    {
        renderer->AddActor(patch->actor);
        //add to global list
        patches->AddItem(patch);
        hex->globalPatches->AddItem(patch);
    }
    else
    {
        //patch already exist, only add reference to
        //the existing one, needed by extrude.
        //std::cout << "item is present" << std::endl;
        vtkSmartPointer<hexPatch> existingPatch =
                hexPatch::SafeDownCast(patches->GetItemAsObject(pId));
        existingPatch->setHex(hex);
        hex->globalPatches->AddItem(existingPatch);
    }
}


void HexBlocker::moveVertices(vtkSmartPointer<vtkIdList> ids,double dist[3])
{
    double pos[3];
    for(vtkIdType i = 0;i<ids->GetNumberOfIds();i++)
    {
        std::cout << "moving point " << ids->GetId(i) << " (";
        vertices->GetPoint(ids->GetId(i),pos);
        std::cout << pos[0] << " " << pos[1] << " " << pos[2] << "), (";
        pos[0]+=dist[0];
        pos[1]+=dist[1];
        pos[2]+=dist[2];
        std::cout << pos[0] << " " << pos[1] << " " << pos[2] << ")." << std::endl;
        vertices->SetPoint(ids->GetId(i),pos);

        vertices->Modified();
    }

}

vtkIdType HexBlocker::isPatchInGlobalList(vtkSmartPointer<hexPatch> p)
{
    for(vtkIdType i=0;i<patches->GetNumberOfItems();i++)
    {
        //std::cout << "checking with patch: " << i << std::endl;
        vtkSmartPointer<hexPatch> op =
                hexPatch::SafeDownCast(patches->GetItemAsObject(i) );
        if(op->operator ==(p))
        {
            //std::cout << "Patch" << i <<" already exist" << std::endl;
            return i;
        }
    }

    return -1;
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
