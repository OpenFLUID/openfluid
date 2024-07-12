# Single-column value generator {#apdx_generators}

This appendix provides insight about use cases and set up of generators to provide variable data easily. It deals only with single-column csv, for multi-column data files, see @ref apdx_multicolumn-generators 

[TOC]

# What data types can be generated?

The available data types depend from the kind of wanted generation:

| Generator type                 | Double (scalar) | Double (vector) | Double (matrix) | Integer | Boolean | String |
| ------------------------------ | ------ | ------ | ------ | ------- | ------- | ------ |
| Fixed values                   | Yes    | Yes*   | Yes*   | Yes     | Yes     | Yes    |
| Random values                  | Yes    | Yes    | Yes    | Yes     | Yes     | No     |
| Values from file interpolation | Yes    | No     | No     | No      | No      | No     |
| Values from file injection     | Yes    | No     | No     | No      | No      | No     |


## Floating-point variables

*The non-scalar double generator can be used either with a single value for all, or a value for each cell of the container. Eg, for every time step and every unit of the given unit class, for a generator of vector of size 4 containing doubles: 
- "3" will produce as variable value \[3.0, 3.0, 3.0, 3.0\] 
- "[1,4,2.2,3]" will produce as variable value \[1.0, 4.0, 2.2, 3.0\] 

An option is available for the random generator, to be able to have identical or different values inside the container. 
Eg with a vector of 4 double: 
- same value activated: t0: [2.1, 2.1, 2.1, 2.1], t1: [43, 43, 43, 43]
- same value disabled: t0: [4.5, 2.0, 22.1, 1.7], t1: [10.2, 2.3, 7,4, 9.9]

## Boolean
Currently the random generator for boolean gives a balanced probability between true and false

# File formats for generators



@note Currently, these files formats are used by _interp_ and _inject_ generators only.


# Sources file {#apdx_generators_sources}

The sources file format is an XML based format which defines a list of sources
files associated to an unique ID.\n
The sources must be defined in a section delimited by the
`<datasources>` tag, inside an `<openfluid>` tag and must be
structured following these rules:

* Inside the `<datasources>` tag, there must be a set of `<filesource>` tags
* Each `<filesource>` tag must bring an `ID`
  attribute giving the identifier of source, and a `file`
  attribute giving the name of the file containing the source of data. The files
  must be placed in the input directory of the simulation.


```xml
<?xml version="1.0" standalone="yes"?>
<openfluid>
 
  <datasources>
    <filesource ID="1" file="source1.dat" />
    <filesource ID="2" file="source2.dat" />    
  </datasources>
  
</openfluid>
```

@note As a sources file is not part of the standard input dataset, it must not have a `.fluidx` extension.
Using the `.fluidx` extension may lead to an unexpected behaviour (such as deletion of the sources file).  
The recommended extension for this file is `.xml` (e.g. _sources.xml_)


An associated source data file is a two columns text file, containing a serie
of values in time. The first column is the date using the ISO format
`YYYY-MM-DD'T'HH:MM:SS`. The second column is the value itself.

```
1999-12-31T12:00:00 -1.0
1999-12-31T23:00:00 -5.0
2000-01-01T00:30:00 -15.0
2000-01-01T00:40:00 -5.0
2000-01-01T01:30:00 -15.0
```


# Distribution file {#apdx_generators_distri}

A distribution file is a two column file associating a unit ID
(first column) to a source ID (second column).

```
1 1
2 2
3 1
4 2
5 1
```
