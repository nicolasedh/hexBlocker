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

Description
    This class contains a list (vtkCollection) of which patches are included
    for each defined boundary condition.
*/

#ifndef HEXBC_H
#define HEXBC_H

#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <iostream>
#include <QTextStream>


//Pre declarations
class vtkIdList;
class vtkCollection;
class HexPatch;



class HexBC : public vtkObject
{
vtkTypeMacro(HexBC,vtkObject)

protected:
    HexBC();
    ~HexBC();
    HexBC(const HexBC&);  // Not implemented in order to comply with vtkObject.
    void operator=(const HexBC&);  // Not implemented in order to comply with vtkObject.

public:
    static HexBC *New();
    // inorder to comply with vtkObect
    void PrintSelf(ostream &os, vtkIndent indent);

    //FUNCTIONS

    //Sets the color of a patch
    void setPatchColors(double r,double g, double b);

    //Resets the color to green or blue for all
    //patches in the bc
    void resetPatchColors();

    //Removes patch and reduces ids a
    void notifyRemovedPatch(HexPatch *p);

    //Removes patch from list
    void removePatchFromList(HexPatch *p);

    // finds id of patch in global list returns true if it was found
    // ids is a list of vertices
    bool insertPatchIfIdsExists(vtkSmartPointer<vtkIdList> ids);

    //insert a list of patches. The list contains ids in globalList
    void insertPatches(vtkIdList *pids);

    void getLocalPatchesIdsInGlobal(vtkIdList *idsInGlobal);
    vtkSmartPointer<vtkCollection> globalPatches; //All patches in the model
    vtkSmartPointer<vtkCollection> localPatches; //All patches in the model
    //DATA
    std::string name;
    std::string type; //e.g. wall or patch
private:


};

#endif // HEXBLOCK_H
