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
 \file DomainIDataModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainIDataModel.hpp"

#include <boost/foreach.hpp>

DomainIDataModelImpl::DomainIDataModelImpl() :
  mp_CoreRepos(0), m_SelectedClass(""), m_AppRequestedSelection("")
{
}
sigc::signal<void> DomainIDataModelImpl::signal_FromAppDataInit()
{
  return m_signal_FromAppDataInit;
}
sigc::signal<void> DomainIDataModelImpl::signal_FromAppDataReplaced()
{
  return m_signal_FromAppDataReplaced;
}
sigc::signal<void> DomainIDataModelImpl::signal_FromAppDataListChanged()
{
  return m_signal_FromAppDataListChanged;
}
sigc::signal<void> DomainIDataModelImpl::signal_FromAppClassSelectionChanged()
{
  return m_signal_FromAppClassSelectionChanged;
}
void DomainIDataModelImpl::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
  m_signal_FromAppDataInit.emit();
}
const openfluid::core::CoreRepository* DomainIDataModelImpl::getCoreRepos()
{
  if (mp_CoreRepos)
    return mp_CoreRepos;
  return 0;
}
void DomainIDataModelImpl::update()
{
  m_signal_FromAppDataInit.emit();

  if(m_AppRequestedSelection!="")
    m_signal_FromAppClassSelectionChanged.emit();
}
void DomainIDataModelImpl::replaceDataValue(
    std::pair<std::string, int> UnitInfo,
    std::pair<std::string, std::string> DataInfo)
{
  openfluid::core::Unit* Unit = mp_CoreRepos->getUnit(UnitInfo.first,
      UnitInfo.second);
  if (Unit)
  {
    std::string DataName = DataInfo.first;
    std::string NewValue = DataInfo.second;
    std::string Value;
    Unit->getInputData()->getValue(DataName, &Value);
    if (Value != NewValue)
    {
      Unit->getInputData()->replaceValue(DataName, NewValue);
      m_signal_FromAppDataReplaced.emit();
    }
  }
}
void DomainIDataModelImpl::removeData(std::string DataName)
{
  BOOST_FOREACH(openfluid::core::Unit Unit,*mp_CoreRepos->getUnits(m_SelectedClass)->getList())
{  mp_CoreRepos->getUnit(m_SelectedClass, Unit.getID())->getInputData()->removeData(
      DataName);
}
m_signal_FromAppDataListChanged.emit();
}
void DomainIDataModelImpl::addData(std::string DataName)
{
  if (mp_CoreRepos->isUnitsClassExist(m_SelectedClass) && DataName != "")
  {
    BOOST_FOREACH(openfluid::core::Unit Unit,*mp_CoreRepos->getUnits(m_SelectedClass)->getList())
{    mp_CoreRepos->getUnit(m_SelectedClass, Unit.getID())->getInputData()->setValue(
        DataName, "");
  }
  m_signal_FromAppDataListChanged.emit();
}
}
void DomainIDataModelImpl::setCurrentClassSelectionByUser(std::string ClassName)
{
  m_SelectedClass = ClassName;
}
void DomainIDataModelImpl::setCurrentClassSelectionByApp(std::string ClassName)
{
  m_AppRequestedSelection = ClassName;
  m_signal_FromAppClassSelectionChanged.emit();
}
std::string DomainIDataModelImpl::getSelectedClass()
{
  return m_SelectedClass;
}
std::string DomainIDataModelImpl::getAppRequestedClass()
{
  return m_AppRequestedSelection;
}
