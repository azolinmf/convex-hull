#!/bin/bash
DIR_NAME=1942808_1508920_1936344
mkdir $DIR_NAME
cp -f tsp.cpp $DIR_NAME
cp -f Makefile $DIR_NAME
zip -r $DIR_NAME.zip $DIR_NAME
rm -rf $DIR_NAME
