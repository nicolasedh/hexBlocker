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
    This class makes it possible to select one, two or multiple patches.
    This is governed by enumerable selectionMode which takes the values
    single, pair or multi.
    If single then the signal selectionDone is emited upen lef mouse
    click. If pair then the signal is emitted after the second selection.
    If multi then the signal is emited by middleclick. Deselection is done
    by rightclicking when applicable.
*/

#ifndef INTERACTORSTYLEPATCHPICK_H
#define INTERACTORSTYLEPATCHPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <QObject>

class vtkCollection;
class vtkIdList;
class HexBC;

class InteractorStylePatchPick : public QObject, public vtkInteractorStyleTrackballActor
{
    Q_OBJECT

    vtkTypeMacro(InteractorStylePatchPick,vtkInteractorStyleTrackballActor);
protected:
    InteractorStylePatchPick();
    ~InteractorStylePatchPick();
    InteractorStylePatchPick(InteractorStylePatchPick&);// Not implemented in order to comply with vtkObject.
    void operator=(const InteractorStylePatchPick&);// Not implemented in order to comply with vtkObject.

    vtkSmartPointer<vtkCollection> patches;

public:
    static InteractorStylePatchPick* New();
    void PrintSelf(ostream &os, vtkIndent indent);

    void SetPatches(vtkSmartPointer<vtkCollection> pts);
    virtual vtkIdType findClickedPatch(int x, int y);
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp(){}
    virtual void OnMiddleButtonDown(){}
    virtual void OnMiddleButtonUp();
    virtual void OnRightButtonDown(){}
    virtual void OnRightButtonUp();
    virtual void OnMouseMove(){}

    void OnChar(); //override from superclass

    vtkIdList *selectedPatches;

//    enum selectionModes{single=1,pair=2,multi=3};//select single patch, multiple patches or a pair.
//    selectionModes  selectionMode;
    enum selectionModes{single=1,pair=2,multi=3}selectionMode   ;
signals:
    void selectionDone(vtkIdList *selectedPatches);

private:

};

#endif // INTERACTORSTYLEPATCHPICK_H
