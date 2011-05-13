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
 \file DomainUnitCreationModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitCreationModel.hpp"

#include <glibmm/i18n.h>


void DomainUnitCreationModelImpl::initialize()
{
  m_ClassNames.clear();

  mp_Unit = 0;

  m_ClassName = "";
  m_Id = 1;
}
void DomainUnitCreationModelImpl::extractClassNames()
{
  m_ClassNames.clear();

  const openfluid::core::UnitsListByClassMap_t* UnitsByClass =
      mp_CoreRepos->getUnitsByClass();
  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      UnitsByClass->begin(); it != UnitsByClass->end(); ++it)
  {
    m_ClassNames.push_back(it->first);
  }
}
int DomainUnitCreationModelImpl::getNextId()
{
  //TODO : sort by Id before
  openfluid::core::UnitsListByClassMap_t::const_iterator it =
      mp_CoreRepos->getUnitsByClass()->find(m_ClassName);
  if (it != mp_CoreRepos->getUnitsByClass()->end())
  {
    openfluid::core::UnitsList_t::const_iterator itt =
        it->second.getList()->end();
    itt--;
    if (itt != it->second.getList()->end())
    {
      int LastId = itt->getID();
      return (LastId + 1);
    }
  }
  return 1;
}
bool DomainUnitCreationModelImpl::checkUnitInfo()
{
  if (m_ClassName == "")
  {
    m_signal_ErrorDialogAsked.emit(_("Class Unit can't be empty"));
    return false;
  }
  if (mp_CoreRepos->getUnit(m_ClassName, m_Id))
  {
    m_signal_ErrorDialogAsked.emit(_("This Id stil exists in this Class"));
    return false;
  }
  return true;
}
void DomainUnitCreationModelImpl::createUnit()
{
  mp_Unit = new openfluid::core::Unit(m_ClassName, m_Id, 1,
      openfluid::core::Unit::DESCRIPTOR);
}

DomainUnitCreationModelImpl::DomainUnitCreationModelImpl()
{
  initialize();
}
sigc::signal<void> DomainUnitCreationModelImpl::signal_InitForCreationAsked()
{
  return m_signal_InitForCreationAsked;
}
sigc::signal<void> DomainUnitCreationModelImpl::signal_ShowDialogAsked()
{
  return m_signal_ShowDialogAsked;
}
sigc::signal<void> DomainUnitCreationModelImpl::signal_CloseDialogAsked()
{
  return m_signal_CloseDialogAsked;
}
sigc::signal<void, std::string> DomainUnitCreationModelImpl::signal_ErrorDialogAsked()
{
  return m_signal_ErrorDialogAsked;
}
void DomainUnitCreationModelImpl::setEngineRequirements(
    openfluid::core::CoreRepository& CoreRepos)
{
  mp_CoreRepos = &CoreRepos;
}
void DomainUnitCreationModelImpl::initialize(std::string SelectedClassName)
{
  initialize();
  extractClassNames();
  if (SelectedClassName != "")
  {
    m_ClassName = SelectedClassName;
    m_Id = getNextId();
  }
  m_signal_InitForCreationAsked.emit();
}
void DomainUnitCreationModelImpl::showDialog()
{
  m_signal_ShowDialogAsked.emit();
}
openfluid::core::Unit* DomainUnitCreationModelImpl::getUnit()
{
  return mp_Unit;
}
std::vector<std::string> DomainUnitCreationModelImpl::getClassNames()
{
  return m_ClassNames;
}
void DomainUnitCreationModelImpl::setClass(std::string ClassName)
{
  m_ClassName = ClassName;
}
void DomainUnitCreationModelImpl::setId(int Id)
{
  m_Id = Id;
}
std::string DomainUnitCreationModelImpl::getClass()
{
  return m_ClassName;
}
int DomainUnitCreationModelImpl::getId()
{
  return m_Id;
}
void DomainUnitCreationModelImpl::save()
{
  if (checkUnitInfo())
  {
    createUnit();
    m_signal_CloseDialogAsked.emit();
  }
}

void DomainUnitCreationModelSub::initialize()
{
  DomainUnitCreationModelImpl::initialize();
}
void DomainUnitCreationModelSub::initialize(std::string SelectedClassName)
{
  DomainUnitCreationModelImpl::initialize(SelectedClassName);
}
void DomainUnitCreationModelSub::extractClassNames()
{
  DomainUnitCreationModelImpl::extractClassNames();
}
int DomainUnitCreationModelSub::getNextId()
{
  return DomainUnitCreationModelImpl::getNextId();
}
bool DomainUnitCreationModelSub::checkUnitInfo()
{
  return DomainUnitCreationModelImpl::checkUnitInfo();
}
void DomainUnitCreationModelSub::createUnit()
{
  DomainUnitCreationModelImpl::createUnit();
}
void DomainUnitCreationModelSub::setClassName(std::string ClassName)
{
  m_ClassName = ClassName;
}
