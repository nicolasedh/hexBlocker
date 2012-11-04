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
    allPatches = vtkSmartPointer<vtkCollection>::New();
    //delete name;
    //name = new QString("foo");
}

HexBC::~HexBC()
{

}

void HexBC::setPatchColors(double r, double g, double b)
{
    for(vtkIdType i=0; i<patchIds->GetNumberOfIds();i++)
    {
        hexPatch *p =
                hexPatch::SafeDownCast(allPatches->GetItemAsObject(patchIds->GetId(i)));
        p->setColor(r,g,b);
    }
}

void HexBC::resetPatchColors()
{
    for(vtkIdType i=0; i<patchIds->GetNumberOfIds();i++)
    {
        hexPatch *p = hexPatch::SafeDownCast(
                    allPatches->GetItemAsObject(patchIds->GetId(i)));
        p->resetColor();

    }
}

void HexBC::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is BC! " << std::endl;
}


