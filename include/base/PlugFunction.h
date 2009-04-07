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

#include <string>
#include "openfluid-core.h"
#include "SimStatus.h"
#include "ExecMsgs.h"
#include "FunctionEnv.h"
#include "FuncSignature.h"


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
  SDK version hook name
*/
#define PLUGSDKVERSION_PROC_NAME "GetPlugSDKVersion"



/**
  Macro for declaration of function and signature hooks
*/
#define DECLARE_PLUGIN_HOOKS \
  extern "C" \
  { \
    DLLIMPORT std::string GetPlugSDKVersion(); \
    DLLIMPORT openfluid::base::PluggableFunction* GetPlugFunction(); \
    DLLIMPORT openfluid::base::FunctionSignature* GetPlugSignature(); \
  };




// =======================================of==============================
// =====================================================================


/**
  Macro for definition of function hook
  \param[in] pluginclassname The name of the class to instanciate
*/
#define DEFINE_FUNCTION_HOOK(pluginclassname) \
  std::string GetPlugSDKVersion() \
  { \
    return std::string(STRINGIFY(OFELIB_VERSION)); \
  } \
  \
  openfluid::base::PluggableFunction* GetPlugFunction() \
  { \
    return new pluginclassname(); \
  }



// =====================================================================
// =====================================================================


#define DECLARE_UNITS_ORDERED_LOOP(loopid) \
  openfluid::core::UnitsList_t::iterator _M_##loopid##_it;\
  openfluid::core::UnitsList_t* _M_##loopid##_UList;


#define BEGIN_UNITS_ORDERED_LOOP(loopid,unitclass,unit) \
  if (!mp_CoreData->isUnitsClassExist(unitclass)) \
    throw openfluid::base::OFException("ofelib","BEGIN_UNITS_ORDERED_LOOP","Unit class " + std::string(unitclass) + " does not exists"); \
  _M_##loopid##_UList = mp_CoreData->getUnits(unitclass)->getList(); \
  for (_M_##loopid##_it = _M_##loopid##_UList->begin();_M_##loopid##_it != _M_##loopid##_UList->end();++_M_##loopid##_it) \
  { \
    unit = &(*_M_##loopid##_it);




/**
  Macro for declaration of a loop processing SUs, following their process order
*/
#define DECLARE_SU_ORDERED_LOOP \
  openfluid::core::UnitsList_t::iterator _M_SUsOrdIter; \
  openfluid::core::UnitsList_t* _M_SUsOrderedList = mp_CoreData->getUnits("SU")->getList();

/**
  Macro for declaration of a loop processing a list of SUs
*/
#define DECLARE_SU_LIST_LOOP \
  openfluid::core::UnitList_t::iterator _M_SUListIter; \


/**
  Macro for the begining of a loop processing SUs, following their process order
  \param[out] suobj pointer to a openfluid::core::SurfaceUnit object, pointing to the current processed SU
*/
#define BEGIN_SU_ORDERED_LOOP(suobj) \
  for(_M_SUsOrdIter=_M_SUsOrderedList->begin(); _M_SUsOrdIter != _M_SUsOrderedList->end(); _M_SUsOrdIter++) \
  { \
    suobj = &(*_M_SUsOrdIter); \

/**
  Macro for the begining of a loop processing a list of SUs
  \param[out] sulist pointer to a list of openfluid::core::SurfaceUnit
  \param[out] suobj pointer to a openfluid::core::SurfaceUnit object, pointing to the current processed SU
*/
#define BEGIN_SU_LIST_LOOP(sulist,suobj) \
  for(_M_SUListIter=sulist->begin(); _M_SUListIter != sulist->end(); _M_SUListIter++) \
  { \
    suobj = &(*_M_SUListIter); \




/**
  Macro for declaration of a loop processing RSs, following their process order
*/
#define DECLARE_RS_ORDERED_LOOP \
  openfluid::core::UnitsList_t::iterator _M_RSsOrdIter; \
  openfluid::core::UnitsList_t* _M_RSsOrderedList = mp_CoreData->getUnits("RS")->getList();

/**
  Macro for declaration of a loop processing a list of RSs
*/
#define DECLARE_RS_LIST_LOOP \
  std::list<openfluid::core::ReachSegment*>::iterator _M_RSListIter; \


/**
  Macro for the begining of a loop processing RSs, following their process order
  \param[out] rsobj pointer to a openfluid::core::ReachSegment object, pointing to the current processed RS
*/
#define BEGIN_RS_ORDERED_LOOP(rsobj) \
  for(_M_RSsOrdIter=_M_RSsOrderedList->begin(); _M_RSsOrdIter != _M_RSsOrderedList->end(); _M_RSsOrdIter++) \
  { \
    rsobj = &(*_M_RSsOrdIter); \

/**
  Macro for the begining of a loop processing a list of RSs
  \param[out] rslist pointer to a list of openfluid::core::ReachSegment
  \param[out] rsobj pointer to a openfluid::core::ReachSegment object, pointing to the current processed RS
*/
#define BEGIN_RS_LIST_LOOP(rslist,rsobj) \
  for(_M_RSListIter=rslist->begin(); _M_RSListIter != rslist->end(); _M_RSListIter++) \
  { \
    rsobj = &(*_M_RSListIter); \





/**
  Macro for declaration of a loop processing GUs, following their process order
*/
#define DECLARE_GU_ORDERED_LOOP \
  openfluid::core::UnitsList_t::iterator _M_GUsOrdIter; \
  openfluid::core::UnitsList_t* _M_GUsOrderedList = mp_CoreData->getUnits("GU")->getList();

/**
  Macro for declaration of a loop processing a list of GUs
*/
#define DECLARE_GU_LIST_LOOP \
  std::list<openfluid::core::GroundwaterUnit*>::iterator _M_GUListIter; \


/**
  Macro for the begining of a loop processing GUs, following their process order
  \param[out] guobj pointer to a openfluid::core::GroundwaterUnit object, pointing to the current processed GU
*/
#define BEGIN_GU_ORDERED_LOOP(guobj) \
  for(_M_GUsOrdIter=_M_GUsOrderedList->begin(); _M_GUsOrdIter != _M_GUsOrderedList->end(); _M_GUsOrdIter++) \
  { \
    guobj = &(*_M_GUsOrdIter); \

/**
  Macro for the begining of a loop processing a list of GUs
  \param[out] gulist pointer to a list of openfluid::core::GroundwaterUnit
  \param[out] guobj pointer to a openfluid::core::GroundwaterUnit object, pointing to the current processed GU
*/
#define BEGIN_GU_LIST_LOOP(gulist,guobj) \
  for(_M_GUListIter=gulist->begin(); _M_GUListIter != gulist->end(); _M_GUListIter++) \
  { \
    guobj = &(*_M_GUListIter); \



#define DECLARE_EVENT_COLLECTION_LOOP \
  std::list<openfluid::core::DistributedEvent*>::iterator _M_EvListiter;

#define BEGIN_EVENT_COLLECTION_LOOP(evlist,evobj) \
  for(_M_EvListiter=(evlist)->begin(); _M_EvListiter != (evlist)->end(); _M_EvListiter++) \
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
  \brief Abstract class for plugin interface

  Abstract class for plugin interface, defining the minimal structure for a
  simulation function \n
  All simulation functions must inherit from this class.

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class PluggableFunction
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
      Function execution environmentAdd
    */
    const openfluid::base::FunctionEnvironment* mp_FunctionEnv;


    /**
      Function parameters
    */
    openfluid::core::FuncParamsMap_t m_ParamsMap;

  protected:

    // TODO check if const
    /**
      Pointer to the core repository (const). It should be used with care. Prefer to use the OPENFLUID_Xxxx methods.
    */
    openfluid::core::CoreRepository* mp_CoreData;


    /**
      Gets the distributed variable value for a unit at a time step
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the requested variable
      \param[out] Value the value of the requested variable (scalar)
    */
    void OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr,
                               openfluid::core::VariableName_t VarName,
                               openfluid::core::TimeStep_t Step,
                               openfluid::core::ScalarValue *Value);

    /**
      Gets the distributed variable value for a unit at a time step
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the requested variable
      \param[out] Value the value of the requested variable (vector)
    */
    void OPENFLUID_GetVariable(openfluid::core::Unit *UnitPtr,
                               openfluid::core::VariableName_t VarName,
                               openfluid::core::TimeStep_t Step,
                               openfluid::core::VectorValue *Value);

    /**
      Gets a distributed property for a unit
      \param[in] UnitPtr a Unit
      \param[in] PropName the name of the requested property
      \param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr,
                                openfluid::core::InputDataName_t InputName,
                                openfluid::core::ScalarValue *Value);

    /**
      Returns true if a distributed input data exists, false otherwise
      \param[in] UnitPtr a Unit
      \param[in] PropName the name of the queried variable
    */
    bool OPENFLUID_IsInputDataExist(openfluid::core::Unit *UnitPtr,
                                    openfluid::core::InputDataName_t InputName);



    /**
      Returns true if a distributed variable exists, false otherwise (searches both scalar and vector variables)
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr,
                                    openfluid::core::VariableName_t VarName);

    /**
      Returns true if a distributed scalar variable exists, false otherwise
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsScalarVariableExist(openfluid::core::Unit *UnitPtr,
                                          openfluid::core::VariableName_t VarName);

    /**
      Returns true if a distributed vector variable exists, false otherwise
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsVectorVariableExist(openfluid::core::Unit *UnitPtr,
                                          openfluid::core::VariableName_t VarName);


   /**
      Returns true if a distributed variable exists and if a value has been set for the given step, false otherwise (searches both scalar and vector variables)
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the requested variable
      \param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsVariableExist(openfluid::core::Unit *UnitPtr,
                                    openfluid::core::VariableName_t VarName,
                                    openfluid::core::TimeStep_t Step);

    /**
       Returns true if a distributed scalar variable exists and if a value has been set for the given step, false otherwise
       \param[in] UnitPtr a Unit
       \param[in] VarName the name of the requested variable
       \param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsScalarVariableExist(openfluid::core::Unit *UnitPtr,
                                          openfluid::core::VariableName_t VarName,
                                          openfluid::core::TimeStep_t Step);

    /**
       Returns true if a distributed vector variable exists and if a value has been set for the given step, false otherwise
       \param[in] UnitPtr a Unit
       \param[in] VarName the name of the requested variable
       \param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsVectorVariableExist(openfluid::core::Unit *UnitPtr,
                                          openfluid::core::VariableName_t VarName,
                                          openfluid::core::TimeStep_t Step);

    /**
      Appends a distributed scalar variable value for a unit at the end of the previously added values for this variable
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the variable
      \param[in] Value the added value of the variable
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  openfluid::core::VariableName_t VarName,
                                  openfluid::core::ScalarValue Value);

    /**
      Appends a distributed vector variable value for a unit at the end of the previously added values for this variable
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the variable
      \param[in] Value the added value of the variable
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  openfluid::core::VariableName_t VarName,
                                  openfluid::core::VectorValue& Value);

    /**
      Sets a distributed scalar variable value for a unit at a time step
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the variable
      \param[in] Step the time step for the value of the variable
      \param[in] Value the added value of the variable
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               openfluid::core::VariableName_t VarName,
                               openfluid::core::TimeStep_t Step,
                               openfluid::core::ScalarValue Value);

    /**
      Sets a distributed vector variable value for a unit at a time step
      \param[in] UnitPtr a Unit
      \param[in] VarName the name of the variable
      \param[in] Step the time step for the value of the variable
      \param[in] Value the added value of the variable
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               openfluid::core::VariableName_t VarName,
                               openfluid::core::TimeStep_t Step,
                               openfluid::core::VectorValue Value);

    /**
      Gets a function parameter from the parameters set, as a double
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        double *Value);

    /**
      Gets a function parameter value from the parameters set, as a long int
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        long *Value);

    /**
      Gets a function parameter from the parameters set, as a float
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        float *Value);

    /**
      Gets a function parameter value from the parameters set, as an int
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        int *Value);

    /**
      Gets a function parameter value from the parameters set, as a string
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        std::string *Value);

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of strings
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<std::string> *Values);


    /**
      Gets a function parameter vector of values from the parameters set, as a vector of doubles
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<double> *Values);


    /**
      Gets a function parameter vector of values from the parameters set, as a vector of long ints
      \param[in] Params the parameters set for the simulation function
      \param[in] ParamName the name of the requested parameter
      \param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(openfluid::core::FuncParamsMap_t Params,
                                        openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<long> *Values);


    /**
      Gets discrete events happening on a unit during a time period
      \param[in] UnitPtr a Unit
      \param[in] BeginDate the beginning of the time period
      \param[in] EndDate the ending of the time period
      \param[out] EventColl the collection of event corresponding to the request
    */
    void OPENFLUID_GetEvents(openfluid::core::Unit *UnitPtr,
                             openfluid::core::DateTime BeginDate,
                             openfluid::core::DateTime EndDate,
                             openfluid::core::EventCollection* EventColl);


    bool OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName);

    bool OPENFLUID_GetUnitsCount(openfluid::core::UnitClass_t ClassName, unsigned int *UnitsCount);


    /**
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      \param[in] Sender the sender of the message
      \param[in] TimeStep the time step number when the message occurred
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises a warning message to the kernel. This do not stops the simulation
      \param[in] Sender the sender of the message
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, std::string Msg);

    /**
      Raises a time-marked error message to the kernel. This stops the simulation the next time the kernel has the control
      \param[in] Sender the sender of the message
      \param[in] TimeStep the time step number when the message occurred
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      \param[in] Sender the sender of the message
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, std::string Msg);

    /**
      Raises a time-marked error message to the kernel. This stops the simulation the next time the kernel has the control
      \param[in] Sender the sender of the message
      \param[in] Source of the message (location in the sender)
      \param[in] TimeStep the time step number when the message occurred
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      \param[in] Sender the sender of the message
      \param[in] Source of the message (location in the sender)
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, std::string Source, std::string Msg);


    /**
      Gets an environment string value associated to a Key
      \param[in] Key the sender of the message
      \param[out] Value the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, std::string *Value);

    /**
      Gets an environment boolean value associated to a Key
      \param[in] Key the sender of the message
      \param[out] Value the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, bool *Value);


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
    virtual bool initParams(openfluid::core::FuncParamsMap_t Params)=0;

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


typedef PluggableFunction* (*GetPluggableFunctionProc)();

typedef FunctionSignature* (*GetSignatureProc)();

typedef std::string (*GetSDKVersionProc)();



} } // namespace openfluid::base



#endif

