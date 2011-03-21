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
 \file DomainIDataAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataAdapterModel.hpp"

#include <boost/foreach.hpp>


void DomainIDataAdapterModelImpl::extractClassNames()
{
  m_ClassNames.clear();

  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      mp_CoreRepos->getUnitsByClass()->begin(); it
      != mp_CoreRepos->getUnitsByClass()->end(); ++it)
  {
    if (!it->second.getList()->empty())
      m_ClassNames.push_back(it->first);
  }
}
void DomainIDataAdapterModelImpl::createDataColumnsAndStoreForClass(
    std::string ClassName)
{
  DomainIDataColumns* ThisClassColumnDesc = new DomainIDataColumns();

  /* Extract first Unit data names of this class to get data names of this class
   * (supposed to be the same for all the class)
   */
  openfluid::core::Unit FirstUnit =
      *(mp_CoreRepos->getUnits(ClassName)->getList()->begin());

  BOOST_FOREACH(std::string DataName, FirstUnit.getInputData()->getInputDataNames())
        {
          Gtk::TreeModelColumn<std::string> * DataColumn =
              new Gtk::TreeModelColumn<std::string>();
          ThisClassColumnDesc->addWithTitle(DataName, *DataColumn);
        }
  m_ByClassColumns[ClassName] = ThisClassColumnDesc;
  m_ByClassUnitsStores[ClassName] = BuilderListStore::create(
      *ThisClassColumnDesc);
}
void DomainIDataAdapterModelImpl::populateStoreForClass(std::string ClassName)
{
  BOOST_FOREACH(openfluid::core::Unit Unit,*(mp_CoreRepos->getUnits(ClassName)->getList()))
        {
          Gtk::TreeRow Row = *(m_ByClassUnitsStores[ClassName]->append());

          // Id column
          Row.set_value(*m_ByClassColumns[ClassName]->getIdColumn(),
              (int) Unit.getID());

          // Data columns
          BOOST_FOREACH(std::string DataName, Unit.getInputData()->getInputDataNames())
                {
                  std::string Val;
                  Unit.getInputData()->getValue(DataName, &Val);
                  Row.set_value(
                      *m_ByClassColumns[ClassName]->getColumnWithTitle(DataName),
                      Val);
                }
        }
}
bool DomainIDataAdapterModelImpl::isClassNameValid(std::string ClassName)
{
  return m_ByClassUnitsStores.find(ClassName) != m_ByClassUnitsStores.end();
}
void DomainIDataAdapterModelImpl::setFirstClassSelected()
{
  if (!m_ByClassUnitsStores.empty())
    m_RequestedSelectedClass = m_ByClassUnitsStores.begin()->first;
}

DomainIDataAdapterModelImpl::DomainIDataAdapterModelImpl() :
  mp_CoreRepos(0), mref_ClassStore(BuilderClassListStore::create()),
      m_RequestedSelectedClass("")
{
}
void DomainIDataAdapterModelImpl::dataInit(
    const openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;

  extractClassNames();
  mref_ClassStore->setClasses(m_ClassNames);

  m_ByClassColumns.clear();
  m_ByClassUnitsStores.clear();

  BOOST_FOREACH(std::string ClassName, m_ClassNames)
        {
          createDataColumnsAndStoreForClass(ClassName);
          populateStoreForClass(ClassName);
        }

  setFirstClassSelected();
}
Glib::RefPtr<Gtk::TreeModel> DomainIDataAdapterModelImpl::getClassesTreeModel()
{
  return mref_ClassStore;
}
DomainIDataColumns* DomainIDataAdapterModelImpl::getUnitsTreeColumns()
{
  if (isClassNameValid(m_RequestedSelectedClass))
    return m_ByClassColumns[m_RequestedSelectedClass];
  return (DomainIDataColumns*) 0;
}
Glib::RefPtr<Gtk::TreeModel> DomainIDataAdapterModelImpl::getUnitsTreeModel()
{
  if (isClassNameValid(m_RequestedSelectedClass))
    return m_ByClassUnitsStores[m_RequestedSelectedClass];
  return Gtk::ListStore::create(*new DomainIDataColumns());
}
Gtk::TreeIter DomainIDataAdapterModelImpl::getRequestedClassSelection()
{
  return mref_ClassStore->getIterFromClassName(m_RequestedSelectedClass);
}
std::string DomainIDataAdapterModelImpl::getClassNameFromIter(
    Gtk::TreeIter Iter)
{
  return mref_ClassStore->getClassNameFromIter(Iter);
}
int DomainIDataAdapterModelImpl::getUnitIdFromIter(Gtk::TreeIter Iter)
{
  int Id = -1;
  if (Iter)
    Iter->get_value(0, Id);
  return Id;
}
void DomainIDataAdapterModelImpl::updateEditedData(std::pair<Gtk::TreeIter,
    Gtk::TreeIter> UnitIters, std::pair<std::string, std::string> DataInfo)
{
  std::string ClassName = getClassNameFromIter(UnitIters.first);
  std::string DataName = DataInfo.first;
  Gtk::TreeModelColumn<std::string>* DataColumn =
      m_ByClassColumns[ClassName]->getColumnWithTitle(DataName);
  if (UnitIters.second)
  {
    UnitIters.second->set_value(*DataColumn, DataInfo.second);
  }
}
void DomainIDataAdapterModelImpl::updateDataForClass(Gtk::TreeIter ClassIter)
{
  std::string ClassName = getClassNameFromIter(ClassIter);

  m_ByClassColumns.erase(ClassName);
  m_ByClassUnitsStores.erase(ClassName);

  createDataColumnsAndStoreForClass(ClassName);
  populateStoreForClass(ClassName);
}
void DomainIDataAdapterModelImpl::setSelectedClass(Gtk::TreeIter Iter)
{
  m_RequestedSelectedClass = getClassNameFromIter(Iter);
}

void DomainIDataAdapterModelSub::setCoreRepository(
    const openfluid::core::CoreRepository* CoreRepos)
{
  mp_CoreRepos = CoreRepos;
}
void DomainIDataAdapterModelSub::extractClassNames()
{
  DomainIDataAdapterModelImpl::extractClassNames();
}
std::vector<std::string> DomainIDataAdapterModelSub::getClassNames()
{
  return m_ClassNames;
}
void DomainIDataAdapterModelSub::createDataColumnsAndStoreForClass(
    std::string ClassName)
{
  DomainIDataAdapterModelImpl::createDataColumnsAndStoreForClass(ClassName);
}
std::map<std::string, DomainIDataColumns*> DomainIDataAdapterModelSub::getByClassColumns()
{
  return m_ByClassColumns;
}
std::map<std::string, Glib::RefPtr<BuilderListStore> > DomainIDataAdapterModelSub::getByClassUnitsStores()
{
  return m_ByClassUnitsStores;
}
void DomainIDataAdapterModelSub::populateStoreForClass(std::string ClassName)
{
  DomainIDataAdapterModelImpl::populateStoreForClass(ClassName);
}
Glib::RefPtr<BuilderClassListStore> DomainIDataAdapterModelSub::getClassStore()
{
  return mref_ClassStore;
}

