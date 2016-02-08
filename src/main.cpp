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

  //Assume that a blockMeshDict is given on the commandline
  if(argc > 1)
  {
      myMainWindow.openFileName = app.arguments().at(1);
      myMainWindow.slotReOpenBlockMeshDict();
  }
  return app.exec();
}
