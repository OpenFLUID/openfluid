/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __MODULE_H__
#define __MODULE_H__


#include "sdk-core.h"
#include "sdk-base.h"

#include <wx/list.h>


WX_DECLARE_LIST(mhydasdk::base::Function*, FunctionsList);


/**

*/
class Module : public mhydasdk::base::ComputationBlock
{

  protected:

    FunctionsList m_Functions;

  public:
    /**
      Constructor
    */
    Module(mhydasdk::core::CoreRepository* CoreData, FunctionsList Functions);

    /**
      Destructor
    */
    ~Module();

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(mhydasdk::base::SimulationStatus* SimStatus);

    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);

    bool finalizeRun(mhydasdk::base::SimulationStatus* SimStatus);



};



#endif



