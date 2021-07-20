#!/bin/bash

mkdir simulate
echo -e "Quantos Pontos?"
read INPUT
cd scripts
make
./genpoints $INPUT
make clean
mv input.txt ../simulate
cd ..
make
mv hull simulate
cd simulate
./hull input.txt
cp ../scripts/fecho.plot .
cp ../scripts/pontos.plot .
npts=$(head -1 input.txt); tail -$npts input.txt > input2.txt
gnuplot ../scripts/pontos.plot
cp fecho.txt fecho2.txt; head -1 fecho.txt >> fecho2.txt
gnuplot fecho.plot
open pontos.pdf fecho.pdf 
