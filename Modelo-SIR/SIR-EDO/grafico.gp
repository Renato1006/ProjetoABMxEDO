set term pngcairo 
set output "SIR-EDO.png"

set xlabel "Dias"
set ylabel "Proporção"
set xrange [0:365]

plot "Dados.dat" u 1:2 w l t "Suscetivel" lc "green" lw 2, "Dados.dat" u 1:3 w l t "Infectado" lc "red" lw 2, "Dados.dat" u 1:4 w l t "Recuperado" lc "blue" lw 2
