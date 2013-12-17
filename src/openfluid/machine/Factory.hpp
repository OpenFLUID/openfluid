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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#ifndef __FACTORY_HPP__
#define __FACTORY_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/MonitoringDescriptor.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>

namespace openfluid {
namespace base {
class ExecutionMessages;
class OutputDescriptor;
}
namespace core {
class CoreRepository;
class Datastore;
}
namespace fluidx {
class DomainDescriptor;
class RunDescriptor;
class DatastoreDescriptor;
}
}


namespace openfluid { namespace machine {

class SimulationBlob;
class ModelInstance;
class MonitoringInstance;


class DLLEXPORT Factory
{
  public:

    static void buildDomainFromDescriptor(openfluid::fluidx::DomainDescriptor& Descriptor,
                                          openfluid::core::CoreRepository& CoreRepos);

    static void buildDatastoreFromDescriptor(openfluid::fluidx::DatastoreDescriptor& Descriptor,
                                             openfluid::core::Datastore& Store);


    static void buildModelInstanceFromDescriptor(openfluid::fluidx::CoupledModelDescriptor& ModelDesc,
                                                 ModelInstance& MInstance);

    static void buildMonitoringInstanceFromDescriptor(openfluid::fluidx::MonitoringDescriptor& MonDesc,
                                                      MonitoringInstance& MonInstance);

    static void fillRunEnvironmentFromDescriptor(openfluid::fluidx::RunDescriptor& RunDescr);

    static void buildSimulationBlobFromDescriptors(openfluid::fluidx::FluidXDescriptor& FluidXDesc,
                                                   SimulationBlob& SimBlob);

    static std::string buildGeneratorID(const openfluid::core::VariableName_t& VarName,
                                        bool IsVector,
                                        const openfluid::core::UnitClass_t& ClassName)
    {
      // <varname>.<unitclass>.gen<type> where <type> can be scalar or vector

      std::string GenID(VarName);
      GenID += ".";
      GenID += ClassName;
      GenID += ".gen";
      if (IsVector) GenID += "vector";
      else GenID += "scalar";

      return GenID;
    }

};


} } //namespaces


#endif /* __FACTORY_HPP__ */
