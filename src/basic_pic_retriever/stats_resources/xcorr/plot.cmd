# Picor - Cross correlation result plotting

# Copyright (C) 2015-2025 Mathieu Abati
#
# This file is part of Picor.
# Picor is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 2 of the License, or (at your option) any later
# version.

# Picor is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.

# You should have received a copy of the GNU General Public License along with
# Picor. If not, see <https://www.gnu.org/licenses/>.

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
