set xlabel "size"
set ylabel "time"
plot "plot.dat" using 1:2 title "insertions" with linespoints
replot "plot.dat" using 1:3 title "searches" with linespoints
replot "plot.dat" using 1:4 title "deletions" with linespoints

set term png
set out "plot.png"
rep
set out
set term x11
pause -1 "Tap \"Enter\" to leave"
