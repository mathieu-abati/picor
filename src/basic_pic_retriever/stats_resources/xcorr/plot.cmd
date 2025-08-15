set xlabel "Images index"
set ylabel "Cross-correlation coeff"
stats 'stats_results/plot.dat' using 3 name 'S' nooutput
stats 'stats_results/plot.dat' every ::S_index_max::S_index_max using 1 name 'SX' nooutput
Xmax = SX_max
arrow_height = S_max * 1.05
set arrow from Xmax,0 to Xmax,arrow_height nohead lc rgb "red" lw 2
set terminal png truecolor size 1200,200
set key off
set output "stats_results/plot.png"
plot [] [-1.0:1.0] 'stats_results/plot.dat' using 1:3 lt rgb "blue", \
     '' using (Xmax):(S_max) with points pt 7 lc rgb "red"
set output "stats_results/plot_zoom.png"
set terminal png truecolor size 1200,600
plot [] [-1.0:1.0] 'stats_results/plot.dat' using 1:3 lt rgb "blue", \
     '' using (Xmax):(S_max) with points pt 7 lc rgb "red"
