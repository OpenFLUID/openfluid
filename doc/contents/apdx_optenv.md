# Environment variables and command line usage {#apdx_optenv}

[TOC]

## Environment variables {#apdx_optenv_envvar}

The OpenFLUID framework takes into account the following environment
variables (if they are set in the current running environment):

* `OPENFLUID_INSTALL_PREFIX`: overrides automatic detection of install path, useful on Windows systems.
* `OPENFLUID_USERDATA_PATH`: overrides the default user data home directory (set by default to `$HOME/.openfluid` on Unix systems)
* `OPENFLUID_TEMP_PATH`: overrides the default OpenFLUID temporary directory, used by OpenFLUID software components for temporary data.
* `OPENFLUID_SIMS_PATH`: extra search paths for OpenFLUID simulators. 
The path are separated by colon on UNIX systems, and by semicolon on Windows systems. 
* `OPENFLUID_OBSS_PATH`: extra search paths for OpenFLUID observers. 

The path are separated by colon on UNIX systems, and by semicolon on Windows systems. 


## Command line usage {#apdx_optenv_cmdopt}

Usage : `openfluid [<command>] [<options>] [<args>]`


Available commands:
* About OpenFLUID: 
   * `info`:                 Display information about OpenFLUID
   * `version`:              Display OpenFLUID version
   * `install-examples`:     Install or reinstall examples in the user directory
   * `prepare-workspace`:    Prepare an OpenFLUID workspace

* About wares: 
   * `report`:               Display report about available wares
   * `show-paths`            Show search paths for wares
   * `create-ware`:          Create ware sources
   * `check`:                Checks ware sources for potential issues
   * `configure`:            Configure ware sources for build
   * `build`:                Build configured ware sources
   * `docalyze`:             Build documentation of a ware
   * `purge`:                purge build outputs on ware sources
   * `migrate-ware`:         Migrate ware sources to current version
   * `migrate-ghostsim`:     Migrate ghost simulator to current version
   * `info2build`:           Generate build files from ware information

* About simulation: 
   * `run`:                  Run a simulation
   * `create-data`:          Create project or dataset


Available options:

* `--help,-h`: display the help message
* `--version`: display version


### Running simulations {#apdx_optenv_cmdopt_run}

Run a simulation from a project or an input dataset.

Usage : `openfluid run [<options>] [<args>]`

Available options:

* `--help,-h` : display this help message
* `--auto-output-dir, -a` : create automatic output directory
* `--clean-output-dir, -c` : clean output directory before simulation
* `--max-threads=<arg>, -t <arg>` : set maximum number of threads for threaded spatial loops (default is 4)
* `--observers-paths=<arg>, -n <arg>` : add extra observers search paths (colon separated)
* `--profiling, -k` : enable simulation profiling
* `--quiet, -q` : quiet display during simulation
* `--simulators-paths=<arg>, -p <arg>` : add extra simulators search paths (colon separated)
* `--verbose, -v` : verbose display during simulation


_Example of running a simulation from an input dataset:_
```
openfluid run /path/to/dataset /path/to/results
``` 

_Example of running a simulation from a project_:
```
openfluid run /path/to/project
``` 


### Wares reporting {#apdx_optenv_cmdopt_report}

Display informations about available wares

Usage : `openfluid report [<options>] [<args>]`

Available options:

* `--help,-h` : display this help message
* `--format=<arg>` : output format, argument can be text (default) or json
* `--list, -l` : display as simple list of wares IDs
* `--observers-paths=<arg>, -n <arg>` : add extra observers search paths (colon separated)
* `--simulators-paths=<arg>, -p <arg>` : add extra simulators search paths (colon separated)
* `--with-errors, -e` : report errors if any

_Example of detailed reporting about available simulators:_
```
openfluid report simulators
``` 

_Example of reporting as a list about available observers:_
```
openfluid report observers --list
``` 


### Paths {#apdx_optenv_cmdopt_paths}
Show search paths for wares

Usage : `openfluid show-paths [<options>] [<args>]`

Available options:

* `--help,-h` : display this help message
* `--observers-paths=<arg>, -n <arg>` : add extra observers search paths (colon separated)
* `--simulators-paths=<arg>, -p <arg>` : add extra simulators search paths (colon separated)
