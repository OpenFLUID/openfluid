@page use_fluidx Format of input datasets

[TOC]

The FluidX file format is used to define a simulation dataset. An OpenFLUID input dataset includes different informations, defined in one or
many files:
* the **coupled model** definition
* the **spatial domain** definition, including spatial connectivity, attributes and events
* the **datastore** content
* the **monitoring** configuration
* the **run** configuration

All files must be placed into a directory that can be reached by the OpenFLUID program used.
As OpenFLUID-Builder uses the FluidX format natively, the entire input dataset can be created through the OpenFLUID-Builder software.
Out of OpenFLUID-Builder, these FluidX files can be created by hand or using external tools : 
R, Geographic Information Systems (GIS), Text editors, ...


# Overview {#user_fluidx_overview}

The FluidX file format is an XML based format defined for OpenFLUID input datasets.
An OpenFLUID dataset can be provided by a one or many files using
this FluidX format. The file (s) name(s) must use the `.fluidx` file extension.

Whatever the input information is put into one or many files, the following
sections must be defined in the input file(s) set:
* The [model](@ref user_fluidx_sections_model) section defined by the `<model>` tag
* The [spatial domain](@ref user_fluidx_sections_spatialdomain) section defined by the `<domain>` tag
* The [datastore](@ref user_fluidx_sections_datastore) section defined by the `<datastore>` tag
* The [monitoring](@ref user_fluidx_sections_monitoring) section defined by the `<monitoring>` tag
* The [run](@ref user_fluidx_sections_run) section defined by the `<run>` tag


The order of these sections is not significant. All of these sections must be
inclosed into an _openfluid_ section defined by the `<openfluid>`
tag.



Summary view of the XML structure of FluidX files:
```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>

  <model>
    // here is the model definition
  </model>

  <domain>
    // here is the spatial domain definition
    // with connectivity, attributes and events
  </domain>

  <datastore>
    // here is the datastore content
  </datastore>

  <monitoring>
    // here is the monitoring definition
  </monitoring>

  <run>
    // here is the run configuration
  </run>

</openfluid>
```

# Sections {#user_fluidx_sections}


## Model section {#user_fluidx_sections_model}


The coupled model is defined by an ordered set of simulators and/or data generators 
that will be automatically plugged in and run by the OpenFLUID environment.
It can also include a section for global parameters which
apply to all simulators and generators. The global parameters may
be overridden by local parameters of simulators or generators.


The coupled model must be defined in a section delimited by the
`<model>` tag, and must be structured following these rules:
* Inside the `<model>` tag, there must be a set made of at least one 
  `<simulator>` or `<generator>` tags, and an optional `<gparams>` tag.
* Each `<simulator>` tag must bring an `ID` attribute giving
  the identifier of the simulator; the value of the `ID`
  attribute must match the ID of an available and
  pluggable simulator. It also brings an `enabled` tag giving the active state 
  of the simulator (if missing, default value is _1_ for active).
* Each `<simulator>` tag may include zero to many `<param>` tags giving
  parameters to the simulator. Each `<param>` tag must bring a `name` attribute giving
  the name of the parameter and a `value` attribute giving the value of the parameter.
* Each `<generator>` tag must bring a `varname` attribute giving 
  the name of the produced variable, a `unitsclass` attribute giving the 
  unit class of the produced variable, a `method` attribute giving the 
  method used to produce the variable (`fixed` for constant value,
  `random` for random value in a range, `interp` for a time-interpolated
  value from given data series, `inject` for an
  injected value -no time interpolation- from given data series). An optional `<varsize>`
  attribute can be set in order to produce a vector variable instead of a scalar variable.
  It also brings an `enabled` tag giving the active state 
  of the simulator (if missing, default value is _1_ for active).
* Each `<generator>` tag may include zero to many `<param>`
  tags giving parameters to the generator. Each `<param>` tag must bring
  a `name` attribute giving the name of the parameter and a `value` 
  attribute giving the value of the parameter.
* A generator using the `fixed` method must provide a
  parameter named _fixedvalue_ for the value to produce.
* A generator using the `random` method must provide a
  parameter named _min_ and a parameter named _max_ delimiting the
  random range for the value to produce.
* A generator using the `inject` or `interp` method must provide a
  parameter named _sources_ giving the data sources filename and a param
  named _distribution_ giving the distribution filename for the value to
  produce (see also @ref apdx_generators).
* Each `<gparams>` tag may include zero to many `<param>`
  tags giving the global parameters. Each `<param>` tag
  must bring a `name` attribute giving the name of the parameter and a `value` 
  attribute giving the value of the parameter.

```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <model>

    <gparams>
      <param name="gparam1" value="100" />
      <param name="gparam2" value="0.1" />
    </gparams>

    <simulator ID="example.simulatorA" enabled="1" />

    <generator varname="example.generator.fixed" unitsclass="EU1" 
               method="fixed" varsize="11" enabled="1">
      <param name="fixedvalue" value="20" />
    </generator>

    <generator varname="example.generator.random" unitsclass="EU2" method="random" enabled="1">
      <param name="min" value="20.53" />
      <param name="max" value="50" />
    </generator>

    <simulator ID="example.simulatorB" enabled="0">
      <param name="param1" value="strvalue" />
      <param name="param2" value="1.1" />
      <param name="gparam1" value="50" />
    </simulator>

  </model>
</openfluid>
```


@warning There must be only one model definition in the input dataset.

@warning The order of the simulators and data generators in the `<model>` section is important : 
this order will be the call order at initialization time and during simulations in synchronized 
coupled model (not applicable for variable time coupled models)



## Spatial domain section {#user_fluidx_sections_spatialdomain}

### Definition and connectivity {#user_fluidx_sections_spatialdomain_def}


The spatial domain is defined by a set of spatial units that are connected each others.
These spatial units are defined by a numerical identifier (ID) and a class.
They also include informations about the processing order of the unit in the class.
Each unit can be connected to zero or many other units from the same or a different unit class.
The spatial domain definition must be defined in a section delimited
by the `<definition>` tag, which is a sub-section of the `<domain>`
tag, and must be structured following these rules:
* Inside the `<definition>` tag, there must be a set of `<unit>` tags
* Each `<unit>` tag must bring an `ID` attribute giving
  the identifier of the unit, a `class` attribute giving the class of
  the unit, a `pcsorder` attribute giving the process order in the
  class of the unit
* Each `<unit>` tag may include zero or many `<to>` tags giving
  the _to_ connections to other units. Each `<to>` tag must bring an
  `ID` attribute giving the identifier of the connected unit and a
  `class` attribute giving the class of the connected unit
* Each `<unit>` tag may include zero or many `<childof>`
  tags giving the _child-parent_ connections to other units. Each `<childof>` tag must bring an
  `ID` attribute giving the identifier of the parent unit and a
  `class` attribute giving the class of the parent unit

```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <domain>
    <definition>

      <unit class="PU" ID="1" pcsorder="1" />

      <unit class="EU1" ID="3" pcsorder="1">
        <to class="EU1" ID="11" />
        <childof class="PU" ID="1" />
      </unit>
      
      <unit class="EU1" ID="11" pcsorder="3">
        <to class="EU2" ID="2" />
      </unit>
      
      <unit class="EU2" ID="2" pcsorder="1" />

    </definition>
  </domain>
</openfluid>
```


### Attributes {#user_fluidx_sections_spatialdomain_attrs}

The spatial attributes are static data associated to each spatial unit,
usually properties and initial conditions.  
The spatial domain attributes must be defined in a section delimited
by the `<attributes>` tag, which is a sub-section of the `<domain>`
tag, and must be structured following these rules:
* The `<attributes>` tag must bring an `unitsclass`
  attribute giving the unit class to which the attributes must be attached, and a
  `colorder` attribute giving the order of the contained column-formatted
  data
* Inside the `<attributes>` tag, there must be the attributes as 
  row-column text. As a rule, the first column is the ID of the unit in the class
  given through the `unitsclass` attribute of `<attributes>`
  tag, the following columns are values following the column order given
  through the `colorder` attribute of the `<attributes>` tag.
  Values for the data can be double, integer, boolean, string, vector, 
  matrix or map formatted as strings (see part @ref apdx_values).

```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <domain>
  
    <attributes unitsclass="EU1" colorder="indataA">
      3 1.1
      11 7.5
    </attributes>

    <attributes unitsclass="EU2" colorder="indataB1;indataB3">
      2 18 STRVALX
    </attributes>

  </domain>
</openfluid>
```



### Discrete events {#user_fluidx_sections_spatialdomain_events}

The discrete events are events occurring on units, and can be processed by simulators. 
The spatial events must be defined in a section delimited
by the `<calendar>` tag, which is a sub-section of the `<domain>`
tag, and must be structured following these rules:


* Inside the `<calendar>` tag, there must be a set of `<event>` tags 
* Each `<event>` tag must bring an `unitID` and an 
  `unitsclass` attribute giving the unit on which occurs the event, a 
  `date` attribute giving the date and time of the event. The date
  format must be "YYYY-MM-DD hh:mm:ss".
* Each `<event>` tag may include zero to many `<info>` tags.
* Each `<info>` tag give information about the event and must
  bring a `key` attribute giving the name (the "key") of the info, and a
  `value` attribute giving the value for this key.
  
  
```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <domain>
    <calendar>

      <event unitsclass="EU1" unitID="11" date="1999-12-31 23:59:59">
        <info key="when" value="before" />
        <info key="where" value="1" />
        <info key="var1" value="1.13" />
        <info key="var2" value="EADGBE" />
      </event>
      <event unitsclass="EU2" unitID="3" date="2000-02-05 12:37:51">
        <info key="var3" value="152.27" />
        <info key="var4" value="XYZ" />
      </event>
      <event unitsclass="EU1" unitID="11" date="2000-02-25 12:00:00">
        <info key="var1" value="1.15" />
        <info key="var2" value="EADG" />
      </event>

    </calendar>
  </domain>
</openfluid>
```



## Datastore section {#user_fluidx_sections_datastore}

The datastore lists external data which is available during the simulation.
The datastore content must be defined in a section delimited by the
`<datastore>` tag, and must be structured following these rules:

* Inside the `<datastore>` tag, there must be a set of `<dataitem>` tags
* Each `<dataitem>` tag must bring an `ID` attribute giving
  the unique identifier of the dataitem, a `type` attribute giving the type
  of the dataitem (only the geovector and georaster types are currently available), and a `source` 
  attribute giving the source of the dataitem. An optional `unitsclass` 
  attribute is possible for giving the spatial unit class associated to the data. 


```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <datastore>
  
    <dataitem id="TUlayer" type="geovector" source="TestUnits_wgs84.shp" 
              unitsclass="TestUnits" />
    <dataitem id="Ground" type="geovector" source="data/ground.shp" />
    <dataitem id="Ground" type="georaster" source="data/DEM.tiff" />

  </datastore>
</openfluid>
```



## Monitoring section {#user_fluidx_sections_monitoring}

The monitoring is defined by a set of observers that will be automatically 
plugged and executed by the OpenFLUID environment.
Observers are usually used for exporting formatted data from the simulation or performs
continuous control during the simulation.  

@note OpenFLUID provides observers for exporting data to CSV formatted files,
KML formatted files (for use with Google Earth), and DOT formatted files (for graph representations). 


The monitoring must be defined in a section delimited by the
`<monitoring>` tag, and must be structured following these rules:

* Inside the `<monitoring>` tag, there may be a set of
  `<observer>` tags
* Each `<observer>` tag must bring an `ID` attribute giving
  the identifier of the observer; the value of the `ID`
  attribute must match the ID of an available and
  pluggable observer. It also brings an `enabled` tag giving the active state 
  of the simulator (if missing, default value is _1_ for active).
* Each `<observer>` tag may include zero to many `<param>` tags giving
  parameters to the observer. Each `<param>` tag must bring a `name` attribute giving
  the name of the parameter and a `value` attribute giving the value of the parameter.


@note Refer to observers signatures for details about specific parameters for each observer. 


```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <monitoring>
    
    <observer ID="export.vars.files.csv" enabled="1">
      <param name="format.f1.header" value="colnames-as-comment" />
      <param name="format.f1.date" value="%Y-%m-%d %H:%M:%S" />
      <param name="format.f1.precision" value="8" />
            
      <param name="format.f2.header" value="full" />  
  
      <param name="set.s1.unitsclass" value="TestUnits" />
      <param name="set.s1.unitsIDs" value="*" />
      <param name="set.s1.vars" value="*" />
      <param name="set.s1.format" value="f1" />
      
      <param name="set.s2.unitsclass" value="TestUnits" />
      <param name="set.s2.unitsIDs" value="5;3;11" />
      <param name="set.s2.vars" value="tests.double;tests.string" />
      <param name="set.s2.format" value="f2" />
    </observer>
    
    <observer ID="export.vars.files.kml-anim" enabled="1">
      <param name="layers.anim.unitsclass" value="TestUnits" />
      <param name="layers.anim.varname" value="tests.double" />
      <param name="layers.anim.sourcetype" value="file" />
      <param name="layers.anim.sourcefile" value="TestUnits_wgs84.shp" />
      <param name="layers.anim.linewidth" value="4" />
      <param name="layers.anim.colorscale" 
        value="ff00ff00;14;ff00ff76;18;ff00ffdc;22;ff00faff;26;ff0099ff;28;ff001cff"/>

      <param name="layers.static.1.unitsclass" value="OtherUnits" />
      <param name="layers.static.1.sourcetype" value="file" />
      <param name="layers.static.1.sourcefile" value="OtherUnits_wgs84.shp" />
      <param name="layers.static.1.linewidth" value="3" />
      <param name="layers.static.1.color" value="ffffffff" />
    </observer>        
    
  </monitoring>
</openfluid>
```

@warning There must be only one monitoring definition in the input dataset.



## Run configuration section {#user_fluidx_sections_run}

The configuration of the simulation gives the simulation period, the default coupling time step
and the optional coupling constraint.
The run configuration must be defined in a section delimited by the
`<run>` tag, and must be structured following these rules:

* Inside the `<run>` tag, there must be a `<scheduling>` tag
  giving the scheduling informations of the model coupling.
* The `<scheduling>` tag must bring a `deltat` attribute 
  giving the number of second for the default DeltaT time step, 
  and a `constraint` attribute giving an optional constraint applied to the coupling.
  The values for the `constraint` attribute can be `none` for no constraint,
  `dt-checked` to check that coupling is synchronized with the default DeltaT time step,
  `dt-forced` to force coupling at the default DeltaT time step 
  ignoring the scheduling requests from simulators or generators.
* Inside the `<run>` tag, there must be a `<period>` tag
  giving the simulation period.
* The `<period>` tag must bring a `begin` and an
  `end` attributes, giving the dates of the beginning and the end of the
  simulation period. The date format for these attributes must be
  `YYYY-MM-DD hh:mm:ss`
* Inside the `<run>` tag, there may be a `<valuesbuffer>`
  tag for the number of produced values kept in memory. The number of values is given
  through a `size` attribute. If not present, all values are kept in memory.


```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <run>
  
    <scheduling deltat="3600" constraint="none" />
    <period begin="2000-01-01 00:00:00" end="2000-06-30 23:59:00" />
    
    <valuesbuffer size="10" />
    
  </run>
</openfluid>
```


# Runtime variables in parameters {#user_fluidx_params_replacements}

Parameters of simulators and observers can include variables that will be replaced by corresponding values at runtime.
These variables are :

* `${dir.input}` is replaced by the complete path to the input dataset directory
* `${dir.output}` is replaced by the complete path to the output results directory
* `${dir.temp}` is replaced by the complete path to the directory dedicated to temporary files
 

```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
  <model>

    <gparams>
      <param name="globaldata" value="${dir.input}/data/global" />
    </gparams>

    <simulator ID="example.simulatorA" >
      <param name="temppath" value="${dir.temp}/simA" />
    </simulator>
    
  </model>
</openfluid>
```


# Example of an input dataset as a single FluidX file {#use_fluidx_exmpl}

```.xml
<?xml version="1.0" standalone="yes"?>
<openfluid>

  <model>
    <gparams>
      <param name="gparam1" value="100" />
      <param name="gparam2" value="0.1" />
    </gparams>
    <simulator fileID="example.simulatorA" enabled="1"/>
    <generator varname="example.generator.fixed" unitsclass="EU1"
               method="fixed" varsize="11" enabled="1">
      <param name="fixedvalue" value="20" />
    </generator>
    <generator varname="example.generator.random" unitsclass="EU2" 
               method="random" enabled="1">
      <param name="min" value="20.53" />
      <param name="max" value="50" />
    </generator>
    <simulator fileID="example.simulatorB" enabled="0">
      <param name="param1" value="strvalue" />
      <param name="param2" value="1.1" />
      <param name="gparam1" value="50" />
    </simulator>
  </model>


  <domain>

    <definition>
      <unit class="PU" ID="1" pcsorder="1" />
      <unit class="EU1" ID="3" pcsorder="1">
        <to class="EU1" ID="11" />
        <childof class="PU" ID="1" />
      </unit>
      <unit class="EU1" ID="11" pcsorder="3">
        <to class="EU2" ID="2" />
      </unit>
      <unit class="EU2" ID="2" pcsorder="1" />
    </definition>

    <attributes unitsclass="EU1" colorder="indataA">
      3 1.1
      11 7.5
    </attributes>
    
    <attributes unitsclass="EU2" colorder="indataB1;indataB3">
      2 18 STRVALX
    </attributes>
    
    <calendar>
      <event unitsclass="EU1" unitID="11" date="1999-12-31 23:59:59">
        <info key="when" value="before" />
        <info key="where" value="1" />
        <info key="var1" value="1.13" />
        <info key="var2" value="EADGBE" />
      </event>
      <event unitsclass="EU2" unitID="3" date="2000-02-05 12:37:51">
        <info key="var3" value="152.27" />
        <info key="var4" value="XYZ" />
      </event>
      <event unitsclass="EU1" unitID="11" date="2000-02-25 12:00:00">
        <info key="var1" value="1.15" />
        <info key="var2" value="EADG" />
      </event>
    </calendar>
    
  </domain>


  <run>
    <scheduling deltat="3600" constraint="none" />
    <period begin="2000-01-01 00:00:00" end="2000-06-30 23:59:00" />
    <valuesbuffer size="10" />
  </run>


  <monitoring>
    <observer ID="export.vars.files.csv" enabled="1">
      <param name="format.f1.header" value="colnames-as-comment" />
      <param name="format.f1.date" value="%Y-%m-%d %H:%M:%S" />
      <param name="format.f1.precision" value="8" />
      <param name="format.f2.header" value="full" />  
      <param name="set.s1.unitsclass" value="TestUnits" />
      <param name="set.s1.unitsIDs" value="*" />
      <param name="set.s1.vars" value="*" />
      <param name="set.s1.format" value="f1" />
      <param name="set.s2.unitsclass" value="TestUnits" />
      <param name="set.s2.unitsIDs" value="5;3;11" />
      <param name="set.s2.vars" value="tests.double;tests.string" />
      <param name="set.s2.format" value="f2" />
    </observer>   
    <observer ID="export.vars.files.kml-anim" enabled="1">
      <param name="layers.anim.unitsclass" value="TestUnits" />
      <param name="layers.anim.varname" value="tests.double" />
      <param name="layers.anim.sourcetype" value="file" />
      <param name="layers.anim.sourcefile" value="TestUnits_wgs84.shp" />
      <param name="layers.anim.linewidth" value="4" />
      <param name="layers.anim.colorscale" 
        value="ff00ff00;14;ff00ff76;18;ff00ffdc;22;ff00faff;26;ff0099ff;28;ff001cff"/>
      <param name="layers.static.1.unitsclass" value="OtherUnits" />
      <param name="layers.static.1.sourcetype" value="file" />
      <param name="layers.static.1.sourcefile" value="OtherUnits_wgs84.shp" />
      <param name="layers.static.1.linewidth" value="3" />
      <param name="layers.static.1.color" value="ffffffff" />
    </observer>         
  </monitoring>

</openfluid>
```

