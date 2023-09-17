#!/bin/bash

rm SIS-ABM.png
rm -r Dados

date
gcc SIS-ABM.c -lm
time ./a.out

gnuplot Quant.gp

mkdir Dados
mv *dat Dados
