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
 \file DomainIDataAdapterModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINIDATAADAPTERMODEL_HPP__
#define __DOMAINIDATAADAPTERMODEL_HPP__

#include <gtkmm.h>

#include <openfluid/core.hpp>

#include "DomainIDataColumns.hpp"

#include "BuilderClassListStore.hpp"

class DomainIDataAdapterModel
{
  public:
    virtual void
    dataInit(const openfluid::core::CoreRepository& CoreRepos) = 0;
    virtual Glib::RefPtr<Gtk::TreeModel> getClassesTreeModel() = 0;
    virtual DomainIDataColumns* getUnitsTreeColumns() = 0;
    virtual Glib::RefPtr<Gtk::TreeModel> getUnitsTreeModel() = 0;
    virtual Gtk::TreeIter getRequestedClassSelection() = 0;
    virtual std::string getClassNameFromIter(Gtk::TreeIter Iter) = 0;
    virtual Gtk::TreeIter getIterFromClassName(std::string ClassName) = 0;
    virtual int getUnitIdFromIter(Gtk::TreeIter Iter) = 0;
    virtual void setSelectedClass(Gtk::TreeIter Iter) = 0;
    virtual void updateEditedData(
        std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters, std::pair<
            std::string, std::string> DataInfo) = 0;
    virtual void updateDataForClass(Gtk::TreeIter ClassIter) = 0;
};

class DomainIDataAdapterModelImpl: public DomainIDataAdapterModel
{
  protected:
    const openfluid::core::CoreRepository* mp_CoreRepos;
    std::vector<std::string> m_ClassNames;
    std::map<std::string, DomainIDataColumns*> m_ByClassColumns;
    std::map<std::string, Glib::RefPtr<BuilderListStore> > m_ByClassUnitsStores;
    Glib::RefPtr<BuilderClassListStore> mref_ClassStore;
    std::string m_RequestedSelectedClass;
    void extractClassNames();
    void createDataColumnsAndStoreForClass(std::string ClassName);
    void populateStoreForClass(std::string ClassName);
    bool isClassNameValid(std::string ClassName);
    void setFirstClassSelected();
  public:
    DomainIDataAdapterModelImpl();
    void dataInit(const openfluid::core::CoreRepository& CoreRepos);
    Glib::RefPtr<Gtk::TreeModel> getClassesTreeModel();
    DomainIDataColumns* getUnitsTreeColumns();
    Glib::RefPtr<Gtk::TreeModel> getUnitsTreeModel();
    Gtk::TreeIter getRequestedClassSelection();
    std::string getClassNameFromIter(Gtk::TreeIter Iter);
    Gtk::TreeIter getIterFromClassName(std::string ClassName);
    int getUnitIdFromIter(Gtk::TreeIter Iter);
    void updateEditedData(std::pair<Gtk::TreeIter, Gtk::TreeIter> UnitIters,
        std::pair<std::string, std::string> DataInfo);
    void updateDataForClass(Gtk::TreeIter ClassIter);
    void setSelectedClass(Gtk::TreeIter Iter);
};

class DomainIDataAdapterModelSub: public DomainIDataAdapterModelImpl
{
  public:
    void setCoreRepository(const openfluid::core::CoreRepository* CoreRepos);
    void extractClassNames();
    std::vector<std::string> getClassNames();
    void createDataColumnsAndStoreForClass(std::string ClassName);
    std::map<std::string, DomainIDataColumns*> getByClassColumns();
    std::map<std::string, Glib::RefPtr<BuilderListStore> >
    getByClassUnitsStores();
    void populateStoreForClass(std::string ClassName);
    Glib::RefPtr<BuilderClassListStore> getClassStore();
};

#endif /* __DOMAINIDATAADAPTERMODEL_HPP__ */
