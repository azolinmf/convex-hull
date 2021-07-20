#!/bin/bash

npts=$(head -1 $1); tail -$npts $1 > input2.txt; gnuplot pontos.plot
cp $2 fecho2.txt; head -1 $2 >> fecho2.txt; gnuplot fecho.plot
open fecho.pdf
