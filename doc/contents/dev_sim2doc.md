@page dev_sim2doc Documenting your simulators

The scientific documentation of simulators is important to clearly describe
the scientific concepts and methods applied in source code of simulators.
In order to facilitate the writing and maintenance of these documentation,
OpenFLUID provides the Sim2Doc system for simulators designers and developers.  
<br/>

The Sim2Doc system uses the simulator signature and an optional
 @if DocIsLaTeX
\latexonly
\LaTeX 
 \endlatexonly
\else 
LaTeX 
 @endif 
-formatted text to build a PDF or HTML document. The 
 @if DocIsLaTeX
\latexonly
 \LaTeX 
  \endlatexonly
\else 
LaTeX 
 @endif 
-formatted text can be placed in the main file of the simulator source code, 
 into a single C++ comment block, and between the `<sim2doc>` and `</sim2doc>` tags.  
<br/>


_Example of a part of source code including sim2doc informations:_
```
/*
<sim2doc>
This part of the documentation will be included in the 
It can be formatted using \LaTeX and is fully compatible with all \LaTeX commands,
including tables, scientific formulae, figures, and many more.
</sim2doc>
*/

BEGIN_SIMULATOR_SIGNATURE("example.simulator")

  DECLARE_NAME("Example simulator");
  DECLARE_DESCRIPTION("This simulator is an example");
  DECLARE_VERSION("13.05");
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);
  DECLARE_AUTHOR("John","john@foobar.org");
  DECLARE_AUTHOR("Dave","dave@foobar.org");
  DECLARE_AUTHOR("Mike","mike@foobar.org");
  
  DECLARE_REQUIRED_PARAMETER("meanspeed","mean speed to use","m/s")
  
  DECLARE_REQUIRED_ATTRIBUTE("area","TU","area of the Test Units","m")
  DECLARE_USED_ATTRIBUTE("landuse","OU","landuse of the Other Units","")
  
  DECLARE_REQUIRED_VARIABLE("varA[double]","TU","","m")
  DECLARE_USED_VARIABLE("varB","OU","simple var on Other Units","kg")
  DECLARE_PRODUCED_VARIABLE("VarB[vector]","TU","vectorized var on Test Units","kg")
  DECLARE_UPDATED_VARIABLE("VarC","TU","","")
  
  DECLARE_USED_EVENTS("TU")
  
END_SIMULATOR_SIGNATURE
```

<br/>

The final document can be generated using the OpenFLUID Sim2Doc buddy, 
included in the OpenFLUID command line program. See also @ref apdx_optenv_cmdopt_buddies command line for available options.

_Example of OpenFLUID command line to generate the PDF document using the Sim2Doc tool:_
```
openfluid buddy sim2doc -o inputcpp=MySimFile.cpp,pdf=1
```
