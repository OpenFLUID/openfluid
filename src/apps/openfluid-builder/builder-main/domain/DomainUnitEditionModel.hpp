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
 \file DomainUnitEditionModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINUNITEDITIONMODEL_HPP__
#define __DOMAINUNITEDITIONMODEL_HPP__

#include <sigc++/sigc++.h>

#include <openfluid/core.hpp>


class DomainUnitEditionModel
{
  public:
    virtual sigc::signal<void> signal_InitAsked() = 0;
    virtual sigc::signal<void> signal_ShowDialogAsked() = 0;
    virtual sigc::signal<void> signal_SelectedRelationRemoved() = 0;
    virtual sigc::signal<void> signal_RelationAdded() = 0;
    virtual void setEngineRequirements(
        openfluid::core::CoreRepository& CoreRepos) = 0;
    virtual void initialize(openfluid::core::Unit& SelectedUnit) = 0;
    virtual void showDialog() = 0;
    virtual std::string getClassName() = 0;
    virtual int getId() = 0;
    virtual int getProcessOrder() = 0;
    virtual std::map<std::string, openfluid::core::UnitsPtrList_t>
    getRelationsByName() = 0;
    virtual void setProcessOrder(int PcsOrder) = 0;
    virtual void setSelectedRelation(std::pair<std::string, std::pair<
        std::string, int> > SelectedRelation) = 0;
    virtual void removeSelectedRelation() = 0;
    virtual void addRelationAsked(std::string RelationName,
        std::string AddClassName, int AddId) = 0;
};

class DomainUnitEditionModelImpl: public DomainUnitEditionModel
{
  private:
    sigc::signal<void> m_signal_InitAsked;
    sigc::signal<void> m_signal_ShowDialogAsked;
    sigc::signal<void> m_signal_SelectedRelationRemoved;
    sigc::signal<void> m_signal_RelationAdded;
    openfluid::core::CoreRepository* mp_CoreRepos;
    openfluid::core::Unit* mp_Unit;
    std::vector<std::string> m_ClassNames;
    int m_ProcessOrder;
    std::map<std::string, openfluid::core::UnitsPtrList_t> m_RelationsByNameMap;
  protected:
    std::pair<std::string, std::pair<std::string, int> > m_SelectedRelation;
    void initialize();
    void extractClassNames();
    void extractRelation(std::string RelationName,
        openfluid::core::UnitsPtrList_t* UnitsPtrList);
    void extractUnitAttributes();
  public:
    DomainUnitEditionModelImpl();
    sigc::signal<void> signal_InitAsked();
    sigc::signal<void> signal_ShowDialogAsked();
    sigc::signal<void> signal_SelectedRelationRemoved();
    sigc::signal<void> signal_RelationAdded();
    void setEngineRequirements(openfluid::core::CoreRepository& CoreRepos);
    void initialize(openfluid::core::Unit& SelectedUnit);
    void showDialog();
    void setProcessOrder(int PcsOrder);
    std::string getClassName();
    int getId();
    int getProcessOrder();
    std::map<std::string, openfluid::core::UnitsPtrList_t> getRelationsByName();
    void setSelectedRelation(
        std::pair<std::string, std::pair<std::string, int> > SelectedRelation);
    void removeSelectedRelation();
    void addRelationAsked(std::string RelationName, std::string AddClassName,
        int AddId);
};

class DomainUnitEditionModelSub: public DomainUnitEditionModelImpl
{
  public:
    std::pair<std::string, std::pair<std::string, int> > getSelectedRelation();
};

#endif /* __DOMAINUNITEDITIONMODEL_HPP__ */
