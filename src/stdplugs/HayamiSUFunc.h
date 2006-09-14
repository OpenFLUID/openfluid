/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __HAYAMISUFUNC_H__
#define __HAYAMISUFUNC_H__

#include "sdk-base.h"
#include "sdk-core.h"

/**

*/
class HayamiSUFunc : public mhydasdk::base::Function
{
  private:


  public:
    /**
      Constructor
    */
    HayamiSUFunc(mhydasdk::core::CoreRepository *CoreData);

    /**
      Destructor
    */
    ~HayamiSUFunc();


    bool initParams(mhydasdk::core::ParamsMap Params);

    bool initialize();

    bool checkConsistency();

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalize();


};




#endif



