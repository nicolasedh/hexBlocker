/*
Copyright 2016
Author Leonardo Rosa
user "leorosa" at github.com

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


#include "HexBlocker.h"
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderer.h>

void HexBlocker::readGeometry(char* openFileName)
{
    printf("reading: %s\n", openFileName);

/* VTK 5.10 can't open ASCII STL files?
 * http://www.vtk.org/pipermail/vtkusers/2012-October/077163.html
 * http://www.paraview.org/Bug/view.php?id=14326
 * http://www.cfd-online.com/Forums/openfoam-paraview/124839-problems-ascii-stl.html
*/
    vtkSmartPointer<vtkSTLReader> Greader = vtkSmartPointer<vtkSTLReader>::New();
    Greader->SetFileName(openFileName);
    Greader->Update();

    vtkSmartPointer<vtkPolyDataMapper> Gmapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    Gmapper->SetInputConnection(Greader->GetOutputPort());

    vtkSmartPointer<vtkActor> Gactor = vtkSmartPointer<vtkActor>::New();
    Gactor->GetProperty()->SetOpacity(0.5);
    Gactor->SetMapper(Gmapper);

    renderer->AddActor(Gactor);
}

