/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


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



#ifndef __PLUGFUNCTION_HPP__
#define __PLUGFUNCTION_HPP__

#include <string>
#include <openfluid/core.hpp>
#include <openfluid/base/SimStatus.hpp>
#include <openfluid/base/ExecMsgs.hpp>
#include <openfluid/base/FunctionEnv.hpp>
#include <openfluid/base/FuncSignature.hpp>


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
  }




// =====================================================================
// =====================================================================


/**
  Macro for definition of function hook
  \param[in] pluginclassname The name of the class to instanciate
*/
#define DEFINE_FUNCTION_HOOK(pluginclassname) \
  std::string GetPlugSDKVersion() \
  { \
    return std::string(STRINGIFY(OPENFLUID_VERSION)); \
  } \
  \
  openfluid::base::PluggableFunction* GetPlugFunction() \
  { \
    return new pluginclassname(); \
  }



// =====================================================================
// =====================================================================

/**
  Macro for declaration of a loop processing all units of a class, following their process order
  \param[in] loopid ID of the loop
*/
#define DECLARE_UNITS_ORDERED_LOOP(loopid) \
  openfluid::core::UnitsList_t::iterator _M_##loopid##_ordit;\
  openfluid::core::UnitsList_t* _M_##loopid##_UList;


/**
  Macro for the beginning of a loop processing all units of a class, following their process order
  \param[in] loopid ID of the loop, must match declaration
  \param[in] unitclass name of the unit class
  \param[out] unit pointer to a openfluid::core::Unit objects, pointing to the current processed SU
*/
#define BEGIN_UNITS_ORDERED_LOOP(loopid,unitclass,unit) \
  if (!mp_CoreData->isUnitsClassExist(unitclass)) \
    throw openfluid::base::OFException("ofelib","BEGIN_UNITS_ORDERED_LOOP","Unit class " + std::string(unitclass) + " does not exists"); \
  _M_##loopid##_UList = mp_CoreData->getUnits(unitclass)->getList(); \
  for (_M_##loopid##_ordit = _M_##loopid##_UList->begin();_M_##loopid##_ordit != _M_##loopid##_UList->end();++_M_##loopid##_ordit) \
  { \
    unit = &(*_M_##loopid##_ordit);


/**
  Macro for declaration of a loop processing a list of units
  \param[in] loopid ID of the loop
*/
#define DECLARE_UNITS_LIST_LOOP(loopid) \
  openfluid::core::UnitsPtrList_t::iterator _M_##loopid##_lstit;\


/**
  Macro for the beginning of a loop processing a list of units
  \param[in] loopid ID of the loop, must match declaration
  \param[in] ulist pointer to a list of openfluid::core::Unit
  \param[out] unit pointer to a openfluid::core::Unit objects, pointing to the current processed SU
*/
#define BEGIN_UNITS_LIST_LOOP(loopid,ulist,unit) \
  for(_M_##loopid##_lstit=ulist->begin(); _M_##loopid##_lstit != ulist->end(); _M_##loopid##_lstit++) \
  { \
    unit = *_M_##loopid##_lstit; \


/**
  Macro for declaration of a loop processing events in an event collection
*/
#define DECLARE_EVENT_COLLECTION_LOOP \
  std::list<openfluid::core::Event*>::iterator _M_EvListiter;

/**
  Macro for the beginning of a loop processing an event list from an event collection
  \param[in] evlist pointer to a list of events
  \param[out] evobj the current processed event
*/
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
      Gets input data for a unit, as a double
      \param[in] UnitPtr a Unit
      \param[in] InputName the name of the requested property
      \param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr,
                                openfluid::core::InputDataName_t InputName,
                                double *Value);

    /**
      Gets input data for a unit, as a long integer
      \param[in] UnitPtr a Unit
      \param[in] InputName the name of the requested property
      \param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr,
                                openfluid::core::InputDataName_t InputName,
                                long *Value);

    /**
      Gets input data for a unit, as a string
      \param[in] UnitPtr a Unit
      \param[in] InputName the name of the requested property
      \param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(openfluid::core::Unit *UnitPtr,
                                openfluid::core::InputDataName_t InputName,
                                std::string *Value);


    /**
      Returns true if a distributed input data exists, false otherwise
      \param[in] UnitPtr a Unit
      \param[in] InputName the name of the queried variable
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
      \param[out] Events the collection of event corresponding to the request
    */
    void OPENFLUID_GetEvents(openfluid::core::Unit *UnitPtr,
                             openfluid::core::DateTime BeginDate,
                             openfluid::core::DateTime EndDate,
                             openfluid::core::EventsCollection* Events);


    /**
      Appends an event on a unit
      \param[in] UnitPtr a Unit
      \param[in] Ev the event to append
    */
    void OPENFLUID_AppendEvent(openfluid::core::Unit *UnitPtr,
                               openfluid::core::Event& Ev);



    /**
      Returns true if the queried unit class exists
      \param[in] ClassName the queried class name
    */
    bool OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName);

    /**
      Returns the number of units of the class if the queried unit class exists
      \param[in] ClassName the queried class name
      \param[out] UnitsCount the queried class name
      \return false if the unit class does not exist
    */
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
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      \param[in] Sender the sender of the message
      \param[in] Source the source of the message
      \param[in] TimeStep the time step number when the message occurred
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises a warning message to the kernel. This do not stops the simulation
      \param[in] Sender the sender of the message
      \param[in] Source the source of the message
      \param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, std::string Source, std::string Msg);


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

    void setDataRepository(openfluid::core::CoreRepository* CoreData) { mp_CoreData = CoreData; mp_InternalCoreData = CoreData;};

    void setExecutionMessages(openfluid::base::ExecutionMessages* ExecMsgs) { mp_ExecMsgs = ExecMsgs; };

    void setFunctionEnvironment(openfluid::base::FunctionEnvironment* FuncEnv) { mp_FunctionEnv = FuncEnv; };


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

