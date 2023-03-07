# export.vars.files.geovector observer

This observer exports simulation variables to geographical vector files
Parameters can be
  format : the GDAL format for output files (mandatory)
  outsubdir : the subdirectory to store output files, relative to the output directory (optional)
  geoserie.<seriename>.sourcefile : the sourcefile for geometry of the serie (mandatory)
  geoserie.<seriename>.unitsclass : the units class of the serie (mandatory)
  geoserie.<seriename>.vars : the list of variables of the serie (mandatory)
The field name for the variable can be explicitely given by using the varname=>fieldname
  geoserie.<seriename>.when : the time mode for output files production (optional). Values for geoserie.<seriename>.when can be init for output at initialization only, final for output at finalization only, continuous for continuous output. Continuous output can be parameterized with a minimal delay in seconds between two outputs (e.g. continuous;7200 for a minimal delay of 2 hours).
