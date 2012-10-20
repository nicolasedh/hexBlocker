/*
 * Nicos hexBlocker, gpl?
 */



#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "MoveVerticesWidget.h"
#include "CreateBlockWidget.h"
#include "HexBlocker.h"
#include "HexBlock.h" //apparently needed inorder to acces it's actors
#include "InteractorStyleVertPick.h"
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>


#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>

#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkIdList.h>
#include <vtkAreaPicker.h>

#define VTK_CREATE(type, name) \
    vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


// Constructor
MainWindow::MainWindow() 
{
    this->ui = new Ui_MainWindow;
    this->ui->setupUi(this);


    hexBlocker = new HexBlocker();
    hexBlocker->renderer->SetBackground(.2, .3, .4);

    // VTK/Qt wedded
    renwin = this->ui->qvtkWidget->GetRenderWindow();
    renwin->AddRenderer(hexBlocker->renderer);

    //Qt widgets
    moveWidget = new MoveVerticesWidget();
    createBlockWidget = new CreateBlockWidget();

    // Axes interactor and widget
    axes = vtkSmartPointer<vtkAxesActor>::New();
    widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    widget->SetOrientationMarker( axes );
    widget->SetInteractor( renwin->GetInteractor() );
    widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
    widget->SetEnabled( 1 );
    widget->InteractiveOff();

    //Area Picker and InteractorStyleVertPicker
    areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
    style = vtkSmartPointer<InteractorStyleVertPick>::New();
    style->SetPoints(hexBlocker->vertData);
    style->SelectedSphere=hexBlocker->vertSphere;

    defStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    renwin->Render();

    // Set up action signals and slots
    connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->actionCreate_HexBlock,SIGNAL(triggered()),this,SLOT(slotOpenCreateHexBlockDialog()));
    connect(createBlockWidget,SIGNAL(apply()),this,SLOT(slotCreateHexBlock()));
    connect(this->ui->actionExtrude_patch,SIGNAL(triggered()),this,SLOT(slotExtrudePatch()));
    connect(this->ui->actionHelp,SIGNAL(triggered()),this,SLOT(slotPrintHexBlocks()));
    connect(this->ui->actionSelectVertices,SIGNAL(triggered()),this,SLOT(slotSelectVertices()));
    connect(moveWidget,SIGNAL(moveDone()),this,SLOT(slotResetInteractor()));
    connect(moveWidget,SIGNAL(moveVertices()),this,SLOT(slotMoveVertices()));
}

MainWindow::~MainWindow()
{
    // The smart pointers should clean up

}

// Action to be taken upon file open 
void MainWindow::slotOpenFile()
{
    hexBlocker->resetBounds();
    renwin->Render();
}

void MainWindow::slotOpenCreateHexBlockDialog()
{
    if(moveWidget->isVisible())
        moveWidget->close();
    this->addDockWidget(Qt::LeftDockWidgetArea,createBlockWidget);
    createBlockWidget->show();
}

void MainWindow::slotCreateHexBlock()
{

    hexBlocker->createHexBlock(createBlockWidget->c0,createBlockWidget->c1);
    createBlockWidget->close();
    hexBlocker->resetBounds();
    renwin->Render();
}

void MainWindow::slotPrintHexBlocks()
{
    hexBlocker->PrintHexBlocks();
}

void MainWindow::slotExtrudePatch()
{
    hexBlocker->extrudePatch();
    renwin->Render();
}

void MainWindow::slotSelectVertices()
{
    renwin->GetInteractor()->SetPicker(areaPicker);
    renwin->GetInteractor()->SetInteractorStyle(style);

    style->StartSelect();

    this->addDockWidget(Qt::LeftDockWidgetArea,moveWidget);

    ui->statusbar->showMessage(tr("Left button to select, press cntrl to rotate"),10000);
    moveWidget->show();

}

void MainWindow::slotMoveVertices()
{
    std::cout << "slot is connected, selected IDs: ";
    for(vtkIdType i=0;i<style->SelectedList->GetNumberOfIds();i++)
        std::cout << style->SelectedList->GetId(i)<< " ";
    std::cout << "."<< std::endl;

    if(moveWidget->delta)
        hexBlocker->moveVertices(style->SelectedList,moveWidget->dist);

    renwin->Render();
}

void MainWindow::slotResetInteractor()
{
    style->SelectedActor->SetVisibility(0);
    style->clearSelection();
    renwin->GetInteractor()->SetInteractorStyle(defStyle);

}

void MainWindow::slotExit() {
    qApp->exit();
}
