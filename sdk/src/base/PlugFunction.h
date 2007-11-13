/**
  \file PlugFunction.h
  \brief header file for generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>

  The PluggableFunction class defines the minimal structure for a 
  pluggable function
  EVERY PLUGGABLE FUNCTION MUST INHERIT FROM THIS CLASS TO BE LOADED
  It sets some essential methods
  The PluggableFunctionProc type defines the handle method for the 
  function integration into the host application. 
  EVERY PLUGGABLE FUNCTION MUST DEFINE AND IMPLEMENT THE FUNCTION:
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


// compilation directives for shared libs linkage
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


#define DECLARE_SIGNATURE_ID(id) mp_Signature->ID = id;

#define DECLARE_SIGNATURE_NAME(name) mp_Signature->Name = name;

#define DECLARE_SIGNATURE_DESCRIPTION(desc) mp_Signature->Description = desc;

#define DECLARE_SIGNATURE_DOMAIN(domain) mp_Signature->Domain = domain;

#define DECLARE_SIGNATURE_PROCESS(process) mp_Signature->Process = process;

#define DECLARE_SIGNATURE_METHOD(method) mp_Signature->Method = method;

#define DECLARE_SIGNATURE_AUTHORNAME(name) mp_Signature->Author = name;

#define DECLARE_SIGNATURE_AUTHOREMAIL(email) mp_Signature->AuthorEmail = email;

#define DECLARE_SIGNATURE_VERSION(version) mp_Signature->Version = version;

#define DECLARE_SIGNATURE_SDKVERSION mp_Signature->setSDKVersion(MHYDASDK_MAJORVER,MHYDASDK_MINORVER,MHYDASDK_REVISION);

// =====================================================================
// =====================================================================


/**
  Macro for declaration of a function parameter
  \param[in] name name of the parameter 
  \param[in] description description of the parameter  
  \param[in] unit unit of the parameter. Could be an empty string if there is no unit  
*/ 
#define DECLARE_FUNCTION_PARAM(name,description,unit) \
  mp_Signature->HandledData.FunctionParams.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT(""),description,unit));


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all SUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/ 
#define DECLARE_SU_PRODUCED_VAR(name,description,unit) \
  mp_Signature->HandledData.ProducedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));  
  
/**
  Macro for declaration of an updated variable on all SUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/ 
#define DECLARE_SU_UPDATED_VAR(name,description,unit) \
  mp_Signature->HandledData.UpdatedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));  

/**
  Macro for declaration of a required variable on all SUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/
#define DECLARE_SU_REQUIRED_VAR(name,description,unit) \
  mp_Signature->HandledData.RequiredVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

#define DECLARE_SU_REQUIRED_PREVVAR(name,description,unit) \
  mp_Signature->HandledData.RequiredPrevVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of an used variable on all SUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/
#define DECLARE_SU_USED_VAR(name,description,unit) \
  mp_Signature->HandledData.UsedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

#define DECLARE_SU_USED_PREVVAR(name,description,unit) \
  mp_Signature->HandledData.UsedPrevVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));  
  
/**
  Macro for declaration of a required property on all SUs
  \param[in] name name of the property
  \param[in] description description of the property  
  \param[in] unit unit of the property. Could be an empty string if there is no unit    
*/
#define DECLARE_SU_REQUIRED_PROPERTY(name,description,unit) \
  mp_Signature->HandledData.RequiredProps.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

#define DECLARE_SU_USED_PROPERTY(name,description,unit) \
  mp_Signature->HandledData.UsedProps.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));
  
  
/**
  Macro for declaration of a required initial condition on all SUs
  \param[in] name name of the initial condition 
  \param[in] description description of the initial condition  
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit    
*/
#define DECLARE_SU_REQUIRED_INICOND(name,description,unit) \
  mp_Signature->HandledData.RequiredIniconds.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));  

#define DECLARE_SU_USED_INICOND(name,description,unit) \
  mp_Signature->HandledData.UsedIniconds.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));  


  
// =====================================================================
// =====================================================================

  
/**
  Macro for declaration of a produced variable on all RSs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/
#define DECLARE_RS_PRODUCED_VAR(name,description,unit) \
  mp_Signature->HandledData.ProducedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));    
  
/**
  Macro for declaration of an updated variable on all RSs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/  
#define DECLARE_RS_UPDATED_VAR(name,description,unit) \
  mp_Signature->HandledData.UpdatedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));  

/**
  Macro for declaration of a required variable on all RSs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/  
#define DECLARE_RS_REQUIRED_VAR(name,description,unit) \
  mp_Signature->HandledData.RequiredVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));  

#define DECLARE_RS_REQUIRED_PREVVAR(name,description,unit) \
  mp_Signature->HandledData.RequiredPrevVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));  


/**
  Macro for declaration of an used variable on all RSs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/
#define DECLARE_RS_USED_VAR(name) \
  mp_Signature->HandledData.UsedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));    

#define DECLARE_RS_USED_PREVVAR(name) \
  mp_Signature->HandledData.UsedPrevVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));    


/**
  Macro for declaration of a required property on all RSs
  \param[in] name name of the property 
  \param[in] description description of the property  
  \param[in] unit unit of the property. Could be an empty string if there is no unit    
*/
#define DECLARE_RS_REQUIRED_PROPERTY(name,description,unit) \
  mp_Signature->HandledData.RequiredProps.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

#define DECLARE_RS_USED_PROPERTY(name,description,unit) \
  mp_Signature->HandledData.UsedProps.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

/**
  Macro for declaration of a required initial condition on all RSs
  \param[in] name name of the initial condition 
  \param[in] description description of the initial condition  
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit    
*/
#define DECLARE_RS_REQUIRED_INICOND(name,description,unit) \
  mp_Signature->HandledData.RequiredIniconds.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

#define DECLARE_RS_USED_INICOND(name,description,unit) \
  mp_Signature->HandledData.UsedIniconds.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

// =====================================================================
// =====================================================================
  
  
/**
  Macro for declaration of a produced variable on all GUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/
#define DECLARE_GU_PRODUCED_VAR(name,description,unit) \
  mp_Signature->HandledData.ProducedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));
  
/**
  Macro for declaration of an updated variable on all GUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/  
#define DECLARE_GU_UPDATED_VAR(name,description,unit) \
  mp_Signature->HandledData.UpdatedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));
  
/**
  Macro for declaration of a required variable on all GUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/  
#define DECLARE_GU_REQUIRED_VAR(name,description,unit) \
  mp_Signature->HandledData.RequiredVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

#define DECLARE_GU_REQUIRED_PREVVAR(name,description,unit) \
  mp_Signature->HandledData.RequiredPrevVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

  
/**
  Macro for declaration of an used variable on all GUs
  \param[in] name name of the variable 
  \param[in] description description of the variable  
  \param[in] unit unit of the variable. Could be an empty string if there is no unit    
*/
#define DECLARE_GU_USED_VAR(name) \
  mp_Signature->HandledData.UsedVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

#define DECLARE_GU_USED_PREVVAR(name) \
  mp_Signature->HandledData.UsedPrevVars.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

  
/**
  Macro for declaration of a required property on all GUs
  \param[in] name name of the property 
  \param[in] description description of the property  
  \param[in] unit unit of the property. Could be an empty string if there is no unit    
*/
#define DECLARE_GU_REQUIRED_PROPERTY(name,description,unit) \
  mp_Signature->HandledData.RequiredProps.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

#define DECLARE_GU_USED_PROPERTY(name,description,unit) \
  mp_Signature->HandledData.UsedProps.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));
  

/**
  Macro for declaration of a required initial condition on all GUs
  \param[in] name name of the initial condition 
  \param[in] description description of the initial condition  
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit    
*/
#define DECLARE_GU_REQUIRED_INICOND(name,description,unit) \
  mp_Signature->HandledData.RequiredIniconds.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

#define DECLARE_GU_USED_INICOND(name,description,unit) \
  mp_Signature->HandledData.UsedIniconds.push_back(mhydasdk::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));
  
  
// =====================================================================
// =====================================================================

#define DECLARE_REQUIRED_EXTRAFILE(name) \
  mp_Signature->HandledData.RequiredExtraFiles.Add(name);


#define DECLARE_USED_EXTRAFILE(name) \
  mp_Signature->HandledData.UsedExtraFiles.Add(name);


// =====================================================================
// =====================================================================

#define DECLARE_REQUIRED_SU_RAIN mp_Signature->HandledData.RequiredRainOnSU = true;

#define DECLARE_REQUIRED_RS_RAIN mp_Signature->HandledData.RequiredRainOnRS = true;

// =====================================================================
// =====================================================================




/**
  Macro for declaration of a loop processing SUs, following their process order
*/
#define DECLARE_SU_ORDERED_LOOP \
  list<mhydasdk::core::SurfaceUnit*>::iterator _M_SUiter; \
  list<mhydasdk::core::SurfaceUnit*>* _M_SUsList = mp_CoreData->getSpatialData()->getSUsOrderedList();


/**
  Macro for the begining of a loop processing SUs, following their process order
  \param[out] suobj pointer to a mhydasdk::core::SurfaceUnit object, pointing to the current processed SU 
*/
#define BEGIN_SU_ORDERED_LOOP(suobj) \
  for(_M_SUiter=_M_SUsList->begin(); _M_SUiter != _M_SUsList->end(); _M_SUiter++) \
  { \
    suobj = *_M_SUiter; \

/**
  Macro for declaration of a loop processing RSs, following their process order
*/
#define DECLARE_RS_ORDERED_LOOP \
  list<mhydasdk::core::ReachSegment*>::iterator _M_RSiter; \
  list<mhydasdk::core::ReachSegment*>* _M_RSsList = mp_CoreData->getSpatialData()->getRSsOrderedList();

/**
  Macro for the begining of a loop processing RSs, following their process order
  \param[out] rsobj pointer to a mhydasdk::core::ReachSegment object, pointing to the current processed RS 
*/
#define BEGIN_RS_ORDERED_LOOP(rsobj) \
  for(_M_RSiter=_M_RSsList->begin(); _M_RSiter != _M_RSsList->end(); _M_RSiter++) \
  { \
    rsobj = *_M_RSiter; \


/**
  Macro for declaration of a loop processing GUs, following their process order
*/
#define DECLARE_GU_ORDERED_LOOP \
  list<mhydasdk::core::GroundwaterUnit*>::iterator _M_GUiter; \
  list<mhydasdk::core::GroundwaterUnit*>* _M_GUsList = mp_CoreData->getSpatialData()->getGUsOrderedList(); 

/**
  Macro for the begining of a loop processing GUs, following their process order
  \param[out] guobj pointer to a mhydasdk::core::GroundwaterUnit object, pointing to the current processed GU 
*/
#define BEGIN_GU_ORDERED_LOOP(guobj) \
  for(_M_GUiter=_M_GUsList->begin(); _M_GUiter != _M_GUsList->end(); _M_GUiter++) \
  { \
    guobj = *_M_GUiter; \

/**
  Macro for the ending of a loop
*/  
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

struct SignatureHandledItem
{
  wxString Name;
  wxString Type;
  wxString Distribution; // "SU", "RS", "GU", or empty if none
  wxString Description;
  wxString Unit; // empty if none, "?" if unknown
  
  SignatureHandledItem()
  {
    Name = wxT("");
    Distribution = wxT("");
    Description = wxT("");
    Unit = wxT("");
  }

  SignatureHandledItem(wxString ZeName, wxString ZeDistribution,
                       wxString ZeDescription, wxString ZeUnit)
  {
    Name = ZeName;
    Distribution = ZeDistribution;
    Description = ZeDescription;
    Unit = ZeUnit;
  }
  
};




struct SignatureHandledData
{
  std::vector<SignatureHandledItem> ProducedVars;
  
  std::vector<SignatureHandledItem> UpdatedVars;
  
  std::vector<SignatureHandledItem> RequiredVars;
  
  std::vector<SignatureHandledItem> UsedVars;
  
  std::vector<SignatureHandledItem> RequiredPrevVars;  
  
  std::vector<SignatureHandledItem> UsedPrevVars;  
  
  std::vector<SignatureHandledItem> FunctionParams;

  std::vector<SignatureHandledItem> RequiredProps;  
  
  std::vector<SignatureHandledItem> RequiredIniconds;  

  std::vector<SignatureHandledItem> UsedProps;  
  
  std::vector<SignatureHandledItem> UsedIniconds;  
  
  wxArrayString RequiredExtraFiles;

  wxArrayString UsedExtraFiles;
  
  bool RequiredRainOnSU;

  bool RequiredRainOnRS;
  
  SignatureHandledData()
  {
    RequiredRainOnSU = false;
    RequiredRainOnRS = false;    
  }
  
};


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
    Plug-in version number
  */
  wxString Version;

  /**
    SDK version number
  */
  wxString SDKVersion;
    
  /**
    Author's name
  */
  wxString Author;

  /**
    Author's email
  */
  wxString AuthorEmail;
  
  SignatureHandledData HandledData;
  
  Signature()
  {
    FunctionType = SIMULATION;
    ID = wxT("");
    Name = wxT("");
    Description = wxT("");
    Domain = wxT("");
    Process = wxT("");
    Method = wxT("");
    Author = wxT("");
    AuthorEmail = wxT("");
    Version = wxT("");
    SDKVersion = wxT("");   
  }
  
  void setSDKVersion(int Major, int Minor, int Revision)
  {
    SDKVersion << Major << wxT(".") << Minor << wxT("-") << Revision;
  }

};

// =====================================================================
// =====================================================================

struct FunctionEnvironment
{
  wxString InputDir;
  wxString OutputDir;
};


// =====================================================================
// =====================================================================

WX_DECLARE_HASH_MAP(mhydasdk::core::HOID, float, wxIntegerHash, wxIntegerEqual, IDFloatMap);

WX_DECLARE_HASH_MAP(mhydasdk::core::HOID, float, wxIntegerHash, wxIntegerEqual, IDDoubleMap);

WX_DECLARE_HASH_MAP(mhydasdk::core::HOID, float, wxIntegerHash, wxIntegerEqual, IDMHYDASValueMap);

WX_DECLARE_HASH_MAP(mhydasdk::core::HOID, int, wxIntegerHash, wxIntegerEqual, IDIntMap);

WX_DECLARE_HASH_MAP(mhydasdk::core::HOID, bool, wxIntegerHash, wxIntegerEqual, IDBoolMap);

WX_DECLARE_HASH_MAP(mhydasdk::core::HOID, mhydasdk::core::VectorOfMHYDASValue*, wxIntegerHash, wxIntegerEqual, IDVectOfMHYDASValueMap);

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

    mhydasdk::base::FunctionEnvironment m_FunctionEnv;

    /**
      Gets the distributed variable value for a spatial object at a time step
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the requested variable            
      \param[out] Value the value of the requested variable      
    */ 
    bool MHYDAS_GetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, mhydasdk::core::MHYDASValue *Value);

    bool MHYDAS_GetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, mhydasdk::core::VectorizedMHYDASValue *Value);    
    
    /**
      Gets a distributed property for a spatial object
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] PropName the name of the requested property          
      \param[out] Value the value of the requested property     
    */
    bool MHYDAS_GetDistributedProperty(mhydasdk::core::HydroObject *HO, wxString PropName, mhydasdk::core::PropertyValue *Value);

    /**
      Returns true if a distributed property exists, false otherwise
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] PropName the name of the queried variable               
    */            
    bool MHYDAS_IsDistributedPropertyExists(mhydasdk::core::HydroObject *HO, wxString PropName);    
    
    /**
      Gets an initial condition for a spatial object
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] IniCondName the name of the requested initial condition          
      \param[out] Value the value of the requested initial condition     
    */
    bool MHYDAS_GetDistributedIniCondition(mhydasdk::core::HydroObject *HO, wxString IniCondName, mhydasdk::core::PropertyValue *Value);
  
    /**
      Returns true if a distributed initial condition exists, false otherwise
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] IniCondName the name of the queried variable               
    */        
    bool MHYDAS_IsDistributedIniConditionExists(mhydasdk::core::HydroObject *HO, wxString IniCondName);    
    
    /**
      Gets the rain intensity for a surface unit at a time step
      \param[in] SU the surface unit
      \param[in] Step the time step for rain intensity
      \param[out] Value the value of the requested rain intensity     
    */
    bool MHYDAS_GetDistributedRainValue(mhydasdk::core::SurfaceUnit *SU, int Step, mhydasdk::core::RainValue *Value); 

    /**
      Gets the rain intensity for a reach segment at a time step
      \param[in] RS the reach segment
      \param[in] Step the time step for rain intensity
      \param[out] Value the value of the requested rain intensity     
    */
    bool MHYDAS_GetDistributedRainValue(mhydasdk::core::ReachSegment *RS, int Step, mhydasdk::core::RainValue *Value); 
    
    
    
    /**
      Returns true if a distributed variable exists, false otherwise
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable               
    */
    bool MHYDAS_IsDistributedVarExists(mhydasdk::core::HydroObject *HO, wxString VarName);

    bool MHYDAS_IsDistributedScalarVarExists(mhydasdk::core::HydroObject *HO, wxString VarName);    
    
    bool MHYDAS_IsDistributedVectorVarExists(mhydasdk::core::HydroObject *HO, wxString VarName);
    
    
   /**
      Returns true if a distributed variable exists and if a value has been set for the given step false otherwise
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the variable                     
    */
    bool MHYDAS_IsDistributedVarValueExists(mhydasdk::core::HydroObject *HO, wxString VarName, int Step);
    
    bool MHYDAS_IsDistributedScalarVarValueExists(mhydasdk::core::HydroObject *HO, wxString VarName, int Step);    

    bool MHYDAS_IsDistributedVectorVarValueExists(mhydasdk::core::HydroObject *HO, wxString VarName, int Step);
    
    /**
      Appends a distributed variable value for a spatial object at the end of the previously added values for this variable
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the variable            
      \param[in] Value the added value of the variable      
    */ 
    bool MHYDAS_AppendDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, mhydasdk::core::MHYDASValue Value);

    bool MHYDAS_AppendDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, mhydasdk::core::VectorizedMHYDASValue Value);    

    /**
      Sets a distributed variable value for a spatial object at a time step
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the variable
      \param[in] Step the time step for the value of the variable            
      \param[in] Value the added value of the variable      
    */     
    bool MHYDAS_SetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, mhydasdk::core::MHYDASValue Value);

    bool MHYDAS_SetDistributedVarValue(mhydasdk::core::HydroObject *HO, wxString VarName, int Step, mhydasdk::core::VectorizedMHYDASValue Value);    

    /**
      Gets a function parameter from the parameters set, as a double
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Value the value of the requested parameter
    */ 
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, double *Value);
    
    /**
      Gets a function parameter value from the parameters set, as a long int
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Value the value of the requested parameter
    */ 
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, long *Value);
        
    /**
      Gets a function parameter from the parameters set, as a float
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Value the value of the requested parameter
    */ 
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, float *Value);
    
    /**
      Gets a function parameter value from the parameters set, as an int
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Value the value of the requested parameter
    */ 
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, int *Value);    

    /**
      Gets a function parameter value from the parameters set, as a string
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Value the value of the requested parameter
    */ 
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, wxString *Value);


    
    /**
      Gets a function parameter vector of values from the parameters set, as a vector of strings
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Values the vector of values of the requested parameter
    */     
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, std::vector<wxString> *Values);    

    
    /**
      Gets a function parameter vector of values from the parameters set, as a vector of doubles
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Values the vector of values of the requested parameter
    */     
    
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, std::vector<double> *Values);

    
    /**
      Gets a function parameter vector of values from the parameters set, as a vector of long ints
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter            
      \param[out] Values the vector of values of the requested parameter
    */         
    bool MHYDAS_GetFunctionParam(mhydasdk::core::ParamsMap Params, wxString ParamName, std::vector<long> *Values);    
   
    
    void MHYDAS_RaiseWarning(wxString Sender, int TimeStep, wxString WarningMsg);
    
    void MHYDAS_RaiseWarning(wxString Sender, wxString WarningMsg);

    void MHYDAS_RaiseError(wxString Sender, int TimeStep, wxString WarningMsg);
    
    void MHYDAS_RaiseError(wxString Sender, wxString WarningMsg);

    void MHYDAS_GetEnvironmentInputDir(wxString *Directory);
    
    void MHYDAS_GetEnvironmentOutputDir(wxString *Directory);
    
    
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
    
    bool setFunctionEnvironment(mhydasdk::base::FunctionEnvironment FuncEnv) { m_FunctionEnv = FuncEnv; };

    /**
      Iinitializes function parameters of the function, given as a hash map
    */
    virtual bool initParams(mhydasdk::core::ParamsMap Params)=0;

    /**
      Adds needed data.

    */
    virtual bool prepareData()=0;

    /**
      Checks needed data.
    
    */
    virtual bool checkConsistency()=0;
    
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

