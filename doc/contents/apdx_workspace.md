@page apdx_workspace Organization of an OpenFLUID workspace

An OpenFLUID workspace is a directory organized to contain OpenFLUID projects for simulations 
and source code of simulators, observers and builder-extensions.  
<br/>

The default workspace is located in `${HOME}/.openfluid/workspace` for Linux and MacOS systems 
or in `%%HOMEPATH%\openfluid\workspace` for Windows systems. 
Any user can create as many workspaces as needed using the _Environment_ section of the _Preferences_ dialog.  
<br/>

A workspace directory is organized as following:

* a `project` directory containing OpenFLUID projects
* a `wares-dev` directory containing source code of wares 
  in which there are the `simulators`, `observers` and `builder-extensions` directories for each ware category.
* an `openfluid-waresdev.conf` file containing the workspace configuration for wares development
