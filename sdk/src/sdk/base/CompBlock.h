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
    virtual ~ComputationBlock() { };


    /**
      Pure virtual method, must be overloaded.
      Place data preparing processes here in overloads
    */
    virtual bool prepareData()=0;

    /**
      Pure virtual method, must be overloaded.
      Place consistency checking here in overloads
    */
    virtual bool checkConsistency()=0;

    /**
      Pure virtual method, must be overloaded.
      Place pre-simulation initialization here in overloads
    */
    virtual bool initializeRun()=0;

    /**
      Pure virtual method, must be overloaded.
      Place simulation process that must be done here in overloads
    */
    virtual bool runStep(SimulationStatus* SimStatus)=0;

    /**
      Pure virtual method, must be overloaded.
      Place post-simulation finalization here in overloads
    */
    virtual bool finalizeRun()=0;

};




} } // namespace mhydasdk::base


#endif



