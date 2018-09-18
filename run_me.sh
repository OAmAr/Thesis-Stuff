#!/bin/bash

rm -rf data
make clean
make
mkdir data
python3 generate.py
python3 combine.py
echo "If no errors occured, combined data should be found in data/combined.csv"
