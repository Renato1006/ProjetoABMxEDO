#!/bin/bash

rm SEIR-ABM.png
date
gcc Agente.c -lm
time ./a.out

gnuplot Quant.gp
