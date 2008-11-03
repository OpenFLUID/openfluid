/**
  \file tools.cpp
  \brief Common super class dor openfluid-tools classes
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/



#include "Tools.h"

namespace openfluid { namespace tools
{

Tools::Tools()
{
  m_ErrorMessage = wxT("");
}


// =====================================================================
// =====================================================================


Tools::~Tools()
{
}

} } // namespace
