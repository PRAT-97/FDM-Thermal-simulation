# Set the terminal to PNG format
set terminal png enhanced font "Arial,10"

# Set the output file name
set output "vector_plot_new.png"
set isosamples 31, 31
# Set axis labels
set xlabel "plate length [mm]"
set ylabel "plate length [mm]"
set xrange [0:1000]
set yrange [0:1000]

# Set the aspect ratio to be equal
set size ratio 0

# Plot vectors using the 'vectors' style
plot "vector(9,9).dat" using 1:2:3:4 with vectors backhead lw 2 lc rgb "blue" lt 2 notitle

