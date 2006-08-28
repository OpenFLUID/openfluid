/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __FUNCTION_H__
#define __FUNCTION_H__


#include "CompBlock.h"
#include "sdk-core.h"

namespace mhydasdk { namespace base {


/**

*/
class Function : public ComputationBlock
{

  private:
    mhydasdk::core::ParamsMap m_ParamsMap;

  public:
    /**
      Constructor
    */
    Function(mhydasdk::core::CoreRepository* CoreData);

    /**
      Destructor
    */
    virtual ~Function();

    /**
      initializes of global parameters of the function, given as a hash map (Params["name"] gives the value)
    */
    virtual bool initParams(mhydasdk::core::ParamsMap Params)=0;


};





} } // namespace mhydasdk::base



#endif





