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
set -x
# Modifyable data
baseDir=$PWD
vtkBuild=$baseDir/build/VTK
vtkInstall=$baseDir/VTK
vtkSRC=$baseDir/vtkSrc
hexBuild=$baseDir/build/hexBlocker
hexBin=$baseDir/bin
hexSRC=$baseDir/src
nprocs=4
debug="true" #set to true to build debug version aswell.
#---------------------------------------------------------------------------
# Try to find qt exes
QMAKE=$(which qmake)
MOC=$(which moc-qt4 moc | head -n1)
UIC=$(which uic-qt4 uic | head -n1)
if [ -z "$QMAKE" -o -z "$MOC" -o -z "$UIC" ]; then
    echo Did not find qmake. 
    exit 1
fi

#Get vtk:
vtkpgk=VTK-6.3.0.tar.gz
if [ ! -f $vtkpgk ];then
    wget -c http://www.vtk.org/files/release/6.3/VTK-6.3.0.tar.gz ||
	( echo "could not download vtk" ; exit 1)
fi

#unpack vtk src
if [ ! -d $vtkSRC ];then
    tar -xzf $vtkpgk
    mv VTK-6.3.0 $vtkSRC
fi

#build VTK
if [ ! -d $vtkBuild ];then
    mkdir -p $vtkBuild
    cd $vtkBuild

#cmake --build --config=Release
    cmake \
	-DVTK_USE_GUISUPPORT:BOOL=ON \
	-DVTK_USE_QT:BOOL=ON \
	-DVTK_USE_QVTK:BOOL=ON \
	-DVTK_Group_Qt=ON \
	-DBUILD_SHARED_LIBS:BOOL=ON \
	-DCMAKE_INSTALL_PREFIX=$vtkInstall \
	-DCMAKE_C_FLAGS=-DGLX_GLXEXT_LEGACY \
	-DCMAKE_CXX_FLAGS=-DGLX_GLXEXT_LEGACY \
	$vtkSRC || (echo "error in cmake, VTK"; exit 1)

    make -j $nprocs || (echo "error in make, VTK"; exit 1)

    make install
fi
 
#Build HexBlocker
cd $baseDir
mkdir -p $hexBuild
cd $hexBuild 
cmake \
    -DVTK_DIR=$vtkInstall/lib/cmake/vtk-6.3 \
    -DQT_QMAKE_EXECUTABLE=$QMAKE \
    -DQT_MOC_EXECUTABLE=$MOC \
    -DQT_UIC_EXECUTABLE=$UIC \
    -DCMAKE_INSTALL_PREFIX=$hexBin \
    $hexSRC || (echo "error in cmake for hexBlocker";exit 1)

make -j $nprocs || (echo "error in make for hexBlocker";exit 1)

make install

#Buil debug version (optional)

if [ $debug == "true" ];then
    mkdir -p $hexBuild-dbg
    cd $hexBuild-dbg
    cmake \
	-DCMAKE_BUILD_TYPE=Debug \
	-DVTK_DIR=$vtkInstall/lib/cmake/vtk-6.3  \
	-DQT_QMAKE_EXECUTABLE=$QMAKE \
	-DQT_MOC_EXECUTABLE=$MOC \
	-DQT_UIC_EXECUTABLE=$UIC \
	-DCMAKE_INSTALL_PREFIX=$hexBin \
	$hexSRC || (echo "error in cmake for hexBlocker";exit 1)

    make -j $nprocs || (echo "error in make for hexBlocker";exit 1)

    make install
fi

exit 0


