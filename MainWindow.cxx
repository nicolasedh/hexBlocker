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



#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "HexBlocker.h"
#include "HexBlock.h"
#include "InteractorStyleVertPick.h"
#include "InteractorStylePatchPick.h"
#include "InteractorStyleEdgePick.h"
#include "ToolBoxWidget.h"
#include "CreateBlockWidget.h"
#include "MoveVerticesWidget.h"
#include "SetBCsWidget.h"
#include "HexBC.h"
#include "HexExporter.h"
#include "HexReader.h"

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkIdList.h>
#include <vtkAreaPicker.h>

//#include <QInputDialog>
//#include <QFileDialog>
#include <QtGui>

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

    // Axes interactor and widget
    axes = vtkSmartPointer<vtkAxesActor>::New();
    widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    widget->SetOutlineColor( 0.9300, 0.5700, 0.1300 );
    widget->SetOrientationMarker( axes );
    widget->SetInteractor( renwin->GetInteractor() );
    widget->SetViewport( 0.0, 0.0, 0.4, 0.4 );
    widget->SetEnabled( 1 );
    widget->InteractiveOff();

    //Area Picker and InteractorStyles
    areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
    styleVertPick = vtkSmartPointer<InteractorStyleVertPick>::New();
    styleVertPick->SetPoints(hexBlocker->vertData);
    styleVertPick->SelectedSphere=hexBlocker->vertSphere;
    stylePatchPick = vtkSmartPointer<InteractorStylePatchPick>::New();
    stylePatchPick->SetPatches(hexBlocker->patches);
    styleEdgePick = vtkSmartPointer<InteractorStyleEdgePick>::New();
    styleEdgePick->SetEdges(hexBlocker->edges);

    defStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    //Qt widgets
    toolbox = new ToolBoxWidget();
    toolbox->setBCsW->hexBCs = hexBlocker->hexBCs;
    toolbox->setBCsW->allPatches = hexBlocker->patches;
    this->addDockWidget(Qt::LeftDockWidgetArea,toolbox);


    renwin->Render();

    // Set up action signals and slots
    connect(this->ui->actionView_tool_bar,SIGNAL(triggered()),this,SLOT(slotViewToolBar()));
    connect(this->ui->actionView_tool_box,SIGNAL(triggered()),this,SLOT(slotViewToolBox()));
    connect(this->ui->actionZoomOut, SIGNAL(triggered()), this, SLOT(slotZoomOut()));
    connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
    connect(this->ui->actionCreateHexBlock,SIGNAL(triggered()),this,SLOT(slotOpenCreateHexBlockDialog()));
    connect(toolbox->createBlockW,SIGNAL(apply()),this,SLOT(slotCreateHexBlock()));
    connect(this->ui->actionExtrudePatch,SIGNAL(triggered()),this,SLOT(slotStartExtrudePatch()));
    connect(this->ui->actionDumpData,SIGNAL(triggered()),this,SLOT(slotPrintHexBlocks()));
    connect(this->ui->actionOpenMoveVerticesDialog,SIGNAL(triggered()),this,SLOT(slotOpenMoveVerticesDialog()));
    connect(toolbox->moveVerticesW,SIGNAL(moveDone()),this,SLOT(slotResetInteractor()));
    connect(toolbox->moveVerticesW,SIGNAL(moveVertices()),this,SLOT(slotMoveVertices()));
    connect(toolbox,SIGNAL(cancel()),this,SLOT(slotResetInteractor()));
    connect(this->ui->actionSetBCs,SIGNAL(triggered()),this,SLOT(slotOpenSetBCsDialog()));
    connect(toolbox->setBCsW,SIGNAL(startSelectPatches(vtkIdType)),this,SLOT(slotStartSelectPatches(vtkIdType)));
    connect(toolbox->setBCsW,SIGNAL(resetInteractor()), this, SLOT(slotResetInteractor()));
    connect(toolbox->setBCsW,SIGNAL(render()),this,SLOT(slotRender()));
    connect(this->ui->actionSave,SIGNAL(triggered()),this,SLOT(slotExportBlockMeshDict()));

    connect(this->ui->actionSetNumber,SIGNAL(triggered()),this,SLOT(slotStartSelectEdges()));
    connect(toolbox,SIGNAL(setStatusText(QString)),this,SLOT(slotShowStatusText(QString)));

    connect(this->ui->actionReadBlockMeshDict,SIGNAL(triggered()),this, SLOT(slotReadBlockMeshDict()));


    connect(this->ui->actionAbout_Qt,SIGNAL(triggered()),
            qApp,SLOT(aboutQt()));
    connect(this->ui->actionAbout_hexBlocker,SIGNAL(triggered()),
            this,SLOT(slotAboutDialog()));
}

MainWindow::~MainWindow()
{
    // The smart pointers should clean up

}

// Action to be taken upon file open 
void MainWindow::slotZoomOut()
{

    hexBlocker->resetBounds();
    renwin->Render();


    //renwin->GetInteractor()->SetInteractorStyle(stylePatchPick);
}

void MainWindow::slotOpenCreateHexBlockDialog()
{
    //set index i toolbox
    toolbox->show();
    toolbox->setCurrentIndex(1);
}

void MainWindow::slotCreateHexBlock()
{
    hexBlocker->createHexBlock(toolbox->createBlockW->c0,toolbox->createBlockW->c1);
    hexBlocker->resetBounds();
    renwin->Render();
}

void MainWindow::slotPrintHexBlocks()
{
    hexBlocker->PrintHexBlocks();
}

void MainWindow::slotStartExtrudePatch()
{
    ui->statusbar->showMessage(
                tr("Left button to select a patch, middle button to confirm, rightbutton to deselect"));
    renwin->GetInteractor()->SetInteractorStyle(stylePatchPick);
    connect(stylePatchPick,SIGNAL(selectionDone(vtkIdList *)),
            this,SLOT(slotExtrudePatch(vtkIdList *)));
}

void MainWindow::slotExtrudePatch(vtkIdList *selectedPatches)
{
    ui->statusbar->clearMessage();
    if(selectedPatches->GetNumberOfIds()>0)
    {
        bool ok;
        QString title = tr("Distance");
        QString label = tr("Set the distance to extrude, has to be a positive number");

        double dist = QInputDialog::getDouble(this,title,label,1.0,0,1e12,1,&ok);
        if(ok && (dist >0.0) )
        {
            hexBlocker->extrudePatch(selectedPatches,dist);
        }
        else
        {
            ui->statusbar->showMessage("Cancled",3000);
        }
    }
    else
    {
        ui->statusbar->showMessage("No patches selected!",3000);
    }

    disconnect(stylePatchPick,SIGNAL(selectionDone(vtkIdList *)),
               this,SLOT(slotExtrudePatch(vtkIdList *)));

    renwin->GetInteractor()->SetInteractorStyle(defStyle);
    renwin->Render();
}

void MainWindow::slotOpenMoveVerticesDialog()
{

    renwin->GetInteractor()->SetInteractorStyle(styleVertPick);
    renwin->GetInteractor()->SetPicker(areaPicker);

    styleVertPick->StartSelect();

    toolbox->setCurrentIndex(2);

    ui->statusbar->showMessage(tr("Left button to select, press cntrl to rotate"),10000);
}

void MainWindow::slotRender()
{
    renwin->Render();
}

void MainWindow::slotMoveVertices()
{

    if(toolbox->moveVerticesW->delta)
        hexBlocker->moveVertices(styleVertPick->SelectedList,toolbox->moveVerticesW->dist);

    renwin->Render();
}

void MainWindow::slotResetInteractor()
{
    styleVertPick->SelectedActor->SetVisibility(0);
    styleVertPick->clearSelection();
    hexBlocker->resetColors();
    renwin->GetInteractor()->SetInteractorStyle(defStyle);
    renwin->Render();

}

void MainWindow::slotOpenSetBCsDialog()
{
    toolbox->setCurrentIndex(3);


}

void MainWindow::slotStartSelectPatches(vtkIdType bcID)
{
    renwin->GetInteractor()->SetInteractorStyle(stylePatchPick);
    connect(stylePatchPick,SIGNAL(selectionDone(vtkIdList *)),
            toolbox->setBCsW,SLOT(slotSelectionDone(vtkIdList*)));
    connect(stylePatchPick,SIGNAL(selectionDone(vtkIdList*)),
            this,SLOT(slotPatchSelectionDone()));

}

void MainWindow::slotPatchSelectionDone()
{
    disconnect(stylePatchPick,SIGNAL(selectionDone(vtkIdList *)),
               toolbox->setBCsW,SLOT(slotSelectionDone(vtkIdList*)));
    disconnect(stylePatchPick,SIGNAL(selectionDone(vtkIdList*)),
               this,SLOT(slotPatchSelectionDone()));
//    std::cout << "disconnecting" << std::endl;
}

void MainWindow::slotExportBlockMeshDict()
{
    QFileDialog::Options options;

    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this,
                QString("Select blockMeshDict"),
                QString("blockMeshDict"),
                QString("Any file (*)"),
                &selectedFilter,
                options
                );

    HexExporter * exporter = new HexExporter(hexBlocker);
    exporter->exporBlockMeshDict(filename);
}



void MainWindow::slotShowStatusText(QString text)
{
    ui->statusbar->showMessage(text,15000);
}

void MainWindow::slotStartSelectEdges()
{
    toolbox->setCurrentIndex(0); // show empty page
    ui->statusbar->showMessage(tr("Select an edge, middle button to cancel"),5000);

    renwin->GetInteractor()->SetInteractorStyle(styleEdgePick);
    connect(styleEdgePick,SIGNAL(selectionDone(vtkIdType)),
            this,SLOT(slotEdgeSelectionDone(vtkIdType)));

    renwin->Render();
}



void MainWindow::slotEdgeSelectionDone(vtkIdType edgeId)
{

    disconnect(styleEdgePick,SIGNAL(selectionDone(vtkIdType)),
               this,SLOT(slotEdgeSelectionDone(vtkIdType)));

    int prevNCells=hexBlocker->showParallelEdges(edgeId);

    QString title = tr("Number");
    QString label = tr("Set the number of cells of this and parallel edges.");
    bool ok;
    int nCells = QInputDialog::getInt(this,title,label,prevNCells,1,2147483647,1,&ok);
    if(ok && (nCells >= 1) )
    {
        hexBlocker->setNumberOnParallelEdges(edgeId,nCells);
        ui->statusbar->showMessage(QString("Number of Cells has been set"),3000);
    }
    else
    {
        ui->statusbar->showMessage(QString("Cancelled or bad integer"),3000);
        hexBlocker->resetColors();
    }
    QString msg("Total number of cells: ");
    long int NCells = hexBlocker->calculateTotalNumberOfCells();
    msg=msg.append(QString::number(NCells));
    msg.append("\n which is approximately ");
    double aNCells = (double)NCells;
    msg.append(QString::number(aNCells,'g',3));
    msg.append(".");
    ui->statusbar->showMessage(msg,10000);
    renwin->GetInteractor()->SetInteractorStyle(defStyle);
    renwin->Render();

}

void MainWindow::slotReadBlockMeshDict()
{

    QFileDialog::Options options;

    QString selectedFilter,filter;
    QString dir = "blockMeshDict";

    QString filename = QFileDialog::getOpenFileName(
                this,
                "Select a blockMeshDict file to read",
                dir,
                filter,
                &selectedFilter,
                options);

    if(filename.isNull())
    {
        this->ui->statusbar->showMessage("Reading Aborted",10000);
        return;
    }

    HexReader * reader = new HexReader();
    reader->readBlockMeshDict(filename);

    renwin->RemoveRenderer(hexBlocker->renderer);

    hexBlocker = new HexBlocker();
    hexBlocker->renderer->SetBackground(.2, .3, .4);

    renwin->AddRenderer(hexBlocker->renderer);

    hexBlocker->readBlockMeshDict(reader);
    // repoint Axes interactor and widget
    widget->SetInteractor( renwin->GetInteractor() );

    //Repoint interactors.
    styleVertPick->SetPoints(hexBlocker->vertData);
    styleVertPick->SelectedSphere=hexBlocker->vertSphere;
    stylePatchPick->SetPatches(hexBlocker->patches);
    styleEdgePick->SetEdges(hexBlocker->edges);

    //Repoint widgets
    toolbox->setBCsW->changeBCs(reader);

    renwin->Render();
}

void MainWindow::slotAboutDialog()
{
    QString title("hexBlocker");
    QString text("\t hexBlocker\t\t version 0.1 \n"
                 "A GUI for generating a blockMeshDict for use with OpenFOAM\n\n"

                 "Current capabilitys are: \n\n"
                 "\t* Creating and extruding blocks.\n"
                 "\t* Exporting and importing blockMeshDicts.\n"
                 "\t  Please not that only 2.1.x has been tested.\n"
                 "\t* Selecting boundary patches.\n"
                 "\t* Setting the number of cells on each edge.\n"
                 "\t* Moving vertices.\n"
                 "\t* Bugs -- This is an alpha realease and\n"
                 "\t  has plenty of them\n\n"

                 "Copyright 2012, 2013\n"
                 "Author Nicolas Edh, nicolas.edh@gmail.com, or \n"
                 "user \"nsf\" at www.cfd-online.com\n\n"

                 "hexBlocker is free software: you can redistribute it and/or modify "
                 "it under the terms of the GNU General Public License as published by "
                 "the Free Software Foundation, either version 3 of the License, or "
                 "(at your option) any later version.\n\n"

                 "hexBlocker is distributed in the hope that it will be useful, "
                 "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                 "GNU General Public License for more details.\n\n"

                 "You should have received a copy of the GNU General Public License "
                 "along with hexBlocker.  If not, see <http://www.gnu.org/licenses/>. \n\n"

                 "This program is not approved or endorsed by OpenCFD, "
                 "the producer of the OpenFOAM(R) software and owner of "
                 "the OpenFOAM(R) trade mark."
                 );
    QMessageBox::about(this,title, text);
}

void MainWindow::slotViewToolBar()
{
    //isChecked is no longer true when
    //this function is called
    if(! ui->actionView_tool_bar->isChecked())
    {
        ui->toolBar->hide();
    }
    else
    {
        ui->toolBar->show();
    }
}

void MainWindow::slotViewToolBox()
{
    //isChecked is no longer true when
    //this function is called
    if(! ui->actionView_tool_box->isChecked())
    {
        toolbox->hide();
    }
    else
    {
        toolbox->show();
    }
}

void MainWindow::slotExit()
{
    qApp->exit();

}


