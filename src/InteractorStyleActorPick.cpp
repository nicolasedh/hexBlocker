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

#include "InteractorStyleActorPick.h"
#include "HexBlocker.h"
#include "HexBlock.h"
#include "HexPatch.h"
#include "HexEdge.h"
//#include "HexBC.h"
#include <vtkCollection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <iostream>

#include <vtkIdList.h>
#include <vtkCollection.h>
#include <vtkProp3D.h>
#include <vtkProperty.h>
#include <vtkCellPicker.h>


vtkStandardNewMacro(InteractorStyleActorPick);

InteractorStyleActorPick::InteractorStyleActorPick() : QObject()
{
    selectionMode = multi;
    selectionType = block;
    hexBlocker=0;
    selectedIds = vtkSmartPointer<vtkIdList>::New();

    InteractionPicker = vtkSmartPointer<vtkCellPicker>::New();
    InteractionPicker->SetTolerance(0.001);
}

InteractorStyleActorPick::~InteractorStyleActorPick()
{

}

void InteractorStyleActorPick::PrintSelf(ostream &os, vtkIndent indent)
{

}

void InteractorStyleActorPick::OnChar()
{
    char k = this->GetInteractor()->GetKeyCode();
    if(k == ' ')
    {
        InteractorStyleActorPick::OnMiddleButtonUp();
    }
}

void InteractorStyleActorPick::setHexBlocker(HexBlocker *hexb)
{
    hexBlocker=hexb;
    this->SetCurrentRenderer(hexBlocker->renderer);
}

void InteractorStyleActorPick::setSelection(selectionTypes type, selectionModes mode)
{
    selectionType=type;
    selectionMode=mode;
    selectedIds->Initialize();
}

vtkIdType InteractorStyleActorPick::findClickedActorId()
{
    vtkIdType clickedActorId=-1;

    if(hexBlocker == 0)
        return clickedActorId;

    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    this->FindPokedRenderer(x, y);
    this->findPickedActor(x, y);

    switch(selectionType)
    {
    case block:
    {
        for(vtkIdType i = 0; i< hexBlocker->hexBlocks->GetNumberOfItems();i++)
        {
            HexBlock * hb = HexBlock::SafeDownCast(hexBlocker->hexBlocks->GetItemAsObject(i));
            if(this->InteractionProp == hb->hexBlockActor)
            {
                clickedActorId = i;
            }
        }
        break;
    }
    case patch:
    {
        for(vtkIdType i = 0; i< hexBlocker->patches->GetNumberOfItems();i++)
        {
            HexPatch *p = HexPatch::SafeDownCast(
                        hexBlocker->patches->GetItemAsObject(i));
            if(this->InteractionProp == p->actor)
            {
                clickedActorId = i;
            }
        }
        break;
    }
    case edge:
    {
        for(vtkIdType i = 0; i< hexBlocker->edges->GetNumberOfItems();i++)
        {
            HexEdge *e = HexEdge::SafeDownCast(
                        hexBlocker->edges->GetItemAsObject(i));
            if(this->InteractionProp == e->actor)
            {
                clickedActorId = i;
            }
        }
        break;
    }
    } //no need for default - will in that case return -1

    return clickedActorId;
}

bool InteractorStyleActorPick::didMouseMove()
{
    vtkRenderWindowInteractor *rwi = this->Interactor;
    int dx=rwi->GetEventPosition()[0] - lastXpos;
    int dy=rwi->GetEventPosition()[1] - lastYpos;
    double change = pow(dx,2) + pow(dy,2);
//    int * size = this->CurrentRenderer->GetRenderWindow()->GetSize()[0]
    return change > 5;
}

void InteractorStyleActorPick::storePosition()
{
    vtkRenderWindowInteractor *rwi = this->Interactor;
    lastXpos = rwi->GetEventPosition()[0];
    lastYpos = rwi->GetEventPosition()[1];
}

void InteractorStyleActorPick::OnLeftButtonDown()
{
    if (this->CurrentRenderer == NULL)
      return;
    storePosition();
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

void InteractorStyleActorPick::OnLeftButtonUp()
{
    if (this->CurrentRenderer == NULL)
        return;

    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();

    //only look for actor if the user din't rotate the model
    if(didMouseMove())
        return;

    vtkIdType clickedActor = findClickedActorId();

    if(clickedActor<0)
        return;

    vtkActor * act = vtkActor::SafeDownCast(this->InteractionProp);

    //return from function depending on mode
    switch(selectionMode)
    {
    case single:
    {
        if(selectedIds->GetNumberOfIds() >0)
            return;
    }
    case pair:
    {
        switch(selectedIds->GetNumberOfIds())
        {
        case 0:
        {
            break; //go below and set the color to red
        }
        case 1:
        {//Set blue color and return
            act->GetProperty()->SetColor(0.1,0.1,1);
            hexBlocker->render();
            selectedIds->InsertUniqueId(clickedActor);
            return;
        }
        default: //two are already selected so end the function
        {
            return;
        }
        }

        break;
    }
    default: //e.g. multi selection just set the color to blue
            break;
    }
    //set red color
    selectedIds->InsertUniqueId(clickedActor);
    act->GetProperty()->SetColor(1,0.1,0.1);
    hexBlocker->render();

}

void InteractorStyleActorPick::OnMouseMove()
{
    vtkInteractorStyleTrackballCamera::OnMouseMove();
}

void InteractorStyleActorPick::OnRightButtonDown()
{
    if (this->CurrentRenderer == NULL)
      return;
    storePosition();
    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
}

void InteractorStyleActorPick::OnRightButtonUp()
{
    if (this->CurrentRenderer == NULL)
        return;

    vtkInteractorStyleTrackballCamera::OnLeftButtonUp();

    //only look for actor if the user din't zoom the model
    if(didMouseMove())
        return;

    vtkIdType clickedActor = findClickedActorId();

    if(clickedActor>-1 && selectedIds->GetNumberOfIds() >0)
    {
        selectedIds->DeleteId(clickedActor);
        switch(selectionType)
        {
        case block:
        {
            HexBlock *hb = HexBlock::SafeDownCast(
                        hexBlocker->hexBlocks->GetItemAsObject(clickedActor));
            hb->resetColor();
            break;
        }
        case patch:
        {
            HexPatch * p = HexPatch::SafeDownCast(
                        hexBlocker->patches->GetItemAsObject(clickedActor));
            p->resetColor();
            break;
        }
        case edge:
        {
            HexEdge * e = HexEdge::SafeDownCast(
                        hexBlocker->edges->GetItemAsObject(clickedActor));
            e->resetColor();
            break;
        }
        }//end switch

        hexBlocker->render();
    }
}

void InteractorStyleActorPick::OnMiddleButtonDown()
{
    if (this->CurrentRenderer == NULL)
      return;
    storePosition();
    vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
}

void InteractorStyleActorPick::OnMiddleButtonUp()
{
    if (this->CurrentRenderer == NULL)
        return;

    vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();

    //only emit the user din't move the model
    if(didMouseMove())
        return;

    emit selectionDone();

}

void InteractorStyleActorPick::findPickedActor(int x, int y)
{
  this->InteractionPicker->Pick(x, y, 0.0, this->CurrentRenderer);
  vtkProp *prop = this->InteractionPicker->GetViewProp();
  if (prop != NULL)
    {
    this->InteractionProp = vtkProp3D::SafeDownCast(prop);
    }
  else
    {
    this->InteractionProp = NULL;
    }
}

