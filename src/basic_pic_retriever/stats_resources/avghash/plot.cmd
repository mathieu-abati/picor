set xlabel "Images index"
set ylabel "Distance"
stats 'stats_results/plot.dat' using 3 name 'S' nooutput
stats 'stats_results/plot.dat' every ::S_index_min::S_index_min using 1 name 'SX' nooutput
Xmin = SX_min
arrow_height = S_max * 1.05
set arrow from Xmin,0 to Xmin,arrow_height nohead lc rgb "red" lw 2
set terminal png truecolor size 1200,200
set key off
set output "stats_results/plot.png"
plot [] [] 'stats_results/plot.dat' using 1:3 lt rgb "blue", \
     '' using (Xmin):(S_min) with points pt 7 lc rgb "red"
set output "stats_results/plot_zoom.png"
set terminal png truecolor size 1200,600
plot [] [] 'stats_results/plot.dat' using 1:3 lt rgb "blue", \
     '' using (Xmin):(S_min) with points pt 7 lc rgb "red"
