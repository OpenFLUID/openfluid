# Documenting your simulators {#dev_sim2doc}
The scientific documentation of simulators is important to clearly describe
the scientific concepts and methods applied in source code of simulators.

## docalyze (OpenFLUID >= 2.2.0)
In order to facilitate the writing and maintenance of these documentation,
OpenFLUID provides the docalyzer system for simulators designers and developers.  

The documentation can be provided inside a `README.md` file located at simulator root folder, or inside `doc/` directory. It can handle LateX (as before), but also markdown and Rmarkdown.

These files will be converted into a pdf file by using the _pandoc_ tool.

Docalyze operation can be done through DevStudio ware operation "Build doc" or from command-line:
```
openfluid docalyze --src-path=<simulator_path> --output-path=<output_path>
```

## sim2doc (OpenFLUID < 2.2)
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

<br/>

The final document can be generated using the OpenFLUID Sim2Doc buddy, 
included in the OpenFLUID command line program. See also @ref apdx_optenv_cmdopt_buddies command line for available options.

_Example of OpenFLUID command line to generate the PDF document using the Sim2Doc tool:_
```
openfluid buddy sim2doc -o inputcpp=MySimFile.cpp,pdf=1
```
