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

#include "PointsTableModel.h"
#include <vtkIdList.h>
#include <vtkPoints.h>
PointsTableModel::PointsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    hasPointsBeenSet=false;
    showOnlyIds=0;
    points=0;
}


int PointsTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(showOnlyIds != 0)
        return showOnlyIds->GetNumberOfIds();
    else if(points !=0)
        return points->GetNumberOfPoints();
    else
        return 0;
}

int PointsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant PointsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row,col;
    row = index.row();
    col = index.column();

    if (row < 0 || col < 0 || col > 3)
        return QVariant();

    if(role == Qt::DisplayRole && col == 0)
        return row; //show index
    else
        col=col-1; //shift col (index is not part of vtkPoints)

    if (role == Qt::DisplayRole && hasPointsBeenSet)
    {
        double pos[3];
        if(showOnlyIds != 0 &&
                row < showOnlyIds->GetNumberOfIds())
        {
                points->GetPoint(showOnlyIds->GetId(row),pos);
                return QVariant(pos[col]);
        }
        else if(row < points->GetNumberOfPoints())
        {
            points->GetPoint(row,pos);
            return QVariant(pos[col]);
        }
        else
            return QVariant();
    }
    return QVariant();
}

QVariant PointsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QVariant("NR");
        case 1:
            return QVariant("X");

        case 2:
            return QVariant("Y");
        case 3:
            return QVariant("Z");
        default:
                return QVariant();
        }
    }
    return QVariant();
}

bool PointsTableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    endInsertRows();
    return true;
}

bool PointsTableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);


    endRemoveRows();
    return true;
}

bool PointsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    if(col == 0)
        return false;

    bool isDouble;
    double val = value.toDouble(&isDouble);
    if (isDouble && index.isValid() && role == Qt::EditRole && hasPointsBeenSet && col < 4)
    {
        double pos[3];
        if( showOnlyIds != 0 && row < showOnlyIds->GetNumberOfIds())
        {
            points->GetPoint(showOnlyIds->GetId(row),pos);
            pos[col-1]=val;
            points->SetPoint(showOnlyIds->GetId(row),pos);
        }
        else if(row < points->GetNumberOfPoints())
        {
            points->GetPoint(row,pos);
            pos[col-1]=val;
            points->SetPoint(row,pos);
        }
        else
        {
            return false;
        }

        points->Modified();
        emit pointEdited();
//        emit dataChanged(index, index);

        return true;
    }

    return false;
}

Qt::ItemFlags PointsTableModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fgs;

    if (!index.isValid())
        return Qt::ItemIsDragEnabled;

    if (index.column() > 0)
    {
        fgs = QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    }
    else
    {
        fgs = Qt::ItemIsEditable;
    }

    return fgs;
//        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}


void PointsTableModel::setPoints(vtkPoints *pnts)
{
    points=0;
    hasPointsBeenSet=false;
    beginResetModel();
    endResetModel();
    showOnlyIds=0;
    points = pnts;
    hasPointsBeenSet=true;
    int numPoints = points->GetNumberOfPoints();
    if(numPoints>0)
    {
        beginInsertRows(QModelIndex(),0,numPoints-1);
        //actual data will be updated by calls to "data(...)"
        endInsertRows();
    }

}

void PointsTableModel::setPoints(vtkPoints *pnts, vtkIdList *shwOnlyIds)
{

    beginResetModel();
    endResetModel();
    points = pnts;
    showOnlyIds = shwOnlyIds;
    hasPointsBeenSet=true;

    int numPoints = shwOnlyIds->GetNumberOfIds();

    if(numPoints>0)
    {
        beginInsertRows(QModelIndex(),0,numPoints-1);
        //actual data will be updated by calls to "data(...)"
        endInsertRows();
    }
}

void PointsTableModel::update()
{
    int rows = rowCount(QModelIndex());
    int cols = columnCount(QModelIndex());
    //clear table
    beginResetModel();
    endResetModel();
    //reinsert points
    if(rows>0)
    {
        beginInsertRows(QModelIndex(),0,rows-1);
        //actual data will be updated by calls to "data(...)"
        endInsertRows();
    }
    QModelIndex ind0 = this->createIndex(0,0);
    QModelIndex ind1 =this->createIndex(rows,cols);
    emit dataChanged(ind0,ind1);
}
