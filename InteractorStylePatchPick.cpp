#include "InteractorStylePatchPick.h"
#include "hexPatch.h"
#include <vtkObjectFactory.h>

#include <iostream>


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
  vtkInteractorStyleTrackballActor::OnLeftButtonDown();

  std::cout << "It's working" << std::endl;
//  if(this->InteractionProp == this->Cube)
//    {
//    std::cout << "Picked cube." << std::endl;
//    }

}
