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
  @file SimulationBlob.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_SIMULATIONBLOB_HPP__
#define __OPENFLUID_MACHINE_SIMULATIONBLOB_HPP__

#include <openfluid/fluidx/RunDescriptor.hpp>
#include <openfluid/core/Datastore.hpp>
#include <openfluid/base/SimulationStatus.hpp>
#include <openfluid/core/SpatialGraph.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API SimulationBlob
{
  private:

    openfluid::core::SpatialGraph m_SpatialGraph;

    openfluid::fluidx::RunDescriptor m_RunDescriptor;

    openfluid::core::Datastore m_Datastore;

    openfluid::base::SimulationStatus m_Status;


  public:

    SimulationBlob();

    ~SimulationBlob();

    inline openfluid::core::SpatialGraph& spatialGraph()
    { return m_SpatialGraph; };

    inline openfluid::fluidx::RunDescriptor& runDescriptor()
    {return m_RunDescriptor; };

    inline openfluid::base::SimulationStatus& simulationStatus()
    {return m_Status; };

    inline openfluid::core::Datastore& datastore()
    { return m_Datastore; };

    inline const openfluid::core::SpatialGraph& spatialGraph() const
    { return m_SpatialGraph; };

    inline const openfluid::fluidx::RunDescriptor& runDescriptor() const
    {return m_RunDescriptor; };

    inline const openfluid::base::SimulationStatus& simulationStatus() const
    {return m_Status; };

    inline const openfluid::core::Datastore& datastore() const
    { return m_Datastore; };

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_SIMULATIONBLOB_HPP__ */
