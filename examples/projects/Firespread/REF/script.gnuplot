set terminal pdfcairo size 11.7,8.3
set output "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/example-firespread.pdf"
set xtics rotate font ",5"
set ytics font ",7"
set xdata time
set timefmt "%Y-%m-%dT%H:%M:%S"
set datafile separator " "
set datafile commentschars "#"
set format x "%Y-%m-%d\n%H:%M:%S"
set datafile missing "NaN"
set xlabel "Time"
set multiplot layout 2,2 rowsfirst scale 1,1
set title "Wind direction" font ",10"
set key default
set ylabel "Wind direction"
plot "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s1_data.gnuplot" using 1:2 with points title "Main wind direction for AU#1", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s2_data.gnuplot" using 1:2 with points title "Corrected wind direction for AU#1"
set title "Wind speed" font ",10"
set key default
set ylabel "Wind speed"
plot "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s3_data.gnuplot" using 1:2 with lines title "Wind speed for AU#1"
set title "Stock remaining on land units" font ",10"
set key default
set ylabel "Stock"
plot "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s8_data.gnuplot" using 1:2 with lines title "Stock for LU#387", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s9_data.gnuplot" using 1:2 with lines title "Stock for LU#323", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s10_data.gnuplot" using 1:2 with lines title "Stock for LU#1237", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s11_data.gnuplot" using 1:2 with lines title "Stock for LU#1287"
set title "Stock ratio on land units" font ",10"
set key default
set ylabel "Stock ratio"
plot "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s4_data.gnuplot" using 1:2 with lines title "Stock ratio for LU#387", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s5_data.gnuplot" using 1:2 with lines title "Stock ratio for LU#323", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s6_data.gnuplot" using 1:2 with lines title "Stock ratio for LU#1237", "/home/athoni/Documents/OF_Factory/development/openfluid/_build/tests-output/OPENFLUID.OUT.Examples.Firespread/s7_data.gnuplot" using 1:2 with lines title "Stock ratio for LU#1287"
unset multiplot
