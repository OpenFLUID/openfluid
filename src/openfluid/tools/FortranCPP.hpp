/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file FortranCPP.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>


  @see http://www-h.eng.cam.ac.uk/help/tpl/languages/mixinglanguages.html
  @see http://www.neurophys.wisc.edu/comp/docs/notes/not017.html
  @see http://www.yolinux.com/TUTORIALS/LinuxTutorialMixingFortranAndC.html
  @see http://arnholm.org/software/cppf77/cppf77.htm
  @see http://www.aei.mpg.de/~jthorn/c2f.html

  Below are examples of C++ simulators calling Fortran subroutines.


  Fortran source code:
  @include help.snippets.fortran-more/FortranMoreSubr.f90


  Declaration of Fortran subroutines to call (In C++ code):
  @snippet help.snippets.fortran-more/FortranMoreSim.cpp fortran_decl


  Use of Fortran subroutines tFrom C++ code:
  @snippet help.snippets.fortran-more/FortranMoreSim.cpp fortran_use


  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration"],
    "title" : "Include FortranCPP header",
    "text" : "#include <openfluid/tools/FortranCPP.hpp>"
  }
  @endcond
*/


#ifndef __OPENFLUID_TOOLS_FORTRANCPP_HPP__
#define __OPENFLUID_TOOLS_FORTRANCPP_HPP__

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code"],
    "title" : "Declaration block",
    "text" : "BEGIN_EXTERN_FORTRAN\n%%CURSOR%%\nEND_EXTERN_FORTRAN"
  }
  @endcond

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code"],
    "title" : "Beginning of declaration block",
    "text" : "BEGIN_EXTERN_FORTRAN"
  }
  @endcond

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code"],
    "title" : "End of declaration block",
    "text" : "END_EXTERN_FORTRAN"
  }
  @endcond
*/
#ifdef __cplusplus
#  define BEGIN_EXTERN_FORTRAN \
   extern "C" \
   {
#  define END_EXTERN_FORTRAN }
#else
#  define BEGIN_EXTERN_FORTRAN
#  define END_EXTERN_FORTRAN
#endif


// =====================================================================
// =====================================================================


// calling function or subroutine from a F90 module

/**
  Macro for declaration of an external fortran function
  @param[in] x the name of the function

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code", "Fortran"],
    "title" : "Declare function",
    "text" : "EXTERN_FFUNCTION(%%SEL_START%%Name%%SEL_END%%)(%%CMT_START%%parameters%%CMT_END%%)"
  }
  @endcond
*/
#define EXTERN_FFUNCTION(x) x##_

/**
  Macro for calling an external fortran function
  @param[in] x the name of the function

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Call of external code", "Fortran"],
    "title" : "Call function",
    "text" : "CALL_FFUNCTION(%%SEL_START%%Name%%SEL_END%%)(parameters)"
  }
  @endcond
*/
#define CALL_FFUNCTION(x) x##_

/**
  Macro for declaration of an external fortran subroutine
  @param[in] x the name of the subroutine

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code", "Fortran"],
    "title" : "Declare subroutine",
    "text" : "EXTERN_FSUBROUTINE(%%SEL_START%%Name%%SEL_END%%)(parameters)"
  }
  @endcond
*/
#define EXTERN_FSUBROUTINE(x) void x##_

/**
  Macro for calling an external fortran subroutine
  @param[in] x the name of the subroutine


  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Call of external code", "Fortran"],
    "title" : "Call subroutine",
    "text" : "CALL_FSUBROUTINE(%%SEL_START%%Name%%SEL_END%%)(parameters)"
  }
  @endcond
*/
#define CALL_FSUBROUTINE(x) x##_


// =====================================================================
// =====================================================================


/**
  Macro for declaration of an external fortran90 function in a module
  @param[in] x the name of the module
  @param[in] y the name of the function

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code", "Fortran90"],
    "title" : "Declare function in module",
    "text" : "EXTERN_FMODFUNCTION(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  }
  @endcond
*/
#define EXTERN_FMODFUNCTION(x,y) __##x##_MOD_##y

/**
  Macro for calling an external fortran90 function in a module
  @param[in] x the name of the module
  @param[in] y the name of the function

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Call of external code", "Fortran90"],
    "title" : "Call function in module",
    "text" : "CALL_FMODFUNCTION(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  }
  @endcond
*/
#define CALL_FMODFUNCTION(x,y) __##x##_MOD_##y

/**
  Macro for declaration of an external fortran90 subroutine in a module
  @param[in] x the name of the module
  @param[in] y the name of the subroutine

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Declaration of external code", "Fortran90"],
    "title" : "Declare subroutine in module",
    "text" : "EXTERN_FMODSUBROUTINE(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  }
  @endcond
*/
#define EXTERN_FMODSUBROUTINE(x,y) void __##x##_MOD_##y

/**
  Macro for calling an external fortran90 subroutine in a module
  @param[in] x the name of the module
  @param[in] y the name of the subroutine

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Call of external code", "Fortran90"],
    "title" : "Call subroutine in module",
    "text" : "CALL_FMODSUBROUTINE(%%SEL_START%%Module%%SEL_END%%,Name)(parameters)"
  }
  @endcond
*/
#define CALL_FMODSUBROUTINE(x,y) __##x##_MOD_##y


// =====================================================================
// =====================================================================


// Numeric data handling

/**
  Macro for fortran REAL type in C++ (float)

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Single precision float",
    "text" : "FREAL %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FREAL float

/**
  Macro for fortran REAL type in C++ (double)

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Double precision float",
    "text" : "FREAL8 %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FREAL8 double

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Double double precision float",
    "text" : "FREAL16 %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FREAL16 long double

/**
  Macro for fortran INT type in C++ (int)

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Integer",
    "text" : "FINT %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FINT int

/**
  Macro for fortran INT*2 type in C++ (short int)

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Short integer",
    "text" : "FINT2 %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FINT2 short int

/**
  Macro for fortran INT*8 type in C++ (long int)

  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Long integer",
    "text" : "FINT8 %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FINT8 long int


/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Logical",
    "text" : "FLOGICAL %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FLOGICAL int

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Logical (boolean)",
    "text" : "FLOGICAL1 %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FLOGICAL1 bool


// =====================================================================
// =====================================================================


// Character and string handling

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "Character",
    "text" : "FCHARACTER %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FCHARACTER char

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "String",
    "text" : "FSTRING %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FSTRING char*

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Types"],
    "title" : "String length",
    "text" : "FSTRINGLEN %%SEL_START%%Val%%SEL_END%%"
  }
  @endcond
*/
#define FSTRINGLEN int


// =====================================================================
// =====================================================================


// Conversion of std::string

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Functions"],
    "title" : "Convert std::string to Fortran string content",
    "text" : "STD2FSTRING(%%SEL_START%%str%%SEL_END%%)"
  }
  @endcond
*/
#define STD2FSTRING(str) (strdup((str).c_str()))

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Functions"],
    "title" : "Convert std::string to Fortran string length",
    "text" : "STD2FSTRINGLEN(%%SEL_START%%str%%SEL_END%%)"
  }
  @endcond
*/
#define STD2FSTRINGLEN(str) (strlen((str).c_str()))

/**
  @cond OpenFLUID:completion
  {
    "contexts" : ["SIMULATOR", "OBSERVER"],
    "menupath" : ["Fortran integration", "Functions"],
    "title" : "Convert std::string to Fortran string content and length",
    "text" : "STD2FSTRINGFULL(%%SEL_START%%str%%SEL_END%%)"
  }
  @endcond
*/
#define STD2FSTRINGFULL(str) (strdup((str).c_str()),strlen((str).c_str()))


#endif /* __OPENFLUID_TOOLS_FORTRANCPP_HPP__ */

