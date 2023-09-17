set term pngcairo 
set output "SEIR-ABM.png"

set xlabel "Dias"
set ylabel "Proporção"
set xrange [0:150]

plot "Sucetivel.dat" u 1:2 w l t "S" lc "green" lw 2, "Exposto.dat" u 1:2 w l t "E" lc "orange" lw 2, "Infectado.dat" u 1:2 w l t "I" lc "red" lw 2, "Recuperado.dat" u 1:2 w l t "R" lc "blue" lw 2

