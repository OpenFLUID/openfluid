/**
  \file tools.cpp
  \brief Common super class dor mhydasdk-tools classes
  
  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/



#include "Tools.h"

namespace mhydasdk { namespace tools
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
