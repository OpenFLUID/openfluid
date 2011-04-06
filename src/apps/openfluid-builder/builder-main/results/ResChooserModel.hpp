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
 \file ResChooserModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __RESCHOOSERMODEL_HPP__
#define __RESCHOOSERMODEL_HPP__

#include <sigc++/sigc++.h>

#include <openfluid/base.hpp>
#include <openfluid/core.hpp>
#include <openfluid/machine.hpp>

class ResChooserModel
{
  public:

    virtual sigc::signal<void> signal_FromAppInit() = 0;

    virtual sigc::signal<void> signal_ViewResultAsked() = 0;

    virtual sigc::signal<void, std::string> signal_MessageAsked() = 0;

    virtual void setEngineRequirements(
        openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance) = 0;

    virtual void initialize() = 0;

    virtual std::map<std::string, std::string> getBySetNameClassName() = 0;

    virtual std::map<std::string, std::vector<unsigned int> >
    getBySetNameIDs() = 0;

    virtual void setSelectedUnitInfo(std::pair<std::string, int> UnitInfo) = 0;

    virtual void setSelectedSetName(std::string SetName) = 0;

    virtual openfluid::core::Unit* getSelectedUnit() = 0;

    virtual openfluid::base::OutputSetDescriptor* getSelectedSetDescriptor() = 0;

    virtual void validate() = 0;

    virtual void update() = 0;
};


class ResChooserModelImpl: public ResChooserModel
{
  private:

    sigc::signal<void> m_signal_FromAppInit;

    sigc::signal<void> m_signal_ViewResultAsked;

    sigc::signal<void, std::string> m_signal_MessageAsked;

    openfluid::base::OutputDescriptor* mp_OutDesc;

    openfluid::core::CoreRepository* mp_CoreRepos;

    openfluid::machine::ModelInstance* mp_ModelInstance;

    std::map<std::string, std::string> m_BySetNameClassName;

    std::map<std::string, std::vector<unsigned int> > m_BySetNameIDs;

    std::map<std::string, openfluid::base::OutputSetDescriptor*>
        m_BySetNameSetDesc;

  protected:

    std::string m_SelectedSetName;

    std::pair<std::string, unsigned int> m_SelectedUnitInfo;

    void extractSets();

  public:

    ResChooserModelImpl();

    sigc::signal<void> signal_FromAppInit();

    sigc::signal<void> signal_ViewResultAsked();

    sigc::signal<void, std::string> signal_MessageAsked();

    void setEngineRequirements(openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance);

    void initialize();

    std::map<std::string, std::string> getBySetNameClassName();

    std::map<std::string, std::vector<unsigned int> > getBySetNameIDs();

    void setSelectedUnitInfo(std::pair<std::string, int> UnitInfo);

    void setSelectedSetName(std::string SetName);

    openfluid::core::Unit* getSelectedUnit();

    openfluid::base::OutputSetDescriptor* getSelectedSetDescriptor();

    void validate();

    void update();
};


class ResChooserModelSub: public ResChooserModelImpl
{
  public:

    void extractSets();

    std::string getSelectedSetName();

    std::pair<std::string, unsigned int> getSelectedUnitInfo();
};

#endif /* __RESCHOOSERMODEL_HPP__ */
