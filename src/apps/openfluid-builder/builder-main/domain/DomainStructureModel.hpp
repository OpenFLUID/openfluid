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
 \file DomainStructureModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINSTRUCTUREMODEL_HPP__
#define __DOMAINSTRUCTUREMODEL_HPP__

#include <sigc++/sigc++.h>
#include <map>
#include <string>

#include <openfluid/guicommon/BuilderDomain.hpp>

namespace openfluid {
namespace fluidx {
class UnitDescriptor;
}
namespace guicommon {
class BuilderUnit;
}
}

// =====================================================================
// =====================================================================


class DomainStructureModel
{
  public:

    virtual sigc::signal<void> signal_FromAppDomainChanged() = 0;

    virtual sigc::signal<void> signal_FromAppUnitDeleted() = 0;

    virtual sigc::signal<void> signal_Activated() = 0;

    virtual sigc::signal<void, openfluid::fluidx::UnitDescriptor&>
    signal_FromAppUnitAdded() = 0;

    virtual sigc::signal<void>
    signal_FromUserSelectionChanged() = 0;

    virtual const std::map<std::string, std::map<int, openfluid::guicommon::BuilderUnit> >& getUnitListByClass() = 0;

    virtual bool isEmpty() = 0;

    virtual void addUnit(openfluid::fluidx::UnitDescriptor* Unit) = 0;

    virtual void deleteSelectedUnit() = 0;

    virtual void setCurrentSelectionByUser(
        std::pair<std::string, int> UnitInfos) = 0;

    virtual const openfluid::fluidx::UnitDescriptor* getSelectedUnit() = 0;

    virtual std::string getSelectedClass() = 0;

    virtual void update() = 0;

};

// =====================================================================
// =====================================================================


class DomainStructureModelImpl: public DomainStructureModel
{
  private:

    sigc::signal<void> m_signal_FromAppDomainChanged;

    sigc::signal<void> m_signal_FromAppUnitDeleted;

    sigc::signal<void> m_signal_Activated;

    sigc::signal<void, openfluid::fluidx::UnitDescriptor&> m_signal_FromAppUnitAdded;

    sigc::signal<void> m_signal_FromUserSelectionChanged;

    openfluid::guicommon::BuilderDomain* mp_Domain;

    const openfluid::fluidx::UnitDescriptor* mp_SelectedUnit;

    std::string m_SelectedClass;

    void updateUnitListByClass();

  protected:

    void deleteUnit(const openfluid::fluidx::UnitDescriptor* Unit);

  public:

    DomainStructureModelImpl(openfluid::guicommon::BuilderDomain& Domain);

    sigc::signal<void> signal_FromAppDomainChanged();

    sigc::signal<void> signal_FromAppUnitDeleted();

    sigc::signal<void, openfluid::fluidx::UnitDescriptor&> signal_FromAppUnitAdded();

    sigc::signal<void> signal_FromUserSelectionChanged();

    sigc::signal<void> signal_Activated();

    const openfluid::guicommon::BuilderDomain::UnitsByIdByClass_t& getUnitListByClass();

    bool isEmpty();

    void addUnit(openfluid::fluidx::UnitDescriptor* Unit);

    void deleteSelectedUnit();

    void setCurrentSelectionByUser(std::pair<std::string, int> UnitInfos);

    const openfluid::fluidx::UnitDescriptor* getSelectedUnit();

    std::string getSelectedClass();

    void update();
};

// =====================================================================
// =====================================================================


class DomainStructureModelSub: public DomainStructureModelImpl
{
  public:

    DomainStructureModelSub(openfluid::guicommon::BuilderDomain& Domain);

    void deleteUnit(const openfluid::fluidx::UnitDescriptor* Unit);
};

#endif /* __DOMAINSTRUCTUREMODEL_HPP__ */
