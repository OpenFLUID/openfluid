# Multi-column generators : setup and use {#apdx_multicolumn-generators}

[TOC]

# Sources file {#apdx_multicolumn-generators_sources}

The format of the source file is a CSV format which defines the values of the desired variables according 
to a simulation date. The first column defines the simulation dates using the ISO format `YYYYMMDDThhmmss`. 
The first row of the column is labelled \#datetime. All others columns define a variable for a desired spatial unit. 
The first row of these columns are labelled using the format `UnitClass#UnitID:VarName`.
* `UnitClass` defines the unit class,
* `UnitId` defines the unit ID,
* `VarName` defines the variable name.

If you want to define the same variable for all the units in a given class, you can use the format 
`UnitClass#*:VarName` as a column label, where `*` replace all unit classes.

```.csv
#datetime;            UnitClass1#1:var1;      UnitClass2#*:var2;      UnitClass2#5:var2;
19910812T090000;      25;                     5;                      10;
19910812T100000;      30;                     NA;                     15;
19910812T110000;      35;                     NA;                     25;
19910812T120000;      40;                     NA;                     40;
19910812T130000;      45;                     NA;                     60;
19910812T140000;      NA;                     20;                     85;
```

@note Currently, the column separator is the character `;`. 
@note If a variable does not have a value at a specific date, we indicate it using the value *NA*.

This example shows that we inject values for the dates in the first column. The second column indicates 
that we are injecting values for the *var1* variable for spatial unit *1* of the *UnitClass1* class. 
The third column indicates that we inject values for the *var2* variable for all the spatial units in 
the *UnitClass2* class. The last column override the third one for the spatial unit 5 because it is more specific.

# Usage {#apdx_multicolumn-generators_usage}

## Builder

In OpenFLUID-Builder, the multi-column CSV can be set with the same dialog than for other generators. 
It will use a chosen multi-column csv file.
You can either use all the variables defined in the file or specify just a selection of them. 
The generator will produce all selected variables from the csv file.

@image html extract-columns.png
@image latex extract-columns.png "Multi-column generator setup tab" width=11.5cm

In this example (taken from *Firespread example*), we are importing the file *multi_inject.csv* which defines 
2 variables for all spatial units in the unit class *AU* : `AU#*gas.atm.V.windspeed` and 
`AU#*:gas.atm.degree.mainwinddir`. The *Signature* parameter defines all the selected columns from the file.

@image html generator-variables.png "Generator variables"
@image latex generator-variables.png "Generator variables" width=11.5cm

In this image, we can see that the generators produces the selected variables *gas.atm.V.windspeed*
and *gas.atm.degree.mainwinddir*. 

## FluidX format

Here is an example of model.fluidx file containing a multi-column generator:

```.xml
<?xml version="1.0" encoding="UTF-8"?>
<openfluid format="fluidx 4">
    <model>
        <generator variables="AU#*:gas.atm.V.windspeed;AU#*:gas.atm.degree.mainwinddir" method="inject-multicol" enabled="true">
            <param name="datafile" value="multi_inject.csv"/>
        </generator>
        ...
    </model>
</openfluid>
```

The generator `method` is `inject-multicol` and there is also has a mandatory attribute `variables` where the selected 
columns headers are specified.
The multi-column generator block contains a single parameter called `datafile` indicating the path of 
csv file used for injection. 