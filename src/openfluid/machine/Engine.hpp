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
*/


#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/base/SimulationLogger.hpp>

namespace openfluid {
namespace base {
class RuntimeEnvironment;
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
class DLLEXPORT Engine
{

  private:

     SimulationBlob& m_SimulationBlob;

     openfluid::base::RuntimeEnvironment* mp_RunEnv;

     openfluid::base::SimulationStatus* mp_SimStatus;

     MachineListener* mp_MachineListener;

     ModelInstance& m_ModelInstance;

     MonitoringInstance& m_MonitoringInstance;

     openfluid::base::SimulationLogger* mp_SimLogger;



     void checkSimulationVarsProduction(int ExpectedVarsCount);

     void checkModelConsistency();

     void checkAttributesConsistency();

     void checkExtraFilesConsistency();

     void checkExistingVariable(const openfluid::core::VariableName_t& VarName,
                                const openfluid::core::Value::Type& VarType,
                                const openfluid::core::UnitClass_t& ClassName,
                                const std::string& SimulatorID);

     void createVariable(const openfluid::core::VariableName_t& VarName,
                         const openfluid::core::Value::Type& VarType,
                         const openfluid::core::UnitClass_t& ClassName,
                         bool UpdateMode,
                         const std::string& SimulatorID);

     void checkExistingAttribute(openfluid::core::AttributeName_t AttrName,
                                 openfluid::core::UnitClass_t ClassName,
                                 const std::string& SimulatorID);

     void createAttribute(openfluid::core::AttributeName_t AttrName,
                          openfluid::core::UnitClass_t ClassName,
                          const std::string& SimulatorID);

     void prepareOutputDir();


  public:
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


    void initialize();

    void initParams();

    void prepareData();

    void checkConsistency();

    void run();

    void finalize();

    const openfluid::base::SimulationStatus* getSimulationStatus() { return mp_SimStatus; };

    SimulationBlob*  getSimulationBlob() { return &m_SimulationBlob; };

    MachineListener* getMachineListener() { return mp_MachineListener; };

    ModelInstance* getModelInstance() { return &m_ModelInstance; };

    unsigned int getWarningsCount() const { return mp_SimLogger->getWarningsCount(); };
};


} } //namespaces


#endif







