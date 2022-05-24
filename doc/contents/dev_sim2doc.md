# Documenting your simulators {#dev_sim2doc}

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
@snippet help.snippets.sim2doc/Sim2DocSim.cpp sim2doc


<br/>

The final document can be generated using the OpenFLUID Sim2Doc buddy, 
included in the OpenFLUID command line program. See also @ref apdx_optenv_cmdopt_buddies command line for available options.

_Example of OpenFLUID command line to generate the PDF document using the Sim2Doc tool:_
```
openfluid buddy sim2doc -o inputcpp=MySimFile.cpp,pdf=1
```
