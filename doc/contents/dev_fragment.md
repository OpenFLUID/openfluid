# Code fragments: Working with redundant code {#dev_fragment}

[TOC]

## Use case
When working on several simulators, it often appears that parts are redundant between them. 
These identical codes can become a problem when a change is required, it has to be done in each simulator, which generates a real risk of discrepency.

## Git solution: Submodule
To solve these cases, we decided to use a git feature named [Submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) that allows us to encapsulate this common code in a dedicated location with its own versioning history. This feature fits well our existing use of `git` as versioning control system and it can be integrated in our existing workflows.

For now, OpenFLUID environment can only deal with simple fragments with two constraints: 
- they should be *header only*, meaning that it can only contain .h/.hpp files containing all the code and not .c/.cpp files.
- they can not use functions or helpers depending on the PluggableSimulator (but other helper functions remain available)

The current handling of fragments is sufficient for many use cases, such as:
- using the same solver function, 
- share global variables (even if the excessive use of global variables is strongly discouraged!)
- provide a class for a common object structure

Example of simulator structure including a fragment:
```
this.very.simulator
├── .git
├── .gitmodules
├── README.md
├── CMakeLists.txt
├── openfluid-ware.json
├── doc/
└── src
    ├── WareMain.cpp
    ├── CMakeLists.txt
    └── fragments
        └── fragmentName (https://gitlab.abc/fragmentName)
            ├── .git
            ├── myCommonFunctions.h
            ├── openfluid-ware.json
            └── doc/
```

## Impact on code
Source files import are slightly different when using a fragment, compilation system triggered by the user knows the location `src/fragments/` and syntax such as `#include <fragmentName/myCommonFunctions.hpp>` can be used.

## Operations
### In DevStudio
To add a new fragment or import one from a distant repository, right-click on the simulator and chose the wanted action in the "add fragment" submenu.
If you want to add an existing fragment, you will then be able to add it from a Hub or from a direct git repository.

It is also possible to remove a submodule by doing a right-click on the targetted fragment and select the "remove fragment" option.

### In a console
For operations such as:
- pushing commits on the fragment, 
- adding a fragment to a simulator, 
- updating the state of a fragment contained in a simulator
see the dedicated page for direct console git operations (in french for now: @ref apdx_submodule)

### Good practices
- Metadata: a `openfluid-fragment.json` file should be added at root directory of the fragment to describe useful information, especially the field `openfluid-components` in `fragment` sub-dictionary to inform the simulator developer to add any required OpenFLUID components to ware `CMakeLists.txt` file.
- Namespace: it is advised to use a two level namespace to encapsulate the code with "fragment" as first level. For example:
```
namespace fragment { namespace hydro

double myHydroVar = 9000;

int myHydroFunction(int param1, double param2)
{
    ...
}

myHydroClass
{
    ...
};

} }  // end of namespace

```

these function will then be usable in the simulator code as follows:
```
double b = fragment::hydro::myHydroVar + 2;
int result = fragment::hydro::myHydroFunction(3, b);
```
