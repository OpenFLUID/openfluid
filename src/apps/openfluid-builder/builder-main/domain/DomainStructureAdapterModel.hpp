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
 \file DomainStructureAdapterModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINSTRUCTUREADAPTERMODEL_HPP__
#define __DOMAINSTRUCTUREADAPTERMODEL_HPP__


#include <openfluid/core/Unit.hpp>

#include "BuilderClassListStore.hpp"
#include "DomainStructureColumns.hpp"

// =====================================================================
// =====================================================================


class DomainStructureAdapterModel
{
  public:

    virtual void
        setDomainStructure(
            openfluid::core::UnitsListByClassMap_t UnitListByClass) = 0;

    virtual Glib::RefPtr<Gtk::TreeModel> getClassesTreeModel() = 0;

    virtual Glib::RefPtr<Gtk::TreeModel> getUnitsTreeModel() = 0;

    virtual Gtk::TreeIter getRequestedClassSelection() = 0;

    virtual Gtk::TreeIter getRequestedUnitSelection() = 0;

    virtual std::string getClassNameFromIter(Gtk::TreeIter Iter) = 0;

    virtual int getUnitIdFromIter(Gtk::TreeIter Iter) = 0;

    virtual void
    deleteUnit(std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters) = 0;

    virtual void deleteClass(std::string ClassName) = 0;

    virtual void addUnit(openfluid::core::Unit& Unit) = 0;

    virtual void setSelectedClass(Gtk::TreeIter Iter) = 0;

    virtual void setSelectedUnit(Gtk::TreeIter UnitIter) = 0;

    virtual void
    setNewPcsOrder(std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters,
        int NewProcessOrder) = 0;
};

// =====================================================================
// =====================================================================


class DomainStructureAdapterModelImpl: public DomainStructureAdapterModel
{
  protected:

    DomainStructureColumns& m_Columns;

    std::map<std::string, Glib::RefPtr<BuilderListStore> > m_ByClassUnitsStores;

    Glib::RefPtr<BuilderClassListStore> mref_ClassStore;

    std::string m_RequestedSelectedClass;

    std::map<std::string, int> m_ByClassSelectedUnits;

    void appendUnitRowToUnitListStore(
        Glib::RefPtr<BuilderListStore> ClassListStore,
        openfluid::core::Unit* Unit);

    void createUnitStoreForClass(std::string ClassName);

    bool isClassNameValid(std::string ClassName);

    bool isClassNameEmpty(std::string ClassName);

    void setFirstClassSelected();

    void setLastClassSelected();

    void setFirstUnitSelected(std::string ClassName);

    void setLastUnitSelected(std::string ClassName);

  public:

    DomainStructureAdapterModelImpl(DomainStructureColumns& Columns);

    void setDomainStructure(
        openfluid::core::UnitsListByClassMap_t UnitListByClass);

    Glib::RefPtr<Gtk::TreeModel> getClassesTreeModel();

    Glib::RefPtr<Gtk::TreeModel> getUnitsTreeModel();

    Gtk::TreeIter getRequestedClassSelection();

    Gtk::TreeIter getRequestedUnitSelection();

    std::string getClassNameFromIter(Gtk::TreeIter Iter);

    int getUnitIdFromIter(Gtk::TreeIter Iter);

    void deleteUnit(std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters);

    void deleteClass(std::string ClassName);

    void addUnit(openfluid::core::Unit& Unit);

    void setSelectedClass(Gtk::TreeIter Iter);

    void setSelectedUnit(Gtk::TreeIter UnitIter);

    void setNewPcsOrder(std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters,
        int NewProcessOrder);
};

// =====================================================================
// =====================================================================


class DomainStructureAdapterModelSub: public DomainStructureAdapterModelImpl
{
  public:

    DomainStructureAdapterModelSub(DomainStructureColumns& Columns);

    std::map<std::string, Glib::RefPtr<BuilderListStore> >
    getByClassUnitsStores();

    std::string getRequestedSelectedClass();

    Glib::RefPtr<BuilderClassListStore> getClassStore();
};

#endif /* __DOMAINSTRUCTUREADAPTERMODEL_HPP__ */
