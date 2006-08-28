/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __SDKBASE_H__
#define __SDKBASE_H__

#ifdef __WXMSW__
  #ifdef BUILDINGDLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
#else
  #define DLLEXPORT
#endif



#include "base/RuntimeEnv.h"
#include "base/SimStatus.h"
#include "base/CompBlock.h"
#include "base/Function.h"
#include "base/Plugin.h"
#include "base/ColFileParser.h"
#include "base/LastError.h"



namespace mhydasdk { namespace base
{

class DLLEXPORT ColumnFileParser;
class DLLEXPORT RuntimeEnvironment;
class DLLEXPORT SimulationStatus;
class DLLEXPORT ComputationBlock;
class DLLEXPORT ColumnFileParser;
class DLLEXPORT Function;
class DLLEXPORT Plugin;

} } // namespace mhydasdk::base



#endif




