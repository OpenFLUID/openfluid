/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __SDKCORE_H__
#define __SDKCORE_H__

#ifdef __WXMSW__
  #ifdef BUILDINGDLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
#else
  #define DLLEXPORT
#endif


#include <wx/hashmap.h>

#include "core/DateTime.h"
#include "core/TimeSerieItem.h"
#include "core/TimeSerie.h"
#include "core/ChronDataSource.h"
#include "core/RainEvent.h"
#include "core/CoreRepository.h"
#include "core/HydroObject.h"
#include "core/SurfaceUnit.h"
#include "core/ReachSegment.h"
#include "core/GroundwaterUnit.h"




namespace mhydasdk { namespace core {

class DLLEXPORT DateTime;
class DLLEXPORT TimeSerieItem;
class DLLEXPORT TimeSerie;
class DLLEXPORT ChronDataSource;
class DLLEXPORT RainEvent;
class DLLEXPORT ValueFactory;
class DLLEXPORT CoreRepository;
class DLLEXPORT SurfaceUnit;
class DLLEXPORT GroundwaterUnit;
class DLLEXPORT HydroObject;
class DLLEXPORT ReachSegement;


} } // namespace mhydasdk::core



#endif






