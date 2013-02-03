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

#include "InteractorStylePatchPick.h"
#include "HexPatch.h"
#include "HexBC.h"
#include <vtkCollection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <iostream>

#include <vtkIdList.h>
#include <vtkCollection.h>
#include <vtkProp3D.h>


vtkStandardNewMacro(InteractorStylePatchPick);

InteractorStylePatchPick::InteractorStylePatchPick() : QObject()
{
    selectedPatches = vtkIdList::New();
    selectionMode = multi;
}

InteractorStylePatchPick::~InteractorStylePatchPick()
{

}

void InteractorStylePatchPick::PrintSelf(ostream &os, vtkIndent indent)
{

}

void InteractorStylePatchPick::OnChar()
{
    char k = this->GetInteractor()->GetKeyCode();
    if(k == ' ')
    {
        InteractorStylePatchPick::OnMiddleButtonUp();
    }
}

void InteractorStylePatchPick::SetPatches(vtkSmartPointer<vtkCollection> pts)
{
    patches=pts;
}

vtkIdType InteractorStylePatchPick::findClickedPatch(int x,int y)
{
    this->FindPokedRenderer(x, y);
    this->FindPickedActor(x, y);

    vtkIdType clickedPatch=-1;

    for(vtkIdType i = 0; i< patches->GetNumberOfItems();i++)
    {
        HexPatch *patch = static_cast<HexPatch *>(patches->GetItemAsObject(i));
        if(this->InteractionProp == patch->actor)
        {
            clickedPatch = i;
//            std::cout << "You selected patch " << clickedPatch << ", ";
//            for(vtkIdType j=0;j<4;j++)
//                std::cout << patch->vertIds->GetId(j) << " ";
//            std::cout <<"."<< std::endl;
        }
    }


    return clickedPatch;
}

void InteractorStylePatchPick::OnLeftButtonDown()
{
    // Forward events
    //vtkInteractorStyleTrackballActor::OnLeftButtonDown();

    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    vtkIdType clickedPatch = findClickedPatch(x,y);

    //only insert if not already in list and only if we clicked a patch
    if(selectedPatches->IsId(clickedPatch) == -1 && clickedPatch > -1)
    {

        selectedPatches->InsertNextId(clickedPatch);
        HexPatch *patch = HexPatch::SafeDownCast(patches->GetItemAsObject(clickedPatch));
        if (selectionMode == single)
        {
            patch->setColor(1.0,0.0,0.0);
            this->GetInteractor()->GetRenderWindow()->Render();
            emit selectionDone(selectedPatches);
            return;
        }
        else if(selectionMode == pair && selectedPatches->GetNumberOfIds()>1)
        {
//            std::cout << "blue selection" << std::endl;
            patch->setColor(0.0,1.0,0);
            this->GetInteractor()->GetRenderWindow()->Render();
            emit selectionDone(selectedPatches);
            return;
        }
        else
        {
//            std::cout << "red selection" << std::endl;
            patch->setColor(1.0,0,0);
            this->GetInteractor()->GetRenderWindow()->Render();
        }
//    std::cout <<"Selected Patches: ";
//    for(vtkIdType i =0; i<selectedPatches->GetNumberOfIds();i++)
//        std::cout << selectedPatches->GetId(i) <<", ";
//    std::cout << std::endl;
    }
}

void InteractorStylePatchPick::OnRightButtonUp()
{
    if(selectedPatches->GetNumberOfIds() == 0)
    {
        return;
    }
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    vtkIdType clickedPatch = findClickedPatch(x,y);

    if(clickedPatch > -1 )
    {
        selectedPatches->DeleteId(clickedPatch);
        HexPatch *patch = HexPatch::SafeDownCast(patches->GetItemAsObject(clickedPatch));
        patch->resetColor();
    }
    this->GetInteractor()->GetRenderWindow()->Render();


//    std::cout <<"Selected Patches: ";
//    for(vtkIdType i =0; i<selectedPatches->GetNumberOfIds();i++)
//        std::cout << selectedPatches->GetId(i) <<", ";
//    std::cout << std::endl;
}

void InteractorStylePatchPick::OnMiddleButtonUp()
{
    if(selectedPatches->GetNumberOfIds()>0)
    {
//        std::cout<< "emit signal here" << std::endl;
        for(vtkIdType i = 0; i<selectedPatches->GetNumberOfIds();i++)
        {
            HexPatch * p = HexPatch::SafeDownCast(patches->GetItemAsObject(selectedPatches->GetId(i)));
            p->resetColor();
        }
        this->GetInteractor()->GetRenderWindow()->Render();
        emit selectionDone(selectedPatches);
        selectedPatches->Initialize();
    }


}


