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
  @file Factory.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_FACTORY_HPP__
#define __OPENFLUID_MACHINE_FACTORY_HPP__


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
class SpatialGraph;
class Datastore;
}
namespace fluidx {
class SpatialDomainDescriptor;
class RunDescriptor;
class DatastoreDescriptor;
} }


namespace openfluid { namespace machine {

class SimulationBlob;
class ModelInstance;
class MonitoringInstance;


class OPENFLUID_API Factory
{

  public:

    Factory() = delete;

    /**
      Builds a spatial graph from a domain descriptor
      @param[in] Descriptor the spatial domain descriptor
      @param[out] SGraph the instanciated spatial graph
    */
    static void buildDomainFromDescriptor(const openfluid::fluidx::SpatialDomainDescriptor& Descriptor,
                                          openfluid::core::SpatialGraph& SGraph);

    /**
      Builds a datastore from a datastore descriptor
      @param[in] Descriptor the datastore descriptor
      @param[out] Store the instanciated datastore
    */
    static void buildDatastoreFromDescriptor(const openfluid::fluidx::DatastoreDescriptor& Descriptor,
                                             openfluid::core::Datastore& Store);

    /**
      Builds a coupled model instance from a model descriptor
      @param[in] ModelDesc the model descriptor
      @param[out] MInstance the instanciated coupled model
    */
    static void buildModelInstanceFromDescriptor(const openfluid::fluidx::CoupledModelDescriptor& ModelDesc,
                                                 ModelInstance& MInstance);

    /**
      Builds a monitoring instance from a monitoring descriptor
      @param[in] MonDesc the monitoring descriptor
      @param[out] MonInstance the instanciated monitoring
    */
    static void buildMonitoringInstanceFromDescriptor(const openfluid::fluidx::MonitoringDescriptor& MonDesc,
                                                      MonitoringInstance& MonInstance);

    /**
      Fills the current run context from a run configuration descriptor
      @param[in] RunDesc the run configuration descriptor
    */
    static void fillRunContextFromDescriptor(const openfluid::fluidx::RunConfigurationDescriptor& RunDesc);

    /**
      Builds a simulation blob from descriptors
      @param[in] FluidXDesc the descriptors
      @param[out] SimBlob the instanciated simulation blob
    */
    static void buildSimulationBlobFromDescriptors(const openfluid::fluidx::FluidXDescriptor& FluidXDesc,
                                                   SimulationBlob& SimBlob);

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_FACTORY_HPP__ */
