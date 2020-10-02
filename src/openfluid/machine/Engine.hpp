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
  @file Engine.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_ENGINE_HPP__
#define __OPENFLUID_MACHINE_ENGINE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/DateTime.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/base/SimulationLogger.hpp>


namespace openfluid {
namespace base {
class SimulationStatus;
}
namespace core {
class Value;
class DateTime;
}
}


namespace openfluid { namespace machine {

class ModelInstance;
class MonitoringInstance;
class MachineListener;
class SimulationBlob;


// =====================================================================
// =====================================================================


/**
  Computation engine class
*/
class OPENFLUID_API Engine
{

  private:

     SimulationBlob& m_SimulationBlob;

     openfluid::base::SimulationStatus* mp_SimStatus;

     MachineListener* mp_MachineListener;

     ModelInstance& m_ModelInstance;

     MonitoringInstance& m_MonitoringInstance;

     openfluid::base::SimulationLogger* mp_SimLogger;


     void checkSimulationVarsProduction(int ExpectedVarsCount);

     void checkParametersConsistency();

     void checkModelConsistency();

     void checkAttributesConsistency();

     void checkExtraFilesConsistency();

     void checkExistingVariable(const openfluid::core::VariableName_t& VarName,
                                const openfluid::core::Value::Type& VarType,
                                const openfluid::core::UnitsClass_t& ClassName,
                                const std::string& SimulatorID);

     void createVariable(const openfluid::core::VariableName_t& VarName,
                         const openfluid::core::Value::Type& VarType,
                         const openfluid::core::UnitsClass_t& ClassName,
                         bool UpdateMode,
                         const std::string& SimulatorID);

     void checkExistingAttribute(const openfluid::core::AttributeName_t AttrName,
                                 const openfluid::core::UnitsClass_t ClassName,
                                 const std::string& SimulatorID);

     void createAttribute(const openfluid::core::AttributeName_t AttrName,
                          const openfluid::core::UnitsClass_t ClassName,
                          const std::string& SimulatorID);

     void prepareOutputDir();


  public:
    
    Engine() = delete;
    
    static std::size_t computeValuesBuffersDefaultSize(const openfluid::core::Duration_t Duration, 
                                                       const openfluid::core::Duration_t DeltaT)
    {
        return (Duration/DeltaT)+2;
    }
    
    /**
      Constructor
    */
    Engine(SimulationBlob& SimBlob,
           ModelInstance& MInstance, MonitoringInstance& OLInstance,
           openfluid::machine::MachineListener* MachineListener);

    /**
      Destructor
    */
    ~Engine();

    /**
      Initializes the simulation engine
    */
    void initialize();

    /**
      Executes the initParams() method of each item of the model
    */
    void initParams();

    /**
      Executes the prepareData() method of each item of the model
    */
    void prepareData();

    /**
      Executes the checkConsistency() method of each item of the model
    */
    void checkConsistency();

    /**
      Executes the initializeRun(), runStep() and finalizeRun() methods of each item of the model
    */
    void run();

    /**
      Finalizes the simulation engine
    */
    void finalize();

    const openfluid::base::SimulationStatus* simulationStatus()
    {
      return mp_SimStatus;
    }

    SimulationBlob* simulationBlob()
    {
      return &m_SimulationBlob;
    }

    MachineListener* machineListener()
    {
      return mp_MachineListener;
    }

    ModelInstance* modelInstance()
    {
      return &m_ModelInstance;
    }

    unsigned int getWarningsCount() const
    {
      return mp_SimLogger->getWarningsCount();
    }
};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_ENGINE_HPP__ */

