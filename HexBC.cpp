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
    patchIds = vtkSmartPointer<vtkIdList>::New();
    globalPatches = vtkSmartPointer<vtkCollection>::New();
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
        HexPatch *p =
                HexPatch::SafeDownCast(globalPatches->GetItemAsObject(patchIds->GetId(i)));
        p->setColor(r,g,b);
    }
}

void HexBC::resetPatchColors()
{
    for(vtkIdType i=0; i<patchIds->GetNumberOfIds();i++)
    {
        HexPatch *p = HexPatch::SafeDownCast(
                    globalPatches->GetItemAsObject(patchIds->GetId(i)));
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
//            std::cout << "patch exists " << i <<std::endl;
            patchIds->InsertUniqueId(i);
            foundIt=true;
        }
    }

//    for(vtkIdType i=0;i<allPatches->GetNumberOfItems();i++)
//    {

//        HexPatch * p = HexPatch::SafeDownCast(allPatches->GetItemAsObject(i));
//        bool idsInP[4];
//        //return true if all ids in ids exist in patch
//        for(vtkIdType j=0;j<ids->GetNumberOfIds();j++)
//        {
//            idsInP[j]=false;
//            for(vtkIdType k=0;p->vertIds->GetNumberOfIds();k++)
//            {
//                if(p->vertIds->GetId(k) == ids->GetId(j))
//                {

//                    idsInP[j]=true;
//                }
//            }

//        }
//        if(foundIt)
//        {
//            std::cout << "patches are equal, glbpatc" << i << std::endl;
//            patchIds->InsertNextId(i);
//        }
//    }
    return foundIt;
}

void HexBC::notifyRemovedPatch(HexPatch *p)
{
    vtkIdType pId = globalPatches->IsItemPresent(p)-1;
    patchIds->DeleteId(pId);

    //reduce pointers since pId will be removed in global
    for(vtkIdType i=0;i<patchIds->GetNumberOfIds();i++)
    {
        vtkIdType k = patchIds->GetId(i);
        if(k>pId)
        {
            patchIds->SetId(i,k-1);
        }
    }
}

void HexBC::removePatchFromList(HexPatch *p)
{
    vtkIdType pId = globalPatches->IsItemPresent(p)-1;
    patchIds->DeleteId(pId);
}
