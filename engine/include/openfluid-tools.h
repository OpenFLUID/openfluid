/**
  \file openfluid-tools.h
  \brief Header for inclusion of openfluid::tools part

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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
#include "tools/DistribInterp.h"




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






