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


    virtual bool prepareData()=0;

    virtual bool checkConsistency()=0;

    virtual bool initializeRun()=0;

    virtual bool runStep(SimulationStatus* SimStatus)=0;

    virtual bool finalizeRun()=0;

};




} } // namespace mhydasdk::base


#endif



