#!/bin/bash

cd src/CellularAutomatonSynth
qmake 
make
sudo make install

cd ..
mkdir -p ../utils
gcc rule.c -o ../utils/rule

cd EnvelopeFollower
qmake 
make
sudo make install


echo "install complete!"