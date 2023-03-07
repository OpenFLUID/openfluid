# export.vars.files.csv observer

This observer exports variables to CSV files
Parameters can be
  format.<formatname>.date : the date format using the standard C date format
  format.<formatname>.commentchar : the character for comment lines
  format.<formatname>.header : the header type
  format.<formatname>.precision : the precision for real values
  format.<formatname>.float-format : the formatting for decimal numbers
  set.<setname>.unitsclass : the unit class of the set
  set.<setname>.unitsIDs : the unit IDs included in the set. Use * to include all units of the class
  set.<setname>.vars : the variable included in the set, separated by semicolons. Use * to include all variables
  set.<setname>.format : the <formatname> used, must be defined by a format parameter
