/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/



#ifndef __MODULE_H__
#define __MODULE_H__


#include "sdk-core.h"
#include "sdk-base.h"


/**

*/
class Module : public mhydasdk::base::ComputationBlock
{

  protected:

  public:
    /**
      Constructor
    */
    Module(mhydasdk::core::CoreRepository* CoreData);

    /**
      Destructor
    */
    virtual ~Module();

    virtual bool loadData()=0;

    virtual bool saveResults()=0;


};



#endif



