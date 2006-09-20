/**
  \file Function.h
  \brief header of abstract base class for functions to plug

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
      Adds a spatial simulation var serie to each SU
      \param[in] Name the name of the var to add
      \param[in] SUsCollection the SU collection
    */
    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap* SUsCollection);

    /**
      Adds a spatial simulation var serie to each RS
      \param[in] Name the name of the var to add
      \param[in] RSsCollection the RS collection
    */
    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::RSMap* RSsCollection);

    /**
      Adds a spatial simulation var serie to each GU
      \param[in] Name the name of the var to add
      \param[in] GUsCollection the GU collection
    */
    bool addSpatialSimulationVar(wxString Name, mhydasdk::core::GUMap* GUsCollection);

    /**
      Checks if a spatial simulation var serie exists for each SU
      \param[in] Name the name of the var to check
      \param[in] SUsCollection the SU collection
    */
    bool checkSpatialSimulationVar(wxString Name, mhydasdk::core::SUMap* SUsCollection);

    /**
      Checks if a spatial simulation var serie exists for each RS
      \param[in] Name the name of the var to check
      \param[in] RSsCollection the RS collection
    */
    bool checkSpatialSimulationVar(wxString Name, mhydasdk::core::RSMap* RSsCollection);

    /**
      Checks if a spatial simulation var serie exists for each GU
      \param[in] Name the name of the var to check
      \param[in] GUsCollection the GU collection
    */
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
    virtual bool initParams(mhydasdk::core::ParamsMap Params)=0;


    /**
      initialize the functions, checks if requested vars exist, adds needed vars.
      may be overloaded and called if you want to add more initialization instructions
    */
    bool initialize();

 /*
    virtual bool checkConsistency()=0;

    virtual bool runStep(SimulationStatus* SimStatus)=0;

    virtual bool finalize()=0;

*/

};





} } // namespace mhydasdk::base



#endif





