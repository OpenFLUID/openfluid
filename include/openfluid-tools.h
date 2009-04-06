/**
  \file openfluid-tools.h
  \brief Header for inclusion of openfluid::tools part

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/




#ifndef __SDKTOOLS_H__
#define __SDKTOOLS_H__

#ifdef __WIN32__
  #ifdef BUILDINGDLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
#else
  #define DLLEXPORT
#endif




#include "tools/FortranCPP.h"
#include "tools/ColTextParser.h"
#include "tools/DataSrcFile.h"
#include "tools/DTSerie.h"
#include "tools/DistribInterp.h"
#include "tools/SwissTools.h"




// =====================================================================
// =====================================================================

// unicode management macros
// inspired from Code::Blocks macros http://www.codeblocks.org/
#ifdef wxUSE_UNICODE
    #define _U(x) wxString(x,wxConvUTF8) // char* to wxString
    #define _UU(x,y) wxString(x,y)       // char* to wxString
    #define _C(x) (x).mb_str(wxConvUTF8) // wxString to char*
    #define _CC(x,y) (x).mb_str(y)       // wxString to char*
    #define _S(x) (std::string(wxString(x).mb_str(wxConvUTF8))) //wxString to std::string
#else
    #define _U(x) (x)          // char* to wxString
    #define _C(x) (x).c_str()  // wxString to char*
    #define _S(x) (std::string((x).c_str()))//wxString to std::string
#endif

// =====================================================================
// =====================================================================



namespace openfluid {

/**
  Available tools for data processing
*/
namespace tools {

class DLLEXPORT ColumnTextParser;
class DLLEXPORT DataSourcesFile;
class DLLEXPORT DateTimeSerie;
class DLLEXPORT DistributeInterpolate;
class DLLEXPORT Tools;
struct DLLEXPORT IndexedSerie;

} }

#endif






