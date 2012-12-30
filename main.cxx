/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */
// QT includes
#include <QApplication>
#include <QCleanlooksStyle>
#include "MainWindow.h"
#include <iostream>

extern int qInitResources_icons();

int main( int argc, char** argv )
{

  // QT Stuff
  QApplication app( argc, argv );

  QCleanlooksStyle * cleanlooks = new QCleanlooksStyle;

  QApplication::setStyle(cleanlooks);

  app.setStyle("QToolTip {background-color: black;}");
  
  qInitResources_icons();

  MainWindow myMainWindow;

  myMainWindow.setStyleSheet("QToolTip {background-color: black;}");
  myMainWindow.show();

  return app.exec();
}
