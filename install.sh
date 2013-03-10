#!/bin/bash
#Copyright 2012, 2013
#Author Nicolas Edh,
#Nicolas.Edh@gmail.com,
#or user "nsf" at cfd-online.com
#
#License
#    This file is part of hexBlocker.
#
#    hexBlocker is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    hexBlocker is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with hexBlocker.  If not, see <http://www.gnu.org/licenses/>.
#
#    The license is included in the file COPYING.
#
#Description
#     This script first download  VTK version 5.10.0 then builds vtk and 
#     then builds HexBlocker.
#

# Modifyable data
baseDir=$PWD
vtkBuild=$baseDir/build/VTK
vtkSRC=$baseDir/VTK
hexBuild=$baseDir/build/hexBlocker
hexBin=$baseDir/bin
hexSRC=$baseDir/hexBlocker
nprocs=4
#---------------------------------------------------------------------------
# Try to find qt exes
QMAKE=$(which qmake)
MOC=$(which moc-qt4)
UIC=$(which uic-qt4)
if [ -z "$QMAKE" -o -z "$MOC" -o -z "$UIC" ]; then
    echo Did not find qmake. 
    exit 1
fi

#Get vtk:
vtkpgk=vtk-5.10.0.tar.gz
if [ ! -f $vtkpgk ];then
    wget -c http://www.vtk.org/files/release/5.10/$vtkpgk || \
	( echo "could not download vtk" ; exit 1)
fi

#unpack vtk src
tar -xzf $vtkpgk

#build VTK
mkdir -p $vtkBuild
cd $vtkBuild
#cmake --build --config=Release
cmake \
    -DVTK_USE_GUISUPPORT:BOOL=ON \
    -DVTK_USE_QT:BOOL=ON \
    -DVTK_USE_QVTK:BOOL=ON \
    -DBUILD_SHARED_LIBS:BOOL=ON \
    $vtkSRC || (echo "error in cmake, VTK"; exit 1)

make -j $nprocs || (echo "error in make, VTK"; exit 1)

#Build HexBlocker
cd $baseDir
mkdir -p $hexBuild
cd $hexBuild 
cmake -DVTK_DIR=$vtkBuild \
    -DQT_QMAKE_EXECUTABLE=$QMAKE \
    -DQT_MOC_EXECUTABLE=$MOC \
    -DQT_UIC_EXECUTABLE=$UIC \
    -DCMAKE_INSTALL_PREFIX=$hexBin \
    $hexSRC || (echo "error in cmake for hexBlocker";exit 1)

make -j $nprocs || (echo "error in make for hexBlocker";exit 1)

make install

echo "Congratz everythin compiled"
exit 0


