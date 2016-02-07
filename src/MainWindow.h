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
    This is the main GUI window. It's and interface between the user
    and the main model class, HexBlocker. The idea MainWindow class is
    responsible for collection user inputs and data but all logics should
    be handled by HexBlocker. The exception is InteractorStyle* classes
    that have to pass Qt signals to the MainWindow class or other GUIs.
    Most connections are set up in the constructor but some (thos that
    require diconnecting) are set up in slots.

*/
#ifndef MainWindow_H
#define MainWindow_H

#include <vtkSmartPointer.h>    // Required for smart pointer internal ivars.
#include <QMainWindow>



// Forward Qt class declarations
class Ui_MainWindow;

// Forward VTK class declarations
class vtkRenderer;
class vtkRenderWindow;
class vtkPolyData;
class vtkPoints;
class HexBlocker;
class vtkRenderWindowInteractor;
class vtkAxesActor;
class vtkOrientationMarkerWidget;
class vtkAreaPicker;
class InteractorStyleVertPick;
class InteractorStylePatchPick;
class InteractorStyleEdgePick;
class InteractorStyleActorPick;
class vtkInteractorStyleTrackballCamera;
class ToolBoxWidget;
class VerticeEditorWidget;
class vtkIdList;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  // Constructor/Destructor
  MainWindow();
  ~MainWindow();

  //instances of interactor stiles.
  vtkSmartPointer<InteractorStyleVertPick> styleVertPick;
  vtkSmartPointer<InteractorStyleActorPick> styleActorPick;

  //open file name, main needs access
  QString openFileName;

public slots:

  void slotViewToolBar();
  void slotViewToolBox();
  void slotZoomOut();
  void slotExit();
  void slotCreateHexBlock();
  void slotOpenCreateHexBlockDialog();
  void slotStartDeleteHexBlock();
  void slotDeleteHexBlock();
  void slotPrintHexBlocks();
  void slotStartExtrudePatch();
  void slotExtrudePatch();
  void slotOpenMoveVerticesDialog();
  void slotStartSelectVertices();
  void slotEndSelectVertices();
  void slotMoveVertices();
  void slotOpenRotateVerticesDialog();
  void slotRotateVertices();
  void slotResetInteractor();
  void slotOpenSetBCsDialog();
  void slotPatchSelectionDone();
  void slotStartSelectPatches(vtkIdList * selectedPatches);
  void slotNewCase();
  void slotOpenBlockMeshDict();
  void slotReOpenBlockMeshDict();
  void slotOpenGeometry();
  void slotSaveAsBlockMeshDict();
  void slotSaveBlockMeshDict();
  void slotRender();
  void slotShowStatusText(QString text);
  void slotOpenSetEdgePropsDialog();
  void slotStartSelectEdges();
  void slotEdgeSelectionDone();
  void slotAboutDialog();
  void slotArbitraryTest();
  void slotStartMergePatch();
  void slotMergePatch();
  void slotHexObjVisibility();

  void slotStartSelectPatchForEdgeSetType();
  void slotStartSelectPatchForEdgeSetTypeDone();


protected:
   
protected slots:

private:
  vtkRenderWindow                         *renwin;
  HexBlocker                              *hexBlocker;
  vtkSmartPointer<vtkInteractorStyleTrackballCamera> defStyle;
  vtkSmartPointer<vtkAreaPicker> areaPicker;

  ToolBoxWidget *toolbox;
  VerticeEditorWidget *verticeEditor;
  // Designer form
  Ui_MainWindow *ui;
  QString saveFileName;
};

#endif // MainWindow_H
