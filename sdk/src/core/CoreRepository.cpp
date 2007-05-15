/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "CoreRepository.h"
#include <iostream>

namespace mhydasdk { namespace core {


CoreRepository::CoreRepository()
{
  mp_SpatialData = new SpatialRepository();
  mp_RainEvent = new RainEvent();
  
//  mp_PcssRainEvent = NULL;

}

// =====================================================================
// =====================================================================

CoreRepository::~CoreRepository()
{

}

// =====================================================================
// =====================================================================
/*
bool CoreRepository::ProcessRainEvent(int TimeStep)
{
  mp_PcssRainEvent = new ProcessedRainEvent(mp_RainEvent,TimeStep);   
  return true;
}
*/

} } // namespace mhydasdk::core
