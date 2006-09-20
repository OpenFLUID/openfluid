/**
  \file Function.h
  \brief header of abstract base class for functions to plug

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __FUNCTION_H__
#define __FUNCTION_H__


#include "CompBlock.h"
#include "sdk-core.h"



// =====================================================================
// =====================================================================

#define SU_VARIABLE_TO_CREATE(name) m_SUVarsToCreate.Add(wxT(name))
#define SU_VARIABLE_TO_UPDATE(name) m_SUVarsToUpdate.Add(wxT(name))
#define SU_VARIABLE_TO_CHECK(name) m_SUVarsToCheck.Add(wxT(name))
#define SU_PROPERTY_TO_CHECK(name) m_SUPropsToCheck.Add(wxT(name))
#define SU_INICOND_TO_CHECK(name) m_SUInicondsToAdd.Add(wxT(name))

#define RS_VARIABLE_TO_CREATE(name) m_RSVarsToCreate.Add(wxT(name))
#define RS_VARIABLE_TO_UPDATE(name) m_RSVarsToUpdate.Add(wxT(name))
#define RS_VARIABLE_TO_CHECK(name) m_RSVarsToCheck.Add(wxT(name))
#define RS_PROPERTY_TO_CHECK(name) m_RSPropsToCheck.Add(wxT(name))
#define RS_INICOND_TO_CHECK(name) m_RSInicondsToAdd.Add(wxT(name))

#define GU_VARIABLE_TO_CREATE(name) m_GUVarsToCreate.Add(wxT(name))
#define GU_VARIABLE_TO_UPDATE(name) m_GUVarsToUpdate.Add(wxT(name))
#define GU_VARIABLE_TO_CHECK(name) m_GUVarsToCheck.Add(wxT(name))
#define GU_PROPERTY_TO_CHECK(name) m_GUPropsToCheck.Add(wxT(name))
#define GU_INICOND_TO_CHECK(name) m_GUInicondsToAdd.Add(wxT(name))


// =====================================================================
// =====================================================================


namespace mhydasdk { namespace base {


/**

*/
class Function : public ComputationBlock
{


  protected:
    mhydasdk::core::ParamsMap m_ParamsMap;

    wxArrayString m_SUVarsToCheck;
    wxArrayString m_SUVarsToCreate;
    wxArrayString m_SUVarsToUpdate;
    wxArrayString m_SUInicondsToCheck;
    wxArrayString m_SUPropsToCheck;

    wxArrayString m_RSVarsToCheck;
    wxArrayString m_RSVarsToCreate;
    wxArrayString m_RSVarsToUpdate;
    wxArrayString m_RSInicondsToCheck;
    wxArrayString m_RSPropsToCheck;

    wxArrayString m_GUVarsToCheck;
    wxArrayString m_GUVarsToCreate;
    wxArrayString m_GUVarsToUpdate;
    wxArrayString m_GUInicondsToCheck;
    wxArrayString m_GUPropsToCheck;




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
      adds needed data.
      may be overloaded and called if you want to add more preparation instructions
    */
    bool prepareData();


    /**
      checks needed data.
      may be overloaded and called if you want to add more preparation instructions
    */
    bool checkConsistency();


};





} } // namespace mhydasdk::base



#endif





