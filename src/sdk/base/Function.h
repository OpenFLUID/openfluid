/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include <wx/hashmap.h>


#include "CompBlock.h"
#include "sdk-core.h"

namespace mhydasdk { namespace base {


/**

*/
class Function : public ComputationBlock
{

  protected:
    mhydasdk::core::ParamsMap m_ParamsMap;


    /**

    */
    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap SUsCollection);

    /**

    */
    bool checkSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap SUsCollection);



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
      initializes of global parameters of the function, given as a hash map
      (Params["name"] gives the value of the param named "name")
    */
    virtual bool initParams(mhydasdk::core::ParamsMap Params)=0;

    virtual bool initialize()=0;

    virtual bool checkConsistency()=0;

    virtual bool runStep(SimulationStatus* SimStatus)=0;

    virtual bool finalize()=0;



};





} } // namespace mhydasdk::base



#endif





