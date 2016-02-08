These are the installation instructions for HexBlocker an Open Source program
for creating blockMeshDict files for OpenFOAM.

If you can compile and install OpenFOAM with ParaView then the install.sh script
should run just fine. Just run 

./install.sh

And everything should work. Right.

I have only tested to build hexBlocker on Fedora 23. HexBlocker workds with the fedora packaged vtk. You can install it with
dnf install vtk vtk-devel vtk-qt

In wich case you should modify the installscript so it doesn't download
VTK.


For OpenSUSE the following packages names are different.
libqt4-dev ---> libqt4-devel
zlib1g-dev ---> zlib-devel
libreadline-dev ---> readline-devel
libncurses-dev ---> ncurses-devel
libxt-dev ---> libXt-devel

I would also recommend to install qtcreator if you want to modify hexBlocker

Best of luck

