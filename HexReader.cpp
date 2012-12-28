#include "HexReader.h"
#include "HexBlocker.h"
#include "HexEdge.h"
#include "hexPatch.h"
#include "HexBlock.h"
#include "HexBC.h"

#include "vtkCollection.h"
#include "vtkPoints.h"
#include "vtkIdList.h"

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

    getBlocks();


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

bool HexReader::getBlocks()
{
    QString blocksString = getEntry("blocks");
//    std::cout << "blocks:\n" << blocksString.toAscii().data() << std::endl;

    QStringList hexList = blocksString.split(QString("hex"),QString::SkipEmptyParts);


    for(int i=1;i<hexList.size();i++)
    {
        QStringList entries = hexList.at(i).split(QString(")"),QString::SkipEmptyParts);
        if(entries.size() < 3)
        {
            std::cout << "error while reading block " << readBlocks->GetNumberOfItems() << std::endl;
            return false;
        }

//        for(int k=0;k<entries.size();k++)
//        {
//            std::cout << "entries.at(" << k<< ")" << entries.at(k).toAscii().data() << std::endl;
//        }


        QString vertIdsString = entries.at(0);
        QStringList vertIdsList = vertIdsString
                .replace(QString("("),QString(""))
                .simplified()
                .split(QString(" "),QString::SkipEmptyParts);

        if(vertIdsList.size() < 8)
        {
            std::cout << "error while reading block " << readBlocks->GetNumberOfItems() << std::endl;
            return false;
        }

        vtkSmartPointer<vtkIdList> hexVertIds =
                vtkSmartPointer<vtkIdList>::New();

        for(vtkIdType j=0;j<8;j++)
            hexVertIds->InsertNextId(vertIdsList.at(j).toInt());

        vtkSmartPointer<HexBlock> b =
                vtkSmartPointer<HexBlock>::New();
        b->init(hexVertIds,readVertices,readEdges,readPatches);

        //Reading cellnumbers
        QString cellNumbersStr = entries.at(1);
        QStringList cellNumbersList = cellNumbersStr
                .replace(QString("("),QString(""))
                .simplified()
                .split(QString(" "),QString::SkipEmptyParts);

        int cellNumbers[3]; bool ok[3];
        cellNumbers[0]=cellNumbersList.at(0).toDouble(&ok[0]);
        cellNumbers[1]=cellNumbersList.at(1).toDouble(&ok[1]);
        cellNumbers[2]=cellNumbersList.at(2).toDouble(&ok[2]);

        if(!(ok[0]&&ok[1]&&ok[2]))
        {
            std::cout << "error reading hex sizes of block"<< readBlocks->GetNumberOfItems() << i-2 <<std::endl;
            return false;
        }

        //Set number on edges
        for(vtkIdType j=0;j<12;j++)
        {
            HexEdge * e = HexEdge::SafeDownCast(readEdges->GetItemAsObject(b->edgeIds->GetId(j)));
            if(j<4)
                e->nCells = cellNumbers[0];
            else if(j<8)
                e->nCells = cellNumbers[1];
            else
                e->nCells = cellNumbers[2];
        }


        readBlocks->AddItem(b);
        //entries.at(2) contains type of grading and grading factors.
        //simpleGrading (1 1 1

    }

    // read number, just set them in the edges of the block. e0-e3 gets the same number etc
    return true;
}


