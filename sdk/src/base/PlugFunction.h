/**
  \file PlugFunction.h
  \brief header file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>

  The PluggableFunction class defines the minimal structure for a 
  pluggable function
  ALL PLUGGABLE FUNCTIONS MUST INHERIT FROM THIS CLASS TO BE LOADED
  It sets some essential methods
  The PluggableFunctionProc type defines the handle method for the 
  function integration into the host application. Every pluggable 
  simulation function must have the function:
  \code
  extern "C"
  {
    DLLIMPORT PluggableFunction* GetMHYDASPluggableFunction(wxString SelfPath);
  };
  \endcode
  returning an instance of the pluggable function class, inherited from the class defined here.
*/


#ifndef __PLUGFUNCTION_H__
#define __PLUGFUNCTION_H__

#include <wx/wx.h>
#include "mhydasdk-core.h"
#include "SimStatus.h"
#include "ExecMsgs.h"


// =====================================================================
// =====================================================================


#ifdef WIN32
  #ifdef BUILDPLUGDLL
    #define DLLIMPORT __declspec(dllexport)
  #else
    #define DLLIMPORT __declspec(dllimport)
  #endif
  #define CALL __stdcall
#else
  #define DLLIMPORT
  #define CALL
#endif


// =====================================================================
// =====================================================================

#define PLUGIN_PROC_NAME "GetMHYDASPluggableFunction"

// =====================================================================
// =====================================================================

#define DECLARE_FUNCTION_PARAM(name,description,unit) \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("gpar;--;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));

#define DECLARE_SU_PRODUCED_VAR(name,description,unit) \
  m_SUVarsToCreate.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("pvar;SU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));
  
#define DECLARE_SU_UPDATED_VAR(name,description,unit) \
  m_SUVarsToUpdate.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("uvar;SU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  
  
#define DECLARE_SU_REQUIRED_VAR(name,description,unit) \
  m_SUVarsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("rvar;SU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  

#define DECLARE_SU_USED_VAR(name) \
  m_SUVarsToCreate.Add(wxT(name));

#define DECLARE_SU_REQUIRED_PROPERTY(name,description,unit) \
  m_SUPropsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("prop;SU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  

#define DECLARE_SU_REQUIRED_INICOND(name,description,unit) \
  m_SUInicondsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("inic;SU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));



#define DECLARE_RS_PRODUCED_VAR(name,description,unit) \
  m_RSVarsToCreate.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("pvar;RS;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));
  
#define DECLARE_RS_UPDATED_VAR(name,description,unit) \
  m_RSVarsToUpdate.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("uvar;RS;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  
  
#define DECLARE_RS_REQUIRED_VAR(name,description,unit) \
  m_RSVarsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("rvar;RS;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  

#define DECLARE_RS_USED_VAR(name) \
  m_RSVarsToCreate.Add(wxT(name));

#define DECLARE_RS_REQUIRED_PROPERTY(name,description,unit) \
  m_RSPropsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("prop;RS;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  

#define DECLARE_RS_REQUIRED_INICOND(name,description,unit) \
  m_RSInicondsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("inic;RS;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));



#define DECLARE_GU_PRODUCED_VAR(name,description,unit) \
  m_GUVarsToCreate.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("pvar;GU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));
  
#define DECLARE_GU_UPDATED_VAR(name,description,unit) \
  m_GUVarsToUpdate.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("uvar;GU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  
  
#define DECLARE_GU_REQUIRED_VAR(name,description,unit) \
  m_GUVarsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("rvar;GU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  

#define DECLARE_GU_USED_VAR(name) \
  m_GUVarsToCreate.Add(wxT(name));

#define DECLARE_GU_REQUIRED_PROPERTY(name,description,unit) \
  m_GUPropsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("prop;GU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));  

#define DECLARE_GU_REQUIRED_INICOND(name,description,unit) \
  m_GUInicondsToCheck.Add(wxT(name)); \
  mp_Signature->HandledVarsPropsParams.Add(wxString(wxT("inic;GU;"))+wxString(wxT(name))+wxString(wxT(";"))+wxString(description)+wxString(wxT(";"))+wxString(unit));




// =====================================================================
// =====================================================================



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

#define END_LOOP }

// =====================================================================
// =====================================================================


namespace mhydasdk { namespace base {


/**
  Plugin function list
*/
enum FunctionTypeList
{
  UNKNOWN,
  SIMULATION
};


/**
  Plugin modules list
  \deprecated
*/
enum ModuleTypeList
{
  MOD_UNKNOWN,
  MOD_HYDROLOGY
};




// =====================================================================
// =====================================================================



/**
  Structure encapsulating the plugin signature,
  returned from the plugin to the host app for registering
*/
struct Signature
{

  /**
    Plugin function type
  */
  FunctionTypeList FunctionType;

  /**
    Plugin module type
  */
  // ModuleTypeList ModuleType;

  /**
    Plugin identity
  */
  wxString ID;

  /**
    Plugin name
  */
  wxString Name;

  /**
    Plugin Description
  */
  wxString Description;

  /**
    Plugin domain (i.e. hydrology, pop, erosion, ...)
  */
  wxString Domain;

  /**
    Plugin simulated process (i.e. surface rainfall-runoff production, ditch infiltration, ...)
  */
  wxString Process;

  /**
    Plugin involved method (i.e. morel-seytoux, hayami, ...)
  */
  wxString Method;

  /**
    Major version number
  */
  wxString MajorVersion;

  /**
    Minor version number
  */
  wxString MinorVersion;

  /**
    Author's name
  */
  wxString Author;

  /**
    Author's email
  */
  wxString AuthorEmail;
  
  wxArrayString HandledVarsPropsParams;

};

// =====================================================================
// =====================================================================

WX_DECLARE_HASH_MAP(int, float, wxIntegerHash, wxIntegerEqual, IDFloatValueMap);

WX_DECLARE_HASH_MAP(int, int, wxIntegerHash, wxIntegerEqual, IDIntValueMap);

WX_DECLARE_HASH_MAP(int, bool, wxIntegerHash, wxIntegerEqual, IDBoolValueMap);

WX_DECLARE_HASH_MAP(int, mhydasdk::core::VectorOfDouble*, wxIntegerHash, wxIntegerEqual, IDVectOfDoubleMap);

// =====================================================================
// =====================================================================


// =====================================================================
// =====================================================================


/**
  Abstact class for plugin interface

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class PluggableFunction : public wxObject
{

  protected:

    mhydasdk::base::Signature* mp_Signature;

    mhydasdk::core::ParamsMap m_ParamsMap;

    mhydasdk::core::CoreRepository* mp_CoreData;
    
    mhydasdk::base::ExecutionMessages* mp_ExecMsgs;

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


    bool MHYDAS_GetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, float *Value);

    bool MHYDAS_GetDistributedProperty(mhydasdk::core::HydroObject *HO, wxString PropName, float *Value);
    
    bool MHYDAS_GetDistributedIniCondition(mhydasdk::core::HydroObject *HO, wxString IniCondName, float *Value);
  
    bool MHYDAS_GetDistributedRainValue(mhydasdk::core::SurfaceUnit *SU, int Step, float *Value); 
    
    bool MHYDAS_IsDistributedVarExists(mhydasdk::core::HydroObject *HO, wxString VarName);
    
    bool MHYDAS_AppendDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, float Value);
    
    bool MHYDAS_SetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, float Value);

    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, float *Value);
    
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, int *Value);


  public:
    /**
      Constructor
    */
    PluggableFunction();

    /**
      Virtual destructor
    */
    virtual ~PluggableFunction() { };

    /**
      Returns the pluggable function signature
      \return \link Signature Signature \endlink
    */
    mhydasdk::base::Signature* getSignature() { return mp_Signature; };


    bool setDataRepository(mhydasdk::core::CoreRepository* CoreData) { mp_CoreData = CoreData; };
    
    bool setExecutionMessages(mhydasdk::base::ExecutionMessages* ExecMsgs) { mp_ExecMsgs = ExecMsgs; };

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
    virtual bool checkConsistency();
    
    /**
      Pure virtual method, must be overloaded.
      Place pre-simulation initialization here in overloads
    */
    virtual bool initializeRun(SimulationInfo* SimInfo)=0;

    /**
      Pure virtual method, must be overloaded.
      Place simulation process that must be done here in overloads
    */
    virtual bool runStep(SimulationStatus* SimStatus)=0;

    /**
      Pure virtual method, must be overloaded.
      Place post-simulation finalization here in overloads
    */
    virtual bool finalizeRun(SimulationInfo* SimInfo)=0;
    

};


// =====================================================================
// =====================================================================

typedef PluggableFunction*(*GetPluggableFunctionProc)();


} } // namespace mhydasdk::base



#endif
