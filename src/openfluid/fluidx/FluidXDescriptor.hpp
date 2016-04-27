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

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
 */

#ifndef __OPENFLUID_FLUIDX_FLUIDXDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_FLUIDXDESCRIPTOR_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/RunDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/MonitoringDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <QDomElement>


namespace openfluid {

namespace base {
class IOListener;
}

namespace fluidx {

class OPENFLUID_API FluidXDescriptor
{
  private:

    openfluid::fluidx::CoupledModelDescriptor m_ModelDescriptor;

    openfluid::fluidx::SpatialDomainDescriptor m_DomainDescriptor;

    openfluid::fluidx::RunDescriptor m_RunDescriptor;

    openfluid::fluidx::DatastoreDescriptor m_DatastoreDescriptor;

    openfluid::fluidx::MonitoringDescriptor m_MonitoringDescriptor;

    std::string m_CurrentFile;

    std::string m_CurrentDir;

    bool m_RunConfigDefined;

    bool m_ModelDefined;

    std::string m_IndentStr;

    openfluid::base::IOListener* mp_Listener;

    bool extractWareEnabledFromNode(QDomElement& Node);

    void extractMonitoringFromNode(QDomElement& Node);

    openfluid::ware::WareParams_t extractParamsFromNode(QDomElement& Node);

    openfluid::ware::WareParams_t mergeParams(const openfluid::ware::WareParams_t& Params,
                                              const openfluid::ware::WareParams_t& OverloadParams);

    void extractModelFromNode(QDomElement& Node);

    void extractRunFromNode(QDomElement& Node);

    void extractDomainFomNode(QDomElement& Node);

    openfluid::core::UnitClassID_t extractUnitClassIDFromNode(QDomElement& Node);

    void extractDomainDefinitionFromNode(QDomElement& Node);

    void extractDomainAttributesFromNode(QDomElement& Node);

    void extractDomainCalendarFromNode(QDomElement& Node);

    void extractDatastoreFromNode(QDomElement& Node);

    void parseFile(std::string Filename);

    // =====================================================================
    // =====================================================================

    void prepareFluidXDir(const std::string& DirPath);

    std::string getGeneratorMethodAsStr(openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method) const;

    std::string getParamsAsStr(const openfluid::ware::WareParams_t& Params) const;

    void writeModelToStream(std::ostream& Contents);

    void writeDomainToStream(std::ostream& Contents);

    void writeDomainDefinitionToStream(std::ostream& Contents);

    void writeDomainAttributesToStream(std::ostream& Contents);

    void writeDomainCalendarToStream(std::ostream& Contents);

    void writeRunConfigurationToStream(std::ostream& Contents);

    void writeDatastoreToStream(std::ostream& Contents);

    void writeMonitoringToStream(std::ostream& Contents);


  public:

    FluidXDescriptor(openfluid::base::IOListener* Listener);

    ~FluidXDescriptor();

    void loadFromDirectory(const std::string& DirPath);

    inline openfluid::fluidx::CoupledModelDescriptor& modelDescriptor()
    { return m_ModelDescriptor; }

    inline openfluid::fluidx::SpatialDomainDescriptor& spatialDomainDescriptor()
    { return m_DomainDescriptor; }

    inline openfluid::fluidx::RunDescriptor& runDescriptor()
    { return m_RunDescriptor; }

    inline openfluid::fluidx::DatastoreDescriptor& datastoreDescriptor()
    { return m_DatastoreDescriptor; }

    inline openfluid::fluidx::MonitoringDescriptor& monitoringDescriptor()
    { return m_MonitoringDescriptor; }

    inline const openfluid::fluidx::CoupledModelDescriptor& modelDescriptor() const
    { return m_ModelDescriptor; }

    inline const openfluid::fluidx::SpatialDomainDescriptor& spatialDomainDescriptor() const
    { return m_DomainDescriptor; }

    inline const openfluid::fluidx::RunDescriptor& runDescriptor() const
    { return m_RunDescriptor; }

    inline const openfluid::fluidx::DatastoreDescriptor& datastoreDescriptor() const
    { return m_DatastoreDescriptor; }

    inline const openfluid::fluidx::MonitoringDescriptor& monitoringDescriptor() const
    { return m_MonitoringDescriptor; }

    void writeToManyFiles(const std::string& DirPath);

    void writeToSingleFile(const std::string& FilePath);
};

} } // namespaces

#endif /* __OPENFLUID_FLUIDX_FLUIDXDESCRIPTOR_HPP__ */
