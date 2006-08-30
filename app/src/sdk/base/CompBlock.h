/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __COMPBLOCK_H__
#define __COMPBLOCK_H__


#include "sdk-core.h"
#include "SimStatus.h"


namespace mhydasdk { namespace base {


/**

*/
class ComputationBlock
{

  protected:

    mhydasdk::core::CoreRepository* mp_CoreData;


  public:
    /**
      Constructor
    */
    ComputationBlock(mhydasdk::core::CoreRepository* CoreData);

    /**
      Destructor
    */
    ~ComputationBlock();


    virtual bool initialize()=0;

    virtual bool checkConsistency()=0;

    virtual bool run(SimulationStatus* SimStatus)=0;

    virtual bool finalize()=0;

};




} } // namespace mhydasdk::base


#endif



