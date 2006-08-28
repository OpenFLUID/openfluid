/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __HYDROMODULE_H__
#define __HYDROMODULE_H__

#include "sdk-base.h"

#include "Module.h"


/**

*/
class HydroModule : public Module
{
  private:
    mhydasdk::base::Function* mp_SUProdFunc;
    mhydasdk::base::Function* mp_SUTransFunc;
    mhydasdk::base::Function* mp_RSTransFunc;
    mhydasdk::base::Function* mp_RSGUExchFunc;
    mhydasdk::base::Function* mp_SUGUExchFunc;


  public:
    /**
      Constructor
    */
    HydroModule(mhydasdk::core::CoreRepository* CoreData,
                mhydasdk::base::Function* SUProdFunc,
                mhydasdk::base::Function* SUTransFunc,
                mhydasdk::base::Function* RSTransFunc,
                mhydasdk::base::Function* RSGUExchFunc,
                mhydasdk::base::Function* SUGUExchFunc);

    /**
      Destructor
    */
    ~HydroModule();

    bool loadData();

    bool saveResults();

    bool checkConsistency();

    bool initialize();

    bool run(mhydasdk::base::SimulationStatus SimStatus);

    bool finalize();

};




#endif




