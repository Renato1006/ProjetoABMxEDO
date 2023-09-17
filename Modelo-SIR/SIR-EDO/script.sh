#!/bin/bash
rm Graf-SIR-EDO.png

date
gcc SIR-EDO.c -lm
time ./a.out>Dados.dat

gnuplot grafico.gp
