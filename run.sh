#!/bin/bash

rm -f fecho.txt 
make >/dev/null
./hull debug/input.txt
make clean >/dev/null
mv fecho.txt debug

if cmp --silent -- "debug/fecho.txt" "debug/expected_fecho.txt"; then
    echo -e "\nSUCESSO!"
    echo -e "--------"
    echo -e "\ndebug/fecho.txt == debug/expected_fecho.txt"
else
    echo -e "\nFALHOU!"
    echo -e "-------"
    echo -e "\nDiferença:"
    diff debug/fecho.txt debug/expected_fecho.txt
fi
