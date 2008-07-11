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

#include "core/CoreRepository.h"
#include "core/SpatialRepository.h"
#include "core/HydroObject.h"
#include "core/SurfaceUnit.h"
#include "core/ReachSegment.h"
#include "core/GroundwaterUnit.h"
#include "core/EventCollection.h"
#include "core/TypeDefs.h"



namespace openfluid { namespace core {

class DLLEXPORT ValueFactory;
class DLLEXPORT SpatialRepository;
class DLLEXPORT CoreRepository;
class DLLEXPORT SurfaceUnit;
class DLLEXPORT GroundwaterUnit;
class DLLEXPORT HydroObject;
class DLLEXPORT ReachSegment;
class DLLEXPORT DistributedEvent;
class DLLEXPORT EventCollection;



} } // namespace openfluid::core



#endif






