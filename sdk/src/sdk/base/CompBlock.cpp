/**
  \file
  \brief implements ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/




#include "CompBlock.h"



namespace mhydasdk { namespace base {



ComputationBlock::ComputationBlock(mhydasdk::core::CoreRepository* CoreData)
{
  mp_CoreData = CoreData;
}

// =====================================================================
// =====================================================================

ComputationBlock::~ComputationBlock()
{

}


} } // namespace mhydasdk::base


