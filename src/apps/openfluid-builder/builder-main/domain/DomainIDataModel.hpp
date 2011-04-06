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
 \file DomainIDataModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINIDATAMODEL_HPP__
#define __DOMAINIDATAMODEL_HPP__

#include <sigc++/sigc++.h>

#include <openfluid/core.hpp>

class DomainIDataModel
{
  public:
    virtual sigc::signal<void> signal_FromAppDataInit() = 0;
    virtual sigc::signal<void> signal_FromAppDataReplaced() = 0;
    virtual sigc::signal<void> signal_FromAppDataListChanged() = 0;
    virtual
    sigc::signal<void> signal_FromAppClassSelectionChanged() = 0;
    virtual void setEngineRequirements(
        openfluid::core::CoreRepository& CoreRepos) = 0;
    virtual const openfluid::core::CoreRepository* getCoreRepos() = 0;
    virtual void update() = 0;
    virtual void replaceDataValue(std::pair<std::string, int> UnitInfo,
        std::pair<std::string, std::string> DataInfo) = 0;
    virtual void removeData(std::string DataName) = 0;
    virtual void addData(std::string DataName) = 0;
    virtual void setCurrentClassSelectionByUser(std::string ClassName) = 0;
    virtual void setCurrentClassSelectionByApp(std::string ClassName) = 0;
    virtual std::string getSelectedClass() = 0;
    virtual std::string getAppRequestedClass() = 0;
};

// =====================================================================
// =====================================================================

class DomainIDataModelImpl: public DomainIDataModel
{
  private:
    openfluid::core::CoreRepository* mp_CoreRepos;
    sigc::signal<void> m_signal_FromAppDataInit;
    sigc::signal<void> m_signal_FromAppDataReplaced;
    sigc::signal<void> m_signal_FromAppDataListChanged;
    sigc::signal<void> m_signal_FromAppClassSelectionChanged;
    std::string m_SelectedClass;
    std::string m_AppRequestedSelection;
  public:
    DomainIDataModelImpl();
    sigc::signal<void> signal_FromAppDataInit();
    sigc::signal<void> signal_FromAppDataReplaced();
    sigc::signal<void> signal_FromAppDataListChanged();
    sigc::signal<void> signal_FromAppClassSelectionChanged();
    void setEngineRequirements(openfluid::core::CoreRepository& CoreRepos);
    const openfluid::core::CoreRepository* getCoreRepos();
    void update();
    void replaceDataValue(std::pair<std::string, int> UnitInfo, std::pair<
        std::string, std::string> DataInfo);
    void removeData(std::string DataName);
    void addData(std::string DataName);
    void setCurrentClassSelectionByUser(std::string ClassName);
    void setCurrentClassSelectionByApp(std::string ClassName);
    std::string getSelectedClass();
    std::string getAppRequestedClass();
};

// =====================================================================
// =====================================================================


class DomainIDataModelSub: public DomainIDataModelImpl
{
  public:
};

#endif /* __DOMAINIDATAMODEL_HPP__ */
