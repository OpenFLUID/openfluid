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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>

  Every simulation function must :
  \li define its own signature
  \li define a class inherited from the PluggableFunction class.

  To be linked to an OpenFLUID application, the simulation function must have two hooks declarations.
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

#include <boost/filesystem/path.hpp>
#include <glibmm.h>


#include <openfluid/dllexport.hpp>
#include <openfluid/config.hpp>
#include <openfluid/core.hpp>
#include <openfluid/base/SimStatus.hpp>
#include <openfluid/base/ExecMsgs.hpp>
#include <openfluid/base/EnvProperties.hpp>
#include <openfluid/base/FuncSignature.hpp>
#include <openfluid/base/StdoutFileOStream.hpp>




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
    DLLEXPORT std::string GetPlugSDKVersion(); \
    DLLEXPORT openfluid::base::PluggableFunction* GetPlugFunction(); \
    DLLEXPORT openfluid::base::FunctionSignature* GetPlugSignature(); \
  }




// =====================================================================
// =====================================================================


/**
  Macro for definition of function hook
  @param[in] pluginclassname The name of the class to instanciate
*/
#define DEFINE_FUNCTION_HOOK(pluginclassname) \
  std::string GetPlugSDKVersion() \
  { \
    return std::string(openfluid::config::FULL_VERSION); \
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
  @param[in] loopid ID of the loop
*/
#define DECLARE_UNITS_ORDERED_LOOP(loopid) \
  openfluid::core::UnitsList_t::iterator _M_##loopid##_ordit;\
  openfluid::core::UnitsList_t* _M_##loopid##_UList;


/**
  Macro for declaration of a loop processing all units, following their process order
  @param[in] loopid ID of the loop
*/
#define DECLARE_GLOBAL_UNITS_ORDERED_LOOP(loopid) \
  openfluid::core::UnitsPtrList_t::iterator _M_##loopid##_ordit;\
  openfluid::core::UnitsPtrList_t* _M_##loopid##_GUList;


/**
  Macro for the beginning of a loop processing all units of a class, following their process order
  @param[in] loopid ID of the loop, must match declaration
  @param[in] unitclass name of the unit class
  @param[out] unit pointer to a openfluid::core::Unit objects, pointing to the current processed SU
*/
#define BEGIN_UNITS_ORDERED_LOOP(loopid,unitclass,unit) \
  _M_##loopid##_UList = mp_CoreData->getUnits(unitclass)->getList(); \
  if (_M_##loopid##_UList != NULL) \
    { \
    for (_M_##loopid##_ordit = _M_##loopid##_UList->begin();_M_##loopid##_ordit != _M_##loopid##_UList->end();++_M_##loopid##_ordit) \
    { \
      unit = &(*_M_##loopid##_ordit);


/**
  Macro for the beginning of a loop processing all units, following their process order
  @param[in] loopid ID of the loop, must match declaration
  @param[out] unit pointer to a openfluid::core::Unit objects, pointing to the current processed SU
*/
#define BEGIN_GLOBAL_UNITS_ORDERED_LOOP(loopid,unit) \
  { \
    _M_##loopid##_GUList = mp_CoreData->getUnitsGlobally(); \
    for (_M_##loopid##_ordit = _M_##loopid##_GUList->begin();_M_##loopid##_ordit != _M_##loopid##_GUList->end();++_M_##loopid##_ordit) \
    { \
      unit = (*_M_##loopid##_ordit);


/**
  Macro for applying a threaded function to each units of a class, following their process order
  @param[in] loopid ID of the loop, must match declaration
  @param[in] unitclass name of the unit class
  @param[in] functptr member function name
  @param[in] ... extra parameters to pass to the member function
*/
#define APPLY_UNITS_ORDERED_LOOP_THREADED(loopid,unitclass,funcptr,...) \
  _M_##loopid##_UList = mp_CoreData->getUnits(unitclass)->getList(); \
  if (_M_##loopid##_UList != NULL) \
  { \
    _M_##loopid##_ordit = _M_##loopid##_UList->begin(); \
    if (_M_##loopid##_ordit != _M_##loopid##_UList->end()) \
    { \
      openfluid::core::PcsOrd_t _M_##loopid##_pcsord = _M_##loopid##_ordit->getProcessOrder(); \
      while (_M_##loopid##_ordit != _M_##loopid##_UList->end()) \
      { \
        Glib::ThreadPool _M_##loopid##_pool(OPENFLUID_GetFunctionMaxThreads(),true); \
        while (_M_##loopid##_ordit != _M_##loopid##_UList->end() && _M_##loopid##_ordit->getProcessOrder() == _M_##loopid##_pcsord) \
        { \
          openfluid::core::Unit* _M_##loopid##_unit = &(*_M_##loopid##_ordit); \
          _M_##loopid##_pool.push(sigc::bind(sigc::mem_fun(*this,&funcptr),_M_##loopid##_unit, ## __VA_ARGS__)); \
          ++_M_##loopid##_ordit; \
        } \
        _M_##loopid##_pool.shutdown(); \
        if (_M_##loopid##_ordit != _M_##loopid##_UList->end()) _M_##loopid##_pcsord = _M_##loopid##_ordit->getProcessOrder(); \
      } \
    } \
  }


/**
  Macro for applying a threaded function to each units of the domain, following their process order
  @param[in] loopid ID of the loop, must match declaration
  @param[in] functptr member function name
  @param[in] ... extra parameters to pass to the member function
*/
#define APPLY_GLOBAL_UNITS_ORDERED_LOOP_THREADED(loopid,funcptr,...) \
  _M_##loopid##_GUList = mp_CoreData->getUnitsGlobally(); \
  if (_M_##loopid##_GUList != NULL) \
  { \
    _M_##loopid##_ordit = _M_##loopid##_GUList->begin(); \
    if (_M_##loopid##_ordit != _M_##loopid##_GUList->end()) \
    { \
      openfluid::core::PcsOrd_t _M_##loopid##_pcsord = (*_M_##loopid##_ordit)->getProcessOrder(); \
      while (_M_##loopid##_ordit != _M_##loopid##_GUList->end()) \
      { \
        Glib::ThreadPool _M_##loopid##_pool(OPENFLUID_GetFunctionMaxThreads(),true); \
        while (_M_##loopid##_ordit != _M_##loopid##_GUList->end() && (*_M_##loopid##_ordit)->getProcessOrder() == _M_##loopid##_pcsord) \
        { \
          openfluid::core::Unit* _M_##loopid##_unit = (*_M_##loopid##_ordit); \
          _M_##loopid##_pool.push(sigc::bind(sigc::mem_fun(*this,&funcptr),_M_##loopid##_unit, ## __VA_ARGS__)); \
          ++_M_##loopid##_ordit; \
        } \
        _M_##loopid##_pool.shutdown(); \
        if (_M_##loopid##_ordit != _M_##loopid##_GUList->end()) _M_##loopid##_pcsord = (*_M_##loopid##_ordit)->getProcessOrder(); \
      } \
    } \
  }



/**
  Macro for declaration of a loop processing a list of units
  @param[in] loopid ID of the loop
*/
#define DECLARE_UNITS_LIST_LOOP(loopid) \
  openfluid::core::UnitsPtrList_t::iterator _M_##loopid##_lstit;\


/**
  Macro for the beginning of a loop processing a list of units
  @param[in] loopid ID of the loop, must match declaration
  @param[in] ulist pointer to a list of openfluid::core::Unit
  @param[out] unit pointer to a openfluid::core::Unit objects, pointing to the current processed SU
*/
#define BEGIN_UNITS_LIST_LOOP(loopid,ulist,unit) \
  if ((ulist) != NULL) \
  { \
    for(_M_##loopid##_lstit=ulist->begin(); _M_##loopid##_lstit != ulist->end(); _M_##loopid##_lstit++) \
    { \
      unit = *_M_##loopid##_lstit; \


/**
  Macro for declaration of a loop processing events in an event collection
*/
#define DECLARE_EVENT_COLLECTION_LOOP \
  openfluid::core::EventsList_t::iterator _M_EvListiter;

/**
  Macro for the beginning of a loop processing an event list from an event collection
  @param[in] evlist pointer to a list of events
  @param[out] evobj the current processed event
*/
#define BEGIN_EVENT_COLLECTION_LOOP(evlist,evobj) \
  { \
    for(_M_EvListiter=(evlist)->begin(); _M_EvListiter != (evlist)->end(); _M_EvListiter++) \
    { \
      evobj = &(*_M_EvListiter);

/**
  Macro for the ending of a loop
*/
#define END_LOOP \
    } \
  }




// =====================================================================
// =====================================================================


namespace openfluid { namespace base {



/**
  @brief Abstract class for plugin interface

  Abstract class for plugin interface, defining the minimal structure for a
  simulation function \n
  All simulation functions must inherit from this class.

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class DLLEXPORT PluggableFunction
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
    const openfluid::base::EnvironmentProperties* mp_FunctionEnv;


    /**
      Function parameters
    */
    openfluid::core::FuncParamsMap_t m_ParamsMap;

    /**
      Function ID
    */
    openfluid::base::FuncID_t m_FunctionID;

    unsigned int m_MaxThreads;

    bool m_Initialized;

    static bool IsUnitIDInPtrList(const openfluid::core::UnitsPtrList_t* UnitsList,
                                  const openfluid::core::UnitID_t& ID);

    static std::string generateDotEdge(std::string SrcClass, std::string SrcID,
                                       std::string DestClass, std::string DestID,
                                       std::string Options);


  protected:

    // TODO check if const
    /**
      Pointer to the core repository (const). It should be used with care. Prefer to use the OPENFLUID_Xxxx methods.
    */
    openfluid::core::CoreRepository* mp_CoreData;

    /**
      Appends a distributed variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] aValue the added value of the variable
      @deprecated
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t VarName,
                                  const openfluid::core::Value& aValue);

    /**
      Appends a distributed variable value for a unit at the end of the previously added values for this variable
      @param[in] aUnit a Unit
      @param[in] VarName the name of the variable
      @param[in] aValue the added value of the variable
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit& aUnit,
                                  const openfluid::core::VariableName_t VarName,
                                  const openfluid::core::Value& aValue);

    /**
      Appends a distributed double variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Value the added value of the variable (double)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t VarName,
                                  const double Value);

    /**
      Appends a distributed long variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Value the added value of the variable (long)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t VarName,
                                  const long Value);

    /**
      Appends a distributed boolean variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Value the added value of the variable (bool)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t VarName,
                                  const bool Value);

    /**
      Appends a distributed string variable value for a unit at the end of the previously added values for this variable
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Value the added value of the variable (string)
    */
    void OPENFLUID_AppendVariable(openfluid::core::Unit *UnitPtr,
                                  const openfluid::core::VariableName_t VarName,
                                  const std::string Value);


    /**
      Sets a distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Step the time step for the value of the variable
      @param[in] aValue the added value of the variable
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               const openfluid::core::Value& aValue);

    /**
      Sets a distributed double variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Step the time step for the value of the variable
      @param[in] Value the added value of the variable (double)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               const double Value);

    /**
      Sets a distributed double variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Step the time step for the value of the variable
      @param[in] Value the added value of the variable (long)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               const long Value);

    /**
      Sets a distributed double variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Step the time step for the value of the variable
      @param[in] Value the added value of the variable (bool)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               const bool Value);

    /**
      Sets a distributed double variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the variable
      @param[in] Step the time step for the value of the variable
      @param[in] Value the added value of the variable (string)
    */
    void OPENFLUID_SetVariable(openfluid::core::Unit *UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               const std::string Value);


    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
      @deprecated
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               openfluid::core::Value* aValue) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               openfluid::core::Value& aValue) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
      @deprecated
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               double* aValue) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               double& aValue) const;
    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
      @deprecated
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               long* Value) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               long& Value) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
      @deprecated
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               bool* Value) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               bool& Value) const;


    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
      @deprecated
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               std::string* Value) const;

    /**
      Gets the distributed variable value for a unit at a time step
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the requested variable
      @param[out] aValue the value of the requested variable
    */
    void OPENFLUID_GetVariable(const openfluid::core::Unit* UnitPtr,
                               const openfluid::core::VariableName_t VarName,
                               const openfluid::core::TimeStep_t Step,
                               std::string& Value) const;

    /**
      Returns true if a distributed variable exists, false otherwise
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
    */
    bool OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t VarName) const;

   /**
      Returns true if a distributed variable exists and if a value has been set for the given step, false otherwise
      @param[in] UnitPtr a Unit
      @param[in] VarName the name of the requested variable
      @param[in] Step the time step for the value of the variable
    */
    bool OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t VarName,
                                   const openfluid::core::TimeStep_t Step) const;

    /**
       Returns true if a distributed variable exists and if a value has been set for the given step,
       and if this value is the given type, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
       @param[in] Step the time step for the value of the variable
       @param[in] ValueType the type of the value
     */
     bool OPENFLUID_IsVariableExist(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::VariableName_t VarName,
                                    const openfluid::core::TimeStep_t Step,
                                    const openfluid::core::Value::Type ValueType) const;

    /**
       Returns true if a distributed variable exists
       and if the type set for this variable is the given type, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
       @param[in] VarType the type of the variable
     */
    bool OPENFLUID_IsTypedVariableExist(const openfluid::core::Unit *UnitPtr,
                                   const openfluid::core::VariableName_t VarName,
                                   const openfluid::core::Value::Type VarType) const;

    /**
       Returns true if a distributed variable exists and if a value has been set for the given step
       and if the type set for this variable is the given type, false otherwise
       @param[in] UnitPtr a Unit
       @param[in] VarName the name of the requested variable
       @param[in] Step the time step for the value of the variable
       @param[in] VarType the type of the variable
     */
     bool OPENFLUID_IsTypedVariableExist(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::VariableName_t VarName,
                                    const openfluid::core::TimeStep_t Step,
                                    const openfluid::core::Value::Type VarType) const;


    /**
       Sets input data for a unit
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[in] Val the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const openfluid::core::Value& Val);

     /**
       Sets input data for a unit, as a double
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[in] Value the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const double& Value);

     /**
       Sets input data for a unit, as a long integer
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[in] Value the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const long& Value);

     /**
       Sets input data for a unit, as a string
       @param[in] UnitPtr a Unit
       @param[in] InputName the name of the set property
       @param[out] Value the value of the set property
     */
     void OPENFLUID_SetInputData(openfluid::core::Unit *UnitPtr,
                                 const openfluid::core::InputDataName_t& InputName,
                                 const std::string& Value);


    /**
      Gets input data for a unit, as a StringValue
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Val the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                openfluid::core::StringValue& Val) const;

    /**
      Gets input data for a unit, as a DoubleValue
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
      @deprecated
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                openfluid::core::DoubleValue* Value) const;

    /**
      Gets input data for a unit, as a DoubleValue
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                openfluid::core::DoubleValue& Value) const;

    /**
      Gets input data for a unit, as a VectorValue
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
      @deprecated
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                openfluid::core::VectorValue* Value) const;

    /**
      Gets input data for a unit, as a VectorValue
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                openfluid::core::VectorValue& Value) const;

    /**
      Gets input data for a unit, as a MatrixValue
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                openfluid::core::MatrixValue& Value) const;

    /**
      Gets input data for a unit, as a double
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
      @deprecated
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                double *Value) const;

    /**
      Gets input data for a unit, as a double
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                double& Value) const;

    /**
      Gets input data for a unit, as a long integer
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
      @deprecated
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                long *Value) const;

    /**
      Gets input data for a unit, as a long integer
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                long& Value) const;

    /**
      Gets input data for a unit, as a string
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
      @deprecated
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                std::string *Value) const;

    /**
      Gets input data for a unit, as a string
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the requested property
      @param[out] Value the value of the requested property
    */
    void OPENFLUID_GetInputData(const openfluid::core::Unit *UnitPtr,
                                const openfluid::core::InputDataName_t InputName,
                                std::string& Value) const;

    /**
      Returns true if a distributed input data exists, false otherwise
      @param[in] UnitPtr a Unit
      @param[in] InputName the name of the queried variable
    */
    bool OPENFLUID_IsInputDataExist(const openfluid::core::Unit *UnitPtr,
                                    const openfluid::core::InputDataName_t InputName) const;


    /**
      Gets a function parameter from the parameters set, as a StringValue
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        openfluid::core::StringValue& Val) const;

    /**
      Gets a function parameter from the parameters set, as a DoubleValue
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        openfluid::core::DoubleValue& Val) const;

    /**
      Gets a function parameter from the parameters set, as a DoubleValue
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        openfluid::core::VectorValue& Val) const;

    /**
      Gets a function parameter from the parameters set, as a MatrixValue
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        openfluid::core::MatrixValue& Val) const;

    /**
      Gets a function parameter from the parameters set, as a double
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        double *Value) const;

    /**
      Gets a function parameter from the parameters set, as a double
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        double& Value) const;

    /**
      Gets a function parameter value from the parameters set, as a long int
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        long *Value) const;

    /**
      Gets a function parameter value from the parameters set, as a long int
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        long& Value) const;

    /**
      Gets a function parameter from the parameters set, as a float
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        float *Value) const;

    /**
      Gets a function parameter from the parameters set, as a float
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        float& Value) const;

    /**
      Gets a function parameter value from the parameters set, as an int
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        int *Value) const;

    /**
      Gets a function parameter value from the parameters set, as an int
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        int& Value) const;

    /**
      Gets a function parameter value from the parameters set, as a string
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::string *Value) const;

    /**
      Gets a function parameter value from the parameters set, as a string
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Value the value of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::string& Value) const;

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of strings
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Values the vector of values of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<std::string> *Values) const;

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of strings
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<std::string>& Values) const;

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of doubles
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Values the vector of values of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<double> *Values) const;

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of doubles
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<double>& Values) const;

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of long ints
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Values the vector of values of the requested parameter
      @deprecated
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<long> *Values) const;

    /**
      Gets a function parameter vector of values from the parameters set, as a vector of long ints
      @param[in] Params the parameters set for the simulation function
      @param[in] ParamName the name of the requested parameter
      @param[out] Values the vector of values of the requested parameter
    */
    bool OPENFLUID_GetFunctionParameter(const openfluid::core::FuncParamsMap_t Params,
                                        const openfluid::core::FuncParamKey_t ParamName,
                                        std::vector<long>& Values) const;


    /**
      Appends an event on a unit
      @param[in] UnitPtr a Unit
      @param[in] Ev the event to append
    */
    void OPENFLUID_AppendEvent(openfluid::core::Unit *UnitPtr,
                               openfluid::core::Event& Ev);


    /**
      Gets discrete events happening on a unit during a time period
      @param[in] UnitPtr a Unit
      @param[in] BeginDate the beginning of the time period
      @param[in] EndDate the ending of the time period
      @param[out] Events the collection of event corresponding to the request
      @deprecated
    */
    void OPENFLUID_GetEvents(const openfluid::core::Unit *UnitPtr,
                             const openfluid::core::DateTime BeginDate,
                             const openfluid::core::DateTime EndDate,
                             openfluid::core::EventsCollection* Events) const;

    /**
      Gets discrete events happening on a unit during a time period
      @param[in] UnitPtr a Unit
      @param[in] BeginDate the beginning of the time period
      @param[in] EndDate the ending of the time period
      @param[out] Events the collection of event corresponding to the request
    */
    void OPENFLUID_GetEvents(const openfluid::core::Unit *UnitPtr,
                             const openfluid::core::DateTime BeginDate,
                             const openfluid::core::DateTime EndDate,
                             openfluid::core::EventsCollection& Events) const;

    /**
      Returns true if the queried unit class exists
      @param[in] ClassName the queried class name
    */
    bool OPENFLUID_IsUnitClassExist(openfluid::core::UnitClass_t ClassName);


    /**
      Returns true if the queried unit exists
      @param[in] ClassName the class of the queried unit
      @param[in] ID the ID of the queried unit
    */
    bool OPENFLUID_IsUnitExist(openfluid::core::UnitClass_t ClassName,
                               openfluid::core::UnitID_t ID);

    /**
      Returns the total number of units
      @param[out] UnitsCount the queried class name
    */
    void OPENFLUID_GetUnitsCount(unsigned int& UnitsCount);

    /**
      Returns the number of units of the class if the queried unit class exists
      @param[in] ClassName the queried class name
      @param[out] UnitsCount the queried class name
      @return false if the unit class does not exist
    */
    bool OPENFLUID_GetUnitsCount(const openfluid::core::UnitClass_t ClassName,
                                 unsigned int& UnitsCount);


    /**
      Returns a pointer to the queried unit if exists
      @param[in] ClassName the queried class name
      @param[in] ID the queried unit ID
      @param[out] UnitPtr a pointer to the requested Unit, NULL if the unit does not exist
      @return false if the unit does not exist
    */
    bool OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                           const openfluid::core::UnitID_t& ID,
                           openfluid::core::Unit* aUnit);

    openfluid::core::Unit* OPENFLUID_GetUnit(const openfluid::core::UnitClass_t& ClassName,
                                             const openfluid::core::UnitID_t& ID);

    /**
      Adds a unit to the set of units if not already exists
      @param[in] ClassName class name of the added unit
      @param[in] ID ID of the added unit
    */
    void OPENFLUID_AddUnit(openfluid::core::UnitClass_t ClassName,
                           openfluid::core::UnitID_t ID,
                           openfluid::core::PcsOrd_t PcsOrder);

    /**
      Deletes a unit from the set of units if exists
      @param[in] ClassName class name of the removed unit
      @param[in] ID ID of the added unit
      @return true if the unit has been correctly deleted
    */
    void OPENFLUID_DeleteUnit(openfluid::core::UnitClass_t ClassName,
                              openfluid::core::UnitID_t ID);


    /**
      Adds a from-to connection between two units
      @param[in] ClassNameFrom class name of the "from" unit
      @param[in] IDFrom ID of the "from" unit
      @param[in] ClassNameFrom class name of the "to" unit
      @param[in] IDFrom ID of the "to" unit
      @return false if the unit connection already exists
    */
    bool OPENFLUID_AddFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                       openfluid::core::UnitID_t IDFrom,
                                       openfluid::core::UnitClass_t ClassNameTo,
                                       openfluid::core::UnitID_t IDTo);

    /**
      Adds a from-to connection between two units
      @param[in] FromUnit pointer to the "from" unit
      @param[in] ToUnit pointer to the "to" unit
      @return false if the connection already exists
    */
    bool OPENFLUID_AddFromToConnection(openfluid::core::Unit* FromUnit,
                                       openfluid::core::Unit* ToUnit);

    /**
      Removes a from-to connection between two units
      @param[in] ClassNameFrom class name of the "from" unit
      @param[in] IDFrom ID of the "from" unit
      @param[in] ClassNameFrom class name of the "to" unit
      @param[in] IDFrom ID of the "to" unit
      @return false if the unit connection does not exist
    */
    bool OPENFLUID_RemoveFromToConnection(openfluid::core::UnitClass_t ClassNameFrom,
                                          openfluid::core::UnitID_t IDFrom,
                                          openfluid::core::UnitClass_t ClassNameTo,
                                          openfluid::core::UnitID_t IDTo);

    /**
      Removes a from-to connection between two units
      @param[in] FromUnit pointer to the "from" unit
      @param[in] ToUnit pointer to the "to" unit
      @return false if the connection does not exist
    */
    bool OPENFLUID_RemoveFromToConnection(openfluid::core::Unit* FromUnit,
                                          openfluid::core::Unit* ToUnit);


    /**
      Adds a child-parent connection between two units
      @param[in] ClassNameFrom class name of the "child" unit
      @param[in] IDFrom ID of the "child" unit
      @param[in] ClassNameFrom class name of the "parent" unit
      @param[in] IDFrom ID of the "parent" unit
      @return false if the connection already exists
    */
    bool OPENFLUID_AddChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                            openfluid::core::UnitID_t IDChild,
                                            openfluid::core::UnitClass_t ClassNameParent,
                                            openfluid::core::UnitID_t IDParent);

    /**
      Adds a child-parent connection between two units
      @param[in] ChildUnit pointer to the "child" unit
      @param[in] ToUnit pointer to the "parent" unit
      @return false if the connection already exists
    */
    bool OPENFLUID_AddChildParentConnection(openfluid::core::Unit* ChildUnit,
                                            openfluid::core::Unit* ParentUnit);


    /**
      Removes a child-parent connection between two units
      @param[in] ClassNameFrom class name of the "child" unit
      @param[in] IDFrom ID of the "child" unit
      @param[in] ClassNameFrom class name of the "parent" unit
      @param[in] IDFrom ID of the "parent" unit
      @return false if the connection does not exist
    */
    bool OPENFLUID_RemoveChildParentConnection(openfluid::core::UnitClass_t ClassNameChild,
                                                openfluid::core::UnitID_t IDChild,
                                                openfluid::core::UnitClass_t ClassNameParent,
                                                openfluid::core::UnitID_t IDParent);

    /**
      Removes a child-parent connection between two units
      @param[in] ChildUnit pointer to the "child" unit
      @param[in] ToUnit pointer to the "parent" unit
      @return false if the connection does not exist
    */
    bool OPENFLUID_RemoveChildParentConnection(openfluid::core::Unit* ChildUnit,
                                                openfluid::core::Unit* ParentUnit);

    /**
      Returns true if a given unit is connected "to" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameTo the class name of the other unit to test
      @param[in] IDTo the ID of the other unit to test
      @return true if the given unit is connected "to" the other unit
    */
    bool OPENFLUID_IsUnitConnectedTo(openfluid::core::Unit* aUnit,
                                     const openfluid::core::UnitClass_t& ClassNameTo,
                                     const openfluid::core::UnitID_t& IDTo);


    /**
      Returns true if a given unit is connected "from" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameFrom the class name of the other unit to test
      @param[in] IDFrom the ID of the other unit to test
      @return true if the given unit is connected "from" the other unit
    */
    bool OPENFLUID_IsUnitConnectedFrom(openfluid::core::Unit* aUnit,
                                       const openfluid::core::UnitClass_t& ClassNameFrom,
                                       const openfluid::core::UnitID_t& IDFrom);


    /**
      Returns true if a given unit is "a child of" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameParent the class name of the other unit to test
      @param[in] IDParent the ID of the other unit to test
      @return true if the given unit is "a child of" the other unit
    */
    bool OPENFLUID_IsUnitChildOf(openfluid::core::Unit* aUnit,
                                 const openfluid::core::UnitClass_t& ClassNameParent,
                                 const openfluid::core::UnitID_t& IDParent);


    /**
      Returns true if a given unit is "parent of" another unit
      @param[in] aUnit the given unit
      @param[in] ClassNameChild the class name of the other unit to test
      @param[in] IDChild the ID of the other unit to test
      @return true if the given unit is "parent of" the other unit
    */
    bool OPENFLUID_IsUnitParentOf(openfluid::core::Unit* aUnit,
                                  const openfluid::core::UnitClass_t& ClassNameChild,
                                  const openfluid::core::UnitID_t& IDChild);


    /**
      Builds a ColsNbr x RowsNbr unix matrix with bi-directionnal connections
      @param[in] UnitsClass the name of units class
      @param[in] ColsNbr the number of units on the X axis
      @param[in] RowsNbr the number of units on the Y axis
    */
    void OPENFLUID_BuildUnitsMatrix(const openfluid::core::UnitClass_t& UnitsClass,
                                    const unsigned int& ColsNbr,
                                    const unsigned int& RowsNbr);


    /**
      Exports the graph of the landscape representation as a graphviz dot file.
      The file is saved in the output directory of the simulation.
      @param[in] Filename the name of the export file
    */
    void OPENFLUID_ExportUnitsGraphAsDotFile(const std::string& Filename);

    /**
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      @param[in] Sender the sender of the message
      @param[in] TimeStep the time step number when the message occurred
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises a warning message to the kernel. This do not stops the simulation
      @param[in] Sender the sender of the message
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, std::string Msg);

    /**
      Raises a time-marked warning message to the kernel. This do not stops the simulation
      @param[in] Sender the sender of the message
      @param[in] Source the source of the message
      @param[in] TimeStep the time step number when the message occurred
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises a warning message to the kernel. This do not stops the simulation
      @param[in] Sender the sender of the message
      @param[in] Source the source of the message
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseWarning(std::string Sender, std::string Source, std::string Msg);


    /**
      Raises a time-marked error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Sender the sender of the message
      @param[in] TimeStep the time step number when the message occurred
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Sender the sender of the message
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, std::string Msg);

    /**
      Raises a time-marked error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Sender the sender of the message
      @param[in] Source of the message (location in the sender)
      @param[in] TimeStep the time step number when the message occurred
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string Msg);

    /**
      Raises an error message to the kernel. This stops the simulation the next time the kernel has the control
      @param[in] Sender the sender of the message
      @param[in] Source of the message (location in the sender)
      @param[in] Msg the content of the message
    */
    void OPENFLUID_RaiseError(std::string Sender, std::string Source, std::string Msg);


    /**
      Gets an environment string value associated to a Key
      @param[in] Key the sender of the message
      @param[out] Value the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, std::string *Value);

    /**
      Gets an environment boolean value associated to a Key
      @param[in] Key the sender of the message
      @param[out] Value the value associated with the environment key
    */
    bool OPENFLUID_GetRunEnvironment(std::string Key, bool *Value);

    /**
      Returns the maximum number of threads that can be concurrently run in threaded spatial loops
      @return the number of threads
    */
    inline unsigned int OPENFLUID_GetFunctionMaxThreads() const { return m_MaxThreads; };

    /**
      Sets the maximum number of threads that can be concurrently run in threaded spatial loops
      @param[in] MaxNumThreads the number of threads
    */
    void OPENFLUID_SetFunctionMaxThreads(const unsigned int& MaxNumThreads);


    StdoutAndFileOutputStream OPENFLUID_Logger;


  public:
    /**
      Constructor
    */
    PluggableFunction();

    /**
      Virtual destructor
    */
    virtual ~PluggableFunction();


    void initializeFunction(openfluid::core::CoreRepository* CoreData,
                            openfluid::base::ExecutionMessages* ExecMsgs,
                            openfluid::base::EnvironmentProperties* FuncEnv,
                            const unsigned int& MaxThreads,
                            const openfluid::base::FuncID_t& FuncID);




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

