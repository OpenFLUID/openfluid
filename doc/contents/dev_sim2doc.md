# Documenting your simulators {#dev_sim2doc}
The scientific documentation of simulators is important to clearly describe
the scientific concepts and methods applied in source code of simulators.

## docalyze (OpenFLUID >= 2.2.0)
In order to facilitate the writing and maintenance of these documentation,
OpenFLUID provides the docalyzer system for simulators designers and developers.  

The documentation can be provided inside a `README.md` file located at simulator root folder, or inside `doc/` directory. It can handle LateX (as before), but also markdown and Rmarkdown in GPL build. These files will be converted into a pdf file by using the _pandoc_ tool. In non-GPL builds, only LateX doc can be handled.


Docalyze operation can be done through DevStudio ware operation "Build doc" or from command-line:
```
openfluid docalyze --src-path=<simulator_path> --output-path=<output_path>
```

### Markdown-based documentation
`README.md` or any `.md` file in the `doc` folder will be included into PDF file through pandoc util, which means that you can benefit from several features that are not always handled by markdown converters, such as handling of LaTeX formula inline in the markdown file: `$$x_{1,2} = \frac{-b \pm \sqrt{b^2-4ac}}{2a}$$` (more on [official pandoc manual](https://pandoc.org/MANUAL.html#pandocs-markdown))

### LaTeX-based documentation
You can easily include a custom LaTeX block in the ware documentation after the automatic signature bloc: create a `main.tex` file in the `doc` folder and put inside any LaTeX content.
It will be inserted in the template provided by OpenFLUID (therefore do not insert commands that would usually be put before the `\begin{document}` block).

If you want more flexibility, you can define your own custom template by creating a file called `openfluid-docalyzer.tex` in the `doc` folder, that can contain several placeholders between `##`:

- waretype
- wareid
- signature (the automatic part)
- content (the custom part from `main.tex`)

For example, here is the template used for the simulator documentation of Mhydas-related wares (two columns):

```latex

\documentclass{##docstyle##}

\usepackage{multicol}
\usepackage{color}
\usepackage{graphicx}
\usepackage{amsmath}
\usepackage{floatflt}
%\usepackage{tikz}

\setlength{\columnsep}{0.7cm}

\bibliographystyle{plain}

\definecolor{forestgreen}{rgb}{0.13,0.54,0.13}

\begin{document}

\begin{center}
  {##waretype##}\\
  \vspace{0.8em}
  {\huge \bfseries ##wareid##}\\
\end{center}\\
\vspace{1.5em}

##signature##

\vspace{1.5em}

\begin{multicols}{2}

##content##

\end{multicols}

\vspace{3em}

\begin{small}
\begin{flushright}Rendered from ##source##\\
using OpenFLUID docalyzer\\
\textit{\today}
\end{flushright}
\end{small}

\end{document}
```


<h2> sim2doc (OpenFLUID < 2.2)</h2>
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
included in the OpenFLUID command line program.

_Example of OpenFLUID command line to generate the PDF document using the Sim2Doc tool:_
```
openfluid buddy sim2doc -o inputcpp=MySimFile.cpp,pdf=1
```
