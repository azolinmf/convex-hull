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
