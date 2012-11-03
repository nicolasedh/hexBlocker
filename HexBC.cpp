#include "HexBC.h"

#include <vtkObjectFactory.h>
#include <vtkIdList.h>
#include <vtkCollection.h>
#include <hexPatch.h>


vtkStandardNewMacro(HexBC);

//Default constructor
HexBC::HexBC()
{
    name = std::string("DefaultPatches");
    type = std::string("patch");
    patchIds = vtkIdList::New();
    //delete name;
    //name = new QString("foo");
}

HexBC::~HexBC()
{

}



void HexBC::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is BC! " << std::endl;
}


