/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __MSEYTOUXFUNC_H__
#define __MSEYTOUXFUNC_H__


#include "sdk-base.h"
#include "sdk-core.h"

/**

*/
class MorelSeytouxFunc : public mhydasdk::base::Function
{
  private:


  public:
    /**
      Constructor
    */
    MorelSeytouxFunc(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~MorelSeytouxFunc();


    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initializeRun();

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun();


};




#endif
