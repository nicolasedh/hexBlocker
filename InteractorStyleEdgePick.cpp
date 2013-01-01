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

#include "InteractorStyleEdgePick.h"
#include "HexEdge.h"
#include "HexBC.h"
#include <vtkCollection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <iostream>
#include <vtkIdList.h>
#include <vtkCollection.h>
#include <vtkProp3D.h>


vtkStandardNewMacro(InteractorStyleEdgePick);

InteractorStyleEdgePick::InteractorStyleEdgePick() : QObject()
{
//    selectedEdges = vtkIdList::New();
    globalEdges = vtkSmartPointer<vtkCollection>::New();

}

InteractorStyleEdgePick::~InteractorStyleEdgePick()
{

}

void InteractorStyleEdgePick::PrintSelf(ostream &os, vtkIndent indent)
{

}

void InteractorStyleEdgePick::OnChar()
{

}

void InteractorStyleEdgePick::SetEdges(vtkSmartPointer<vtkCollection> edgs)
{
    globalEdges=edgs;
}

vtkIdType InteractorStyleEdgePick::findClickedEdge(int x,int y)
{
    this->FindPokedRenderer(x, y);
    this->FindPickedActor(x, y);

    vtkIdType clickedEdge=-1;

    for(vtkIdType i = 0; i< globalEdges->GetNumberOfItems();i++)
    {
        HexEdge *edge = HexEdge::SafeDownCast(globalEdges->GetItemAsObject(i));
        if(this->InteractionProp == edge->actor)
        {
            clickedEdge = i;
//            std::cout << "You selected edge " << clickedEdge << ", ( ";
//            for(vtkIdType j=0;j<2;j++)
//                std::cout << edge->vertIds->GetId(j) << " ";
//            std::cout <<")."<< std::endl;
        }
    }


    return clickedEdge;
}

void InteractorStyleEdgePick::OnLeftButtonDown()
{
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    vtkIdType clickedEdge = findClickedEdge(x,y);

    //return only if and edge was clicked
    if(clickedEdge > -1)
    {
        HexEdge * edge = HexEdge::SafeDownCast(globalEdges->GetItemAsObject(clickedEdge));
        edge->setColor(1.0,0,0);
        this->GetInteractor()->GetRenderWindow()->Render();
        emit selectionDone(clickedEdge);
    }
}

void InteractorStyleEdgePick::OnRightButtonUp()
{
//        int x = this->Interactor->GetEventPosition()[0];
//        int y = this->Interactor->GetEventPosition()[1];
//        vtkIdType clickedEdge = findClickedEdge(x,y);

//        if(selectedEdges->IsId(clickedEdge)>-1){

//            selectedEdges->DeleteId(clickedEdge);
//            HexEdge *edge = HexEdge::SafeDownCast(globalEdges->GetItemAsObject(clickedEdge));
//            edge->resetColor();
//        }
//        this->GetInteractor()->GetRenderWindow()->Render();
}

void InteractorStyleEdgePick::OnMiddleButtonUp()
{
//    if(selectedEdges->GetNumberOfIds()>0)
//    {
////        std::cout<< "emit signal here" << std::endl;
//        for(vtkIdType i = 0; i<selectedEdges->GetNumberOfIds();i++)
//        {
//            HexEdge * edge = HexEdge::SafeDownCast(globalEdges->GetItemAsObject(selectedEdges->GetId(i)));
//            edge->resetColor();
//        }
//        this->GetInteractor()->GetRenderWindow()->Render();
//        emit selectionDone(selectedEdges);
//        selectedEdges->Initialize();
//    }
    emit selectionDone(vtkIdType(-1));

}


