## 2.2.0

  * Added git user injection in gitaskpass
  * Added internal logging system for openfluid operations debug
  * Added Qt6 compatibility, Qt5 compatibility kept but to be deprecated 
  * Added CLI commands for ware operations
  * Added multi-column generator
  * Added capabilities about types and dimensions in single-column generator (integer, boolean, string)
  * Added parameter structure to other wares than simulator (API get function, check in DevStudio)
  * Added RNG seed as optional parameter for random generator
  * Added constraints in data name characters (variables, unit class, attributes)
  * Added functions to filesystem API
  * Added template processing utility
  * Added wheel event to clickable label
  * Added build information to targets
  * Added tools and workflows for automatic conversion of wares and ghosts to 2.2
  * Added folder operations in DevStudio: git init, remove folder, remove fragment, add fragment
  * Added code fragment feature to share code between wares
  * Added search paths for code fragments (fragments subdir in ware, at workspace level, at userdata level)
  * Added internal simulation test structure
  * Added ware qualtiy checks
  * Added ware status board in DevStudio for info and actions
  * Added fluidx format version (current is 4)
  * Added code tag types: HACK, TOIMPL
  * Added tinyxml as third party library
  * Added spatial display classes to API in ui/spatial
  * Updated signature format from inline C++ macros to standalone interoperable json file
  * Updated ghost file from xml to json
  * Updated settings files format from conf to json (openfluid, project...)
  * Updated C++ standard from 14 to 17
  * Updated every enum to enum class
  * Updated CMake minimum version to 3.10
  * Updated DevStudio UX by splitting main toolbar
  * Updated packaging stack for Windows to Msys2
  * Updated example Firespread model to use multi-column generator
  * Replaced sim2doc by docalyze utility to handle LaTeX, markdown and Rmd (uses pandoc, pdflatex and bibtex) 
  * Replaced CLI buddies by generic commands
  * Replaced rapidjson by nlohmannjson as third party library for json handling
  * Updated ghost signature edition dialog for signature in DevStudio
  * Updated chronological series classes to template
  * Updated help of CLI commands (content and structure)
  * Updated infra quality checks (more tests, prehook checks, quicker stylecheck)
  * Updated packaging for Linux, Windows and MacOS
  * Updated user documentation template and content
  * Updated example simulators to 2.2
  * Updated CI workflow with GitHub actions
  * Updated translations
  * Fixed GetUnit and diffInSeconds API functions
  * Fixed simulation mode result discrepency
  * Removed Qt from all non-UI libraries
  * Removed landr library from repository (was disabled since 2.1.10)


## 2.1.11

  * Added display of help when openfluid command line is launched 
    without any argument
  * Added build information in wares signature
  * Added display of build information for wares in Builder
  * Added display of build information for wares 
    in reports using openfluid command line
  * Added compatibility with future 1.0 version of FLUIDhub API,
    keeping compatibility with current 0.5 version
  * Added filtering of available wares to import in DevStudio
  * Added cleanPath, absolutePath and renameFile methods 
    in Filesystem class
  * Added CMake targets to build all unit tests of a given library
  * Added CMake configuration option to allow/disallow 
    network requests during tests
  * Added CMake detection of server availability 
    for tests with network requests
  * Added ability to display build information using openfluid command line
  * Added extended ISO format for datet-ime in CSV observers
  * Improved details of build information
  * Improved tests
  * Updated Devstudio UI/UX for importing wares from remote repositories
    to ensure FLUIDhub API compatibility
  * Updated CMakeLists.txt template for wares to match CMake requirements
  * Updated ABI check in plugins manager to strict check
  * Updated deprecated usage of QProcess methods
  * Updated building process for PDF documentation
  * Updated example buddy to display path in error messages
  * Updated management of CMake configuration using OPTIONS
  * Updated wares plugins interface to match C-linkage
  * Updated CI configuration
  * Replaced Qt random functions by stdlib random 
  * Replaced deprecated boost headers in unit tests
  * Replaced 7zip by CMake for background management of zip archives
  * Fixed display of build information in About dialog
  * Fixed Builder crash when cancelling window for ware opening
  * Fixed potential wrong project path management in Builder
  * Fixed input/output/temp directories in wares environment,
    now always returned as absolute paths
  * Fixed generation of KMZ file archive in KML observer
  * Fixed compilation flags for clang compiler
  * Fixed of wind usage in Firespread example
  * Fixed potential bad alloc error in GNUplot observer
  * Fixed display of example installation buddy in CLI mode
  * Cleaned source code
  * Cleaned main documentation
  * Cleaned build configuration and related scripts



## 2.1.10

  * Added colouring of simulators and generators in OpenFLUID-Builder
    that can be applied to background or border
  * Added support of 3D coordinates in map view in Builder
  * Added support of 3D coordinates in Builder extension 
    for importing GIS data
  * Added display of geometry type in map view in Builder
  * Added build and install of documentations for provided examples
  * Added ability to open source code of provided examples in Builder
  * Added ability to open external tools from Devstudio
    at workspace, ware and file levels
  * Added ability to copy full and relative path of files in Devstudio
    using the contextual menu
  * Added optional JSON format for wares reporting using the command line
  * Added ability to set the floating point format in CSV observers
  * Added saving of datasets in bindings for programming languages
  * Improved UI for memory management in Builder
  * Improved detection of geometry types for GIS data
  * Improved compatibility with recent OS
  * Improved internal compatibility with recent compilers
  * Improved internal detection of macOS version
  * Improved macOS packaging, in particular for recent versions
  * Updated build policy of openfluid-landr library, 
    set to not built by default
  * Updated tool for checking coding style
  * Updated tests
  * Updated docs with extended informations in HTML metadata
    for better sharing across social media
  * Fixed problem with empty values in Builder extension 
    for importing GIS data
  * Fixed inconsistent behaviour when saving empty values in parameters
  * Fixed "Open in Terminal" action in DevStudio on Windows systems
  * Fixed potential problem in path separators
    when running simulations as external processes in Builder
  * Fixed minor build problems
  * Cleaned code



## 2.1.9

  * Added command line mode for execution of simulations in Builder 
    as a separate process
  * Added observer to produce multi-columns CSV files
  * Added configurable auto-save mode in Devstudio
  * Added management of path and paths list separators in the Filesystem API,
    depending on operating system
  * Added experimental support of embedded R code in sumlators
  * Added automatic build of documentation examples
  * Added french translation for completion menu
  * Updated management of completion in Devstudio
  * Updated completion items in Devstudio for better consistency
  * Updated checkstyle tool according to the bracketing coding style
  * Updated configuration of external CI services
  * Updated documentation
  * Updated tests
  * Fixed double increment of line width in map view in Builder,
    occurring on large maps
  * Fixed files types detection in Devstudio
  * Fixed UI of new ware dialog in Devstudio
  * Fixed Preferences menu placement on MacOS system
  * Fixed Devstudio window size and placement at startup
  * Fixed behaviour of simulation dialogs in Builder
  * Fixed keyboard shortcut for completion popup on MacOS system
  * Fixed management of internal reference URLs
  * Fixed warnings at build type in MHYDAS model
  * Fixed possible build error of users builder-extensions,
    due to missing header file
  * Cleaned code



## 2.1.8

  * Added syncing of selection between Builder map and attributes table 
    in spatial tab
  * Added multiselection of spatial units in Builder map view 
    and attributes table
  * Added customizable line width in Builder map view
  * Added management of HiDPI icons
  * Added Example of MHYDAS model for surface hydrology applied 
    to Roujan basin (MHYDAS is now bundled with OpenFLUID as an example)
  * Improved CMake configuration files to force explicit 
    source code declaration
  * Improved internal tool for checking coding style
  * Updated command line options for wares reporting
  * Updated OpenFLUID official logo, official icons and splash screens
  * Updated resources images for better rendering on HiDPI displays
  * Updated main documentation 
  * Updated documentation of various API classes
  * Updated IDs of simulators used in examples projects
  * Updated scripts for retreival and conversion of icons 
    with standard and HiDPI modes
  * Fixed values ranges in various Builder spin boxes
  * Fixed display of fill color button for Builder map objects 
    according to the spatial dimension (2D)
  * Fixed correctness of colors represented in line/fill buttons 
    of Builder map view
  * Fixed dark mode management on MacOS (Mojave) for Builder and Devstudio
  * Fixed possible password leak in Git proxy
  * Fixed unnecessary binary built for scientific namespace
  * Fixed Git proxy tests on Windows systems
  * Cleaned code and tests



## 2.1.7

  * Introduced C++14 as the minimal standard for OpenFLUID core 
    and scientific development
  * Introduced generic layout for dialogs with messages
  * Improved GDAL compatibility macros
  * Improved internal tool for checking coding style
  * Refactored FluidX descriptors to unify advanced and classic descriptors
  * Updated documentation of command line parser class
  * Updated CMake configuration files to match new CMake policies 
  * Fixed crash when closing dialog of a running simulation in Builder
  * Fixed weird buttons labels in main toolbar in Builder and DevStudio
  * Fixed possible target names collision in CMake configuration for sim2doc
  * Cleaned code and tests



## 2.1.6

  * Added ability to display simulators and observers names instead of IDs 
    in Builder when adding to model and monitoring
  * Added button in Builder for direct access to documentations associated
    to simulators (if available) 
  * Added parallel build option for wares in Devstudio
  * Added display of execution time for commands such as configure 
    and build in DevStudio
  * Added utilities classes to facilitate bindings with other 
    languages or environments
  * Improved generation of simulators documentation with direct access from DevStudio
  * Improved UI layout in Devstudio and Builder
  * Updated CMake templates for DevStudio
  * Updated CI configurations (Travis, AppVeyor)
  * Updated Homebrew cask file for Qt dependency
  * Fixed ambiguous keyboard shortcuts in Devstudio
  * Fixed CSS for documentation layout to match new Doxygen versions
  * Fixed CMake configuration of OpenFLUID source code for better management 
    of UI/not UI compilation 
  * Cleaned code and tests



## 2.1.5

  * Introduced development dashboard in DevStudio to perform batch actions 
    such as configure, build, check, clean
  * Introduced parameterization UI for GNUplot observer
  * Added numeric version number in binary filenames of wares
  * Added handling of HTTP redirections for FluidHub connections (Qt>5.6 only)
  * Updated About dialog box with richer information about build, changes, ...
  * Improved UI for time period configuration in Builder
  * Improved continuous integration process (Travis CI, AppVeyor)
  * Improved C++11 usage
  * Switched from Qt4 to Qt5 for Linux, as already done for MacOS and Windows
  * Deprecated aggregate headers (core.hpp, base.hpp, ...)
  * Cleaned code
  * Fixed minor bugs : initialization of random generator, potential memory leaks, 
    market paths when disabled, DevStudio crash when closed while building ware,
    GDAL compatibility on 64bits fields, data types management in GIS export observer



## 2.1.4

  * Introduced run options in Builder, acccording to existing 
    command line options
  * Added elapsed execution time for simulations in Builder
  * Added menu entry in Builder to open the current project directory 
    in a file explorer 
  * Added keyboard shortcuts in Builder
  * Updated icons sets for Builder and Devstudio, now based on material design
  * Modified default behaviour when inserting generators in Builder, 
    now inserted at the beginning of the model
  * Modified title for generators widgets in Builder to be more explicit
  * Modified writing system of fluidx files to avoid data fragmentation
  * Refactored Builder software architecture
  * Cleaned code
  * Fixed bugs



## 2.1.3

  * Introduced packaging for Mac OSX, depending on homebrew package manager  
  * Introduced explicit geometry of spatial units, as a technology preview
  * Introduced proxies for easier interactions with external programs
  * Added git, CMake and GRASS GIS proxies
  * Improved dot graph observer with possible selection of data
    to be displayed in nodes
  * Improved DevStudio UI : closing menu for all open wares sources,
    collapsing of source explorer, git decorations in source explorer,
    git dashboard, status bar, ability to restart application when required
    by preferences updates
  * Improved Builder UI : status bar, ability to restart application when
    required by preferences updates
  * Ensured compatibility with GDAL for both 1.xx and 2.xx versions 
    of the library
  * Cleaned code
  * Updated doc content
  * Fixed small bugs



## 2.1.2

  * Introduced import of wares from WaresHub sites in OpenFLUID-DevStudio
  * Introduced git notifications and minimal tools in OpenFLUID-DevStudio
  * Introduced wareshub.json file editor in OpenFLUID-DevStudio
  * Introduced console colors in OpenFLUID command line tool
  * Introduced global completion system in OpenFLUID-Builder
    with specific shortcut
  * Added tooltips on connections and slots of model graphical view 
    in OpenFLUID-Builder
  * Added wares list order in model management, model graphical view 
    and monitoring management in OpenFLUID-Builder
  * Added detection of #prettycode tag in checkstyle tool    
  * Improved use of C++11
  * Updated tests naming
  * Cleaned code 
  * Fixed minor bugs : CMake detection from OpenFLUID-DevStudio, 
    LaTeX formatted SI units in sim2doc buddy, not translated strings, 
    temporary files in multi-simulations, uncaught exceptions in 
    OpenFLUID-DevStudio



## 2.1.1

  * Introduced API client for FluidHub services
  * Introduced packages for exporting and importing wares sources codes
  * Updated UI design for better consistency
  * Updated output of command line reporting of wares
  * Improved usage of C++11 features
  * Improved memory management to avoid potential memory leaks
  * Improved build configuration for OSX
  * Refactored internal management of run context
  * Removed unused dependencies
  * Cleaned code
  * Fixed bugs



## 2.1.0

  * Introduced OpenFLUID-DevStudio, an integrated development environment
    for simulators, observers and builder-extensions
  * Introduced ghost simulators system
  * Introduced parameterization UI for simulators and observers 
    in OpenFLUID-Builder
  * Introduced required/used conditions for parameters of simulators 
  * Introduced ability to provide translations for wares in OpenFLUID-Builder
  * Introduced variables in parameters values, giving informations 
    about run environment
  * Added splash screen for Builder and DevStudio applications
  * Added iterators for VectorValue and MapValue types
  * Added new TreeValue structure for tree-like data
  * Added basic filesystem tools
  * Added command line parser class
  * Added API primitive to get a list of spatial units in a given class
  * Improved OpenFLUID-LandR library
  * Improved OpenFLUID-Builder look'n'feel
  * Improved management of attributes, now stored in their native type
    instead of string
  * Modified usage of OpenFLUID command line application 
  * Renamed classes for better understanding, with compatibility aliases
  * Renamed accessor methods according to the evolution of the coding style
  * Introduced openfluid::scientific namespace for scientific helpers
  * Refactored openfluid::tools namespace with the introduction of 
    the openfluid::scientific namespace
  * Refactored management of literal representations for OpenFLUID values 
  * Removed wares dependencies to boost libraries
  * Updated OpenFLUID applications icons
  * Updated documentation
  * Fixed name mangling for Fortran90 modules in Fortran/C interface
  * Fixed potential conflicts in headers guards
  * Fixed minor errors in documentation 
  * Fixed minor bugs
  * Cleaned source code



## 2.0.2

  * Introduced management of multiple workspaces
  * Added geographic file format observer based on GDAL, in order to export
    simulations variables in geographic vector files
  * Updated simulation dialog in GUI applications
    with control buttons and enhanced informations
  * Renamed information file of projects 
    from openfluid.prj to openfluid-project.conf
  * Refactored UI components namespaces and organization
  * Modified constraints on events appending from simulators,
    now allowed in PREPAREDATA stage and later stages
  * Refactored main packaging configurations
  * Removed unwanted QtNetwork dependency from tools.hpp header  
  * Fixed header guard of config.hpp file
  * Fixed minor bugs



## 2.0.1
  
  * Added display of points, multipoints and multilines
    in map view of OpenFLUID-Builder
  * Added application icon for win32 systems  
  * Improved display of paths with correct paths separators
  * Improved management of file formats for geospatial data
    in OpenFLUID-Builder datastore and extension for geospatial data import     
  * Fixed project consistency checking in OpenFLUID-Builder, 
    now takes into account dynamic landscape creation
    and extra files add and delete
  * Fixed storage of XML special characters in parameters when saving
    in .fluidx files    
  * Fixed resizing problemsin OpenFLUID-Builder
  * Fixed crash in Builder spatial import extension when missing 
    filename extension in selected file to import
  * Cleaned code  



## 2.0.0

  * Updated simulation engine for variable time coupling, 
    including scheduling system with constraints
  * Added monitoring system with observers 
  * Removed deprecated results output system
  * Added observers for exporting simulation results to csv files, 
    kml/kmz files (Google Earth), GNUplot charts, dot graphs
  * Added the openfluid-landr library for spatial data processing
  * Added raster spatial data as accepted data for the datastore
  * Renamed simulation functions into simulators
  * Renamed spatial inputdata into attributes 
    (according to the OGC reference model)
  * Renamed openfluid-engine command line program to openfluid
  * Updated and improved data generators
  * Added CMake OpenFLUID module
  * Added CMake macros for easier build of simulators and observers
  * Redesigned and improved simulators signatures macros
  * Redesigned simulators C++ class for robustness improvement
  * Provided new examples projects, with easy access from OpenFLUID-Builder
  * Provided OpenFLUID-Builder extensions for spatial graph view 
    and spatial data import
  * Redesigned model and monitoring tabs in OpenFLUID-Builder
  * Updated binary names of OpenFLUID plugins (simulators and observers)
  * Improved OpenFLUID plugins recognition and management system
  * Removed pass-by-pointer values in OPENFLUID_* primitives
  * Added OPENFLUID_* primitives for accessing variables in many ways
  * Improved checking of primitives usage regarding the current 
    simulation stage 
  * Updated standard fields names for external spatial data
  * Added observers and datasets as packages for OpenFLUID-market deployment
    system
  * Updated doc as a completely rewritten manual for usage 
    and simulators development
  * Reorganized storage of wares in user directory
  * Added workspace directory for future development system of wares     
  * Added environment variables for overriding default user and temp paths
  * Switched to version 3 of the Boost filesystem library  
  * Refactored OpenFLUID source code, with many internal improvements
  * Improved internal test system
  * Fixed bugs



## 1.7.2

  * Added ROpenFLUID package for using OpenFLUID within the GNU R environment
  * Added improved macros for spatial units loops, as a preview of future
    releases
  * Improved management of simulation functions plugins
  * Refactored pkg-config configuration files, now splitted into 
    openfluid, openfluid-gui and openfluid-other for better build configuration
    of simulation functions and builder extensions 
  * Improved CMake configuration for RelWithDebInfo build type
  * Fixed various bugs : external call of GNUplot, duplicate layers in MapView, 
    output files headers



## 1.7.1

  * Added map view component in OpenFLUID-Builder, for spatial geometry
    vizualization
  * Added datastore system for unstructured or complex data
    (e.g. cartographic layers). This system should be extended
    in future releases 
  * Refactored model instantiation in order to avoid initialization problems
  * Added deprecation notification system at build time (gcc only)
  * Deprecated ScalarValue type
  * Deprecated specific events handling methods  
  * Upated build and tests configuration
  * Updated installation packages dependencies
  * Cleaned source code and tests
  * Fixed various bugs



## 1.7.0

  * Added definitions of value types for OpenFLUID data : 
    DoubleValue, IntegerValue, BooleanValue, StringValue,
    VectorValue, MatrixValue, MapValue, NullValue
  * Modified data type for variables, now can be any of defined value types
  * Modified data type for input data, functions parameters and events 
    informations, now stored as StringValue that can be converted to any 
    other value type.
  * Modified plugins management, now using Glib::module (glibmm)
  * Updated OpenFLUID-Builder GUI labels
  * Checked and cleaned inclusion of header files in source code
  * Updated OpenFLUID API
  * Fixed small bugs



## 1.6.2

  * Released OpenFLUID-Builder application (GUI), as a preview release
  * Released OpenFLUID::guicommon API, as a preview release
  * Added OpenFLUID Market system
  * Added OpenFLUID projects management, embedding simulation input and output
    datasets
  * Added automatic parallelization capabilities based on spatial independancy 
    (using multi-threading) 
  * Added simulation profiling capabilities
  * Added new generator for injection of already time-interpolated data  
  * Added header types for output files
  * Updated .fluidX files I/O
  * Removed .xml file extensions as standard file extension for input dataset
  * Updated OpenFLUID API
  * Fixed various bugs



## 1.6.1

  * Fixed bug in spatial loops when units class does not exist
  * Added getSeconds() method in SimulationStatus, giving the number 
    of simulated seconds since the beginning of the simulation
  * Updated documentation



## 1.6.0

  * Refactored framework architecture
  * Introduced global process order (transverse to units classes)
  * Added primitives methods in order to modify the spatial domain graph
  * Added primitives methods in order to add input data 
    from simulation functions
  * Removed "progressive output" feature, replaced by circular memory manager 
    and files buffers 
  * Refactored structure for outputs savings
  * Added log system for simulation functions messages
  * Added actions listeners
  * Added FluidXWriter feature for writing FluidX files from an instanciated
    simulation   
  * Cleaned and reorganized source code
  * Fixed some bugs



## 1.5.0

  * Added data generators useable in models descriptions
  * Introduced global parameters for models descriptions
  * Added ability to use strings and integers as input data, in addition to 
    double precision values
  * Introduced parents/children relationships for spatial units
  * Introduced FluidX format for input datasets, now with ability to use single 
    or many files
  * Introduced precision setting for output of floating point values   
  * Added environment variable OPENFLUID_FUNCS_PATH (path to search for 
    simulation functions) 
  * Added environment variable OPENFLUID_INSTALL_PREFIX (overriding automatic 
    prefix path, useful on win32 platforms)
  * Updated OpenFLUID user directory on win32 platforms,
    now %APPDATA%\openfluid\engine (OPENFLUID.IN, OPENFLUID.OUT, functions)   
  * Removed variable names checking  
  * Updated console display    
  * Replaced TinyXML library with LibXML2  
  * Refactored internal model description and model instance management
  * Refactored internal IO management
  * Refactored internal messaging management  
  * Cleaned source code
  * Improved development tests
  * Fixed bug in comparison functions for floating point values
  * Fixed bug in plugged simulation functions manager
  * Fixed bug in variable existence checking methods      
  * Fixed various small bugs


## 1.4.2

  * Fixed major bug in OPENFLUID_Is*VariableExist() primitives



## 1.4.1

  * Introduced buddies tools : dataset conversion tools (1.3 to 1.4), function 
    documentation (func2doc), generation of empty dataset , generation of 
    function source code   
  * Added methods to generate events from simulation functions
  * Renamed DistributedEvent class into Event class  
  * Added --show-paths command line option
  * Removed links to wxWidgets libraries
  * Introduced boost libraries 
  * Introduced custom dynamic library loader
  * Fixed some small bugs
  * Cleaned some code
  * Improved win32 and MacOSX compatibility  
  * Added many tests, in order to improve software quality



## 1.4.0

  * Introduced new "generic units" feature
  * Introduced new "progressive output" feature (on data and execution messages)
  * Modified data handling primitives (OPENFLUID_xxxx)
  * Modified declaration of functions signatures 
  * Added macros for units lists loops, according to the "generic units" 
    features
  * Added tool for floating point values equality check
  * Added string conversion methods in openfluid::tools
  * Removed PluggableFunction derivation from wxObject (obsolete)
  * Modified hash tables type (from wxHashMap to std::map)
  * Replaced wxString by std::string where possible
  * Updated ofefunc2doc tool, according to the new declaration of signatures
  * Added functions reporting command line option, using wildcard-based pattern 
    matching (-u, --matching-functions-report)
  * Added pkg-config config file for ofelib
  * Fixed memory allocation bug in Vector<T>
  * Fixed some minor bugs
  * Cleaned source code
  * Added more tests for validation 
  * Added quick reference manual



## 1.3.1

  * Modified management of errors (now uses exceptions)
  * Fixed bug in Vector.h (VectorValue) 
  * Added functions docs building and installing
  * Modified packaging
  * Switched to CMake as build system
  * Fixed some minor bugs



## 1.3.0

  * Changed names to OpenFLUID
  * Modified date and time management
  * Modified vector values management  
  * Modified xml files tags  
  * Fixed some bugs



## 1.2-340

  * Modified simulation period configuration, now with a begin and end date
  * Removed rain from required data for simulation
  * Added simulation functions for distribution and interpolation of rain 
    on SU and RS
  * Modified existing simulation functions to take into account the new access
    method to rain  
  * Added detailed display command line option (--verbose, -v)
  * Updated TinyXML version (2.5.3)  
  * Fixed some bugs
  * Cleaned source code   



## 1.1-300

  * Added loading of distributed discrete events
  * Modified formats of distributed properties and initial conditions files 
  * Modified format of rain sources file   
  * Fixed various bugs
  * Updated TinyXML version (2.5.3)



## 1.0-168

  * Fixed bug in the name of generated trace files (trace mode only)
  * Fixed bug in error messages for distributed properties and initial
    conditions



## 0.7-163

  * Fixed requirement of output.xml file even if results saving is disabled
    (-z command line option)
  * Added vector and scalar variables saving
  * Modified output.xml syntax : "column" attribute is now replaced by 
    "vars" attribute



## 0.7-161

  * Added development status (experimental, beta, stable) in reports 
  (comand line options -r or -x)
  * Cleaned signatures of standard simulation functions
  * Fixed case error in domain name of the HayamiRSGU function
  * Modified messages when wrong variable production (Engine)



## 0.7-155

  * Added save of vector variables in trace mode
  * Added HayamiRSGU simulation function
  * Modified variables and simulation functions names, according to the 
    brand new MHYDAS names nomenclature



## 0.7-153

  * Fixed bug in column text file read method. Double empty lines at the end 
    of files are not needed anymore



## 0.7-152

  * Fixed bug in simulation engine
  * Modified engine for handling of vector simulation variables



## 0.7-150

  * Fixed bug in hayamirs simulation function
  * Added data production consistency checking for NaN values



## 0.7-149

  * Update rebuild taking into account SDK modifications



## 0.7-148

  * Fixed bug in Hayami tools used by hayamisu and hayamirs



## 0.7-147

  * Modified file name for rain distribution on SU
  * Added file loading for rain distribution on RS
  * Modified rain distribution consistency checking



## 0.7-146

  * Fixed -c command line option behaviour
  * Fixed segmentation fault during results saves



## 0.7-143

  * Modified code taking into account last SDK modifications



## 0.7-142

  *  Fixed error in consistency checking for updated simulation variables



## 0.7-141

  * Fixed output directory creation when only the simulation report is saved
  * Added variable production control during simulation
  * Removed display of warning messages for unfinished simulations
  * Modified default user data dir, now $(HOME)/.mhydas/engine 
    instead of $(HOME)/.mhydas
  * Added checking consistency system, with extra files, rain and 
    vars produced at a previous time step
  * Modified simulation functions reports, according to 
    consistency checking system modifications
  * Modified stdplugs simulation functions, according to SDK modifications



## 0.7-135

  * Modified mm/h to m/s rain intensity transformation method
  * Modified model.xml file format, file and ID attributes of the function 
    elements are merged into a fileID attribute
  * Modified domain of in signature of stdplugs
  * Modified filenames of simulation functions binaries
  * Modified XML output when using -x command line option



## 0.7-132

  * Modified simulation functions signatures according to SDK modifications
  * Added display of SDK version number used to build the simulation functions
  * Added -k and -v command line options for versions numbers
    (resp. SDK and application)
  * Modified display output of -f command line option
  * Modified display output of -h command line option
  * Fixed very long time for saving warning messages
  * Fixed the simulation report was always saved when an error occurs during
    simulation, even if --no-simreport option was used
  * Added -t command line option to enable trace mode
  * Added -m command line option to give a specific trace output directory


