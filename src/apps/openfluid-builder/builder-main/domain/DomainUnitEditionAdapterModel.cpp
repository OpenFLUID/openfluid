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
 \file DomainUnitEditionAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitEditionAdapterModel.hpp"

DomainUnitEditionAdapterModelImpl::DomainUnitEditionAdapterModelImpl()
{
  m_ModelsByRelationName["Froms"] = Gtk::ListStore::create(m_Columns);
  m_ModelsByRelationName["Tos"] = Gtk::ListStore::create(m_Columns);
  m_ModelsByRelationName["Parents"] = Gtk::ListStore::create(m_Columns);
  m_ModelsByRelationName["Children"] = Gtk::ListStore::create(m_Columns);
}
void DomainUnitEditionAdapterModelImpl::setRelations(std::map<std::string,
    openfluid::core::UnitsPtrList_t> Relations)
{
  for (std::map<std::string, openfluid::core::UnitsPtrList_t>::iterator it =
      Relations.begin(); it != Relations.end(); ++it)
  {
    m_ModelsByRelationName[it->first]->clear();

    openfluid::core::UnitsPtrList_t UnitList = it->second;

    for (openfluid::core::UnitsPtrList_t::iterator itt = UnitList.begin(); itt
        != UnitList.end(); ++itt)
    {
      Gtk::TreeRow Row = *(m_ModelsByRelationName[it->first]->append());
      Row[m_Columns.m_Class] = (*itt)->getClass();
      Row[m_Columns.m_Id] = (*itt)->getID();
    }
  }
}
Glib::RefPtr<Gtk::TreeModel> DomainUnitEditionAdapterModelImpl::getFromsModel()
{
  return m_ModelsByRelationName["Froms"];
}
Glib::RefPtr<Gtk::TreeModel> DomainUnitEditionAdapterModelImpl::getTosModel()
{
  return m_ModelsByRelationName["Tos"];
}
Glib::RefPtr<Gtk::TreeModel> DomainUnitEditionAdapterModelImpl::getParentsModel()
{
  return m_ModelsByRelationName["Parents"];
}
Glib::RefPtr<Gtk::TreeModel> DomainUnitEditionAdapterModelImpl::getChildrenModel()
{
  return m_ModelsByRelationName["Children"];
}
void DomainUnitEditionAdapterModelImpl::removeSelectedRelation(std::pair<
    std::string, Gtk::TreeIter> SelectedRelationIter)
{
  std::string RelationName = SelectedRelationIter.first;

  if (m_ModelsByRelationName.find(RelationName) != m_ModelsByRelationName.end())
    m_ModelsByRelationName[RelationName]->erase(SelectedRelationIter.second);
}
void DomainUnitEditionAdapterModelImpl::addRelation(std::string RelationName,
    std::string AddClassName, int AddId)
{
  if (m_ModelsByRelationName.find(RelationName) != m_ModelsByRelationName.end())
  {
    Gtk::TreeRow Row = *(m_ModelsByRelationName[RelationName]->append());
    Row[m_Columns.m_Class] = AddClassName;
    Row[m_Columns.m_Id] = AddId;
  }
}
