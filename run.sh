#!/bin/bash

rm -f fecho.txt
make
./hull debug/input.txt
make clean