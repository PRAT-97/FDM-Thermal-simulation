# Set the terminal to PNG format
set terminal png enhanced font "Arial,10"

# Set the output file name
set output "transient_comparision.png"
set key fixed left top vertical Right noreverse enhanced autotitle box lt black linewidth 1.000 dashtype solid
unset parametric
set samples 50, 50
# Set the plot title
set title "temperature at different seconds"
set title font ",12" textcolor lt -1 norotate
# Set axis labels
set xlabel "plate length [m]"
set ylabel "temperature [t °C]"
set xrange [0:0.1]
set yrange [20:100]
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set grid
set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front noinvert default
NO_ANIMATION = 0
# Plot the data from the file
plot "gnu_comp.dat" using 1:2 title "180s" smooth csplines w lp lw 2 lc 10, \
     '' using 1:3 title "720s" smooth csplines w lp lw 2 lc 4, \
     '' using 1:4 title "2880s" smooth csplines w lp lw 2 lc 6, \
     '' using 1:5 title "86400s" smooth csplines w lp lw 2 lc 8