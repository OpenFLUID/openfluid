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
  \file PluggableSimulator.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __PLUGGABLESIMULATOR_HPP__
#define __PLUGGABLESIMULATOR_HPP__


#include <string>
#include <glibmm/threadpool.h>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/base/SchedulingRequest.hpp>
#include <openfluid/base/LoopMacros.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/core/Unit.hpp>
#include <openfluid/core/CoreRepository.hpp>
#include <openfluid/core/Event.hpp>
#include <openfluid/core/EventsColl.hpp>
#include <openfluid/ware/SimulationContributorWare.hpp>


// =====================================================================
// =====================================================================


/**
  Macro for declaration of simulator and signature hooks
*/
#define DECLARE_SIMULATOR_PLUGIN \
  extern "C" \
  { \
    DLLEXPORT std::string GetWareABIVersion(); \
    DLLEXPORT openfluid::ware::PluggableSimulator* GetWareBody(); \
    DLLEXPORT openfluid::ware::SimulatorSignature* GetWareSignature(); \
  }


// =====================================================================
// =====================================================================


/**
  Macro for definition of simulator hook
  @param[in] pluginclassname The name of the class to instantiate
*/
#define DEFINE_SIMULATOR_CLASS(pluginclassname) \
  std::string GetWareABIVersion() \
  { \
    return std::string(openfluid::config::FULL_VERSION); \
  } \
  \
  openfluid::ware::PluggableSimulator* GetWareBody() \
  { \
    return new pluginclassname(); \
  }



// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {


/**
  @brief Abstract class for simulator plugin

  Abstract class for simulator plugin interface,
  defining the minimal structure for a simulator. \n
  All simulators must inherit from this class.

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class DLLEXPORT PluggableSimulator : public SimulationContributorWare
{

  private:

    /**
      Pointer to the core repository, for internal use only
    */
    // TODO check here
   // openfluid::core::CoreRepository* mp_InternalCoreData;


    unsigned int m_MaxThreads;


  protected:

    /**
      Gets a simulator parameter from the parameters set, as a StringValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        openfluid::core::StringValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a DoubleValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        openfluid::core::DoubleValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a VectorValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        openfluid::core::VectorValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a MatrixValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        openfluid::core::MatrixValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a double
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        double& Val) const;

    /**
      Gets a simulator parameter value from the parameters set, as a long int
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        long& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a float
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        float& Val) const;

    /**
      Gets a simulator parameter value from the parameters set, as an int
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        int& Val) const;

    /**
      Gets a simulator parameter value from the parameters set, as a string
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        std::string& Val) const;

    /**
      Gets a simulator parameter vector of values from the parameters set, as a vector of strings
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Vals the vector of values of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        std::vector<std::string>& Vals) const;

    /**
      Gets a simulator parameter vector of values from the parameters set, as a vector of doubles
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Vals the vector of values of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        std::vector<double>& Vals) const;

    /**
      Gets a simulator parameter vector of values from the parameters set, as a vector of long ints
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Vals the vector of values of the requested parameter
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                        const openfluid::ware::WareParamKey_t& ParamName,
                                        std::vector<long>& Vals) const;

    /**
      Returns the maximum number of threads that can be concurrently run in threaded spatial loops
      @return the number of threads
    */
    inline unsigned int OPENFLUID_GetSimulatorMaxThreads() const { return m_MaxThreads; };

    /**
      Sets the maximum number of threads that can be concurrently run in threaded spatial loops
      @param[in] MaxNumThreads the number of threads
    */
    void OPENFLUID_SetSimulatorMaxThreads(const unsigned int& MaxNumThreads);


    inline openfluid::base::SchedulingRequest AtTheEnd() const
    { return openfluid::base::SchedulingRequest(openfluid::base::SchedulingRequest::ATTHEEND); };

    inline openfluid::base::SchedulingRequest Never() const
    { return openfluid::base::SchedulingRequest(openfluid::base::SchedulingRequest::NEVER); };

    inline openfluid::base::SchedulingRequest DefaultDeltaT() const
    { return openfluid::base::SchedulingRequest(OPENFLUID_GetDefaultDeltaT()); };

    inline openfluid::base::SchedulingRequest MultipliedDefaultDeltaT(const double& Mult) const
        { return openfluid::base::SchedulingRequest(Mult * OPENFLUID_GetDefaultDeltaT()); };

    inline openfluid::base::SchedulingRequest Duration(const openfluid::core::Duration_t& D) const
        { return openfluid::base::SchedulingRequest(D); };


  public:
    /**
      Constructor
    */
    PluggableSimulator();

    /**
      Virtual destructor
    */
    virtual ~PluggableSimulator();

    /**
      Internally called by the framework.
    */
    void initializeWare(const WareID_t& SimID,const unsigned int& MaxThreads);

    /**
      Initializes simulator parameters of the simulator, given as a hash map. Internally called by the framework.
    */
    virtual void initParams(const openfluid::ware::WareParams_t& Params)=0;

    /**
      Prepares data. Internally called by the framework.
    */
    virtual void prepareData()=0;

    /**
      Checks the simulator consistency. Internally called by the framework.
    */
    virtual void checkConsistency()=0;

    /**
      Internally called by the framework.
    */
    virtual openfluid::base::SchedulingRequest initializeRun()=0;

    /**
      Internally called by the framework.
    */
    virtual openfluid::base::SchedulingRequest runStep()=0;

    /**
      Internally called by the framework.
    */
    virtual void finalizeRun()=0;

};

typedef PluggableSimulator* (*GetPluggableSimulatorBodyProc)();

typedef SimulatorSignature* (*GetPluggableSimulatorSignatureProc)();

} } // namespaces


#endif /* __PLUGGABLESIMULATOR_HPP__ */
