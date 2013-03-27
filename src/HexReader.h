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
    This class reads a blockMeshDict and stores vertices, patches and
    hexBlocks. It does not support advanced functions, code objects
    nor parameters.
*/

#ifndef HEXREADER_H
#define HEXREADER_H

#include <QObject>
#include <QFile>
#include <vtkSmartPointer.h>

//Pre declarations
class vtkCollection;
class HexEdge;
class HexPatch;
class HexBC;
class HexBlock;
class QTextStream;
class vtkPoints;




class HexReader : public QObject
{
    Q_OBJECT
public:
    HexReader();

    //FUNCTIONS
    int readBlockMeshDict(QTextStream *in);
    //DATA
    vtkSmartPointer<vtkPoints>     readVertices;
    vtkSmartPointer<vtkCollection> readPatches; //global patch list
    vtkSmartPointer<vtkCollection> readBlocks;  //read blocks
    vtkSmartPointer<vtkCollection> readBCs; //read boundary conditions
    vtkSmartPointer<vtkCollection> readEdges; //read edges

    QString edgesDict; //such as egdes or mergePairs
private:
    //FUNCTIONS

    // reads from where ever "in" is until the next ;
    // remove comments //, /*...*/ and line breaks
    QString readFileContents(QTextStream *in);
    // removes // and everyting after from a QString
    QString removeDoubleSlashes(QString line);

    void errorInGrading(vtkIdType hexNum, QString entry);

    //fills vtkPoints with points from fileContents,
    //returns true if succesfull
    bool getVertices();

    //Reads and creates blocks
    bool getBlocks();

    //Reads BCs
    bool getBCs();

    //Read edges, only reads there's not yet
    //any way to change this data in hexBlocker
    bool getEdges();

    //returns an entry as a QString e.g. vertices or blocks from fileContents
    QString getEntry(QString entry,QString container);
    //DATA
    QString fileContents;
};

#endif // HEXEXPORTER_H
