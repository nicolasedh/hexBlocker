#include "HexBC.h"

#include <vtkObjectFactory.h>
#include <vtkIdList.h>
#include <vtkCollection.h>
#include <hexPatch.h>

#include <QString>

vtkStandardNewMacro(HexBC);

//Default constructor
HexBC::HexBC()
{
    name = new QString("DefaultPatches");
    type = new QString("patch");
    //delete name;
    //name = new QString("foo");
}

HexBC::HexBC(QString *nam, QString *typ)
{
    name = nam;
    type = typ;
}

HexBC::~HexBC()
{

}



void HexBC::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is BC! " << std::endl;
}
