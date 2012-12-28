#include "HexReader.h"
#include "HexBlocker.h"
#include "HexEdge.h"
#include "hexPatch.h"
#include "HexBlock.h"
#include "HexBC.h"

#include "vtkCollection.h"
#include "vtkPoints.h"

#include <iostream>
#include <QtGui>

#include <QTextStream>

HexReader::HexReader()
{
    readVertices  = vtkSmartPointer<vtkPoints>::New();
    readPatches = vtkSmartPointer<vtkCollection>::New();
    readBlocks  = vtkSmartPointer<vtkCollection>::New();
    readEdges   = vtkSmartPointer<vtkCollection>::New();
    readBCs     = vtkSmartPointer<vtkCollection>::New();

}

int HexReader::readBlockMeshDict(QString filename)
{


    QString fileContents = readFileContents(filename);

    std::cout << "file contents are \n" << fileContents.toAscii().data() << std::endl;

    //Read vertices
    getVertices();

    //2do: fyll en vtkpoints här!

    return 0;

}


QString HexReader::readFileContents(QString filename)
{
    std::cout << "reading" << std::endl;
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream in(&file);
    QString line = in.readLine();


    int rownr;
    while(!line.isNull())
    {
        line = removeDoubleSlashes(line);

        //remove /* ... */
        int slashstar = line.indexOf(QString("/*"));
        if(slashstar>-1)
        {
            line = line.left(slashstar);
            QString newline = in.readLine();
            int starslash = newline.indexOf("*/");
            while(!newline.isNull() && starslash == -1)
            {
                newline = in.readLine();
                starslash = newline.indexOf("*/");
            }
            if(newline.isNull())
            {
                std::cout << "Error while skiping /* .. */ on row "
                          << rownr << ". end of file reached inside comment block." << std::endl;
            }
            line.append(newline.remove(0,starslash+2));
            line = removeDoubleSlashes(line);
        }

        fileContents.append(line);
        line=in.readLine();
        rownr++;
    }


    file.close();
    return fileContents.simplified();
}

QString HexReader::removeDoubleSlashes(QString line)
{
    int dslash = line.indexOf(QString("//"));
    if(dslash >-1)
    {
        line = line.left(dslash);
    }
    return line;
}

QString HexReader::getEntry(QString entry)
{
    int startEntry = fileContents.indexOf(entry);
    int endEntry = fileContents.indexOf(QString(";"),startEntry);
    if(startEntry<0 || endEntry<0)
    {
        std::cout << "Error when reading entry: " << entry.toAscii().data() << std::endl;
        return QString();
    }
    return fileContents.mid(startEntry,endEntry-startEntry).simplified();
}


bool HexReader::getVertices()
{
    QString verticesString = getEntry(QString("vertices"));
    QStringList vertsList = verticesString.split("(",QString::SkipEmptyParts);

    for(int i=2;i<vertsList.size();i++)
    {
        QString vert = vertsList.at(i);
        vert=vert.replace(QString(","),QString(" ")).replace(QString(")"),QString(" ")).simplified();
        QStringList vertCompList = vert.split(QString(" "),QString::SkipEmptyParts);

        double coords[3]; bool ok[3];
        coords[0] = vertCompList.at(0).toDouble(&ok[0]);
        coords[1] = vertCompList.at(1).toDouble(&ok[1]);
        coords[2] = vertCompList.at(2).toDouble(&ok[2]);
        if(!(ok[0]&&ok[1]&&ok[2]))
        {
            std::cout << "error reading vertice " << i-2 <<std::endl;
            return false;
        }

        readVertices->InsertNextPoint(coords);
    /*
        std::cout << "vert " << i-2 << ": "
                  << coords[0] << " "
                  << coords[1] << " "
                  << coords[2] << " "
                  << std::endl;
    */
    }

    return true;
}


