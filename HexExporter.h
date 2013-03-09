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
    This class handles exporting to blockMeshDict format. It's only
    been tested to work with OpenFOAM-2.1.x. It needs a pointer to
    the HexBlocker class.
*/

#ifndef HEXEXPORTER_H
#define HEXEXPORTER_H

#include <QObject>

class HexBlocker;
class QTextStream;


class HexExporter : public QObject
{
    Q_OBJECT
public:
    HexExporter();
    HexExporter(HexBlocker *);

    void exporBlockMeshDict(QTextStream &out);
    HexBlocker *hexB;
    double conv2meter;

};

#endif // HEXEXPORTER_H
