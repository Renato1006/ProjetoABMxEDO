set term pngcairo 
set output "SEIR-EDO.png"
set xlabel "Dias"
set ylabel "Proporção"
set xrange [0:365]


plot "saida.dat" u 1:2 w l t "Suscetivel" lc "green" lw 2, "saida.dat" u 1:3 w l t "Exposto" lc "orange" lw 2, "saida.dat" u 1:4 w l t "Infectado" lc "red" lw 2, "saida.dat" u 1:5 w l t "Recuperado" lc "blue" lw 2
