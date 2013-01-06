/*
Copyright 2012, 2013
Author Nicolas Edh,
Nicolas.Edh@gmail.com,
or user "nsf" at cfd-online.com

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

#ifndef INTERACTORSTYLEVERTPICK_H
#define INTERACTORSTYLEVERTPICK_H

#include <vtkSmartPointer.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <QObject>

class vtkPolyData;
class vtkGlyph3D;
class vtkSphereSource;
class vtkDataSetMapper;
class vtkActor;
class vtkIdList;
class vtkExtractGeometry;

class InteractorStyleVertPick : public QObject, public vtkInteractorStyleRubberBandPick
{
    Q_OBJECT

    vtkTypeMacro(InteractorStyleVertPick,vtkInteractorStyleRubberBandPick);
protected:
    InteractorStyleVertPick();
    ~InteractorStyleVertPick();
    InteractorStyleVertPick(InteractorStyleVertPick&);// Not implemented in order to comply with vtkObject.
    void operator=(const InteractorStyleVertPick&);// Not implemented in order to comply with vtkObject.

public:
    //Functions
    static InteractorStyleVertPick* New();
    void PrintSelf(ostream &os, vtkIndent indent);
    virtual void OnLeftButtonUp();
    virtual void OnMouseMove();
    virtual void OnLeftButtonDown();
    virtual void OnMiddleButtonUp();
    void SetPoints(vtkSmartPointer<vtkPolyData> points) {this->Points = points;}
    void clearSelection();
    void OnChar(); //override from superclass

    //Data
    vtkSmartPointer<vtkSphereSource> SelectedSphere;
    vtkSmartPointer<vtkActor> SelectedActor;
    vtkSmartPointer<vtkIdList> SelectedList;
    vtkSmartPointer<vtkPolyData> Points;
    vtkSmartPointer<vtkGlyph3D> SelectedGlyph;

signals:
    void selectionDone();

private:
    vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
//    vtkSmartPointer<vtkExtractGeometry> extractGeometry;
};

#endif // INTERACTORVERTPICK_H
