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
#include "ui_MainWindow.h"

#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkCellLocator.h>
#include <vtkIdList.h>
#include <vtkIdFilter.h>

#include <QtGui>
#include <QMainWindow>

void HexBlocker::readGeometry(char* openFileName)
{
    printf("reading: %s\n", openFileName);

//  vtkSmartPointer<vtkSTLReader> GeoReader = vtkSmartPointer<vtkSTLReader>::New();
    GeoReader = vtkSmartPointer<vtkSTLReader>::New();
    GeoReader->SetFileName(openFileName);
    GeoReader->Update();

    vtkSmartPointer<vtkPolyDataMapper> GeoMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    GeoMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    GeoMapper->SetInputConnection(GeoReader->GetOutputPort());

    GeoActor->GetProperty()->SetOpacity(0.5);
    GeoActor->SetMapper(GeoMapper);

    renderer->AddActor(GeoActor);
}

void HexBlocker::showGeometry()
{
    visibilityGeometry(true);
}

void HexBlocker::hideGeometry()
{
    visibilityGeometry(false);
}

void HexBlocker::visibilityGeometry(bool mode)
{
    GeoActor->SetVisibility(mode);
}

void HexBlocker::setModelScale(double scale)
{
    convertToMeters = scale;
}

void HexBlocker::scaleGeometry(double scale)
{
    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Scale(scale, scale, scale);
    GeoActor->SetUserTransform(transform);
    this->render();
}

void HexBlocker:: snapVertices(vtkSmartPointer<vtkIdList> ids)
{
    vtkSmartPointer<vtkIdFilter> cellIdFilter =
        vtkSmartPointer<vtkIdFilter>::New();
    cellIdFilter->SetInputConnection(GeoReader->GetOutputPort());
    cellIdFilter->Update();

    vtkSmartPointer<vtkCellLocator> cellLocator = vtkSmartPointer<vtkCellLocator>::New();
    cellLocator->SetDataSet(cellIdFilter->GetOutput());
    cellLocator->BuildLocator();

//  printf("here\n");
    for(vtkIdType i=0; i<ids->GetNumberOfIds(); i++)
    {
//      printf("%d\n", i);
        //Find the closest points to TestPoint
        double pos[3];              //the coordinates of the vertice to move
        double closestPoint[3];     //the coordinates of the closest point will be returned here
        double closestPointDist2;   //the squared distance to the closest point will be returned here
        vtkIdType cellId;           //the cell id of the cell containing the closest point will be returned here
        int subId;                  //this is rarely used (in triangle strips only, I believe)

        vertices->GetPoint(ids->GetId(i),pos);
        cellLocator->FindClosestPoint(pos, closestPoint, cellId, subId, closestPointDist2);
        vertices->SetPoint(ids->GetId(i),closestPoint);
    }
    vertices->Modified();
    rescaleActors();
}
