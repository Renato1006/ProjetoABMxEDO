#!/bin/bash

rm SIR-ABM.png
rm -r Dados

date
gcc SIR-ABM.c -lm
time ./a.out

gnuplot Quant.gp

mkdir Dados
mv *dat Dados
