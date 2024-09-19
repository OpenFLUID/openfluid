# Development of the simulator source code {#dev_srccode}

[TOC]

## General information about simulators architecture {#dev_srccode_general}

### Simulator methods sequence and framework interactions {#dev_srccode_general_methods}

As previously mentioned, a simulator is a C++ class which defines 
mandatory methods (see @ref dev_oviewsim_cppclass_methods). 
These methods are called by the OpenFLUID framework at the right time during the simulation,
following the interactions sequence in the figure below. 

@image html openfluid_sequence.png
@image latex openfluid_sequence.pdf "Interactions sequence between the OpenFLUID framework and the simulators" width=11.5cm

Among these methods, the 
@if DocIsLaTeX 
**initializeRun()** 
@else 
@link openfluid::ware::PluggableSimulator::initializeRun initializeRun() @endlink @endif and 
@if DocIsLaTeX 
**runStep()** 
@else @link openfluid::ware::PluggableSimulator::runStep runStep()@endlink @endif 
methods have a special behaviour: these two methods must return the simulation duration after which the simulator will be executed again.  

This duration can returned using the following instructions :
* @if DocIsLaTeX 
**Duration()** 
@else @link openfluid::ware::PluggableSimulator::Duration() Duration() @endlink @endif
to be executed in a number of seconds given as a parameter
* @if DocIsLaTeX 
**DefaultDeltaT()** 
@else @link openfluid::ware::PluggableSimulator::DefaultDeltaT() DefaultDeltaT() @endlink @endif
to be executed in a number of seconds given as default DeltaT in the input dataset
* @if DocIsLaTeX 
**MultipliedDefaultDeltaT()**
@else @link openfluid::ware::PluggableSimulator::MultipliedDefaultDeltaT() MultipliedDefaultDeltaT() @endlink @endif
to be executed in a number of seconds given as default DeltaT in the input dataset, multiplied by a the value given as parameter
* @if DocIsLaTeX 
**AtTheEnd()** 
@else @link openfluid::ware::PluggableSimulator::AtTheEnd() AtTheEnd() @endlink @endif
to be executed only once at the end of the simulation
* @if DocIsLaTeX 
**Never()** 
@else @link openfluid::ware::PluggableSimulator::Never() Never() @endlink @endif 
to never be executed again


_Example for a fixed time step simulator, with a time step equal to the default DeltaT value given in the input dataset:_
@snippet wares/SchedFixedSim.cpp scheduling_fixed

_Example for a variable time step simulator, based on the internal computation of the simulator:_
@snippet wares/SchedVariableSim.cpp scheduling_variable



For fully synchronized coupled simulators, all simulators must return the same duration for the next execution,
usually **DefaultDeltaT()** .

### OpenFLUID data types {#dev_srccode_general_datatypes}

Simulation data exchanged through the OpenFLUID framework should be typed with an OpenFLUID defined type.  
The available simple types are:

* **openfluid::core::BooleanValue** for storing boolean values
* **openfluid::core::IntegerValue** for storing long integer values
* **openfluid::core::DoubleValue** for storing double precision values
* **openfluid::core::StringValue** for storing string values


The available compound types are:

* **openfluid::core::VectorValue** for storing vector data
* **openfluid::core::MatrixValue** for storing matrix data
* **openfluid::core::MapValue** for storing associative key-value data
* **openfluid::core::TreeValue** for storing hierarchical key-value data


A specific type is available for storing non-existing or null values:

* **openfluid::core::NullValue**



Simulation data are stored using these types : 

* Simulation variables : stored as their native type
* Spatial attributes : stored as their native type
* Simulator parameters : stored as openfluid::core::StringValue, 
and can be converted to any other type
* Informations associated to events : stored as openfluid::core::StringValue, 
and can be converted to any other type


Each data type can be converted to and from openfluid::core::StringValue (as far as the string format is correct). 
String representations of values are described in the @ref apdx_values part. 


Simulation variables can be typed or untyped. 
This is set at the declaration of these variables (see @ref dev_signature_data_vars).  
In case of typed variables, each value of the variable must be of the type of the variable.
In case of untyped variables, values for the variable can be of any type. 



## Handling the spatial domain {#dev_srccode_space}

### Parsing the spatial graph {#dev_srccode_space_parse}

The spatial graph represents the spatial domain where coupled simulators will operate.
Parsing this graph in different ways is a common task in simulators.
This graph can be browsed using predefined instructions.  

#### Sequential parsing {#dev_srccode_space_parse_seq}

Spatial units can be parsed following the process order by using the following OpenFLUID instructions:

* 
@if DocIsLaTeX 
**OPENFLUID_UNITS_ORDERED_LOOP**
@else
#OPENFLUID_UNITS_ORDERED_LOOP
@endif
for parsing spatial units of a given units class
* 
@if DocIsLaTeX 
**OPENFLUID_ALLUNITS_ORDERED_LOOP**
@else
#OPENFLUID_ALLUNITS_ORDERED_LOOP
@endif
for parsing of all units in the spatial domain 


To parse a specific list of of spatial units, you can use the instruction:

* @if DocIsLaTeX 
**OPENFLUID_UNITSLIST_LOOP**
@else
#OPENFLUID_UNITSLIST_LOOP
@endif
 


The source code below shows examples of spatial graph parsing. 
The first part of the source code shows how to browse all units of the SU units class, 
and how to browse the "From" units for each SU unit.
The second part of the source code shows how to browse all units of the spatial domain. 
@snippet wares/SpatialParsingSeqSim.cpp spatial_parsing_seq


#### Parallel processing using multithreading {#dev_srccode_space_parse_par}

A processing defined as a method of a simulator class can be applied in parallel to the spatial graph, 
following the process order, using the following methods:

* @if DocIsLaTeX **APPLY_UNITS_ORDERED_LOOP_THREADED** @else #APPLY_UNITS_ORDERED_LOOP_THREADED @endif
 for applying a process to a given units class. Extra arguments can be passed (see example below). 
* @if DocIsLaTeX **APPLY_ALLUNITS_ORDERED_LOOP_THREADED** @else #APPLY_ALLUNITS_ORDERED_LOOP_THREADED @endif
 for applying a process to a all units of the spatial domain. Extra arguments can also be passed (see example below).  


The first argument of the method passed to the instruction must be
a pointer to an @if DocIsLaTeX **openfluid::core::SpatialUnit** @endif
 as it represents the currently processed spatial unit. 

<br/>
In order to enable the parallel processing in the spatial graph, 
the following inclusion must be added at the top of the simulator source code:
@snippet wares/SpatialParsingParallelSim.cpp spatial_parsing_parallel_include

The code below shows how to apply a method in parallel over the spatial graph:
@snippet wares/SpatialParsingParallelSim.cpp spatial_parsing_parallel


@note 
* If a spatial loop is used inside another spatial loop, it is recommended to use multithreading in only one loop.
@note 
* In case of concurrent data access, it is strongly recommended to use mutex locks for thread-safe data handling.
@note 
* Concurrent parsing using multithreading should improve computing performance, reducing simulations durations.
But in case of very short computing durations, the cost of multithreading management
may counterbalance the performance improvements of concurrent computing. 



### Querying the spatial graph {#dev_srccode_space_query}

The spatial domain graph can be queried during simulations in order to get
informations about spatial units and connections.

The following methods are available:

@if DocIsLaTeX
* **OPENFLUID_IsUnitExist**
* **OPENFLUID_IsUnitsClassExist**
* **OPENFLUID_GetUnit**
* **OPENFLUID_GetUnits**
* **OPENFLUID_GetUnitsCount**
* **OPENFLUID_IsUnitConnectedTo**
* **OPENFLUID_IsUnitConnectedFrom**
* **OPENFLUID_IsUnitChildOf**
* **OPENFLUID_IsUnitParentOf**
@else
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsUnitExist OPENFLUID_IsUnitExist @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsUnitsClassExist OPENFLUID_IsUnitsClassExist @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetUnit OPENFLUID_GetUnit @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetUnits OPENFLUID_GetUnits @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetUnitsCount OPENFLUID_GetUnitsCount @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsUnitConnectedTo OPENFLUID_IsUnitConnectedTo @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsUnitConnectedFrom OPENFLUID_IsUnitConnectedFrom @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsUnitChildOf OPENFLUID_IsUnitChildOf @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsUnitParentOf OPENFLUID_IsUnitParentOf @endlink
@endif


### Modifying the spatial graph {#dev_srccode_space_mod}

The spatial graph is usually statically defined through the input dataset.
It can also be defined and modified dynamically during simulations, using primitives to 
create and delete spatial units, and also to add and remove connections between these
spatial units.  
Although the creation, deletion and modification of connections are allowed at any stage of the simulation,
the creation, deletion and modification of spatial units are currently allowed only during the data preparation stage
(i.e. in the prepareData() method of the simulator).  
For consistent use of simulators which modify the spatial domain graph, 
please fill the signature with the correct directives.
See the @ref dev_signature_data_spatial part of the signature declaration.

#### Creating and deleting spatial units {#dev_srccode_space_mod_units}

In order to create and delete units, you can use the following methods:
@if DocIsLaTeX

* **OPENFLUID_AddUnit**
* **OPENFLUID_DeleteUnit**

@else

* @link openfluid::ware::PluggableSimulator::OPENFLUID_AddUnit OPENFLUID_AddUnit @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_DeleteUnit OPENFLUID_DeleteUnit @endlink

@endif


#### Adding and removing spatial connections {#dev_srccode_space_mod_cnx}

Connections between spatial units can be of two types:

* "From-To" connections, linking spatial units topologically. These connections are usually used in "fluxes-like" processes.
* "Parent-Child" connections, linking units hierarchically.


In order to add and remove connections, you can use the following methods, whenever during simulations:

@if DocIsLaTeX
* **OPENFLUID_AddFromToConnection**
* **OPENFLUID_AddChildParentConnection**
* **OPENFLUID_RemoveFromToConnection**
* **OPENFLUID_RemoveChildParentConnection**
@else
* @link openfluid::ware::PluggableSimulator::OPENFLUID_AddFromToConnection OPENFLUID_AddFromToConnection @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_AddChildParentConnection OPENFLUID_AddChildParentConnection @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_RemoveFromToConnection OPENFLUID_RemoveFromToConnection @endlink
* @link openfluid::ware::PluggableSimulator::OPENFLUID_RemoveChildParentConnection OPENFLUID_RemoveChildParentConnection @endlink
@endif


_Example:_
@snippet wares/SpatialConnectSim.cpp spatial_connect


#### Generating spatial domain graphs automatically {#dev_srccode_space_mod_gen}

A spatial domain graph can be automatically built or extended using a provided method to create a matrix-like graph:
@if DocIsLaTeX

* **OPENFLUID_BuildUnitsMatrix**

@else

* @link openfluid::ware::PluggableSimulator::OPENFLUID_BuildUnitsMatrix OPENFLUID_BuildUnitsMatrix @endlink

@endif


## Informations about simulation time {#dev_srccode_time}

Simulators can access to informations about simulation time.
There are constant time informations, such as simulation duration or begin and end date,
and evolutive informations such as current time index.

Constant time informations can be accessed from any part of the simulator
(except from the constructor), using the following methods:
@if DocIsLaTeX

* **OPENFLUID_GetBeginDate** returns the beginning date of the simulation
* **OPENFLUID_GetEndDate** returns the end date of the simulation
* **OPENFLUID_GetSimulationDuration** returns the duration of the simulation (in seconds)
* **OPENFLUID_GetDefaultDeltaT** returns the default time step of the simulation (in seconds),
given in the input dataset

@else

* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetBeginDate OPENFLUID_GetBeginDate @endlink 
returns the beginning date of the simulation
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetEndDate OPENFLUID_GetEndDate @endlink 
returns the end date of the simulation
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetSimulationDuration OPENFLUID_GetSimulationDuration @endlink
returns the duration of the simulation (in seconds)
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetDefaultDeltaT OPENFLUID_GetDefaultDeltaT @endlink
returns the default time step of the simulation (in seconds),
given in the input dataset

@endif

Evolutive time informations can be accessed only from specific parts 
of the simulator, using the following methods:

@if DocIsLaTeX
* **OPENFLUID_GetCurrentTimeIndex** 
returns the current time index (in seconds) of the simulation,
and is available from the initializeRun(), runStep() and finalizeRun() methods of the simulator
* **OPENFLUID_GetCurrentDate** returns the current date of the simulation,
and is available from the initializeRun(), runStep() and finalizeRun() methods of the simulator
* **OPENFLUID_GetPreviousRunTimeIndex** returns the time index
corresponding to the previous execution of the simulator,
and is available from the runStep() and finalizeRun() methods of the simulator  
@else
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetCurrentTimeIndex OPENFLUID_GetCurrentTimeIndex @endlink
returns the current time index (in seconds) of the simulation,
and is available from the initializeRun(), runStep() and finalizeRun() methods of the simulator
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetCurrentDate OPENFLUID_GetCurrentDate @endlink 
returns the current date of the simulation,
and is available from the initializeRun(), runStep() and finalizeRun() methods of the simulator
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetPreviousRunTimeIndex OPENFLUID_GetPreviousRunTimeIndex @endlink returns the time index
corresponding to the previous execution of the simulator,
and is available from the runStep() and finalizeRun() methods of the simulator  
@endif


_Example of code:_
@snippet wares/ParamsEnvTimeSim.cpp time


## Simulator parameters {#dev_srccode_simparams}

Simulators parameters can be accessed in the source code from the
@if DocIsLaTeX
initParams()
@else
@link openfluid::ware::PluggableSimulator::initParams initParams @endlink
@endif 
method of the simulator.
Values of simulators parameters can be retreived using:

* @if DocIsLaTeX **OPENFLUID_GetWareParameter.** 
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_GetWareParameter OPENFLUID_GetWareParameter@endlink.
@endif

The requested parameter name must be the same as the one declared in the signature 
and used in the model.fluidx file (see @ref user_fluidx_sections_model of the signature declaration).  
<br/>

_Example of initParams method:_
@snippet wares/ParamsEnvTimeSim.cpp params


To be used in other part of the simulator, 
the C++ variable storing a simulator parameter should be declared as class member.
The types of parameters can be string, double, integer, boolean, vector of string, vector of double
(see API documentation of 
@if DocIsLaTeX
OPENFLUID_GetWareParameter
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_GetWareParameter OPENFLUID_GetWareParameter @endlink
@endif
method to get more informations about other available types
@if DocIsLaTeX , available on the [OpenFLUID community site](https://community.openfluid-project.org/) @endif ).

## Spatial attributes {#dev_srccode_attrs}

In order to access or update values of spatial attributes, or to test if a spatial attribute is present,
you can use the following methods:

@if DocIsLaTeX
* **OPENFLUID_GetAttribute** to get the value of an attribute
* **OPENFLUID_SetAttribute** to set the value of an attribute
* **OPENFLUID_IsAttributeExist** to test if an attribute exists
@else
* @link openfluid::ware::PluggableSimulator::OPENFLUID_GetAttribute OPENFLUID_GetAttribute @endlink
to get the value of an attribute
* @link openfluid::ware::PluggableSimulator::OPENFLUID_SetAttribute OPENFLUID_SetAttribute @endlink
to set the value of an attribute
* @link openfluid::ware::PluggableSimulator::OPENFLUID_IsAttributeExist OPENFLUID_IsAttributeExist @endlink
to to test if an attribute exists
@endif


The methods to test if an attribute exists or to access to an attribute value 
are usable from any simulators part except from the initParams() part.
The methods to update an attribute value are only usable 
from the prepareData() and checkConsistency() parts of the simulator.  
The names of the attributes must match the names in the input dataset (see @ref user_fluidx_sections_spatialdomain), 
or the name of an attribute created by a simulator.

_Example of use:_
@snippet wares/AttributesSim.cpp attributes


## Simulation variables {#dev_srccode_vars}

The values for the simulation variables are attached to the spatial units.

The available methods to access to simulation variables are:

* @if DocIsLaTeX **OPENFLUID_GetVariable**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_GetVariable OPENFLUID_GetVariable @endlink
@endif
to get the value of a variable at the current time index or at a given time index
* @if DocIsLaTeX **OPENFLUID_GetVariables**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_GetVariables OPENFLUID_GetVariables @endlink
@endif
to get values of a variable between two times indexes
* @if DocIsLaTeX **OPENFLUID_GetLatestVariable**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_GetLatestVariable OPENFLUID_GetLatestVariable @endlink
@endif
to get the latest available value for the variable
* @if DocIsLaTeX **OPENFLUID_GetLatestVariables**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_GetLatestVariables OPENFLUID_GetLatestVariables @endlink
@endif
to get the latest values of a variable since a given time index


The available methods to add or update a value of a simulation variable are:

* @if DocIsLaTeX **OPENFLUID_AppendVariable**
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_AppendVariable OPENFLUID_AppendVariable @endlink
@endif
to add a value to a variable for the current time index
* @if DocIsLaTeX **OPENFLUID_SetVariable**
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_SetVariable OPENFLUID_SetVariable @endlink
@endif
to update the value of a variable for the current time index



The available methods to test if a simulation variable exists are:

* @if DocIsLaTeX **OPENFLUID_IsVariableExist**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_IsVariableExist OPENFLUID_IsVariableExist @endlink
@endif
to test if a variable exists or if a value for this variable exists at the given time index 
* @if DocIsLaTeX **OPENFLUID_IsTypedVariableExist**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_IsTypedVariableExist OPENFLUID_IsTypedVariableExist @endlink
@endif
to test if a variable exists or if a value for this variable exists at the given time index,
and its type matches the given type


These methods can be accessed only from the initializeRun(), runStep()
and finalizeRun() parts of the simulator.


_Example:_
@snippet wares/VariablesSim.cpp variables


## Events {#dev_srccode_events}

A discrete event is defined by the @if DocIsLaTeX **openfluid::core::Event** 
@else openfluid::core::Event @endif
 class. It is made of a date and a set of key-value informations
that can be accessed by methods proposed by the openfluid::core::Event class.   
A collection of discrete events can be contained in an @if DocIsLaTeX **openfluid::core::EventsCollection** 
@else openfluid::core::EventsCollection @endif class.  

<br/>

A collection of events occuring during a period on a given spatial unit can be acessed using 

* @if DocIsLaTeX **OPENFLUID_GetEvents**
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_GetEvents OPENFLUID_GetEvents @endlink
@endif

This method returns an @if DocIsLaTeX **openfluid::core::EventsCollection** @endif
 that can be processed.  
 <br/>

The returned event collection can be parsed using the specific loop instruction: 

* @if DocIsLaTeX **OPENFLUID_EVENT_COLLECTION_LOOP**
 @else #OPENFLUID_EVENT_COLLECTION_LOOP
 @endif

At each loop iteration, the next event can be processed.  
<br/>

An event can be added on a specific spatial unit at a given date using:

* @if DocIsLaTeX **OPENFLUID_AppendEvent**
@else @link openfluid::ware::PluggableSimulator::OPENFLUID_AppendEvent OPENFLUID_AppendEvent @endlink
@endif
<br/>

_Example of process of events occurring on the current time step:_
@snippet wares/EventsSim.cpp events



## Internal state data {#dev_srccode_state}

In order to preserve the internal state of the simulator between calls
(from the run step to the next one for example), internal variables can be stored as class members.
The class members are persistant during the whole life of the simulator.  
To store distributed values, data structures are available to associate a spatial unit ID to a storedvalue.
These data structures exist for different types of data:   

* @if DocIsLaTeX 
**openfluid::core::IDFloatMap**
@else 
openfluid::core::IDFloatMap
@endif 
* @if DocIsLaTeX
**openfluid::core::IDDoubleMap**
@else 
openfluid::core::IDDoubleMap
@endif 
* @if DocIsLaTeX 
**openfluid::core::IDIntMap** 
@else 
openfluid::core::IDIntMap
@endif
* @if DocIsLaTeX
**openfluid::core::IDBoolMap**
@else
openfluid::core::IDBoolMap
@endif
* @if DocIsLaTeX
**openfluid::core::IDDoubleValueMap**
@else
openfluid::core::IDDoubleValueMap
@endif
* @if DocIsLaTeX
**openfluid::core::IDVectorValueMap**
@else
openfluid::core::IDVectorValueMap
@endif
* @if DocIsLaTeX
**openfluid::core::IDVectorValuePtrMap**
@else
openfluid::core::IDVectorValuePtrMap
@endif
* @if DocIsLaTeX
**openfluid::core::IDSerieOfDoubleValueMap**
@else
openfluid::core::IDSerieOfDoubleValueMap
@endif
* @if DocIsLaTeX
**openfluid::core::IDSerieOfDoubleValuePtrMap**
@else
openfluid::core::IDSerieOfDoubleValuePtrMap
@endif
<br/>


_Example of declaration of ID-map structures in private members of the simulator class:_
@snippet wares/InternalStateSim.cpp internal_state_decl


_Example of usage of the ID-map structures:_
@snippet wares/InternalStateSim.cpp internal_state_impl


## Runtime environment {#dev_srccode_runenv}

The runtime environment of the simulator are informations about the context during execution of
the simulation: input and output directories, temporary directory,...  
They are accessible from simulators using:

* 
@if DocIsLaTeX
**OPENFLUID_GetRunEnvironment**
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_GetRunEnvironment OPENFLUID_GetRunEnvironment @endlink method.
@endif


_Example:_
@snippet wares/ParamsEnvTimeSim.cpp env


The keys for requesting runtime environment information are:

* dir.input [string] : the current input directory
* dir.output [string] : the current output directory
* dir.temp [string] : the directory for temporary files
* mode.cleanoutput [boolean] : cleaning output dir before data saving is enabled/disabled



## Informations, warnings and errors {#dev_srccode_msgs}

### Informations and warnings from simulators {#dev_srccode_msgs_log}

Simulators can emit informations and warnings to both console and files using various methods

* 
@if DocIsLaTeX
**OPENFLUID_DisplayInfo**
@else
#OPENFLUID_DisplayInfo
@endif
to display informative messages to console only
* 
@if DocIsLaTeX
**OPENFLUID_LogInfo**
@else
#OPENFLUID_LogInfo
@endif
to log informative messages to file only
* 
@if DocIsLaTeX
**OPENFLUID_LogAndDisplayInfo**
@else
#OPENFLUID_LogAndDisplayInfo
@endif
to log and display informative messages simultaneously
* 
@if DocIsLaTeX
**OPENFLUID_DisplayWarning**
@else
#OPENFLUID_DisplayWarning
@endif
to display warning messages to console only
* 
@if DocIsLaTeX
**OPENFLUID_LogWarning**
@else
#OPENFLUID_LogWarning
@endif
to log warning messages to file only
* 
@if DocIsLaTeX
**OPENFLUID_LogAndDisplayWarning**
@else
#OPENFLUID_LogAndDisplayWarning
@endif
to log and display warning messages simultaneously

Using these methods is the recommended way to log and display messages. Please avoid using std::cout
or similar C++ facilities in production or released simulators.

_Example:_
@snippet wares/InfosDebugSim.cpp infowarning

The messages logged to file are put in the `openfluid-messages.log` file
placed in the simulation output directory. 
This file can be browsed using the OpenFLUID-Builder application (_Outputs_ tab) or any text editor. 


### Errors from simulators {#dev_srccode_msgs_warnerr}

Simulators can raise errors to notify the OpenFLUID framework that something wrong or critical had happened. 
An error stops the simulation the next time the OpenFLUID framework has the control.    

Errors can be raised using
@if DocIsLaTeX
**OPENFLUID_RaiseError**
@else
@link openfluid::ware::PluggableSimulator::OPENFLUID_RaiseError OPENFLUID_RaiseError@endlink
@endif

_Example:_
@snippet wares/InfosDebugSim.cpp error 


## Debugging {#dev_srccode_debug}

Debugging instructions allow developpers to trace various information during simulations.   
They are enabled only when debug is enabled at simulators builds. They are ignored for other build types.   

In order to enable debug build mode, the option `-DCMAKE_BUILD_TYPE=Debug`
must be added to the cmake command (e.g. `cmake <srcpath> -DCMAKE_BUILD_TYPE=Debug`).  
<br/>

_Example of build configuration:_
```
cmake .. -DCMAKE_BUILD_TYPE=Debug
```
This debug build mode is disabled using the release build mode, with the option `-DCMAKE_BUILD_TYPE=Release`.  
<br/>

Simulators can emit debug information to both console and files using various methods

* 
@if DocIsLaTeX
**OPENFLUID_DisplayDebug**
@else
#OPENFLUID_DisplayDebug
@endif
to display debug messages to console only
* 
@if DocIsLaTeX
**OPENFLUID_LogDebug**
@else
#OPENFLUID_LogDebug
@endif
to log debug messages to file only
* 
@if DocIsLaTeX
**OPENFLUID_LogAndDisplayDebug**
@else
#OPENFLUID_LogAndDisplayDebug
@endif
to log and display debug messages simultaneously


_Example:_
@snippet wares/InfosDebugSim.cpp debug


Additional instructions are available for debugging, see file debug.hpp:



## Integrating Fortran code {#dev_srccode_fortran}

The C++/Fortran interface is defined in the openfluid/tools/FortranCPP.hpp file. 
It allows to integrate Fortran 77/90 code into simulators.<br/>
In order to execute Fortran code from a simulator,
the Fortran source code have to be wrapped into subroutines 
that are called from the C++ code of the simulator.  
To help developers of simulators to achieve this wrapping operation, 
the FortranCPP.hpp file defines dedicated instructions. 
You are invited to read the FortranCPP.hpp file to get more information about these instructions.  
<br/>


In order to enable the call of Fortran code, the following inclusion must be added at the top of the simulator source code: 
@snippet wares/FortranSim.cpp fortran_include



_Example of Fortran source code (e.g. FortranSubr.f90):_
@include wares/FortranSubr.f90


_Example of declaration block int the .cpp file (e.g. FortranSim.cpp):_
@snippet wares/FortranSim.cpp fortran_decl



_Example of call of the fortran subroutine from the initializeRun method (e.g. FortranSim.cpp):_
@snippet wares/FortranSim.cpp fortran_use



The compilation and linking of Fortran source code is automatically done 
when adding fortran source files to the SIM_FORTRAN variable in the CMake.in.config file 
(See @ref dev_createsim_exmpl_config).


## Embedding R code {#dev_srccode_R}

@note The embedding of R code in simulators is currently an experimental feature.


Thanks to the [RInside package](http://dirk.eddelbuettel.com/code/rinside.html), 
It is possible to embed R code in simulators written in C++. 
It also relies on the [Rcpp package](http://dirk.eddelbuettel.com/code/rcpp.html) 
for handling data from and to the [R environment](https://www.r-project.org/).

In order to embed R code using RInside, the following inclusion must be added at the top of the simulator source code:
@snippet wares/RSim.cpp R_include

A unique RInside variable is used to run R code, it should be declared as a member of the simulator class (named `m_R` in this example).
@snippet wares/RSim.cpp R_decl

The R environment can be acessed through the RInside variable
and R commands can be run using its `parseEvalQ()` method.
@snippet wares/RSim.cpp R_use

In this short example, simple variables and commands are used. 
It is possible to perform complex operations involving external R packages, 
or call R scripts by executing a `source()` R command through RInside.
See the [RInside package](http://dirk.eddelbuettel.com/code/rinside.html) documentation to get more details and examples.


To help configuring the simulator which is using the RInside package, a CMake module is provided with OpenFLUID 
to setup the configuration variables when building the simulator.
It should be used in the `CMake.in.cmake` file of the simulator.
@include wares/CMake.in.cmake.snippet

An adjustment of the CMake module path prefix may be required to find the provided R module for CMake
```
SET(CMAKE_MODULE_PATH "/prefix/lib/openfluidhelpers/cmake;${CMAKE_MODULE_PATH}")
```
where `prefix` depends on the OpenFLUID installation path and operating system (e.g. `/usr`,`/usr/local`, `C:\OpenFLUID-x.x.x`)


@warning Due to limitations of the RInside package, embedding R code in simulators does not support threading. 

@warning Simulations including simulators with RInside must be run using Command line mode in OpenFLUID-Builder or using the `openfluid` command line.

@warning Due to limitations of the RInside package, only one simulator embedding R code 
can be used in a coupled model.


## Miscellaneous helpers {#dev_srccode_misctools}

The OpenFLUID API provides miscellaneous functions and classes to help simulators developpers 
in their setup of data processing or numerical computation. 
They are available in various namespaces:

* openfluid::tools
* openfluid::scientific
* openfluid::utils


In order to use these helpers, the corresponding headers 
files must be included in the simulator source code.

As they are not detailed here in this manual, more informations 
about these helpers are available in the provided header files (.hpp), 
located in the corresponding include directories.

