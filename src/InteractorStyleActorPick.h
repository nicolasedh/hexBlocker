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
    This interactionStyle makes it possible to select, a single object,
    a pair of objects or any number of objects. This is controlled by
    the enumerable selectionMode. Allowable objets for selection are
    HexBlock, HexPatch or HexEdge. The type of object is controlled with
    the enumerable selectionType. The enumerables are set with
    the function setSelection.
    When the user releases the middle mouse button (but hasn't moved the
    model) the Qt signal selectionDone is emitted.
*/

#ifndef INTERACTORSTYLEACTORPICK_H
#define INTERACTORSTYLEACTORPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <QObject>

class vtkCollection;
class vtkIdList;
class HexBC;
class HexBlocker;
class vtkCellPicker;

class InteractorStyleActorPick : public QObject, public vtkInteractorStyleTrackballCamera
{
    Q_OBJECT

    vtkTypeMacro(InteractorStyleActorPick,vtkInteractorStyleTrackballCamera);
protected:
    InteractorStyleActorPick();
    ~InteractorStyleActorPick();
    InteractorStyleActorPick(InteractorStyleActorPick&);// Not implemented in order to comply with vtkObject.
    void operator=(const InteractorStyleActorPick&);// Not implemented in order to comply with vtkObject.


public:
    static InteractorStyleActorPick* New();
    void PrintSelf(ostream &os, vtkIndent indent);

    //selection type, etiher HexBlocks, HexPatches or HexEdges
    enum selectionTypes{block=1,patch=2,edge=3};
    //Selection mode, if set to single then only one object is
    // allowed to be selected. If set to pair then only two are
    // allowed, the second is colored blue.
    enum selectionModes{single=1,pair=2,multi=3};

    //FUNCTIONS
    void setHexBlocker(HexBlocker *hexb);
    virtual vtkIdType findClickedActorId();
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp();
    virtual void OnMiddleButtonDown();
    virtual void OnMiddleButtonUp();
    virtual void OnRightButtonDown();
    virtual void OnRightButtonUp();
    virtual void OnMouseMove();

    void OnChar(); //override from superclass

    //set the selection type and mode, see the enumerables.
    void setSelection(selectionTypes type, selectionModes mode);
    //overload with ints?

    //DATA
    vtkSmartPointer<vtkIdList> selectedIds;

signals:
    void selectionDone();

private:
    //FUNCTIONS
    bool didMouseMove();
    void storePosition();
    void findPickedActor(int x,int y);

    //DATA
    int lastXpos,lastYpos;
    HexBlocker * hexBlocker;

    selectionTypes selectionType;
    selectionModes selectionMode;
    vtkSmartPointer<vtkCellPicker> InteractionPicker;
    vtkSmartPointer<vtkProp3D> InteractionProp;
};

#endif // INTERACTORSTYLEACTORPICK_H
