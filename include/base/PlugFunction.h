/**
  \file PlugFunction.h
  \brief Header of generic pluggable function interface definition

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>

  Every simulation function must :
  \li define its own signature
  \li define a class inherited from the PluggableFunction class.

  To be linked to the OpenFLUID-engine kernel, the simulation function must have two hooks declarations.
  \code
  extern "C"
  {
    DLLIMPORT PluggableFunction* GetPlugFunction();
    DLLIMPORT PluggableFunction* GetPlugSignature();
  };
  \endcode
  returning the signature and  an instance of the derived class inherited from the PluggableFunction class.
*/



#ifndef __PLUGFUNCTION_H__
#define __PLUGFUNCTION_H__

#include <wx/wx.h>
#include "openfluid-core.h"
#include "SimStatus.h"
#include "ExecMsgs.h"
#include "FunctionEnv.h"
#include <string>

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


#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x

// =====================================================================
// =====================================================================

/**
  Function hook name
*/
#define PLUGFUNCTION_PROC_NAME "GetPlugFunction"

/**
  Signature hook name
*/
#define PLUGSIGNATURE_PROC_NAME "GetPlugSignature"


/**
  Macro for declaration of function and signature hooks
*/
#define DECLARE_PLUGIN_HOOKS \
  extern "C" \
  { \
    DLLIMPORT openfluid::base::PluggableFunction* GetPlugFunction(); \
    DLLIMPORT openfluid::base::Signature* GetPlugSignature(); \
  };



// =======================================of==============================
// =====================================================================


/**
  Macro for definition of function hook
  \param[in] pluginclassname The name of the class to instanciate
*/

#define DEFINE_FUNCTION_HOOK(pluginclassname) \
  openfluid::base::PluggableFunction* GetPlugFunction() \
  { \
    return new pluginclassname(); \
  }


// =====================================================================
// =====================================================================

/**
  Macro for the beginning of definition of signature hook
*/
#define BEGIN_SIGNATURE_HOOK \
  openfluid::base::Signature* GetPlugSignature() \
  { \
    openfluid::base::Signature* ZeSignature = new openfluid::base::Signature(); \
    ZeSignature->setSDKVersion(wxT(STRINGIFY(OFELIB_VERSION)));


/**
  Macro for the end of definition of signature hook
*/
#define END_SIGNATURE_HOOK \
    return ZeSignature; \
  }


// =====================================================================
// =====================================================================


/**
  Macro for declaration of the function ID
*/
#define DECLARE_SIGNATURE_ID(id) ZeSignature->ID = id;

/**
  Macro for declaration of the function name
*/
#define DECLARE_SIGNATURE_NAME(name) ZeSignature->Name = name;

/**
  Macro for declaration of the function description
*/
#define DECLARE_SIGNATURE_DESCRIPTION(desc) ZeSignature->Description = desc;

/**
  Macro for declaration of the function application domain
*/
#define DECLARE_SIGNATURE_DOMAIN(domain) ZeSignature->Domain = domain;

/**
  Macro for declaration of the function application processes
*/
#define DECLARE_SIGNATURE_PROCESS(process) ZeSignature->Process = process;

/**
  Macro for declaration of the function application numerical method
*/
#define DECLARE_SIGNATURE_METHOD(method) ZeSignature->Method = method;

/**
  Macro for declaration of the function author name
*/
#define DECLARE_SIGNATURE_AUTHORNAME(name) ZeSignature->Author = name;

/**
  Macro for declaration of the function author email
*/
#define DECLARE_SIGNATURE_AUTHOREMAIL(email) ZeSignature->AuthorEmail = email;

/**
  Macro for declaration of the function version
*/
#define DECLARE_SIGNATURE_VERSION(version) ZeSignature->Version = version;

/**
  Macro for declaration of the function status
*/
#define DECLARE_SIGNATURE_STATUS(status) ZeSignature->Status = status;

/**
  Macro for declaration of SDK version used to build the function
*/
#define DECLARE_SIGNATURE_SDKVERSION ZeSignature->setSDKVersion(wxT(STRINGIFY(OFELIB_VERSION)));

// =====================================================================
// =====================================================================


/**
  Macro for declaration of a function parameter
  \param[in] name name of the parameter
  \param[in] description description of the parameter
  \param[in] unit unit of the parameter. Could be an empty string if there is no unit
*/
#define DECLARE_FUNCTION_PARAM(name,description,unit) \
  ZeSignature->HandledData.FunctionParams.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT(""),description,unit));


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_SU_PRODUCED_VAR(name,description,unit) \
  ZeSignature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of an updated variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_SU_UPDATED_VAR(name,description,unit) \
  ZeSignature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of a required variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_SU_REQUIRED_VAR(name,description,unit) \
  ZeSignature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));



/**
  Macro for declaration of a required variable on all SUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_SU_REQUIRED_PREVVAR(name,description,unit) \
  ZeSignature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of an used variable on all SUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_SU_USED_VAR(name,description,unit) \
  ZeSignature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));


/**
  Macro for declaration of an used variable on all SUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_SU_USED_PREVVAR(name,description,unit) \
  ZeSignature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of a required property on all SUs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_SU_REQUIRED_PROPERTY(name,description,unit) \
  ZeSignature->HandledData.RequiredProps.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of a used property on all SUs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_SU_USED_PROPERTY(name,description,unit) \
  ZeSignature->HandledData.UsedProps.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));


/**
  Macro for declaration of a required initial condition on all SUs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit
*/
#define DECLARE_SU_REQUIRED_INICOND(name,description,unit) \
  ZeSignature->HandledData.RequiredIniconds.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));

/**
  Macro for declaration of a used initial condition on all SUs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit
*/
#define DECLARE_SU_USED_INICOND(name,description,unit) \
  ZeSignature->HandledData.UsedIniconds.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("SU"),description,unit));



// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_RS_PRODUCED_VAR(name,description,unit) \
  ZeSignature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

/**
  Macro for declaration of an updated variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_RS_UPDATED_VAR(name,description,unit) \
  ZeSignature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

/**
  Macro for declaration of a required variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_RS_REQUIRED_VAR(name,description,unit) \
  ZeSignature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

#define DECLARE_RS_REQUIRED_PREVVAR(name,description,unit) \
  ZeSignature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));


/**
  Macro for declaration of an used variable on all RSs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_RS_USED_VAR(name,description,unit) \
  ZeSignature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

#define DECLARE_RS_USED_PREVVAR(name,description,unit) \
  ZeSignature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));


/**
  Macro for declaration of a required property on all RSs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_RS_REQUIRED_PROPERTY(name,description,unit) \
  ZeSignature->HandledData.RequiredProps.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

#define DECLARE_RS_USED_PROPERTY(name,description,unit) \
  ZeSignature->HandledData.UsedProps.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

/**
  Macro for declaration of a required initial condition on all RSs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit
*/
#define DECLARE_RS_REQUIRED_INICOND(name,description,unit) \
  ZeSignature->HandledData.RequiredIniconds.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

#define DECLARE_RS_USED_INICOND(name,description,unit) \
  ZeSignature->HandledData.UsedIniconds.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("RS"),description,unit));

// =====================================================================
// =====================================================================


/**
  Macro for declaration of a produced variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_GU_PRODUCED_VAR(name,description,unit) \
  ZeSignature->HandledData.ProducedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

/**
  Macro for declaration of an updated variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_GU_UPDATED_VAR(name,description,unit) \
  ZeSignature->HandledData.UpdatedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

/**
  Macro for declaration of a required variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_GU_REQUIRED_VAR(name,description,unit) \
  ZeSignature->HandledData.RequiredVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

/**
  Macro for declaration of a required variable on all GUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_GU_REQUIRED_PREVVAR(name,description,unit) \
  ZeSignature->HandledData.RequiredPrevVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));


/**
  Macro for declaration of an used variable on all GUs
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_GU_USED_VAR(name,description,unit) \
  ZeSignature->HandledData.UsedVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

/**
  Macro for declaration of an used variable on all GUs, using a value produced on a previous time step
  \param[in] name name of the variable
  \param[in] description description of the variable
  \param[in] unit unit of the variable. Could be an empty string if there is no unit
*/
#define DECLARE_GU_USED_PREVVAR(name,description,unit) \
  ZeSignature->HandledData.UsedPrevVars.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));


/**
  Macro for declaration of a required property on all GUs
  \param[in] name name of the property
  \param[in] description description of the property
  \param[in] unit unit of the property. Could be an empty string if there is no unit
*/
#define DECLARE_GU_REQUIRED_PROPERTY(name,description,unit) \
  ZeSignature->HandledData.RequiredProps.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

#define DECLARE_GU_USED_PROPERTY(name,description,unit) \
  ZeSignature->HandledData.UsedProps.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));


/**
  Macro for declaration of a required initial condition on all GUs
  \param[in] name name of the initial condition
  \param[in] description description of the initial condition
  \param[in] unit unit of the initial condition. Could be an empty string if there is no unit
*/
#define DECLARE_GU_REQUIRED_INICOND(name,description,unit) \
  ZeSignature->HandledData.RequiredIniconds.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));

#define DECLARE_GU_USED_INICOND(name,description,unit) \
  ZeSignature->HandledData.UsedIniconds.push_back(openfluid::base::SignatureHandledItem(wxT(name),wxT("GU"),description,unit));


// =====================================================================
// =====================================================================

#define DECLARE_REQUIRED_EXTRAFILE(name) \
  ZeSignature->HandledData.RequiredExtraFiles.Add(name);


#define DECLARE_USED_EXTRAFILE(name) \
  ZeSignature->HandledData.UsedExtraFiles.Add(name);


// =====================================================================
// =====================================================================


#define DECLARE_USED_SU_EVENTS ZeSignature->HandledData.UsedEventsOnSU = true;

#define DECLARE_USED_RS_EVENTS ZeSignature->HandledData.UsedEventsOnRS = true;

#define DECLARE_USED_GU_EVENTS ZeSignature->HandledData.UsedEventsOnGU = true;


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a loop processing SUs, following their process order
*/
#define DECLARE_SU_ORDERED_LOOP \
  std::list<openfluid::core::SurfaceUnit*>::iterator _M_SUiter; \
  std::list<openfluid::core::SurfaceUnit*>* _M_SUsList = mp_CoreData->getSpatialData()->getSUsOrderedList();

/**
  Macro for declaration of a loop processing a list of SUs
*/
#define DECLARE_SU_LIST_LOOP \
  std::list<openfluid::core::SurfaceUnit*>::iterator _M_SUListiter; \


/**
  Macro for the begining of a loop processing SUs, following their process order
  \param[out] suobj pointer to a openfluid::core::SurfaceUnit object, pointing to the current processed SU
*/
#define BEGIN_SU_ORDERED_LOOP(suobj) \
  for(_M_SUiter=_M_SUsList->begin(); _M_SUiter != _M_SUsList->end(); _M_SUiter++) \
  { \
    suobj = *_M_SUiter; \

/**
  Macro for the begining of a loop processing a list of SUs
  \param[out] sulist pointer to a list of openfluid::core::SurfaceUnit
  \param[out] suobj pointer to a openfluid::core::SurfaceUnit object, pointing to the current processed SU
*/
#define BEGIN_SU_LIST_LOOP(sulist,suobj) \
  for(_M_SUListiter=sulist->begin(); _M_SUListiter != sulist->end(); _M_SUListiter++) \
  { \
    suobj = *_M_SUListiter; \


/**
  Macro for declaration of a loop processing RSs, following their process order
*/
#define DECLARE_RS_ORDERED_LOOP \
  std::list<openfluid::core::ReachSegment*>::iterator _M_RSiter; \
  std::list<openfluid::core::ReachSegment*>* _M_RSsList = mp_CoreData->getSpatialData()->getRSsOrderedList();

/**
  Macro for declaration of a loop processing a list of RSs
*/
#define DECLARE_RS_LIST_LOOP \
  std::list<openfluid::core::ReachSegment*>::iterator _M_RSListiter; \


/**
  Macro for the begining of a loop processing RSs, following their process order
  \param[out] rsobj pointer to a openfluid::core::ReachSegment object, pointing to the current processed RS
*/
#define BEGIN_RS_ORDERED_LOOP(rsobj) \
  for(_M_RSiter=_M_RSsList->begin(); _M_RSiter != _M_RSsList->end(); _M_RSiter++) \
  { \
    rsobj = *_M_RSiter; \

/**
  Macro for the begining of a loop processing a list of RSs
  \param[out] rslist pointer to a list of openfluid::core::ReachSegment
  \param[out] rsobj pointer to a openfluid::core::ReachSegment object, pointing to the current processed RS
*/
#define BEGIN_RS_LIST_LOOP(rslist,rsobj) \
  for(_M_RSListiter=rslist->begin(); _M_RSListiter != rslist->end(); _M_RSListiter++) \
  { \
    rsobj = *_M_RSListiter; \



/**
  Macro for declaration of a loop processing GUs, following their process order
*/
#define DECLARE_GU_ORDERED_LOOP \
  std::list<openfluid::core::GroundwaterUnit*>::iterator _M_GUiter; \
  std::list<openfluid::core::GroundwaterUnit*>* _M_GUsList = mp_CoreData->getSpatialData()->getGUsOrderedList();

/**
  Macro for declaration of a loop processing a list of GUs
*/
#define DECLARE_GU_LIST_LOOP \
  std::list<openfluid::core::GroundwaterUnit*>::iterator _M_GUListiter; \


/**
  Macro for the begining of a loop processing GUs, following their process order
  \param[out] guobj pointer to a openfluid::core::GroundwaterUnit object, pointing to the current processed GU
*/
#define BEGIN_GU_ORDERED_LOOP(guobj) \
  for(_M_GUiter=_M_GUsList->begin(); _M_GUiter != _M_GUsList->end(); _M_GUiter++) \
  { \
    guobj = *_M_GUiter; \

/**
  Macro for the begining of a loop processing a list of GUs
  \param[out] gulist pointer to a list of openfluid::core::GroundwaterUnit
  \param[out] guobj pointer to a openfluid::core::GroundwaterUnit object, pointing to the current processed GU
*/
#define BEGIN_GU_LIST_LOOP(gulist,guobj) \
  for(_M_GUListiter=gulist->begin(); _M_GUListiter != gulist->end(); _M_GUListiter++) \
  { \
    guobj = *_M_GUListiter; \


#define DECLARE_EVENT_COLLECTION_LOOP \
  std::list<openfluid::core::DistributedEvent*>::iterator _M_EvListiter;

#define BEGIN_EVENT_COLLECTION_LOOP(evlist,evobj) \
  for(_M_EvListiter=evlist->begin(); _M_EvListiter != evlist->end(); _M_EvListiter++) \
  { \
    evobj = *_M_EvListiter;

/**
  Macro for the ending of a loop
*/
#define END_LOOP }

// =====================================================================
// =====================================================================


namespace openfluid { namespace base {


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

/**
  Function status
*/
enum FunctionStatusList
{
  /**
    Experimental status, for testing only
  */
  EXPERIMENTAL,

  /**
    Beta status, on the road to a stable status
  */
  BETA,

  /**
    Stable status
  */
  STABLE
};


// =====================================================================
// =====================================================================
/**
  Structure for storage of the definition an item handled by the function.
*/
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


/**
  \if DOCFORDEV
  Structure for storage of the definition of the data handled by the function. This is part of the signature.
  \endif
*/
struct SignatureHandledData
{
  /*

  */
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

  bool UsedEventsOnSU;

  bool UsedEventsOnRS;

  bool UsedEventsOnGU;

  SignatureHandledData()
  {
    UsedEventsOnSU = false;
    UsedEventsOnRS = false;
    UsedEventsOnGU = false;
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
    Plugin identitypluginclassname
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
    Development status
  */
  FunctionStatusList Status;

  /**
    SDK version number used to build the function
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

  /**
    Handled data
  */
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
    Status = EXPERIMENTAL;
    SDKVersion = wxT("");
  }

  void setSDKVersion(wxString Version)
  {
    SDKVersion = Version;
  }

};
/*
  \endif
*/



// =====================================================================
// =====================================================================


// =====================================================================
// =====================================================================


/**
  \brief Abstract class for plugin interface

  Abstract class for plugin interface, defining the minimal structure for a
  simulation function \n
  All simulation functions must inherit from this class.

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class PluggableFunction : public wxObject
{
  private:

    /**
      Pointer to the core repository, for internal use only
    */
    openfluid::core::CoreRepository* mp_InternalCoreData;


    /**
      Pointer to the execution messages repository
    */
    openfluid::base::ExecutionMessages* mp_ExecMsgs;


    /**
      Function execution environment
    */
    const openfluid::base::FunctionEnvironment* mp_FunctionEnv;


    /**
      Function parameters
    */
    openfluid::core::ParamsMap m_ParamsMap;

  protected:

    /**
      Pointer to the core repository (const). It should be used with care. Prefer to use the OPENFLUID_Xxxx methods.
    */
    const openfluid::core::CoreRepository* mp_CoreData;


    /**
      Gets the distributed variable value for a unit at a time step
      \param[in] HO the spatial object, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the requested variable
      \param[out] Value the value of the requested variable (scalar)
    */
    bool OPENFLUID_GetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::ScalarValue *Value);

    /**
      Gets the distributed variable value for a unit at a time step
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the requested variable
      \param[out] Value the value of the requested variable (vector)
    */
    bool OPENFLUID_GetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::VectorValue*& Value);

    /**
      Gets a distributed property for a unit
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] PropName the name of the requested property
      \param[out] Value the value of the requested property
    */
    bool OPENFLUID_GetProperty(openfluid::core::HydroObject *HO, wxString PropName, openfluid::core::ScalarValue *Value);

    /**
      Returns true if a distributed property exists, false otherwise
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] PropName the name of the queried variable
    */
    bool OPENFLUID_IsPropertyExists(openfluid::core::HydroObject *HO, wxString PropName);

    /**
      Sets a distributed property for a unit
      \param[in] HO the unit
      \param[in] PropName the name of the property to modify
      \param[in] Value the new value of the property
    */
    bool OPENFLUID_SetProperty(openfluid::core::HydroObject *HO, wxString PropName, openfluid::core::ScalarValue Value);



    /**
      Gets an initial condition for a unit
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] IniCondName the name of the requested initial condition
      \param[out] Value the value of the requested initial condition
    */
    bool OPENFLUID_GetIniCondition(openfluid::core::HydroObject *HO, wxString IniCondName, openfluid::core::ScalarValue *Value);

    /**
      Returns true if a distributed initial condition exists, false otherwise
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] IniCondName the name of the queried variable
    */
    bool OPENFLUID_IsIniConditionExists(openfluid::core::HydroObject *HO, wxString IniCondName);


    /**
      Returns true if a distributed variable exists, false otherwise (searches both scalar and vector variables)
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsVariableExists(openfluid::core::HydroObject *HO, wxString VarName);

    /**
      Returns true if a distributed scalar variable exists, false otherwise
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsScalarVariableExists(openfluid::core::HydroObject *HO, wxString VarName);

    /**
      Returns true if a distributed vector variable exists, false otherwise
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsVectorVariableExists(openfluid::core::HydroObject *HO, wxString VarName);


   /**
      Returns true if a distributed variable exists and if a value has been set for the given step, false otherwise (searches both scalar and vector variables)
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsVariableExists(openfluid::core::HydroObject *HO, wxString VarName, int Step);

    /**
       Returns true if a distributed scalar variable exists and if a value has been set for the given step, false otherwise
       \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
       \param[in] VarName the name of the requested variable
       \param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsScalarVariableExists(openfluid::core::HydroObject *HO, wxString VarName, int Step);

    /**
       Returns true if a distributed vector variable exists and if a value has been set for the given step, false otherwise
       \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
       \param[in] VarName the name of the requested variable
       \param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsVectorVariableExists(openfluid::core::HydroObject *HO, wxString VarName, int Step);

    /**
      Appends a distributed scalar variable value for a unit at the end of the previously added values for this variable
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the variable
      \param[in] Value the added value of the variable
    */
    bool OPENFLUID_AppendVariable(openfluid::core::HydroObject *HO, wxString VarName, openfluid::core::ScalarValue Value);

    /**
      Appends a distributed vector variable value for a unit at the end of the previously added values for this variable
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the variable
      \param[in] Value the added value of the variable
    */
    bool OPENFLUID_AppendVariable(openfluid::core::HydroObject *HO, wxString VarName, openfluid::core::VectorValue& Value);

    /**
      Sets a distributed scalar variable value for a unit at a time step
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the variable
      \param[in] Step the time step for the value of the variable
      \param[in] Value the added value of the variable
    */
    bool OPENFLUID_SetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::ScalarValue Value);

    /**
      Sets a distributed vector variable value for a unit at a time step
      \param[in] HO the unit, can be SurfaceUnit, ReachSegment or GroundwaterUnit
      \param[in] VarName the name of the variable
      \param[in] Step the time step for the value of the variable
      \param[in] Value the added value of the variable
    */
    bool OPENFLUID_SetVariable(openfluid::core::HydroObject *HO, wxString VarName, int Step, openfluid::core::VectorValue Value);

    /**
      Gets a function parameter from the parameters set, as a double
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, double *Value);

    /**
      Gets a function parameter value from the parameters set, as a long int
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, long *Value);

    /**
      Gets a function parameter from the parameters set, as a float
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, float *Value);

    /**
      Gets a function parameter value from the parameters set, as an int
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, int *Value);

    /**
      Gets a function parameter value from the parameters set, as a string
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, wxString *Value);

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of strings
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<wxString> *Values);


    /**
      Gets a function parameter vector of values from the parameters set, as a vector of doubles
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<double> *Values);


    /**
      Gets a function parameter vector of values from the parameters set, as a vector of long ints
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::ParamsMap Params, wxString ParamName, std::vector<long> *Values);


    /**
      Gets discrete events happening on a unit during a time period
      \param[in] HO the unit
      \param[in] BeginDate the beginning of the time period
      \param[in] EndDate the ending of the time period
      \param[out] EventColl the collection of event corresponding to the request
    */
    bool OPENFLUID_GetEvents(openfluid::core::HydroObject *HO, openfluid::core::DateTime BeginDate, openfluid::core::DateTime EndDate, openfluid::core::EventCollection* EventColl);


    /**
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      \param[in] Sender the sender of the message
      \param[in] TimeStep the time step number when the message occurred
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(wxString Sender, int TimeStep, wxString Msg);

    /**
      Raises a warning message to the kernel. This do not stops the simulation
      \param[in] Sender the sender of the message
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(wxString Sender, wxString Msg);

    /**
      Raises a time-marked error message to the kernel. This stops the simulation the next time the kernel has the control
      \param[in] Sender the sender of the message
      \param[in] TimeStep the time step number when the message occurred
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(wxString Sender, int TimeStep, wxString Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      \param[in] Sender the sender of the message
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(wxString Sender, wxString Msg);

    /**
      Gets an environment string value associated to a Key
      \param[in] Key the sender of the message
      \param[out] Value the value associated with the environment key
    */
    void OPENFLUID_GetRunEnvironment(wxString Key, wxString *Value);

    /**
      Gets an environment boolean value associated to a Key
      \param[in] Key the sender of the message
      \param[out] Value the value associated with the environment key
    */
    void OPENFLUID_GetRunEnvironment(wxString Key, bool *Value);


  public:
    /**
      Constructor
    */
    PluggableFunction();

    /**
      Virtual destructor
    */
    virtual ~PluggableFunction();

    bool setDataRepository(openfluid::core::CoreRepository* CoreData) { mp_CoreData = CoreData; mp_InternalCoreData = CoreData; };

    bool setExecutionMessages(openfluid::base::ExecutionMessages* ExecMsgs) { mp_ExecMsgs = ExecMsgs; };

    bool setFunctionEnvironment(openfluid::base::FunctionEnvironment* FuncEnv) { mp_FunctionEnv = FuncEnv; };


    /**
      Initializes function parameters of the function, given as a hash map. Called by the kernel.
    */
    virtual bool initParams(openfluid::core::ParamsMap Params)=0;

    /**
      Prepares data. Called by the kernel.
    */
    virtual bool prepareData()=0;

    /**
      Checks the function consistency. Called by the kernel.
    */
    virtual bool checkConsistency()=0;


    /**
      Called by the kernel.
    */
    virtual bool initializeRun(const SimulationInfo* SimInfo)=0;

    /**
      Called by the kernel.
    */
    virtual bool runStep(const SimulationStatus* SimStatus)=0;

    /**
      Called by the kernel.
    */
    virtual bool finalizeRun(const SimulationInfo* SimInfo)=0;

};


// =====================================================================
// =====================================================================

typedef PluggableFunction*(*GetPluggableFunctionProc)();

typedef Signature*(*GetSignatureProc)();


} } // namespace openfluid::base



#endif

