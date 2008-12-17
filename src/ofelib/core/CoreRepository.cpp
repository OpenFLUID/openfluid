/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "CoreRepository.h"
#include <iostream>

namespace openfluid { namespace core {


CoreRepository::CoreRepository()
{
  mp_SpatialData = new SpatialRepository();

}

// =====================================================================
// =====================================================================

CoreRepository::~CoreRepository()
{

}

// =====================================================================
// =====================================================================

} } // namespace openfluid::core
