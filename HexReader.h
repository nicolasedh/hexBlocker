#ifndef HEXREADER_H
#define HEXREADER_H

#include <QObject>
#include <QFile>
#include <vtkSmartPointer.h>

class vtkCollection;
class HexEdge;
class hexPatch;
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
    int readBlockMeshDict(QString filename);
    //DATA
    vtkSmartPointer<vtkPoints>     readVertices;
    vtkSmartPointer<vtkCollection> readPatches; //global patch list
    vtkSmartPointer<vtkCollection> readBlocks;  //read blocks
    vtkSmartPointer<vtkCollection> readBCs; //read boundary conditions
    vtkSmartPointer<vtkCollection> readEdges; //read edges
private:
    //FUNCTIONS

    // reads from whereever "in" is until the next ;
    // remove comments // and /*...*/
    QString readFileContents(QString filename);
    // removes // and everyting after from a QString
    QString removeDoubleSlashes(QString line);


    //fills vtkPoints with points from fileContents,
    //returns true if succesfull
    bool getVertices();

    //Reads and creates blocks
    bool getBlocks();

    //Reads BCs
    bool getBCs();



    //returns an entry as a QString e.g. vertices or blocks from fileContents
    QString getEntry(QString entry,QString container);
    //DATA
    QString fileContents;
};

#endif // HEXEXPORTER_H
