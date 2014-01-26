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
#include "HexBC.h"

#include <vtkObjectFactory.h>
#include <vtkIdList.h>
#include <vtkCollection.h>
#include <HexPatch.h>


vtkStandardNewMacro(HexBC);

//Default constructor
HexBC::HexBC()
{
    name = std::string("DefaultPatches");
    type = std::string("patch");
    globalPatches = vtkSmartPointer<vtkCollection>::New();
    localPatches = vtkSmartPointer<vtkCollection>::New();
    //delete name;
    //name = new QString("foo");
}

HexBC::~HexBC()
{

}

void HexBC::setPatchColors(double r, double g, double b)
{
    for(vtkIdType i=0; i<localPatches->GetNumberOfItems();i++)
    {
        HexPatch *p =
                HexPatch::SafeDownCast(localPatches->GetItemAsObject(i));
        p->setColor(r,g,b);
    }

}

void HexBC::resetPatchColors()
{
    for(vtkIdType i=0; i<localPatches->GetNumberOfItems();i++)
    {
        HexPatch *p =
                HexPatch::SafeDownCast(localPatches->GetItemAsObject(i));
        p->resetColor();
    }
}

void HexBC::PrintSelf(ostream &os, vtkIndent indent)
{
    os << "This is BC! " << std::endl;
}

bool HexBC::insertPatchIfIdsExists(vtkSmartPointer<vtkIdList> ids)
{

    bool foundIt=false;
    for(vtkIdType i=0;i<globalPatches->GetNumberOfItems();i++)
    {
        HexPatch * p = HexPatch::SafeDownCast(globalPatches->GetItemAsObject(i));
        if(p->equals(ids))
        {
            localPatches->AddItem(p);
            foundIt=true;
        }
    }

    return foundIt;
}

void HexBC::insertPatches(vtkIdList *pids)
{
    for(vtkIdType i=0;i<pids->GetNumberOfIds();i++)
    {
        HexPatch *p = HexPatch::SafeDownCast
                (
                    globalPatches->GetItemAsObject(pids->GetId(i))
                    );
        if(p!=NULL)
            localPatches->AddItem(p);
//        std::cout << "inP ("
//                  << p->vertIds->GetId(0) << " "
//                  << p->vertIds->GetId(1) << " "
//                  << p->vertIds->GetId(2) << " "
//                  << p->vertIds->GetId(3) << ")" << endl;

    }
}

void HexBC::getLocalPatchesIdsInGlobal(vtkIdList *idsInGlobal)
{
    for(vtkIdType i=0;i<localPatches->GetNumberOfItems();i++)
    {
        HexPatch *p = HexPatch::SafeDownCast(localPatches->GetItemAsObject(i));
        vtkIdType idInGlobal = globalPatches->IsItemPresent(p)-1;
        if(idInGlobal>-1)
            idsInGlobal->InsertNextId(idInGlobal);
    }
}


void HexBC::removePatchFromList(HexPatch *p)
{
    localPatches->RemoveItem(p);
}
