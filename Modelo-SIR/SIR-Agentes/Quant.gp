set term pngcairo 
set output "SIR-ABM.png"

set xlabel "Dias"
set ylabel "Proporção"
set xrange [0:365]

plot "Sucetivel.dat" u 1:2 w l t "Suscetível" lc "green" lw 2,"Infectado.dat" u 1:2 w l t "Infectado" lc "red" lw 2, "Recuperado.dat" u 1:2 w l t "Recuperado" lc "blue" lw 2 

