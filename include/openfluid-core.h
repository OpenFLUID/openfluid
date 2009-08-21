/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file openfluid-core.h
  \brief Header for inclusion of openfluid::core part

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/




#ifndef __SDKCORE_H__
#define __SDKCORE_H__

#ifdef __WIN32__
  #ifdef BUILDINGDLL
    #define DLLEXPORT __declspec(dllexport)
  #else
    #define DLLEXPORT __declspec(dllimport)
  #endif
#else
  #define DLLEXPORT
#endif



#include "core/CoreRepository.h"
#include "core/Unit.h"
#include "core/UnitsColl.h"
#include "core/InputData.h"
#include "core/Variables.h"
#include "core/EventCollection.h"
#include "core/TypeDefs.h"
#include "core/DateTime.h"



namespace openfluid {

/**
  Data structures for shared information management
*/
namespace core {

class DLLEXPORT ValueFactory;
class DLLEXPORT CoreRepository;
class DLLEXPORT Unit;
class DLLEXPORT UnitsColl;
class DLLEXPORT DistributedEvent;
class DLLEXPORT EventCollection;
class DLLEXPORT DateTime;



} } // namespace openfluid::core



#endif






