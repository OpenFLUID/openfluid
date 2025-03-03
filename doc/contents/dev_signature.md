# Declaration of the simulator signature {#dev_signature}

[TOC]

Since OpenFLUID 2.2.0, the signature is defined in a dedicated file called "openfluid-ware.json". It is located at the root of the simulator directory.
**It is advised to use DevStudio dialog via "Signature" button to edit its information instead of working directly with json content.**
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

@note From OpenFLUID 2.1 to 2.2: In most cases, the migration of a simulator can be done automatically through DevStudio "Try to migrate" button or command line "openfluid migrate-ware". This migration will automatically convert the signature from the C++ file into a corresponding json file. For more information, see the [page dedicated to ware migration on Community](https://community.openfluid-project.org/scidev/simupdate/).

<br>
@note For advanced users: It is possible to use template inside signature, see @ref apdx_tpl_signature annex

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


## Dependencies

A specific field `dependencies` can be filled (it was previously known as `external-deps` in `wareinfo.json`). This field should contain any system dependency required by the ware to be built or run.

This field value must be a dictionary, eg `{"lib1":">=1.2", "tool2":"*"}`.
For now, it is purely informative since there is not any control is done internally to check that these requirements are filled by the compilation environment.

It is advised to use the same version syntax than here: https://docs.npmjs.com/cli/v10/configuring-npm/package-json#dependencies


## Informations about scientific application {#dev_signature_sci}

The informations about scientific applications are only indicative.
It has no effects on simulator consistency or computational code.
They are stored as string list with key **tags**. These informations can be:

* the domain in which the simulator can be applied, eg `"domain::urban"`
* the processes simulated by the simulator, eg `"process::traffic"`
* the numerical methods used by the simulator, eg `"method::mseytoux"`

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
```json
"events": [
  "ZU",
  "YU"
]
```


### Extra files {#dev_signature_data_extrafiles}

Simulators can declare files that they load and manage. This helps users to provide the needed files, and also  
notifies the OpenFLUID framework to check the presence of the file if it is required.   
These files are declared in the "extrafiles" block.

The declaration instruction takes 1 argument: the file name with relative path to the dataset path.

_Example of extra file declarations:_
```json
"extrafiles": {
  "required": [
    "SUraindistri.dat",
    "rainsources.xml"
  ],
  "used": []
}
```


### Spatial units graph {#dev_signature_data_spatial}

The spatial units graph representing the landscape can be modified by simulators.
These modifications are declared in the signature function using two instructions.  
 
The "spatial_graph" block is used for declaration of the global units graph modification
that will occur during simulation. 
  

_Example of declarations for spatial units graph:_
```json
"spatial_graph": {
  "description": "The spatial graph of LU is modified according to wind directions at each time step",
  "details": [
    {
      "unitsclass": "LU",
      "description": "The LU connections are modified according to wind directions, using the neighbours cells attributes N, NE, E, SE, S, SW, W, NW."
    }
  ]
}
```




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
    "process::traffic",
    "domain::urban"
  ],
  "links": [],
  "issues": [],
  "dependencies": {
    "gdal": ">=3.2"
  },
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
