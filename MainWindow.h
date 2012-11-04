/*=========================================================================

  Program:   Visualization Toolkit
  Module:    MainWindow.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef MainWindow_H
#define MainWindow_H

#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
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
class vtkInteractorStyleTrackballCamera;
class ToolBoxWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  // Constructor/Destructor
  MainWindow();
  ~MainWindow();

  vtkSmartPointer<vtkPoints> mypoints;
  vtkSmartPointer<vtkPolyData> mydata;
  vtkSmartPointer<InteractorStyleVertPick> styleVertPick;
  vtkSmartPointer<InteractorStylePatchPick> stylePatchPick;
public slots:

  virtual void slotOpenFile();
  virtual void slotExit();
  virtual void slotCreateHexBlock();
  virtual void slotOpenCreateHexBlockDialog();
  virtual void slotPrintHexBlocks();
  virtual void slotExtrudePatch();
  virtual void slotOpenMoveVerticesDialog();
  virtual void slotMoveVertices();
  virtual void slotResetInteractor();
  virtual void slotOpenSetBCsDialog();
  void slotStartSelectPatches(vtkIdType bcID);
  void slotExportBlockMeshDict();
  void slotRender();

  //virtual void slotSetBCs();


protected:
   
protected slots:

private:
  vtkRenderWindow                         *renwin;
  HexBlocker                              *hexBlocker;
  vtkSmartPointer<vtkInteractorStyleTrackballCamera> defStyle;
  vtkSmartPointer<vtkAxesActor> axes;
  vtkSmartPointer<vtkOrientationMarkerWidget> widget;
  vtkSmartPointer<vtkAreaPicker> areaPicker;

  ToolBoxWidget *toolbox;
  // Designer form
  Ui_MainWindow *ui;
};

#endif // MainWindow_H
