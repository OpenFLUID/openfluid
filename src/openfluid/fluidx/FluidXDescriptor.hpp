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
 \file FluidXDescriptor.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef FLUIDXDESCRIPTOR_HPP_
#define FLUIDXDESCRIPTOR_HPP_

#include <libxml/tree.h>
#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/DomainDescriptor.hpp>
#include <openfluid/fluidx/CoupledModelDescriptor.hpp>
#include <openfluid/fluidx/RunDescriptor.hpp>
#include <openfluid/fluidx/DatastoreDescriptor.hpp>
#include <openfluid/fluidx/MonitoringDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/dllexport.hpp>

namespace openfluid {

namespace base {
class IOListener;
}

namespace fluidx {

class DLLEXPORT FluidXDescriptor
{
  private:

    openfluid::fluidx::CoupledModelDescriptor m_ModelDescriptor;

    openfluid::fluidx::DomainDescriptor m_DomainDescriptor;

    openfluid::fluidx::RunDescriptor m_RunDescriptor;

    openfluid::fluidx::DatastoreDescriptor m_DatastoreDescriptor;

    openfluid::fluidx::MonitoringDescriptor m_MonitoringDescriptor;

    std::string m_CurrentFile;

    std::string m_CurrentDir;

    bool m_RunConfigDefined;

    bool m_ModelDefined;

    openfluid::base::IOListener* mp_Listener;

    std::string m_IndentStr;

    void extractMonitoringFromNode(xmlNodePtr NodePtr);

    openfluid::ware::WareParams_t extractParamsFromNode(xmlNodePtr NodePtr);

    openfluid::ware::WareParams_t mergeParams(
        const openfluid::ware::WareParams_t& Params,
        const openfluid::ware::WareParams_t& OverloadParams);

    void extractModelFromNode(xmlNodePtr NodePtr);

    void extractRunFromNode(xmlNodePtr NodePtr);

    void extractDomainFomNode(xmlNodePtr NodePtr);

    openfluid::core::UnitClassID_t extractUnitClassIDFromNode(
        xmlNodePtr NodePtr);

    void extractDomainDefinitionFromNode(xmlNodePtr NodePtr);

    void extractDomainAttributesFromNode(xmlNodePtr NodePtr);

    void extractDomainCalendarFromNode(xmlNodePtr NodePtr);

    void extractDatastoreFromNode(xmlNodePtr NodePtr);

    void parseFile(std::string Filename);

    // =====================================================================
    // =====================================================================

    void prepareFluidXDir(std::string DirPath);

    std::string getGeneratorMethodAsStr(
        openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method) const;

    std::string getParamsAsStr(
        const openfluid::ware::WareParams_t& Params) const;

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

    void loadFromDirectory(std::string DirPath);

    inline openfluid::fluidx::CoupledModelDescriptor& getModelDescriptor()
    { return m_ModelDescriptor; }

    inline openfluid::fluidx::DomainDescriptor& getDomainDescriptor()
    { return m_DomainDescriptor; }

    inline openfluid::fluidx::RunDescriptor& getRunDescriptor()
    { return m_RunDescriptor; }

    inline openfluid::fluidx::DatastoreDescriptor& getDatastoreDescriptor()
    { return m_DatastoreDescriptor; }

    inline openfluid::fluidx::MonitoringDescriptor& getMonitoringDescriptor()
    { return m_MonitoringDescriptor; }

    inline const openfluid::fluidx::CoupledModelDescriptor& getModelDescriptor() const
    { return m_ModelDescriptor; }

    inline const openfluid::fluidx::DomainDescriptor& getDomainDescriptor() const
    { return m_DomainDescriptor; }

    inline const openfluid::fluidx::RunDescriptor& getRunDescriptor() const
    { return m_RunDescriptor; }

    inline const openfluid::fluidx::DatastoreDescriptor& getDatastoreDescriptor() const
    { return m_DatastoreDescriptor; }

    inline const openfluid::fluidx::MonitoringDescriptor& getMonitoringDescriptor() const
    { return m_MonitoringDescriptor; }

    // =====================================================================
    // =====================================================================

    void writeToManyFiles(std::string DirPath);

    void writeToSingleFile(std::string FilePath);
};

}
} // namespaces
#endif /* FLUIDXDESCRIPTOR_HPP_ */
