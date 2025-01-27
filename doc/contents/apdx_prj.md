# Structure of an OpenFLUID project {#apdx_prj}

[TOC]

An OpenFLUID project can be run using any of the OpenFLUID programs such as `openfluid` command line, OpenFLUID Builder or ROpenFLUID.  

As an example example, to run a simulation based on the the project located in
`/absolute/path/to/workdir/a_dummy_project` using the `openfluid` command line program, the command to use is:
```sh
openfluid run /absolute/path/to/workdir/a_dummy_project
```


An OpenFLUID project is made of a directory which includes:

* an `openfluid-project.json` file containing informations about the project,
* an `IN` subdirectory containing the input dataset,
* an `OUT` subdirectory as the default output directory, containing the simulation results if any.

The `openfluid-project.json` contains the name of the project, the description, the authors,
the creation date, the date of the latest modification, and a flag for
incremental output directory (this feature is currently disabled). 

```
{
  "name": "a dummy project",
  "description": "",
  "authors": "John Doe",
  "created_at": "2013-09-16 17:00:00",
  "updated_at": "2013-09-16 17:00:00",
  "inc_outdir": false,
  "context": {}
}
```

The `openfluid-project.json` may also contain contextual informations added by OpenFLUID software applications such as OpenFLUID-Builder. These informations can be colors of shapes in map view, placement of models in model view, ...