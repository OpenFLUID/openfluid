/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file PluggableSimulator.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_WARE_PLUGGABLESIMULATOR_HPP__
#define __OPENFLUID_WARE_PLUGGABLESIMULATOR_HPP__


#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/base/SchedulingRequest.hpp>
#include <openfluid/ware/LoopMacros.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/core/Event.hpp>
#include <openfluid/core/EventsCollection.hpp>
#include <openfluid/core/SpatialGraph.hpp>
#include <openfluid/core/SpatialUnit.hpp>
#include <openfluid/ware/SimulationContributorWare.hpp>


// =====================================================================
// =====================================================================


/**
  @deprecated This declaration is not needed anymore since version 2.1.0
*/
#define DECLARE_SIMULATOR_PLUGIN


/**
  Macro for definition of simulator hook
  @param[in] pluginclassname The name of the class to instantiate
*/
#define DEFINE_SIMULATOR_CLASS(pluginclassname) \
  extern "C" \
  { \
    OPENFLUID_PLUGIN const std::string* WAREABIVERSION_PROC_DECL() \
    { \
      return new std::string(openfluid::config::VERSION_FULL); \
    } \
    \
    OPENFLUID_PLUGIN openfluid::ware::PluggableSimulator* WAREBODY_PROC_DECL() \
    { \
      return new pluginclassname(); \
    } \
  }


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {


/**
  @brief Abstract class for simulator plugin

  Abstract class for simulator plugin interface,
  defining the minimal structure for a simulator. \n
  All simulators must inherit from this class.

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/
class OPENFLUID_API PluggableSimulator : public SimulationContributorWare
{

  private:

    /**
      Pointer to the spatial graph, for internal use only
    */
    // TODO check here
   // openfluid::core::SpatialGraph* mp_InternalSpatialGraph;


    int m_MaxThreads;


  protected:

    /**
      Returns true if the parameter exists
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_IsSimulatorParameterExist(const openfluid::ware::WareParams_t& Params,
                                             const openfluid::ware::WareParamKey_t& ParamName) const;

    /**
      Returns a simulator parameter from the parameters set, as a StringValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @return the value of the requested parameter
    */
   // DIRTYCODE ADD DEPRECATED TAG?
    openfluid::core::StringValue OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                                                 const openfluid::ware::WareParamKey_t& ParamName)
                                                                 const;

    /**
      Gets a simulator parameter from the parameters set, as a StringValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         openfluid::core::StringValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a DoubleValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         openfluid::core::DoubleValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a VectorValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         openfluid::core::VectorValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a MatrixValue
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         openfluid::core::MatrixValue& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a boolean
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         bool& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a double
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         double& Val) const;

    /**
      Gets a simulator parameter value from the parameters set, as a long int
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         long& Val) const;

    /**
      Gets a simulator parameter from the parameters set, as a float
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         float& Val) const;

    /**
      Gets a simulator parameter value from the parameters set, as an int
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         int& Val) const;

    /**
      Gets a simulator parameter value from the parameters set, as a string
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Val the value of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         std::string& Val) const;

    /**
      Gets a simulator parameter vector of values from the parameters set, as a vector of strings
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Vals the vector of values of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         std::vector<std::string>& Vals) const;

    /**
      Gets a simulator parameter vector of values from the parameters set, as a vector of doubles
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Vals the vector of values of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         std::vector<double>& Vals) const;

    /**
      Gets a simulator parameter vector of values from the parameters set, as a vector of long ints
      @param[in] Params the parameters set for the simulator
      @param[in] ParamName the name of the requested parameter
      @param[out] Vals the vector of values of the requested parameter
      @return true if the parameter exists, false otherwise
    */
    bool OPENFLUID_GetSimulatorParameter(const openfluid::ware::WareParams_t& Params,
                                         const openfluid::ware::WareParamKey_t& ParamName,
                                         std::vector<long>& Vals) const;

    /**
      Returns the maximum number of threads that can be concurrently run in threaded spatial loops
      @return the number of threads

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Threading"],
        "title" : "Get maximum number of threads",
        "text" : "OPENFLUID_GetSimulatorMaxThreads()"
      }
      @endcond
    */
    inline int OPENFLUID_GetSimulatorMaxThreads() const
    { 
      return m_MaxThreads;
    }

    /**
      Sets the maximum number of threads that can be concurrently run in threaded spatial loops
      @param[in] MaxNumThreads the number of threads

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Threading"],
        "title" : "Set maximum number of threads",
        "text" : "OPENFLUID_SetSimulatorMaxThreads(%%SEL_START%%Number%%SEL_END%%)"
      }
      @endcond
    */
    void OPENFLUID_SetSimulatorMaxThreads(const int& MaxNumThreads);

    /**
      Returns a scheduling request to a single scheduling at the end
      Return the corresponding scheduling request

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Sheduling"],
        "title" : "Run once at the end",
        "text" : "AtTheEnd()"
      }
      @endcond
    */
    inline openfluid::base::SchedulingRequest AtTheEnd() const
    { 
      return openfluid::base::SchedulingRequest(openfluid::base::SchedulingRequest::ATTHEEND);
    }

    /**
      Returns a scheduling request to never be scheduled again
      Return the corresponding scheduling request

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Sheduling"],
        "title" : "Never run again",
        "text" : "Never()"
      }
      @endcond
    */
    inline openfluid::base::SchedulingRequest Never() const
    { 
      return openfluid::base::SchedulingRequest(openfluid::base::SchedulingRequest::NEVER);
    }

    /**
      Returns a scheduling request for the default DeltaT value
      Return the corresponding scheduling request
    
      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Sheduling"],
        "title" : "Run in default deltaT",
        "text" : "DefaultDeltaT()"
      }
      @endcond
    */
    inline openfluid::base::SchedulingRequest DefaultDeltaT() const
    { 
      return openfluid::base::SchedulingRequest(OPENFLUID_GetDefaultDeltaT());
    }

    /**
      Returns a scheduling request for the default DeltaT value multiplied by a factor
      @param[in] Mult the multiplication factor
      Return the corresponding scheduling request

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Sheduling"],
        "title" : "Run in X default deltaT",
        "text" : "MultipliedDefaultDeltaT(%%SEL_START%%X%%SEL_END%%)"
      }
      @endcond
    */
    inline openfluid::base::SchedulingRequest MultipliedDefaultDeltaT(const double& Mult) const
    { 
      return openfluid::base::SchedulingRequest(Mult * OPENFLUID_GetDefaultDeltaT());
    }

    /**
      Returns a scheduling request for a given duration
      @param[in] D a duration in seconds
      Return the corresponding scheduling request

      @cond OpenFLUID:completion
      {
        "contexts" : ["SIMULATOR"],
        "menupath" : ["Sheduling"],
        "title" : "Run in a duration in seconds",
        "text" : "Duration(%%SEL_START%%Seconds%%SEL_END%%)"
      }
      @endcond
    */
    inline openfluid::base::SchedulingRequest Duration(const openfluid::core::Duration_t& D) const
    { 
      return openfluid::base::SchedulingRequest(D);
    }


  public:
    /**
      Constructor
    */
    PluggableSimulator();

    /**
      Virtual destructor
    */
    virtual ~PluggableSimulator();

    using SimulationContributorWare::initializeWare;

    /**
      Internally called by the framework.
    */
    void initializeWare(const WareID_t& SimID,const unsigned int& MaxThreads);

    /**
      Initializes simulator parameters of the simulator, given as a hash map. Internally called by the framework.
    */
    virtual void initParams(const openfluid::ware::WareParams_t& Params) = 0;

    /**
      Prepares data. Internally called by the framework.
    */
    virtual void prepareData() = 0;

    /**
      Checks the simulator consistency. Internally called by the framework.
    */
    virtual void checkConsistency() = 0;

    /**
      Internally called by the framework.
    */
    virtual openfluid::base::SchedulingRequest initializeRun() = 0;

    /**
      Internally called by the framework.
    */
    virtual openfluid::base::SchedulingRequest runStep() = 0;

    /**
      Internally called by the framework.
    */
    virtual void finalizeRun() = 0;

};


typedef PluggableSimulator* (*GetPluggableSimulatorBodyProc)();

typedef SimulatorSignature* (*GetPluggableSimulatorSignatureProc)();


} } // namespaces


#endif /* __OPENFLUID_WARE_PLUGGABLESIMULATOR_HPP__ */
