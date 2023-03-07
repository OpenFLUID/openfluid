# export.vars.files.csv-multicols observer

This observer exports variables to CSV files
Parameters can be
  naemptylinekeep : boolean, set to true to keep lines containing NA for every column
  format.<formatname>.date : the date format using the standard C date format
  format.<formatname>.commentchar : the character for comment lines
  format.<formatname>.header : the header type
  format.<formatname>.missingvalue : the string used when data not available
  format.<formatname>.precision : the precision for real values
  format.<formatname>.float-format : the floating point system used for real values (auto/fixed/scientific)
  set.<setname>.format : the <formatname> used, must be defined by a format parameter.
  set.<setname>.selection : the wanted variables, following the pattern: 
    <UnitsClass1>#<UnitNumber1>:<VarName1>%<digitsPrecision1>;<UnitsClass2>#...
    Float format can be adjusted at column level by adding e (for scientific) or f (for fixed) after thre precision value.
      Example: U#1:myVar%3f;U#1:myOtherVar%4e
