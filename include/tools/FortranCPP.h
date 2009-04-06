/**
  \file FortranCPP.h
  \brief Header for macros to call fortran code.
  \author JC.Fabre <fabrejc@supagro.inra.fr>, C.Dagès <dages@supagro.inra.fr>
*/

/*
  http://www-h.eng.cam.ac.uk/help/tpl/languages/mixinglanguages.html
  http://www.neurophys.wisc.edu/comp/docs/notes/not017.html
  http://www.yolinux.com/TUTORIALS/LinuxTutorialMixingFortranAndC.html
  http://arnholm.org/software/cppf77/cppf77.htm
  http://www.aei.mpg.de/~jthorn/c2f.html
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

#define EXTERN_FFUNCTION(x) x##_
#define CALL_FFUNCTION(x) x##_

#define EXTERN_FSUBROUTINE(x) void x##_
#define CALL_FSUBROUTINE(x) x##_


// =====================================================================
// =====================================================================

// calling function or subroutine from a F90 module

#define EXTERN_FMODFUNCTION(x,y) __##x##__##y
#define CALL_FMODFUNCTION(x,y) __##x##__##y

#define EXTERN_FMODSUBROUTINE(x,y) void __##x##__##y
#define CALL_FMODSUBROUTINE(x,y) __##x##__##y

// =====================================================================
// =====================================================================


// Numeric data handling

#define FREAL float
#define FREAL8 double
#define FREAL16 long double

#define FINT int
#define FINT2 short int
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

// Conversion of wxString

#define WX2FSTRING(str) strdup((str).mb_str(wxConvUTF8))
#define WX2FSTRINGLEN(str) strlen((str).mb_str(wxConvUTF8))
#define WX2FSTRINGFULL(str) strdup((str).mb_str(wxConvUTF8)),strlen((str).mb_str(wxConvUTF8))


// Conversion of std::string

#define STD2FSTRING(str) strdup((str).c_str())
#define STD2FSTRINGLEN(str) strlen((str).c_str())
#define STD2FSTRINGFULL(str) strdup((str).c_str()),strlen((str).c_str())


#endif /*__FORTRAN2CPP_H__*/

