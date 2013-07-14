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

#include "HexReader.h"
#include "HexBlocker.h"
#include "HexEdge.h"
#include "HexPatch.h"
#include "HexBlock.h"
#include "HexBC.h"

#include "vtkCollection.h"
#include "vtkPoints.h"
#include "vtkIdList.h"

#include <iostream>
#include <QtGui>

#include <QTextStream>
#include <QRegExp>

HexReader::HexReader()
{
    readVertices  = vtkSmartPointer<vtkPoints>::New();
    readPatches = vtkSmartPointer<vtkCollection>::New();
    readBlocks  = vtkSmartPointer<vtkCollection>::New();
    readEdges   = vtkSmartPointer<vtkCollection>::New();
    readBCs     = vtkSmartPointer<vtkCollection>::New();
}

int HexReader::readBlockMeshDict(QTextStream *in)
{


    fileContents = readFileContents(in);

    //Read vertices
    getVertices();

    getBlocks();

    getBCs();

    getEdges();
    return 0;

}


QString HexReader::readFileContents(QTextStream *in)
{
    QString line = in->readLine();

    int rownr;
    while(!line.isNull())
    {
        line = removeDoubleSlashes(line);

        //remove /* ... */
        int slashstar = line.indexOf(QString("/*"));
        if(slashstar>-1)
        {
            line = line.left(slashstar);
            QString newline = in->readLine();
            int starslash = newline.indexOf("*/");
            while(!newline.isNull() && starslash == -1)
            {
                newline = in->readLine();
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
        line=in->readLine();
        rownr++;
    }

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

QString HexReader::getEntry(QString entry, QString container)
{
    int startEntry = container.indexOf(entry);
    if(startEntry<0)
    {
        std::cout << "Warning: Entry \""
                  << entry.toAscii().data()
                  << "\" was not found" << std::endl;
        return QString("");
    }
    QString contentsFromStart = container.right(container.size()-startEntry);

    QString returnEntry("");

    int parenthesisLevel=0;
    for(int i =0; i < contentsFromStart.size();i++)
    {
        if( "(" == contentsFromStart.at(i))
        {
            parenthesisLevel++;
        }
        else if( ")" == contentsFromStart.at(i))
        {
            parenthesisLevel--;
        }
        else if(";" == contentsFromStart.at(i) && parenthesisLevel==0)
        {
             //add ";" ?
            break;
        }
        else if(parenthesisLevel<0)
        {
            std::cout << "Error unmatching parenthesis while getting "
                      << entry.toAscii().data() << std::endl;
        }
        returnEntry.append(QChar(contentsFromStart.at(i)));
    }
    return returnEntry.simplified();
}


bool HexReader::getVertices()
{
    QString verticesString = getEntry(QString("vertices"),fileContents);
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
    QString blocksString = getEntry("blocks",fileContents);

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
            std::cout << "error reading cell numbers of hexBlock"<< readBlocks->GetNumberOfItems() << i-2 <<std::endl;
            return false;
        }

        //get gradings // stället för break set OK no och sätt default.
        int gradings[12];bool readGradingsOK=true;
        QString gradingStr = entries.at(2);

        //for removing *grading( from gradingstr
        QRegExp reg=QRegExp("*(",Qt::CaseInsensitive,QRegExp::Wildcard);


        if(gradingStr.contains("simplegrading",Qt::CaseInsensitive))
        {

            QStringList gradingStrList =
                    gradingStr.replace(reg,"").simplified().split(" ",QString::SkipEmptyParts);

            if(gradingStrList.count()!=3)
            {
                readGradingsOK = false;
            }
            //read three doubles
            bool latestOk;
            gradings[0] = gradings[1] = gradings[2] = gradings[3] =
                    gradingStrList.at(0).toDouble(&latestOk);
            readGradingsOK = readGradingsOK && latestOk;
            gradings[4] = gradings[5] = gradings[6] = gradings[7] =
                    gradingStrList.at(1).toDouble(&latestOk);
            readGradingsOK = readGradingsOK && latestOk;
            gradings[8] = gradings[9] = gradings[10] = gradings[11] =
                    gradingStrList.at(2).toDouble(&latestOk);
            readGradingsOK = readGradingsOK && latestOk;

        }
        else if (gradingStr.contains("edgeGrading",Qt::CaseInsensitive))
        {
            QStringList gradingStrList =
                    gradingStr.replace(reg,"").simplified().split(" ",QString::SkipEmptyParts);

            if(gradingStrList.count()!=12)
            {
                readGradingsOK=false;
            }
            bool latestOk;
            for(int ei=0;ei<12;ei++)
            {
                gradings[ei]=gradingStrList.at(ei).toDouble(&latestOk);
                readGradingsOK=readGradingsOK && latestOk;
            }
        }
        else
        {
                readGradingsOK = false;
        }
        //if couldn't read set default value 1.
        if(!readGradingsOK)
        {
            errorInGrading(i,entries.at(2));
            for(int ei=0;ei<12;ei++)
            {
                gradings[ei]=1;
            }
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

            e->grading=gradings[j];
        }


        readBlocks->AddItem(b);


    }

    return true;
}

void HexReader::errorInGrading(vtkIdType hexNum,QString entry)
{
    std::cout << "Error while reading grading of block "
              << hexNum << "the entry was: " << std::endl
              << entry.toAscii().data()
              << "setting it to 1" << std::endl;
}

bool HexReader::getBCs()
{
    QString boun = getEntry(QString("boundary"),fileContents);
    if(boun.isEmpty())
    {
        std::cout << "Did not find the entry \"boundary\". BCs have not been read." << std::endl;
        return false;
    }

    QStringList listBCs = boun.replace(QRegExp("boundary *\\("),QString(" "))
            .split(QString("}"),QString::SkipEmptyParts);


    for(int i=0;i<listBCs.size()-1;i++)
    {
        vtkSmartPointer<HexBC> newBC =
                vtkSmartPointer<HexBC>::New();
        newBC->globalPatches = readPatches;
        //get bc name
        QString name = listBCs.at(i).simplified().split("{").at(0).simplified();
        if(name.isEmpty())
        {
            std::cout << "Something went wrong when reading bc nr " << i<< std::endl;
            return false;
        }
        newBC->name = name.toAscii().data();


        //get bc type
        QStringList typeList = getEntry(QString("type"),listBCs.at(i).simplified())
                .split(QString(" "),QString::SkipEmptyParts);
        if(typeList.size() < 2)
        {
            std::cout << "Error reading boundary " << name.toAscii().data() << std::endl;
            return false;
        }

        QString type = typeList.at(1);
        newBC->type = type.toAscii().data();

        QString patchesStr = getEntry(QString("faces"),listBCs.at(i)).replace("faces","").simplified();

        QStringList patchesList = patchesStr.replace("(","").split(QString(")"),QString::SkipEmptyParts);

        for(int j=0;j<patchesList.size()-1;j++)
        {
            QStringList patchIdsList = patchesList.at(j).simplified().split(" ",QString::SkipEmptyParts);
            if(patchIdsList.size() < 4)
            {
                std::cout << " Error while reading boundary " << name.toAscii().data()
                          << " patch " << j << " has to few entries" <<std::endl;
                return false;
            }
            vtkSmartPointer<vtkIdList> patchVertIds =
                    vtkSmartPointer<vtkIdList>::New();

            bool ok[4];
            patchVertIds->InsertNextId(patchIdsList.at(0).toInt(&ok[0]));
            patchVertIds->InsertNextId(patchIdsList.at(1).toInt(&ok[1]));
            patchVertIds->InsertNextId(patchIdsList.at(2).toInt(&ok[2]));
            patchVertIds->InsertNextId(patchIdsList.at(3).toInt(&ok[3]));

            if(!(ok[0]&&ok[1]&&ok[2]&&ok[3]))
            {
                std::cout << " Error while reading boundary " << name.toAscii().data()
                          << " patch " << j << " invalid entry" <<std::endl;
                return false;
            }



           newBC->insertPatchIfIdsExists(patchVertIds);

        }
        readBCs->AddItem(newBC);
    }


    return true;
}


bool HexReader::getEdges()
{
    // All line breaks and comments are removed
    // so there is no nice structure
    edgesDict = getEntry("edges",fileContents);

    if(!edgesDict.isEmpty())
    {
        //remove edges( and the terminating )
        edgesDict.replace(QRegExp("edges[\\s,\\n]*\\("),"");
        int index=edgesDict.lastIndexOf(")");
        edgesDict = edgesDict.left(index);
        edgesDict = edgesDict.simplified();

        //each entry starts with the type
        QRegExp typeReg = QRegExp("[a-z]+",Qt::CaseInsensitive);
        while(edgesDict.contains(typeReg)) //while? och capa med right?
        {
            index = edgesDict.indexOf(typeReg);
            QString type=typeReg.cap(0);
            int length = typeReg.matchedLength();
            //nextInd = -1 if there are no more matches
            int nextInd = edgesDict.indexOf(typeReg,index+length);
            //returns the entire string if nextInd=-1
            QString edgeDict=edgesDict.mid(index,nextInd-index);

            //fix edgesDict for next iteration
            if(nextInd > -1)
                edgesDict=edgesDict.mid(nextInd-1);//what I think right(nextInd) should do
            else
                edgesDict="";
            //get vertIds

            QStringList edgeDictList =
                    edgeDict.simplified().split(" ");
            if(edgeDictList.size() < 3) //6?
            {
                badEdgeEntry(edgeDict);
                continue;
            }

            bool ok0,ok1;
            vtkIdType vId0 =
                    vtkIdType(edgeDictList.at(1).toInt(&ok0));
            vtkIdType vId1 =
                    vtkIdType(edgeDictList.at(2).toInt(&ok1));
            vtkIdType eId = findEge(vId0,vId1);
            if(eId < 0)
            {
                badEdgeEntry(edgeDict);
                continue;
            }
            HexEdge *e = HexEdge::SafeDownCast(
                        readEdges->GetItemAsObject(eId));
            if(!(ok0 && ok1))
            {
                badEdgeEntry(edgeDict);
                continue;
            }
            if(type == "line")
            {
                //do nothin'
            }
            else if(type == "arc")
            {
                double pos[3];
                int pInd0 = edgeDict.indexOf("(");
                int pInd1 = edgeDict.indexOf(")");
//                QString foo = edgeDict.mid(pInd0+1,pInd1-1);
                QStringList vertList = edgeDict.mid(pInd0+1,pInd1-pInd0-1)
                        .simplified().split(" ",QString::SkipEmptyParts);
                ok1=true;ok0=true;
                pos[0]=vertList.at(0).toDouble(&ok1);
                ok0=ok0 & ok1;
                pos[1]=vertList.at(1).toDouble(&ok1);
                ok0=ok0 & ok1;
                pos[2]=vertList.at(2).toDouble(&ok1);
                ok0=ok0 & ok1;

                e->setType(HexEdge::ARC);
                e->setControlPoint(0,pos);
                e->redrawedge();
                if(!ok0)
                {
                    badEdgeEntry(edgeDict);
                    continue;
                }

            }
            else
            {
                badEdgeEntry(edgeDict);
                continue;
            }

        }

        return true;
    }
    else
    {
        return false;
    }
}

vtkIdType HexReader::findEge(vtkIdType vId0, vtkIdType vId1)
{
    for(vtkIdType i=0;i<readEdges->GetNumberOfItems();i++)
    {
        HexEdge *e = HexEdge::SafeDownCast(
                    readEdges->GetItemAsObject(i));
        if(e->hasVertice(vId0) && e->hasVertice(vId1) )
        {
            //check for correct order
            if(e->vertIds->GetId(0)!=vId0)
                std::cout << "Warning edge (" << vId0 <<" " << vId1 <<")"
                          << " was precribed with wrong order. This could cause problems." <<std::endl;
            return i;
        }
    }
    return -1;

}

void HexReader::badEdgeEntry(QString edgeDict)
{
    std::cout << "warning can\'t parse edgeDict: " << edgeDict.toAscii().data()
              << ". Note that some types are not yet supported." << std::endl;
}
