/**
  \file openfluid-base.h
  \brief Header for inclusion of openfluid::base part

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __SDKBASE_H__
#define __SDKBASE_H__


// =====================================================================
// =====================================================================


// import-export share lib defines
#ifdef __WIN32__
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



#include "base/SimStatus.h"
#include "base/PlugFunction.h"
#include "base/SIFactors.h"
#include "base/ExecMsgs.h"
#include "base/FunctionEnv.h"
#include "base/OFException.h"


namespace openfluid {

/**
  Structures for dynamic binding between the kernel and the simulation functions
*/
namespace base
{

class DLLEXPORT ColumnFileParser;
class DLLEXPORT SimulationStatus;
class DLLEXPORT ExecutionMessages;
class DLLEXPORT PluggableFunction;
class DLLEXPORT FunctionEnvironment;
class DLLEXPORT OFException;


} } // namespace openfluid::base




#endif




