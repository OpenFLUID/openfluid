/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file
  \brief header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__


#include <openfluid/core.hpp>
#include <openfluid/base.hpp>
#include <openfluid/io/IOMan.hpp>
#include <openfluid/engine/PluginManager.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/engine/ModelInstance.hpp>

namespace openfluid { namespace engine {


// =====================================================================
// =====================================================================


/**
  Computation engine class
*/
class Engine
{
  private:

     openfluid::core::CoreRepository* mp_CoreData;

     openfluid::base::RuntimeEnvironment* mp_RunEnv;

     openfluid::base::ExecutionMessages* mp_ExecMsgs;

     openfluid::base::SimulationStatus* mp_SimStatus;

     /**
       Plugin manager
     */
     PluginManager* mp_PlugMan;

     /**
       Memory monitor
     */
     openfluid::core::MemoryMonitor* mp_MemMon;

     /**
       IOManager
     */
     openfluid::io::IOManager* mp_IOMan;


     openfluid::base::ModelDescriptor m_ModelDesc;

     openfluid::base::RunDescriptor m_RunDesc;

     openfluid::base::DomainDescriptor m_DomainDesc;


     const ModelInstance* mp_ModelInstance;


     void checkSimulationVarsProduction(int ExpectedVarsCount);

     void checkModelConsistency();

     void checkDataConsistency();

     void checkExtraFilesConsistency();

     void checkExistingVariable(openfluid::core::VariableName_t VarName,
                                openfluid::core::UnitClass_t ClassName,
                                std::string FunctionName);

     void createVariable(openfluid::core::VariableName_t VarName,
                         openfluid::core::UnitClass_t ClassName,
                         bool UpdateMode,
                         std::string FunctionName);

     void checkExistingInputData(openfluid::core::InputDataName_t DataName,
                                 openfluid::core::UnitClass_t ClassName,
                                 std::string FunctionName);

  public:
    /**
      Constructor
    */
    Engine();

    /**
      Destructor
    */
    ~Engine();

    bool buildModel();

    bool buildSpatialDomain();

    bool loadData();

    bool processRunConfiguration();

    bool initParams();

    bool prepareDataAndCheckConsistency();

    bool run();

    bool saveReports(std::string ErrorMsg);

    bool saveMessages();

    openfluid::base::SimulationInfo *getSimulationInfo() { return (openfluid::base::SimulationInfo*)mp_SimStatus; };

};


} } //namespaces


#endif






