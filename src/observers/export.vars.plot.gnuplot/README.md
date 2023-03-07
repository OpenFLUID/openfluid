# export.vars.plot.gnuplot observer

This observer plots simulation variables using GNUplot formalism and tools
It uses series of values (simulated or from existing data files) that can be combined into one or many graphs.
Plotted variables can only be scalar variables (boolean, integer, double)
Parameters for declaring series can be:
  serie.<seriename>.var : the name of the variable to plot (e.g. var.foo)
  serie.<seriename>.unitsclass : the unit class (e.g. TU)
  serie.<seriename>.unitid : the unit id (e.g. 17)
  serie.<seriename>.sourcefile : the file to plot (e.g. measured_data.dat), if this serie is not based on simulation variables
  serie.<seriename>.style : the GNUplot style to use to plot this serie (e.g. linespoint)
  serie.<seriename>.label : the label to use for this serie (e.g. foo) instead of the automatic label
Parameters for declaring graphs can be:
  graph.<graphname>.series : a semicolon seprated list of <seriename> (e.g. serie1;serie2)
  graph.<graphname>.title : the title to use for this graph (e.g. my graph) instead of the automatic title
  graph.<graphname>.key : the mode for key display
  graph.<graphname>.ylabel : the ylabel for the graph
Global parameters are:
  terminal : the terminal mode to replace the default wxt mode (e.g. pdfcairo size 11.7,8.3)
  output : the output file name when terminal exports graphs to file (e.g. mygraph.pdf)
  tryopengnuplot : set to 1 to run GNUplot after simulation ends
  persistent : set to 1 to set GNUplot persistent if run after simulation ends

