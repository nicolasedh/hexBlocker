#include "InteractorStylePatchPick.h"
#include "hexPatch.h"
#include "HexBC.h"
#include <vtkCollection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>

#include <vtkIdList.h>
#include <vtkCollection.h>
#include <vtkProp3D.h>

vtkStandardNewMacro(InteractorStylePatchPick);

InteractorStylePatchPick::InteractorStylePatchPick() : QObject()
{
    selectedPatches = vtkIdList::New();
}

InteractorStylePatchPick::~InteractorStylePatchPick()
{

}

void InteractorStylePatchPick::PrintSelf(ostream &os, vtkIndent indent)
{

}

void InteractorStylePatchPick::OnChar()
{

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
        hexPatch *patch = static_cast<hexPatch *>(patches->GetItemAsObject(i));
        if(this->InteractionProp == patch->actor)
        {
            clickedPatch = i;
            std::cout << "You selected patch " << clickedPatch << ", ";
            for(vtkIdType j=0;j<4;j++)
                std::cout << patch->vertIds->GetId(j) << " ";
            std::cout <<"."<< std::endl;
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
        selectedPatches->InsertNextId(clickedPatch);

    std::cout <<"Selected Patches: ";
    for(vtkIdType i =0; i<selectedPatches->GetNumberOfIds();i++)
        std::cout << selectedPatches->GetId(i) <<", ";
    std::cout << std::endl;
}

void InteractorStylePatchPick::OnRightButtonUp()
{
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    vtkIdType clickedPatch = findClickedPatch(x,y);

    selectedPatches->DeleteId(clickedPatch);

    std::cout <<"Selected Patches: ";
    for(vtkIdType i =0; i<selectedPatches->GetNumberOfIds();i++)
        std::cout << selectedPatches->GetId(i) <<", ";
    std::cout << std::endl;
}

void InteractorStylePatchPick::OnMiddleButtonUp()
{
    if(selectedPatches->GetNumberOfIds()>0)
    {
        std::cout<< "emit signal here" << std::endl;
        emit selectionDone(selectedPatches);
        selectedPatches->Initialize();
    }


}


