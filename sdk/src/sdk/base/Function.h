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


  protected:
    mhydasdk::core::ParamsMap m_ParamsMap;

    wxArrayString m_SUVarsToCheck;
    wxArrayString m_SUVarsToAdd;

    wxArrayString m_RSVarsToCheck;
    wxArrayString m_RSVarsToAdd;

    wxArrayString m_GUVarsToCheck;
    wxArrayString m_GUVarsToAdd;



    /**

    */
    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap* SUsCollection);

    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::RSMap* RSsCollection);

    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::GUMap* GUsCollection);

    /**

    */
    bool checkSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap* SUsCollection);

    bool checkSpatialSimulationVar(wxString Name, mhydasdk::core::RSMap* RSsCollection);

    bool checkSpatialSimulationVar(wxString Name, mhydasdk::core::GUMap* GUsCollection);




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

    bool initialize();

    virtual bool initParams(mhydasdk::core::ParamsMap Params)=0;

    virtual bool checkConsistency()=0;

    virtual bool runStep(SimulationStatus* SimStatus)=0;

    virtual bool finalize()=0;



};





} } // namespace mhydasdk::base



#endif





