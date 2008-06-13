/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __SDKTOOLS_H__
#define __SDKTOOLS_H__

#ifdef __WXMSW__
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

namespace mhydasdk { namespace tools {

class DLLEXPORT ColumnTextParser;
class DLLEXPORT DataSourcesFile;
class DLLEXPORT DateTimeSerie;
struct DLLEXPORT IndexedSerie;

} }

#endif






