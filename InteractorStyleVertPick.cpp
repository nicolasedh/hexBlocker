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

#include "InteractorStyleVertPick.h"
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>
//#include <vtkDataSetSurfaceFilter.h>
//#include <vtkRendererCollection.h>
#include <vtkProperty.h>
#include <vtkPlanes.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkPointSource.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
//#include <vtkVertexGlyphFilter.h>
//#include <vtkIdFilter.h>

#include <vtkPoints.h>
#include <vtkGlyph3D.h>
#include <vtkSphereSource.h>
#include <vtkLine.h>
#include <vtkQuad.h>
#include <vtkCellArray.h>

vtkStandardNewMacro(InteractorStyleVertPick);

InteractorStyleVertPick::InteractorStyleVertPick()
{
    this->SelectedGlyph = vtkSmartPointer<vtkGlyph3D>::New();
    this->SelectedSphere = vtkSmartPointer<vtkSphereSource>::New();
    this->SelectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    this->SelectedActor = vtkSmartPointer<vtkActor>::New();
    this->SelectedActor->SetMapper(SelectedMapper);
    this->SelectedList = vtkSmartPointer<vtkIdList>::New();
}

InteractorStyleVertPick::~InteractorStyleVertPick()
{

}

void InteractorStyleVertPick::PrintSelf(ostream &os, vtkIndent indent)
{

}
void InteractorStyleVertPick::OnLeftButtonDown()
{
    if(this->Interactor->GetControlKey())
    {
        this->CurrentMode=0;
        this->State=VTKIS_ROTATE;
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
     }
    else
    {
        this->CurrentMode=1;
        vtkInteractorStyleRubberBandPick::OnLeftButtonDown();
    }

}
void InteractorStyleVertPick::OnMouseMove()
{

    if(this->Interactor->GetControlKey())
    {
        vtkInteractorStyleTrackballCamera::OnMouseMove();
     }
    else
    {
        vtkInteractorStyleRubberBandPick::OnMouseMove();
    }

}

void InteractorStyleVertPick::OnLeftButtonUp()
{
    if(this->Interactor->GetControlKey())
    {
//        std::cout<<"you pressed ctnrl"<<std::endl;
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
        return;
     }
    else//kan tas bort? pga return
    {
    // Forward events
    vtkInteractorStyleRubberBandPick::OnLeftButtonUp();

    vtkPlanes* frustum =
            static_cast<vtkAreaPicker*>(this->GetInteractor()->GetPicker())->GetFrustum();

    vtkSmartPointer<vtkExtractGeometry> extractGeometry =
            vtkSmartPointer<vtkExtractGeometry>::New();
    extractGeometry->SetImplicitFunction(frustum);
#if VTK_MAJOR_VERSION <= 5
    extractGeometry->SetInput(this->Points);
#else
    extractGeometry->SetInputData(this->Points);
#endif
    extractGeometry->Update();


    vtkIdType numPoints = extractGeometry->GetOutput()->GetNumberOfPoints();

    SelectedList->Initialize();
    SelectedList->SetNumberOfIds(numPoints);

//    std::cout << "Selected " << numPoints << " point(s)." << std::endl;
    for (vtkIdType selId = 0; selId < numPoints ;selId++ )
    {
        double pos[3];
        extractGeometry->GetOutput()->GetPoint(selId,pos);
        vtkIdType orgId = Points->FindPoint(pos);
//        std::cout << "Selected point "<< selId << ", org id " << orgId
//                  << " location at: " << pos[0] <<"," << pos[1]
//                  << "," << pos[2] << std::endl;
        SelectedList->InsertId(selId,orgId);

    }

    SelectedGlyph->SetInput(extractGeometry->GetOutput());
    SelectedGlyph->SetSourceConnection(SelectedSphere->GetOutputPort());
    SelectedGlyph->SetScaleModeToDataScalingOff();
    SelectedGlyph->Update();

#if VTK_MAJOR_VERSION <= 5
    this->SelectedMapper->SetInput(SelectedGlyph->GetOutput());
#else
    this->SelectedMapper->SetInputData(SelectedGlyph->GetOutputPort());
#endif
    this->SelectedMapper->ScalarVisibilityOff();
    this->SelectedActor->GetProperty()->SetColor(1.0, 0.0, 0.0); //(R,G,B)
    this->SelectedActor->SetVisibility(1);


    this->CurrentRenderer->AddActor(SelectedActor);
    this->GetInteractor()->GetRenderWindow()->Render();
    this->HighlightProp(NULL);

    }
    //this->StartRotate();
    //this->CurrentMode = 0;
}
void InteractorStyleVertPick::clearSelection()
{
    SelectedList->Initialize();
    SelectedList->Modified();
}

void InteractorStyleVertPick::OnChar()
{

}
