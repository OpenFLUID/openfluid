# Declaration of the simulator signature {#dev_signature}

[TOC]

# Since OpenFLUID 2.2
The signature is defined in a dedicated file called "openfluid_ware.json". It is located at the root of the simulator directory.
This file contains the same information than before but in a tree structure:
```json
{
  "id": "traffic.surf.car-transfer",
  "name": "Road Unit (RU) function transfert and stockage for cars",
  "description": "",
  "version": "1.0",
  "status": "experimental",
  "authors": [
    {"name": "MR", "email": "m.r@inrae.fr"}
  ],
  "tags": ["examples"],
  "simulator": {
    ...
  }
}
```

Note: From OpenFLUID 2.1 to 2.2: In most cases, the migration of a simulator can be done automatically through DevStudio "Try to migrate" button or command line "openfluid migrate-ware". This migration will automatically convert the signature from the C++ file into a corresponding json file. For more information, see the page dedicated to ware migration(TOIMPL)

## Identification {#dev_signature_id}

The identification part of the signature must contain at least the ID of the simulator. 
This ID will be used by the framework to load simulators. It is declared in 
the signature with key **id**.
Other optional informations can be included for better description of the simulator:

* the simulator name as string, with key **name**. Allowing to assign a long name to the simulator
* the simulator description as string with key **description**. Allowing to provide a detailed description of what the simulator actually does 
* the name(s) of the author(s) and corresponding email address(es), as list with key **authors**
* the software version of the simulator, as string with key **version**
* the software status of the simulator, as string with key **status**. The value can be
_experimental_, _beta_ or _stable_


See the @ref dev_signature_exmpl part for detailed example. 


## Informations about scientific application {#dev_signature_sci}

The informations about scientific applications are only indicative.
It has no effects on simulator consistency or computational code.
They are stored as string list with key **tags**. These informations can be:

* the domain in which the simulator can be applied, eg `"domain:urban"`
* the processes simulated by the simulator, eg `"process:traffic"`
* the numerical methods used by the simulator, eg `"method:mseytoux"`

or other relevant tags. 

## Data 

The data used by the simulators can be:

* Parameters that are attached to the simulator
* Spatial attributes that are attached to spatial units, giving properties about the spatial units
* Simulation variables that are attached to spatial units, representing the resulting dynamics of modeled processes
over the spatial units
* Discrete events that are attached to spatial units, 
representing the events occurring at a given date and time on a given spatial unit
* Specific file(s) loaded by the simulator 

These data can be accessed, appended and/or modified by the simulator.  
<br/>
The spatial graph representing the landscape can also be accessed or modified 
by simulators during simulations.   
<br/>
The declarations of spatial data access include constraint levels:

* _required_, this means that the data must be available or already produced
* _used_, this means that the data are used only if they are available or already produced

Simulator parameters, variables and attributes are declared in the "simulator" part of the json file, in a "data" dictionary:
```json
    "data": {
      "parameters": {
        "used": [
          {
            "name": "MultiCapacity",
            "description": "Multiplying factor for capacity",
            "siunit": "-", "type": ""
          }
        ]
      },
      "attributes": {
        ...
      },
      "variables": {
        "produced": [
          {
            "name": "examples.RU.S.stock",
            "unitsclass": "RU",
            "description": "number of cars stocked on RU",
            "siunit": "",
            "type": ""
          }
        ],
        "used": [
          {
            "name": "examples.TLU.S.state",
            "unitsclass": "TLU",
            ...
          }
        ]
      }
      ...
    }
```


### Simulator parameters {#dev_signature_data_simparams}

Simulator parameters are values provided to each simulator, 
and are declared using the @if DocIsLaTeX **DECLARE_REQUIRED_PARAMETER** or **DECLARE_USED_PARAMETER**
@else DECLARE_REQUIRED_PARAMETER or DECLARE_USED_PARAMETER @endif
 instructions.
These instructions takes 3 arguments

* the name of the parameter
* the description of the parameter (may be empty)
* the SI unit of the parameter (may be empty)


_Example of a declaration of a required simulator parameter:_
```json
"used": [
          {
            "name": "MultiCapacity",
            "description": "Multiplying factor for capacity",
            "siunit": "-",
            "type": ""
          }
        ]
```


### Spatial attributes {#dev_signature_data_attrs}

Spatial attributes are constant properties attached to each spatial units, 
and are declared in three blocks: "required", "used" or "produced".

These instructions take 4 arguments:

* the name of the attribute
* the units class
* the description of the attribute (may be empty)
* the SI unit of the attribute (may be empty)


### Simulation variables {#dev_signature_data_vars}

Simulation variables are attached to spatial units. They are produced, accessed and modified
by simulators during simulations.  
Accessed variables are declared in 4 blocks: "produced", "required", "used" and "updated".

These instructions take 4 arguments:

* the name of the variable
* the concerned unit class
* the description of the variable (may be empty)
* the SI unit of the variable (may be empty)


These variables can be typed or untyped. 
When they are declared in the signature, the variable names suffixed by the [] symbol with a type name enclosed
are typed, i.e. each value for the variable must match the type of the variable.
If no type is mentioned, values for the variable can be of any type.  
In case of typed variables, the type of a required or used variable by a simulator
must match the type of the variable set when it is produced.   

The type name for a declaration of a variable can be:

* `boolean` for boolean values
* `integer` for long integer values
* `double` for double precision values
* `string` for string values
* `vector` for vector data
* `matrix` for matrix data
* `map` for associative key-value data
* `tree` for hierarchical key-value data


These scenarios of variable exchanges between two A and B simulators run successfully:

* simulator A produces an _untyped_ variable _var1_,
simulator B requires/uses/updates an _untyped_ variable _var1_
* simulator A produces a _typed_ variable _var1_, 
simulator B requires/uses/updates an _untyped_ variable _var1_
* simulator A produces a _typed_ variable _var1_ of type `double`, 
simulator B requires/uses/updates a _typed_ variable _var1_ of type `double`


These scenarios of variable exchanges between two simulators are failing:

* simulator A produces an _untyped_ variable _var1_, 
simulator B requires/uses/updates a _typed_ variable _var1_
* simulator A produces a _typed_ variable _var1_ of type `double`, 
simulator B requires/uses/updates a _typed_ variable _var1_ of type `matrix` 


_Example of variable declarations:_
```
"produced": [
          {
            "name": "examples.RU.S.stock",
            "unitsclass": "RU",
            "description": "number of cars stocked on RU",
            "siunit": "",
            "type": ""
          }
        ]
```


### Discrete events {#dev_signature_data_events}

Discrete events are attached to spatial units, They are accessed or appended by simulators during simulations,
and are declared in the "events" block.

_Example of events declaration:_
**TOIMPL**


### Extra files {#dev_signature_data_extrafiles}

Simulators can declare files that they load and manage. This helps users to provide the needed files, and also  
notifies the OpenFLUID framework to check the presence of the file if it is required.   
These files are declared in the "extrafiles" block.

The declaration instruction takes 1 argument: the file name with relative path to the dataset path.

_Example of extra file declarations:_
**TOIMPL**


### Spatial units graph {#dev_signature_data_spatial}

The spatial units graph representing the landscape can be modified by simulators.
These modifications are declared in the signature function using two instructions.  
 
The "spatial_graph" block is used for declaration of the global units graph modification
that will occur during simulation. 
  

_Example of declarations for spatial units graph:_
**TOIMPL**

# Before OpenFLUID 2.2
The signature has to be defined between the @if DocIsLaTeX **BEGIN_SIMULATOR_SIGNATURE** @else BEGIN_SIMULATOR_SIGNATURE @endif
 and the @if DocIsLaTeX **END_SIMULATOR_SIGNATURE** @else END_SIMULATOR_SIGNATURE @endif 
 instructions.  
The signature is usually placed in the upper part of the simulator main source file, before the C++ class of the simulator.

## Identification

The identification part of the signature must contain at least the ID of the simulator. 
This ID will be used by the framework to load simulators. It is declared in 
the signature as an argument of the @if DocIsLaTeX **BEGIN_SIMULATOR_SIGNATURE** @else BEGIN_SIMULATOR_SIGNATURE @endif
 instruction.   
Other optional informations can be included for better description of the simulator:

* the simulator name, declared through the @if DocIsLaTeX **DECLARE_NAME** @else DECLARE_NAME @endif
 instruction,
allowing to assign a long name to the simulator
* the simulator description, declared through the @if DocIsLaTeX **DECLARE_DESCRIPTION** @else DECLARE_DESCRIPTION @endif
 instruction,
allowing to provide a detailed description of what the simulator actually does 
* the name(s) of the author(s) and corresponding email address(es), declared through the @if DocIsLaTeX **DECLARE_AUTHOR** @else DECLARE_AUTHOR @endif
 instruction.
There may be multiple @if DocIsLaTeX **DECLARE_AUTHOR** @else DECLARE_AUTHOR @endif
 instructions in the signature in case of multiple authors
* the software version of the simulator, declared through the @if DocIsLaTeX **DECLARE_VERSION** @else DECLARE_VERSION @endif
 instruction
* the software status of the simulator, declared through the @if DocIsLaTeX **DECLARE_STATUS** @else DECLARE_STATUS @endif
 instruction. The value can be
_openfluid::ware::EXPERIMENTAL_, _openfluid::ware::BETA_ or _openfluid::ware::STABLE_


## Informations about scientific application

The informations about scientific applications are only indicative.
It has no effects on simulator consistency or computational code.
These informations can be :

* the domain in which the simulator can be applied, declared through the @if DocIsLaTeX **DECLARE_DOMAIN** @else DECLARE_DOMAIN @endif
 instruction
* the processes simulated by the simulator, declared through the @if DocIsLaTeX **DECLARE_PROCESS** @else DECLARE_PROCESS @endif
 instruction
* the numerical methods used by the simulator, declared through the @if DocIsLaTeX **DECLARE_METHOD** @else DECLARE_METHOD @endif
 instruction


## Data and spatial graph

The data used by the simulators can be:

* Parameters that are attached to the simulator
* Spatial attributes that are attached to spatial units, giving properties about the spatial units
* Simulation variables that are attached to spatial units, representing the resulting dynamics of modeled processes
over the spatial units
* Discrete events that are attached to spatial units, 
representing the events occurring at a given date and time on a given spatial unit
* Specific file(s) loaded by the simulator 

These data can be accessed, appended and/or modified by the simulator.  
<br/>
The spatial graph representing the landscape can also be accessed or modified 
by simulators during simulations.   
<br/>
The declarations of spatial data access include constraint levels:

* _REQUIRED_, this means that the data must be available or already produced
* _USED_, this means that the data are used only if they are available or already produced
 

### Simulator parameters

Simulator parameters are values provided to each simulator, 
and are declared using the @if DocIsLaTeX **DECLARE_REQUIRED_PARAMETER** or **DECLARE_USED_PARAMETER**
@else DECLARE_REQUIRED_PARAMETER or DECLARE_USED_PARAMETER @endif
 instructions.
These instructions takes 3 arguments

* the name of the parameter
* the description of the parameter (may be empty)
* the SI unit of the parameter (may be empty)


_Example of a declaration of a required simulator parameter:_
**TOIMPL**


### Spatial attributes

Spatial attributes are constant properties attached to each spatial units, 
and are declared using @if DocIsLaTeX **DECLARE_REQUIRED_ATTRIBUTE**, **DECLARE_USED_ATTRIBUTE** 
or **DECLARE_PRODUCED_ATTRIBUTE**
@else DECLARE_REQUIRED_ATTRIBUTE, DECLARE_USED_ATTRIBUTE or DECLARE_PRODUCED_ATTRIBUTE @endif
 instructions

These instructions take 4 arguments:

* the name of the attribute
* the units class
* the description of the attribute (may be empty)
* the SI unit of the attribute (may be empty)


### Simulation variables

Simulation variables are attached to spatial units. They are produced, accessed and modified
by simulators during simulations.  
Accessed variables are declared using @if DocIsLaTeX **DECLARE_REQUIRED_VARIABLE** or **DECLARE_USED_VARIABLE**
@else DECLARE_REQUIRED_VARIABLE or DECLARE_USED_VARIABLE @endif
 instructions,
produced variables are declared using @if DocIsLaTeX **DECLARE_PRODUCED_VARIABLE**
@else DECLARE_PRODUCED_VARIABLE @endif
 instruction, 
updated variables are declared using @if DocIsLaTeX **DECLARE_UPDATED_VARIABLE**
@else DECLARE_UPDATED_VARIABLE @endif
 instruction.

These instructions take 4 arguments:

* the name of the variable
* the concerned unit class
* the description of the variable (may be empty)
* the SI unit of the variable (may be empty)


These variables can be typed or untyped. 
When they are declared in the signature, the variable names suffixed by the [] symbol with a type name enclosed
are typed, i.e. each value for the variable must match the type of the variable.
If no type is mentioned, values for the variable can be of any type.  
In case of typed variables, the type of a required or used variable by a simulator
must match the type of the variable set when it is produced.   

The type name for a declaration of a variable can be:

* `boolean` for boolean values
* `integer` for long integer values
* `double` for double precision values
* `string` for string values
* `vector` for vector data
* `matrix` for matrix data
* `map` for associative key-value data
* `tree` for hierarchical key-value data


These scenarios of variable exchanges between two A and B simulators run successfully:

* simulator A produces an _untyped_ variable _var1_,
simulator B requires/uses/updates an _untyped_ variable _var1_
* simulator A produces a _typed_ variable _var1_, 
simulator B requires/uses/updates an _untyped_ variable _var1_
* simulator A produces a _typed_ variable _var1_ of type `double`, 
simulator B requires/uses/updates a _typed_ variable _var1_ of type `double`


These scenarios of variable exchanges between two simulators are failing:

* simulator A produces an _untyped_ variable _var1_, 
simulator B requires/uses/updates a _typed_ variable _var1_
* simulator A produces a _typed_ variable _var1_ of type `double`, 
simulator B requires/uses/updates a _typed_ variable _var1_ of type `matrix` 


### Discrete events 

Discrete events are attached to spatial units, They are accessed or appended by simulators during simulations,
and are declared using the @if DocIsLaTeX **DECLARE_USED_EVENTS**
@else DECLARE_USED_EVENTS @endif
 instruction.  

The declaration instruction takes 1 argument: the units class.


### Extra files

Simulators can declare files that they load and manage. This helps users to provide the needed files, and also  
notifies the OpenFLUID framework to check the presence of the file if it is required.   
These files are declared using the @if DocIsLaTeX **DECLARE_USED_EXTRAFILE**
@else DECLARE_USED_EXTRAFILE @endif
 or @if DocIsLaTeX **DECLARE_REQUIRED_EXTRAFILE**
@else DECLARE_REQUIRED_EXTRAFILE @endif
 instructions. 

The declaration instruction takes 1 argument: the file name with relative path to the dataset path.


### Spatial units graph

The spatial units graph representing the landscape can be modified by simulators.
These modifications are declared in the signature function using two instructions.  
 
The @if DocIsLaTeX **DECLARE_UPDATED_UNITSGRAPH**
@else DECLARE_UPDATED_UNITSGRAPH @endif
 instruction is used for declaration of the global units graph modification
that will occur during simulation. 
It is for information purpose only, and takes a description as a single argument.  
 
The @if DocIsLaTeX **DECLARE_UPDATED_UNITSCLASS**
@else DECLARE_UPDATED_UNITSCLASS @endif 
 instruction is used for declaration of units classes that will be affected, and how.
It takes two arguments:

* the units class
* the description of the update (may be empty)
   


## Complete signature example {#dev_signature_exmpl}

The signature code below shows an example of a signature for a simulator.  
  

```json
{
  "id": "traffic.surf.car-transfer",
  "name": "Road Unit (RU) function transfert and stockage for cars",
  "description": "",
  "version": "1.0",
  "status": "experimental",
  "authors": [
    {
      "name": "MR",
      "email": "m.r@inrae.fr"
    }
  ],
  "contacts": [],
  "license": "",
  "tags": [
    "examples",
    "traffic",
    "urban"
  ],
  "links": [],
  "issues": [],
  "simulator": {
    "data": {
      "parameters": {
        "required": [],
        "used": [
          {
            "name": "MultiCapacity",
            "description": "Multiplying factor for capacity",
            "siunit": "-",
            "type": ""
          }
        ]
      },
      "attributes": {
        "required": [],
        "used": [
          {
            "name": "stockini",
            "unitsclass": "RU",
            "description": "",
            "siunit": "-",
            "type": ""
          },
          {
            "name": "capacity",
            "unitsclass": "RU",
            "description": "",
            "siunit": "car/min",
            "type": ""
          }
        ],
        "produced": []
      },
      "variables": {
        "produced": [
          {
            "name": "examples.RU.S.stock",
            "unitsclass": "RU",
            "description": "number of cars stocked on RU",
            "siunit": "",
            "type": ""
          }
        ],
        "required": [],
        "used": [
          {
            "name": "examples.TLU.S.state",
            "unitsclass": "TLU",
            "description": "traffic light unit state",
            "siunit": "",
            "type": ""
          }
        ],
        "updated": []
      },
      "events": [],
      "extrafiles": {
        "required": [],
        "used": []
      }
    },
    "spatial_graph": {
      "description": "",
      "details": []
    },
    "scheduling": {
      "type": "default",
      "min": 0,
      "max": 0
    }
  }
}
```