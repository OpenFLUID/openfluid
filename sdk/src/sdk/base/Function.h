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
#define SU_INICOND_TO_CHECK(name) m_SUInicondsToCheck.Add(wxT(name))

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

// #define GET_SU_RAINVALUE(suobj,step) (suobj->getRainSource()->getTimeSerie()->getItemsCollection()->at(step)->getValue())

#define DECLARE_SU_ORDERED_LOOP \
  list<mhydasdk::core::SurfaceUnit*>::iterator _M_SUiter; \
  list<mhydasdk::core::SurfaceUnit*>* _M_SUsList = mp_CoreData->getSpatialData()->getSUsOrderedList();

#define BEGIN_SU_ORDERED_LOOP(suobj) \
  for(_M_SUiter=_M_SUsList->begin(); _M_SUiter != _M_SUsList->end(); _M_SUiter++) \
  { \
    suobj = *_M_SUiter; \


#define DECLARE_RS_ORDERED_LOOP \
  list<mhydasdk::core::ReachSegment*>::iterator _M_RSiter; \
  list<mhydasdk::core::ReachSegment*>* _M_RSsList = mp_CoreData->getSpatialData()->getRSsOrderedList();

#define BEGIN_RS_ORDERED_LOOP(rsobj) \
  for(_M_RSiter=_M_RSsList->begin(); _M_RSiter != _M_RSsList->end(); _M_RSiter++) \
  { \
    rsobj = *_M_RSiter; \


#define DECLARE_GU_ORDERED_LOOP \
  list<mhydasdk::core::GroundwaterUnit*>::iterator _M_GUiter; \
  list<mhydasdk::core::GroundwaterUnit*>* _M_GUsList = mp_CoreData->getSpatialData()->getGUsOrderedList(); 

#define BEGIN_GU_ORDERED_LOOP(guobj) \
  for(_M_GUiter=_M_GUsList->begin(); _M_GUiter != _M_GUsList->end(); _M_GUiter++) \
  { \
    guobj = *_M_GUiter; \

#define APPEND_SIMVAR_VALUE(hobj,name,value) (hobj->getSimulatedVars()->find(wxT(name))->second)->push_back(value)

#define GET_SIMVAR_VALUE(hobj,name,step) (hobj->getSimulatedVars()->find(wxT(name))->second)->at(step)

#define SIMVAR_EXISTS(hobj,name) (hobj->getSimulatedVars()->find(wxT(name)) != hobj->getSimulatedVars()->end())

#define END_LOOP }



// =====================================================================
// =====================================================================


namespace mhydasdk { namespace base {


WX_DECLARE_HASH_MAP(int, float, wxIntegerHash, wxIntegerEqual, IDFloatValueMap);

WX_DECLARE_HASH_MAP(int, int, wxIntegerHash, wxIntegerEqual, IDIntValueMap);

WX_DECLARE_HASH_MAP(int, bool, wxIntegerHash, wxIntegerEqual, IDBoolValueMap);

WX_DECLARE_HASH_MAP(int, mhydasdk::core::VectorOfDouble*, wxIntegerHash, wxIntegerEqual, IDVectOfDoubleMap);

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
    virtual ~Function() { };

    /**
      initializes of global parameters of the function, given as a hash map
      (Params["name"] gives the value of the param named "name")
    */
    virtual bool initParams(mhydasdk::core::ParamsMap Params)=0;

    /**
      Adds needed data.
      May be overloaded and called if you want to add more preparation instructions.
    */
    bool prepareData();


    /**
      Checks needed data.
      May be overloaded and called if you want to add more consistency checking instructions.
    */
    bool checkConsistency();


    bool MHYDAS_GetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, float *Value);

    bool MHYDAS_GetDistributedProperty(mhydasdk::core::HydroObject *HO, wxString PropName, float *Value);
    
    bool MHYDAS_GetDistributedIniCondition(mhydasdk::core::HydroObject *HO, wxString IniCondName, float *Value);
  
    bool MHYDAS_GetDistributedRainValue(mhydasdk::core::SurfaceUnit *SU, int Step, float *Value); 
    
    bool MHYDAS_IsDistributedVarExists(mhydasdk::core::HydroObject *HO, wxString VarName);
    
    bool MHYDAS_AppendDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, float Value);
    
    bool MHYDAS_SetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, float Value);
    
};





} } // namespace mhydasdk::base



#endif





