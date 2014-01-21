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


#include "ui_MainWindow.h"
#include "MainWindow.h"
#include "HexBlocker.h"
#include "HexBlock.h"
#include "InteractorStyleVertPick.h"
#include "InteractorStyleActorPick.h"
#include "ToolBoxWidget.h"
#include "CreateBlockWidget.h"
#include "MoveVerticesWidget.h"
#include "EdgePropsWidget.h"
#include "SetBCsWidget.h"
#include "HexBC.h"
#include "HexExporter.h"
#include "HexReader.h"
#include "VerticeEditorWidget.h"

#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

//#include <vtkAxesActor.h>
//#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkIdList.h>
#include <vtkAreaPicker.h>

//#include <QInputDialog>
//#include <QFileDialog>
#include <QtGui>

#include <EdgeSetTypeWidget.h>

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
    hexBlocker->initOrientationAxes(renwin);

    //Area Picker and InteractorStyles
    areaPicker = vtkSmartPointer<vtkAreaPicker>::New();
    styleVertPick = vtkSmartPointer<InteractorStyleVertPick>::New();
    styleVertPick->SetPoints(hexBlocker->vertData);
    styleVertPick->SelectedSphere=hexBlocker->vertSphere;
    styleActorPick = vtkSmartPointer<InteractorStyleActorPick>::New();
    styleActorPick->setHexBlocker(hexBlocker);

    defStyle = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    //Qt widgets
    toolbox = new ToolBoxWidget();
    toolbox->setHexBlockerPointer(hexBlocker);
    this->addDockWidget(Qt::LeftDockWidgetArea,toolbox);

    verticeEditor = new VerticeEditorWidget();
    this->addDockWidget(Qt::RightDockWidgetArea,verticeEditor);
    verticeEditor->setHexBlocker(hexBlocker);
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
    connect(toolbox->moveVerticesW,SIGNAL(startSelect()),this,SLOT(slotStartSelectVertices()));
    connect(toolbox->moveVerticesW,SIGNAL(moveDone()),this,SLOT(slotResetInteractor()));
    connect(toolbox->moveVerticesW,SIGNAL(moveVertices()),this,SLOT(slotMoveVertices()));
    connect(styleVertPick,SIGNAL(selectionDone()),this,SLOT(slotEndSelectVertices()));
    connect(toolbox,SIGNAL(cancel()),this,SLOT(slotResetInteractor()));
    connect(this->ui->actionSetBCs,SIGNAL(triggered()),this,SLOT(slotOpenSetBCsDialog()));
    connect(toolbox->setBCsW,SIGNAL(startSelectPatches(vtkIdList *)),this,SLOT(slotStartSelectPatches(vtkIdList *)));
    connect(toolbox->setBCsW,SIGNAL(resetInteractor()), this, SLOT(slotResetInteractor()));
    connect(toolbox->setBCsW,SIGNAL(render()),this,SLOT(slotRender()));

    connect(this->ui->actionSetNumber,SIGNAL(triggered()),this,SLOT(slotOpenSetEdgePropsDialog()));
    connect(toolbox->edgePropsW,SIGNAL(startSelectEdges()),this,SLOT(slotStartSelectEdges()));
    connect(toolbox,SIGNAL(setStatusText(QString)),this,SLOT(slotShowStatusText(QString)));

    connect(this->ui->actionNewCase,SIGNAL(triggered()),this,SLOT(slotNewCase()));
    connect(this->ui->actionOpenBlockMeshDict,SIGNAL(triggered()),this, SLOT(slotOpenBlockMeshDict()));
    connect(this->ui->actionReOpenBlockMeshDict,SIGNAL(triggered()),this, SLOT(slotReOpenBlockMeshDict()));
    connect(this->ui->actionSave,SIGNAL(triggered()),this,SLOT(slotSaveBlockMeshDict()));
    connect(this->ui->actionSaveAs,SIGNAL(triggered()),this, SLOT(slotSaveAsBlockMeshDict()));
    connect(this->ui->actionMergePatch,SIGNAL(triggered()),this,SLOT(slotStartMergePatch()));
    connect(this->ui->actionDeleteBlocks,SIGNAL(triggered()),this,SLOT(slotStartDeleteHexBlock()));

    connect(this->ui->actionBlockVisibility,SIGNAL(triggered()),
            this,SLOT(slotHexObjVisibility()));
    connect(this->ui->actionPatchVisibility,SIGNAL(triggered()),
            this,SLOT(slotHexObjVisibility()));
    connect(this->ui->actionEdgeVisibility,SIGNAL(triggered()),
            this,SLOT(slotHexObjVisibility()));
    connect(this->ui->actionViewVerticeEditor,SIGNAL(toggled(bool)),
            verticeEditor,SLOT(setVisible(bool)));
    connect(toolbox->edgeSetTypeW,SIGNAL(startSelectPatch()),this,SLOT(slotStartSelectPatchForEdgeSetType()));
    connect(this->ui->actionAbout_Qt,SIGNAL(triggered()),
            qApp,SLOT(aboutQt()));
    connect(this->ui->actionAbout_hexBlocker,SIGNAL(triggered()),
            this,SLOT(slotAboutDialog()));
    connect(this->ui->actionArbitraryTest,SIGNAL(triggered()),this,SLOT(slotArbitraryTest()));

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
    verticeEditor->updateVertices();
    renwin->Render();
}

void MainWindow::slotStartDeleteHexBlock()
{
    toolbox->setCurrentIndex(0); // show empty page
    ui->statusbar->showMessage(tr(
      "Select the blocks to delete with left button, middle when done and right to deslect"),
      10000);
    styleActorPick->setSelection(
        InteractorStyleActorPick::block,
        InteractorStyleActorPick::multi
        );
    renwin->GetInteractor()->SetInteractorStyle(styleActorPick);

    hexBlocker->hidePatches();
    renwin->Render();
    connect(styleActorPick,SIGNAL(selectionDone()),this,SLOT(slotDeleteHexBlock()));


}

void MainWindow::slotDeleteHexBlock()
{
    disconnect(styleActorPick,SIGNAL(selectionDone()),this,SLOT(slotDeleteHexBlock()));
    renwin->GetInteractor()->SetInteractorStyle(defStyle);
    vtkIdList * selIds = styleActorPick->selectedIds;
    hexBlocker->removeHexBlocks(selIds);
    hexBlocker->showPatches();
    verticeEditor->updateVertices();
    hexBlocker->render();
}

void MainWindow::slotPrintHexBlocks()
{
    hexBlocker->PrintHexBlocks();
}

void MainWindow::slotStartExtrudePatch()
{
    toolbox->setCurrentIndex(0); // show empty page
    ui->statusbar->showMessage(
                tr("Left button to select a patch, middle button to confirm, rightbutton to deselect"));
    styleActorPick->setSelection(InteractorStyleActorPick::patch,
                                 InteractorStyleActorPick::single);
    renwin->GetInteractor()->SetInteractorStyle(styleActorPick);
    connect(styleActorPick,SIGNAL(selectionDone()),
            this,SLOT(slotExtrudePatch()));
}

void MainWindow::slotExtrudePatch()
{
    ui->statusbar->clearMessage();
    if(styleActorPick->selectedIds->GetNumberOfIds()>0)
    {
        bool ok;
        QString title = tr("Distance");
        QString label = tr("Set the distance to extrude, has to be a positive number");

        double dist = QInputDialog::getDouble(this,title,label,1.0,0,1e12,1,&ok);
        if(ok && (dist >0.0) )
        {
            hexBlocker->extrudePatch(styleActorPick->selectedIds,dist);
        }
        else
        {
            ui->statusbar->showMessage("Cancelled",3000);
        }
    }
    else
    {
        ui->statusbar->showMessage("No patches selected!",3000);
    }

    disconnect(styleActorPick,SIGNAL(selectionDone()),
               this,SLOT(slotExtrudePatch()));

    verticeEditor->updateVertices();
    hexBlocker->render();
    //renwin->GetInteractor()->SetInteractorStyle(defStyle);
    slotResetInteractor();
}

void MainWindow::slotOpenMoveVerticesDialog()
{
    toolbox->setCurrentIndex(2);
}

void MainWindow::slotStartSelectVertices()
{
    renwin->GetInteractor()->SetInteractorStyle(styleVertPick);
    renwin->GetInteractor()->SetPicker(areaPicker);
    styleVertPick->clearSelection();
    styleVertPick->StartSelect();
    ui->statusbar->showMessage(tr("Draw a box with left button to select, middle when finished, cntrl to control camera"),10000);

}

void MainWindow::slotEndSelectVertices()
{
    renwin->GetInteractor()->SetInteractorStyle(defStyle);
    hexBlocker->render();
}

void MainWindow::slotMoveVertices()
{
    if(toolbox->moveVerticesW->delta)
    {
        hexBlocker->moveVertices(styleVertPick->SelectedList,toolbox->moveVerticesW->dist);
    }
    else
    {
        bool setPos[3];
        setPos[0] = toolbox->moveVerticesW->checkedX;
        setPos[1] = toolbox->moveVerticesW->checkedY;
        setPos[2] = toolbox->moveVerticesW->checkedZ;
        hexBlocker->setVerticesPos(styleVertPick->SelectedList,toolbox->moveVerticesW->dist,setPos);
    }

    slotResetInteractor();
    verticeEditor->updateVertices();
    hexBlocker->render();
}

void MainWindow::slotRender()
{
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

void MainWindow::slotStartSelectPatches(vtkIdList *selectedPatches)
{
    styleActorPick->setSelection(InteractorStyleActorPick::patch,
                                 InteractorStyleActorPick::multi);
    styleActorPick->selectedIds->DeepCopy(selectedPatches);
    renwin->GetInteractor()->SetInteractorStyle(styleActorPick);
    connect(styleActorPick,SIGNAL(selectionDone()),
            this,SLOT(slotPatchSelectionDone()));

}

void MainWindow::slotPatchSelectionDone()
{
    disconnect(styleActorPick,SIGNAL(selectionDone()),
               this,SLOT(slotPatchSelectionDone()));
    toolbox->setBCsW->slotSelectionDone(styleActorPick->selectedIds);

}

void MainWindow::slotNewCase()
{

    hexBlocker->removeOrientationAxes();
    renwin->RemoveRenderer(hexBlocker->renderer);

    delete hexBlocker;
    hexBlocker = new HexBlocker();
    hexBlocker->renderer->SetBackground(.2, .3, .4);

    renwin->AddRenderer(hexBlocker->renderer);
    hexBlocker->initOrientationAxes(renwin);
    // repoint Axes interactor and widget
//    widget->SetInteractor( renwin->GetInteractor() );

    //Repoint interactors.
    styleVertPick->SetPoints(hexBlocker->vertData);
    styleVertPick->SelectedSphere=hexBlocker->vertSphere;
    styleActorPick->setHexBlocker(hexBlocker);

    //Repoint widgets
    // rensa bc's
//    toolbox->setBCsW->changeBCs(reader);

    toolbox->setHexBlockerPointer(hexBlocker);
    toolbox->setBCsW->clearBCs();
    verticeEditor->setHexBlocker(hexBlocker);
    slotRender();
}

void MainWindow::slotSaveAsBlockMeshDict()
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
    if(filename.isNull())
    {
        this->ui->statusbar->showMessage("Cancelled",3000);
        return;
    }
    saveFileName = filename;
    slotSaveBlockMeshDict();
}

void MainWindow::slotSaveBlockMeshDict()
{
    if(saveFileName.isNull())
    {
        slotSaveAsBlockMeshDict();
        return;
    }

    QFile file(saveFileName);

    QString title = tr("convertToMeters");
    QString label = tr("Factor to convert to meters. If you modelled in mm this is 0.001.");
    bool ok1;
    double conv2meters = QInputDialog::getDouble(this,title,label,1.0,1e-255,1e255,6,&ok1);

    if(!ok1)
    {
        this->ui->statusbar->showMessage("Cancelled",3000);
        return;
    }

    bool ok2 = file.open(QIODevice::WriteOnly | QIODevice::Text);

    if(!ok2)
    {
        this->ui->statusbar->showMessage("Error opening file",5000);
        return;
    }
    QTextStream out(&file);

    HexExporter * exporter = new HexExporter(hexBlocker);
    exporter->conv2meter=conv2meters;
    exporter->exporBlockMeshDict(out);

    openFileName = saveFileName;
    file.flush();
    file.close();
}


void MainWindow::slotOpenBlockMeshDict()
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
    openFileName=filename;
    slotReOpenBlockMeshDict();

}
void MainWindow::slotReOpenBlockMeshDict()
{
    if(openFileName.isNull())
    {
        slotOpenBlockMeshDict();
        return;
    }

    QFile file(openFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->statusbar->showMessage("Error opening file",3000);
        return ;
    }
    QTextStream * in = new QTextStream(&file);

    HexReader * reader = new HexReader();
    reader->readBlockMeshDict(in);

    file.close();

    hexBlocker->removeOrientationAxes();
    renwin->RemoveRenderer(hexBlocker->renderer);

    delete hexBlocker;
    hexBlocker = new HexBlocker();
    hexBlocker->renderer->SetBackground(.2, .3, .4);

    //reset pointers to hexBlocker in gui-classes
    toolbox->setHexBlockerPointer(hexBlocker);

    renwin->AddRenderer(hexBlocker->renderer);

    hexBlocker->readBlockMeshDict(reader);
    hexBlocker->initOrientationAxes(renwin);
    hexBlocker->edgesDict = reader->edgesDict;

    //Repoint interactors.
    styleVertPick->SetPoints(hexBlocker->vertData);
    styleVertPick->SelectedSphere=hexBlocker->vertSphere;
    styleActorPick->setHexBlocker(hexBlocker);

    slotRender();
    //Repoint widgets
    toolbox->setBCsW->changeBCs(reader);
    verticeEditor->setHexBlocker(hexBlocker);
//    verticeEditor->updateVertices();

}

void MainWindow::slotShowStatusText(QString text)
{
    //Show for 10 secs
    ui->statusbar->showMessage(text,10000);
}

void MainWindow::slotOpenSetEdgePropsDialog()
{
    toolbox->setCurrentIndex(4);
}

void MainWindow::slotStartSelectEdges()
{
    ui->statusbar->showMessage(tr("Select an edge, middle button when finished"),5000);
    hexBlocker->resetColors();
    styleActorPick->setSelection(InteractorStyleActorPick::edge,
                                 InteractorStyleActorPick::single);
    renwin->GetInteractor()->SetInteractorStyle(styleActorPick);
    connect(styleActorPick,SIGNAL(selectionDone()),
            this,SLOT(slotEdgeSelectionDone()));

    renwin->Render();
}


void MainWindow::slotEdgeSelectionDone()
{
    disconnect(styleActorPick,SIGNAL(selectionDone()),
               this,SLOT(slotEdgeSelectionDone()));
    vtkIdType edgeId = styleActorPick->selectedIds->GetId(0);
    toolbox->edgePropsW->setSelectedEdge(edgeId);
    renwin->GetInteractor()->SetInteractorStyle(defStyle);
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

void MainWindow::slotStartMergePatch()
{
    styleActorPick->setSelection(InteractorStyleActorPick::patch,
                                 InteractorStyleActorPick::pair);
    renwin->GetInteractor()->SetInteractorStyle(styleActorPick);
    connect(styleActorPick,SIGNAL(selectionDone()),
            this,SLOT(slotMergePatch()));
    this->ui->statusbar->showMessage(tr("Select first master and then slave, middle when done "),3000);
}

void MainWindow::slotMergePatch()
{
    disconnect(styleActorPick,SIGNAL(selectionDone()),
               this,SLOT(slotMergePatch()));
    vtkIdList * selectedPatches = styleActorPick->selectedIds;
    if(selectedPatches->GetNumberOfIds()<2)
    {
        this->ui->statusbar->showMessage("Cancelled",3000);
        slotResetInteractor();
        return;
    }
    else
    {
        hexBlocker->mergePatch(selectedPatches->GetId(0),selectedPatches->GetId(1));
    }

    verticeEditor->updateVertices();
    slotResetInteractor();
    slotRender();
}

void MainWindow::slotHexObjVisibility()
{
    //maybe sett all here
    hexBlocker->visibilityBlocks(this->ui->actionBlockVisibility->isChecked());
    hexBlocker->visibilityPatches(this->ui->actionPatchVisibility->isChecked());
    hexBlocker->visibilityEdges(this->ui->actionEdgeVisibility->isChecked());
    renwin->Render();
}

void MainWindow::slotStartSelectPatchForEdgeSetType()
{
    this->ui->actionPatchVisibility->setChecked(true);
    hexBlocker->visibilityPatches(true);
    styleActorPick->setSelection(InteractorStyleActorPick::patch,InteractorStyleActorPick::single);
    renwin->GetInteractor()->SetInteractorStyle(styleActorPick);
    ui->statusbar->showMessage("Select a patch with left button, deselect with right, middle when done.",10000);
    connect(styleActorPick,SIGNAL(selectionDone()),
            this,SLOT(slotStartSelectPatchForEdgeSetTypeDone()));
//

}

void MainWindow::slotStartSelectPatchForEdgeSetTypeDone()
{
    disconnect(styleActorPick,SIGNAL(selectionDone()),this,
               SLOT(slotStartSelectPatchForEdgeSetTypeDone()));
    if(styleActorPick->selectedIds->GetNumberOfIds() < 1)
    {
        slotShowStatusText("Cancelled");
        return;
    }
    toolbox->edgeSetTypeW->setSelectedPatch(
                styleActorPick->selectedIds->GetId(0));
}


void MainWindow::slotArbitraryTest()
{
    hexBlocker->arbitraryTest();
//      verticeEditor->updateVertices();
//    hexBlocker->createHexBlock();
//    verticeEditor->slotApply();
//    for(int i=0;i<100;i++)
//    {
//        hexBlocker->createHexBlock();
//    }
//    hexBlocker->render();
//    verticeEditor->slotApply();
}

void MainWindow::slotExit()
{
    qApp->exit(0);

}


