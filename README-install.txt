These are the installation instructions for HexBlocker an Open Source program
for creating blockMeshDict files for OpenFOAM.

If you can compile and install OpenFOAM with ParaView then the install.sh script
should run just fine. Just run 

./install.sh

And everything should work. Right.

I have only tested to build hexBlocker on ubuntu 12.04. The needed packages
are 

sudo apt-get install build-essential flex bison cmake zlib1g-dev qt4-dev-tools libqt4-dev gnuplot libreadline-dev libncurses-dev libxt-dev

I would also recommend to install qtcreator if you want to modify hexBlocker

Best of luck

