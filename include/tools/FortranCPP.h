/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file FortranCPP.h
  \brief Header for macros to call fortran code.
  \author JC.Fabre <fabrejc@supagro.inra.fr>, C.Dagès <dages@supagro.inra.fr>


  \see http://www-h.eng.cam.ac.uk/help/tpl/languages/mixinglanguages.html
  \see http://www.neurophys.wisc.edu/comp/docs/notes/not017.html
  \see http://www.yolinux.com/TUTORIALS/LinuxTutorialMixingFortranAndC.html
  \see http://arnholm.org/software/cppf77/cppf77.htm
  \see http://www.aei.mpg.de/~jthorn/c2f.html

  Below are examples of C++ simulation function calling Fortran subroutines.


  fortran source code:
  \code
subroutine multrealvalue(value,mult,result)

implicit none
real*8 value
real*8 mult
real*8 result

result=value*mult

return

end


! ==============================================================================


subroutine multintvalue(value,mult,result)

implicit none
integer value
integer mult
integer result

result=value*mult

return
end


! ==============================================================================


subroutine multrealmatrix(matrix,dim1,dim2,mult,result)

implicit none
integer dim1
integer dim2
real*8 matrix(dim2,dim1)
integer mult
real*8 result(dim2,dim1)
integer i, j, k
subroutine multrealvalue(value,mult,result)

implicit none
real*8 value
real*8 mult
real*8 result

result=value*mult

return

end


! ==============================================================================


subroutine multintvalue(value,mult,result)

implicit none
integer value
integer mult
integer result

result=value*mult

return
end


! ==============================================================================


subroutine multrealmatrix(matrix,dim1,dim2,mult,result)

implicit none
integer dim1
integer dim2
real*8 matrix(dim2,dim1)
integer mult
real*8 result(dim2,dim1)
integer i, j, k

do j=1,dim1
  do i=1,dim2
    result(i,j) = matrix(i,j) * mult
  end do
end do


return
end

do j=1,dim1
  do i=1,dim2
    result(i,j) = matrix(i,j) * mult
  end do
end do


return
end

\endcode


C++ code calling fortran subroutines:
\code
BEGIN_EXTERN_FORTRAN
  EXTERN_FSUBROUTINE(multrealvalue)(FREAL8*,FREAL8*,FREAL8*);
  EXTERN_FSUBROUTINE(multintvalue)(FINT*,FINT*,FINT*);
  EXTERN_FSUBROUTINE(multrealmatrix)(FREAL8*,FINT*,FINT*,FINT*,FREAL8*);
END_EXTERN_FORTRAN

// =====================================================================
// =====================================================================

bool FortranFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  int i;


  // ====== double ======

  double DValue, DMult, DResult;
  DValue = 1.5436;
  DMult = 2.5;
  DResult = 0.0;

  CALL_FSUBROUTINE(multrealvalue)(&DValue,&DMult,&DResult);

  if (abs(DResult - (DValue*DMult)) > m_Precision)
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealvalue)");


  // ====== int ======

  int IValue, IMult, IResult;
  IValue = 45;
  IMult = 18;
  IResult = 0;

  CALL_FSUBROUTINE(multintvalue)(&IValue,&IMult,&IResult);

  if (IResult != (IValue*IMult))
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multintvalue)");


  // ====== matrix of double ======

  int MMult, MDim1,MDim2;
  double *MValue;
  double *MResult;
  MMult = 3;
  MDim1 = 2;
  MDim2 = 3;


  MValue = new double[MDim1*MDim2];
  MResult = new double[MDim1*MDim2];

  for (i=0; i < MDim1*MDim2;i++) MValue[i] = 1.5;
  for (i=0; i < MDim1*MDim2;i++) MResult[i] = 0.0;

  CALL_FSUBROUTINE(multrealmatrix)(MValue,&MDim1,&MDim2,&MMult,MResult);

  for (i=0; i < MDim1*MDim2;i++)
  {
    if (abs(MResult[i] - (MValue[i] * MMult)) > m_Precision)
      OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealmatrix)");
  }

  return true;
}

\endcode

*/




#ifndef __FORTRAN2CPP_H__
#define __FORTRAN2CPP_H__


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
  \param[in] x the name of the function
*/
#define EXTERN_FFUNCTION(x) x##_

/**
  Macro for calling an external fortran function
  \param[in] x the name of the function
*/
#define CALL_FFUNCTION(x) x##_

/**
  Macro for declaration of an external fortran subroutine
  \param[in] x the name of the subroutine
*/
#define EXTERN_FSUBROUTINE(x) void x##_

/**
  Macro for calling an external fortran subroutine
  \param[in] x the name of the subroutine
*/
#define CALL_FSUBROUTINE(x) x##_


// =====================================================================
// =====================================================================


/**
  Macro for declaration of an external fortran90 function in a module
  \param[in] x the name of the module
  \param[in] y the name of the function
*/
#define EXTERN_FMODFUNCTION(x,y) __##x##__##y

/**
  Macro for calling an external fortran90 function in a module
  \param[in] x the name of the module
  \param[in] y the name of the function
*/
#define CALL_FMODFUNCTION(x,y) __##x##__##y

/**
  Macro for declaration of an external fortran90 subroutine in a module
  \param[in] x the name of the module
  \param[in] y the name of the subroutine
*/
#define EXTERN_FMODSUBROUTINE(x,y) void __##x##__##y

/**
  Macro for calling an external fortran90 subroutine in a module
  \param[in] x the name of the module
  \param[in] y the name of the subroutine
*/
#define CALL_FMODSUBROUTINE(x,y) __##x##__##y

// =====================================================================
// =====================================================================


// Numeric data handling

/**
  Macro for fortran REAL type in C++ (float)
*/
#define FREAL float

/**
  Macro for fortran REAL type in C++ (double)
*/
#define FREAL8 double

#define FREAL16 long double

/**
  Macro for fortran INT type in C++ (int)
*/
#define FINT int

/**
  Macro for fortran INT*2 type in C++ (short int)
*/
#define FINT2 short int

/**
  Macro for fortran INT*8 type in C++ (long int)
*/
#define FINT8 long int

#define FLOGICAL int
#define FLOGICAL1 bool

// =====================================================================
// =====================================================================

// Character and string handling

#define FCHARACTER char

#define FSTRING char*
#define FSTRINGLEN int

// =====================================================================
// =====================================================================


// Conversion of std::string

#define STD2FSTRING(str) strdup((str).c_str())
#define STD2FSTRINGLEN(str) strlen((str).c_str())
#define STD2FSTRINGFULL(str) strdup((str).c_str()),strlen((str).c_str())


#endif /*__FORTRAN2CPP_H__*/

