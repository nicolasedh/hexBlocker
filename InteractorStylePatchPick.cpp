#include "InteractorStylePatchPick.h"
#include "hexPatch.h"
#include <vtkCollection.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindowInteractor.h>
#include <iostream>

#include <vtkIdList.h>
#include <vtkCollection.h>
#include <vtkProp3D.h>

vtkStandardNewMacro(InteractorStylePatchPick);

InteractorStylePatchPick::InteractorStylePatchPick()
{

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

void InteractorStylePatchPick::OnLeftButtonDown()
{
  // Forward events
  //vtkInteractorStyleTrackballActor::OnLeftButtonDown();

  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];

  this->FindPokedRenderer(x, y);
  this->FindPickedActor(x, y);

  std::cout << "It's working" << std::endl;

  for(vtkIdType i = 0; i< patches->GetNumberOfItems();i++)
  {
      hexPatch *patch = static_cast<hexPatch *>(patches->GetItemAsObject(i));
      if(this->InteractionProp == patch->actor)
      {
          std::cout << "You selected patch " << i << ", ";
          for(vtkIdType j=0;j<4;j++)
              std::cout << patch->vertIds->GetId(j) << " ";
          std::cout <<"."<< std::endl;
       }
  }
//  if(this->InteractionProp == this->Cube)
//    {
//    std::cout << "Picked cube." << std::endl;
//    }

}
