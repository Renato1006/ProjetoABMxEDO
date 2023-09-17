#!/bin/bash
rm Graf-SIS-EDO.png

date
gcc SIS-EDO.c -lm
time ./a.out>Dados.dat

gnuplot grafico.gp
