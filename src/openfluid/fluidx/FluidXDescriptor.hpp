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
  @file FluidXDescriptor.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <aline.libres@gmail.com>
*/


#ifndef __OPENFLUID_FLUIDX_FLUIDXDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_FLUIDXDESCRIPTOR_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/MonitoringDescriptor.hpp>
#include <openfluid/fluidx/RunConfigurationDescriptor.hpp>
#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <openfluid/fluidx/AttributesTableDescriptor.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API FluidXDescriptor
{
  private:

    friend class FluidXReaderImplementation;
    friend class FluidXWriterImplementation;

    openfluid::fluidx::CoupledModelDescriptor m_ModelDescriptor;

    openfluid::fluidx::SpatialDomainDescriptor m_DomainDescriptor;

    openfluid::fluidx::RunConfigurationDescriptor m_RunDescriptor;

    openfluid::fluidx::DatastoreDescriptor m_DatastoreDescriptor;

    openfluid::fluidx::MonitoringDescriptor m_MonitoringDescriptor;


  public:

    // NOTE : apply the rule of zero to ensure corect usage of copy/move/assign

    inline openfluid::fluidx::CoupledModelDescriptor& model()
    {
      return m_ModelDescriptor;
    }

    inline const openfluid::fluidx::CoupledModelDescriptor& model() const
    {
      return m_ModelDescriptor;
    }

    inline openfluid::fluidx::SpatialDomainDescriptor& spatialDomain()
    {
      return m_DomainDescriptor;
    }

    inline const openfluid::fluidx::SpatialDomainDescriptor& spatialDomain() const
    {
      return m_DomainDescriptor;
    }

    inline openfluid::fluidx::RunConfigurationDescriptor& runConfiguration()
    {
      return m_RunDescriptor;
    }

    inline const openfluid::fluidx::RunConfigurationDescriptor& runConfiguration() const
    {
      return m_RunDescriptor;
    }

    inline openfluid::fluidx::DatastoreDescriptor& datastore()
    {
      return m_DatastoreDescriptor;
    }

    inline const openfluid::fluidx::DatastoreDescriptor& datastore() const
    {
      return m_DatastoreDescriptor;
    }

    inline openfluid::fluidx::MonitoringDescriptor& monitoring()
    {
      return m_MonitoringDescriptor;
    }

    inline const openfluid::fluidx::MonitoringDescriptor& monitoring() const
    {
      return m_MonitoringDescriptor;
    }

    void reset()
    {
      m_ModelDescriptor = openfluid::fluidx::CoupledModelDescriptor();
      m_DomainDescriptor = openfluid::fluidx::SpatialDomainDescriptor();
      m_RunDescriptor = openfluid::fluidx::RunConfigurationDescriptor();
      m_DatastoreDescriptor = openfluid::fluidx::DatastoreDescriptor();
      m_MonitoringDescriptor = openfluid::fluidx::MonitoringDescriptor();
    }
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_FLUIDXDESCRIPTOR_HPP__ */
