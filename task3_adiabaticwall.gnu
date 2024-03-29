# set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400 
set output 'wall_mesh40.png'
set grid nopolar
set grid layerdefault   lt 0 linecolor 0 linewidth 0.500,  lt 0 linecolor 0 linewidth 0.500
unset parametric
set samples 81, 81
set isosamples 81, 81
set style data lines
set title "Plate Temperature 2D Steady State Conduction" 
set xlabel "through plate length" 
set xlabel  offset character 0, 0, 0 font "" textcolor lt -1 norotate
set xrange [ 0.0000 : 40.0000 ] noreverse nowriteback
set x2range [ * : * ] noreverse writeback
set ylabel "through plate length" 
set ylabel  offset character 3, 0, 0 font "" textcolor lt -1 rotate
set yrange [ 0.0000 : 40.0000 ] noreverse nowriteback
set y2range [ * : * ] noreverse writeback
set zlabel "Temperature [°c]" 
set zlabel  offset character 0, 0, 0 font "" textcolor lt -1 rotate
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set colorbox vertical origin screen 0.9, 0.2 size screen 0.05, 0.6 front  noinvert bdefault
NO_ANIMATION = 1
set grid 
set hidden3d

# Define the filename containing the matrix data
datafile = "mesh40.txt"

# splot the matrix data from the file
splot datafile matrix with lines notitle
