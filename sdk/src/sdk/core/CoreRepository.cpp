/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "CoreRepository.h"


namespace mhydasdk { namespace core {


CoreRepository::CoreRepository()
{
  mp_SpatialData = new SpatialRepository();
  mp_RainEvent = new RainEvent();

}

// =====================================================================
// =====================================================================

CoreRepository::~CoreRepository()
{

}




} } // namespace mhydasdk::core
