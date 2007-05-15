/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SDKBASE_H__
#define __SDKBASE_H__


// =====================================================================
// =====================================================================


// import-export share lib defines
#ifdef __WXMSW__
  #ifdef BUILDINGDLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
#else
  #define DLLEXPORT
#endif



// =====================================================================
// =====================================================================


// unicode management macros
// inspired from Code::Blocks macros http://www.codeblocks.org/
#ifdef wxUSE_UNICODE
    #define _U(x) wxString(x,wxConvUTF8) // char* to wxString
    #define _UU(x,y) wxString(x,y)       // char* to wxString
    #define _C(x) (x).mb_str(wxConvUTF8) // wxString to char*
    #define _CC(x,y) (x).mb_str(y)       // wxString to char*
#else
    #define _U(x) (x)          // char* to wxString
    #define _C(x) (x).c_str()  // wxString to char*
#endif




// =====================================================================
// =====================================================================



#include "base/SimStatus.h"
#include "base/PlugFunction.h"
#include "base/LastError.h"
#include "base/SIFactors.h"



namespace mhydasdk { namespace base
{

class DLLEXPORT ColumnFileParser;
class DLLEXPORT SimulationStatus;
class DLLEXPORT ComputationBlock;
class DLLEXPORT Function;
class DLLEXPORT Plugin;

} } // namespace mhydasdk::base




#endif




