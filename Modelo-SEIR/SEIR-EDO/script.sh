#!/bin/bash
rm GrafSEIR.png

date
gcc SEIR.c -lm
time ./a.out>saida.dat

gnuplot grafico.gp
