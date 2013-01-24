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
 \file BuilderDomain.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "BuilderDomain.hpp"

#include <openfluid/fluidx/DomainDescriptor.hpp>
#include <openfluid/fluidx/UnitDescriptor.hpp>
#include <openfluid/fluidx/IDataDescriptor.hpp>
#include <stdexcept>

// =====================================================================
// =====================================================================

BuilderDomain::BuilderDomain() :
    mp_DomainDesc(0)
{
}

// =====================================================================
// =====================================================================

BuilderDomain::~BuilderDomain()
{
}

// =====================================================================
// =====================================================================

void BuilderDomain::setDomainDescriptor(
    openfluid::fluidx::DomainDescriptor& DomainDesc)
{
  mp_DomainDesc = &DomainDesc;

  dispatchUnits();

  dispatchIData();
  checkIDataConsistency();

  dispatchEvents();
}

// =====================================================================
// =====================================================================

void BuilderDomain::dispatchUnits()
{
  std::list<openfluid::fluidx::UnitDescriptor>* Units =
      &(mp_DomainDesc->getUnits());

  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    if (!m_Units[it->getUnitClass()].insert(
        std::make_pair(it->getUnitID(), BuilderUnit(*it))).second)
      throw openfluid::base::OFException(
          "OpenFLUID-Builder", "BuilderDomain::dispatchUnits",
          "trying to add a Unit that already exists");
  }
}

// =====================================================================
// =====================================================================

void BuilderDomain::dispatchIData()
{
  std::list<openfluid::fluidx::InputDataDescriptor>* IData =
      &(mp_DomainDesc->getInputData());

  for (std::list<openfluid::fluidx::InputDataDescriptor>::iterator it =
      IData->begin(); it != IData->end(); ++it)
  {
    std::map<openfluid::core::UnitID_t,
        openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>* Data =
        &(it->getData());

    for (std::map<openfluid::core::UnitID_t,
        openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>::iterator it2 =
        Data->begin(); it2 != Data->end(); ++it2)
    {
      std::map<openfluid::core::InputDataName_t, std::string>* Data =
          &(it2->second);
      for (std::map<openfluid::core::InputDataName_t, std::string>::iterator it3 =
          Data->begin(); it3 != Data->end(); ++it3)
      {
        m_IDataNames[it->getUnitsClass()].insert(it3->first);

        try
        {
          if (!m_Units.at(it->getUnitsClass()).at(it2->first).m_IData.insert(
              std::make_pair(it3->first, &it3->second)).second)
            throw openfluid::base::OFException(
                "OpenFLUID-Builder", "BuilderDomain::dispatchIData",
                "trying to add an input data that already exists");
        }
        catch (std::out_of_range& e)
        {
          throw openfluid::base::OFException(
              "OpenFLUID-Builder", "BuilderDomain::dispatchIData",
              "trying to add an input data to a Unit that doesn't exist");
        }
      }
    }
  }
}

// =====================================================================
// =====================================================================

void BuilderDomain::checkIDataConsistency()
{
  for (std::map<std::string, std::set<std::string> >::iterator it =
      m_IDataNames.begin(); it != m_IDataNames.end(); ++it)
  {
    std::string ClassName = it->first;
    std::set<std::string> Names = it->second;

    if (!isClassNameExists(ClassName))
      throw openfluid::base::OFException(
          "OpenFLUID-Builder", "BuilderDomain::checkIDataConsistency",
          "class " + ClassName + " doesn't exist");

    std::map<int, BuilderUnit>* Units = &(m_Units.at(ClassName));

    for (std::set<std::string>::iterator itName = Names.begin();
        itName != Names.end(); ++itName)
    {
      std::string IDataName = *itName;

      for (std::map<int, BuilderUnit>::iterator itU = Units->begin();
          itU != Units->end(); ++itU)
      {
        int ID = itU->first;

        if (!itU->second.m_IData.count(IDataName))
        {
          std::ostringstream ss;
          ss << "Input data " << IDataName << " doesn't exist for Unit " << ID
             << " of class " << ClassName;
          throw openfluid::base::OFException(
              "OpenFLUID-Builder", "BuilderDomain::checkIDataConsistency",
              ss.str());
        }
      }
    }
  }

}

// =====================================================================
// =====================================================================

void BuilderDomain::dispatchEvents()
{
  std::list<openfluid::fluidx::EventDescriptor>* Events =
      &(mp_DomainDesc->getEvents());

  for (std::list<openfluid::fluidx::EventDescriptor>::iterator it =
      Events->begin(); it != Events->end(); ++it)
  {
    try
    {
      m_Units.at(it->getUnitClass()).at(it->getUnitID()).m_Events.push_back(
          &(it->getEvent()));
    }
    catch (std::out_of_range& e)
    {
      // Factory does nothing in this case
//      throw openfluid::base::OFException(
//          "OpenFLUID-Builder",
//          "BuilderDomain::dispatchEvents",
//          "trying to add an event to a Unit that doesn't exist");
    }
  }
}

// =====================================================================
// =====================================================================

const std::map<std::string, std::map<int, BuilderUnit> >& BuilderDomain::getUnitsByIdByClass()
{
  return m_Units;
}

// =====================================================================
// =====================================================================

const BuilderUnit& BuilderDomain::getUnit(std::string ClassName, int ID)
{
  try
  {
    return m_Units.at(ClassName).at(ID);
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::getUnit",
        "trying to get a Unit that doesn't exist");
  }
}

// =====================================================================
// =====================================================================

const openfluid::fluidx::UnitDescriptor& BuilderDomain::getUnitDescriptor(
    std::string ClassName, int ID)
{
  return *(getUnit(ClassName, ID).mp_UnitDesc);
}

// =====================================================================
// =====================================================================

std::set<int> BuilderDomain::getIDsOfClass(std::string ClassName)
{
  std::set<int> IDs;

  if (isClassNameExists(ClassName))
  {
    for (std::map<int, BuilderUnit>::iterator it =
        m_Units.at(ClassName).begin(); it != m_Units.at(ClassName).end(); ++it)
      IDs.insert(it->first);
  }

  return IDs;
}

// =====================================================================
// =====================================================================

bool BuilderDomain::isClassNameExists(std::string ClassName)
{
  return m_Units.count(ClassName);
}

// =====================================================================
// =====================================================================

void BuilderDomain::addUnit(openfluid::fluidx::UnitDescriptor* UnitDesc)
{
  // add in m_Units
  if (!m_Units[UnitDesc->getUnitClass()].insert(
      std::make_pair(UnitDesc->getUnitID(), BuilderUnit(*UnitDesc))).second)
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::addUnit",
        "trying to add a Unit that already exists");

  // add in DomainDesc
  mp_DomainDesc->getUnits().push_back(*UnitDesc);
}

// =====================================================================
// =====================================================================

void BuilderDomain::deleteUnit(std::string ClassName, int ID)
{
  // delete in m_Units
  m_Units[ClassName].erase(ID);
  if (m_Units.at(ClassName).empty())
    m_Units.erase(ClassName);

  // delete in UnitDesc list
  std::list<openfluid::fluidx::UnitDescriptor>* Units =
      &(mp_DomainDesc->getUnits());
  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    if (it->getUnitClass() == ClassName && (int) it->getUnitID() == ID)
    {
      Units->erase(it);
      break;
    }
  }

  // delete in IDataDesc list and in IDataNames
  std::list<openfluid::fluidx::InputDataDescriptor>* IData =
      &(mp_DomainDesc->getInputData());
  std::list<openfluid::fluidx::InputDataDescriptor>::iterator itData =
      IData->begin();
  while (itData != IData->end())
  {
    if (itData->getUnitsClass() == ClassName)
    {
      itData->getData().erase(ID);

      if (itData->getData().empty())
      {
        itData = IData->erase(itData);
        m_IDataNames.erase(ClassName);
      }
      else
        itData++;
    }
    else
      itData++;
  }

  // delete in EventDesc list
  std::list<openfluid::fluidx::EventDescriptor>* Events =
      &(mp_DomainDesc->getEvents());
  std::list<openfluid::fluidx::EventDescriptor>::iterator itEv =
      Events->begin();
  while (itEv != Events->end())
  {
    if (itEv->getUnitClass() == ClassName && (int) itEv->getUnitID() == ID)
      itEv = Events->erase(itEv);
    else
      itEv++;
  }

}

// =====================================================================
// =====================================================================

std::string& BuilderDomain::getInputData(std::string ClassName, int ID,
                                         std::string IDataName)
{
  try
  {
    return *(m_Units.at(ClassName).at(ID).m_IData.at(IDataName));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::getInputData",
        "trying to get an Input data that doesn't exist");
  }
}

// =====================================================================
// =====================================================================

std::set<std::string> BuilderDomain::getInputDataNames(std::string ClassName)
{
  std::set<std::string> Names;

  if (m_IDataNames.count(ClassName))
    Names = m_IDataNames.at(ClassName);

  return Names;
}

// =====================================================================
// =====================================================================

void BuilderDomain::addInputData(std::string ClassName, std::string IDataName,
                                 std::string DefaultValue)
{
  if (!isClassNameExists(ClassName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::addInputData",
        "trying to add an Input data to a Class that doesn't exist");

  if (getInputDataNames(ClassName).count(IDataName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::addInputData",
        "trying to add an Input data that already exists");

  // add in DomainDesc
  openfluid::fluidx::InputDataDescriptor IDataDesc;

  IDataDesc.getUnitsClass() = ClassName;
  IDataDesc.getColumnsOrder().push_back(IDataName);

  std::set<int> IDs = getIDsOfClass(ClassName);
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
    IDataDesc.getData()[*it][IDataName] = DefaultValue;

  mp_DomainDesc->getInputData().push_back(IDataDesc);

  // add in m_Units
  std::map<openfluid::core::UnitID_t,
      openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>* Data =
      &(mp_DomainDesc->getInputData().end().operator --()->getData());

  for (std::map<openfluid::core::UnitID_t,
      openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>::iterator it =
      Data->begin(); it != Data->end(); ++it)
  {
    m_Units.at(ClassName).at(it->first).m_IData[IDataName] = &(it->second.at(
        IDataName));
  }

  // add in IDataNames
  m_IDataNames[ClassName].insert(IDataName);

}

// =====================================================================
// =====================================================================

void BuilderDomain::deleteInputData(std::string ClassName,
                                    std::string IDataName)
{
  if (!isClassNameExists(ClassName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::deleteInputData",
        "trying to delete an Input data from a Class that doesn't exist");

  if (!getInputDataNames(ClassName).count(IDataName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::deleteInputData",
        "trying to delete an Input data that doesn't exist");

  // delete in m_Units
  for (std::map<int, BuilderUnit>::iterator it = m_Units.at(ClassName).begin();
      it != m_Units.at(ClassName).end(); ++it)
    it->second.m_IData.erase(IDataName);

  // delete in DomainDesc
  std::list<openfluid::fluidx::InputDataDescriptor>* IData =
      &(mp_DomainDesc->getInputData());

  for (std::list<openfluid::fluidx::InputDataDescriptor>::iterator it =
      IData->begin(); it != IData->end(); ++it)
  {
    if (it->getUnitsClass() == ClassName)
    {
      std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>* DataById =
          &(it->getData());

      for (std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>::iterator it2 =
          DataById->begin(); it2 != DataById->end(); ++it2)
      {
        if (it2->second.count(IDataName))
          it2->second.erase(IDataName);
      }
    }
  }

  // delete in IDataNames
  m_IDataNames.at(ClassName).erase(IDataName);
}

// =====================================================================
// =====================================================================

void BuilderDomain::renameInputData(std::string ClassName,
                                    std::string OldIDataName,
                                    std::string NewIDataName)
{
  if (OldIDataName == NewIDataName)
    return;

  if (!isClassNameExists(ClassName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::renameInputData",
        "trying to rename an Input data of a Class that doesn't exist");

  if (!getInputDataNames(ClassName).count(OldIDataName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "BuilderDomain::renameInputData",
        "trying to rename an Input data that doesn't exist");

  //rename in DomainDesc
  std::map<unsigned int, std::string*> DomainDescIData;

  std::list<openfluid::fluidx::InputDataDescriptor>* IData =
      &(mp_DomainDesc->getInputData());

  for (std::list<openfluid::fluidx::InputDataDescriptor>::iterator it =
      IData->begin(); it != IData->end(); ++it)
  {
    if (it->getUnitsClass() == ClassName)
    {
      std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>* DataById =
          &(it->getData());

      for (std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::InputDataDescriptor::InputDataNameValue_t>::iterator it2 =
          DataById->begin(); it2 != DataById->end(); ++it2)
      {
        if (it2->second.count(OldIDataName))
        {
          std::string Value = it2->second.at(OldIDataName);
          it2->second.erase(OldIDataName);
          it2->second[NewIDataName] = Value;
          DomainDescIData[it2->first] = &(it2->second[NewIDataName]);
        }
      }
    }
  }

  // rename in m_Units
  for (std::map<int, BuilderUnit>::iterator it = m_Units.at(ClassName).begin();
      it != m_Units.at(ClassName).end(); ++it)
  {
    it->second.m_IData.erase(OldIDataName);
    it->second.m_IData[NewIDataName] = DomainDescIData[it->first];
  }

  //rename in IDataNames
  m_IDataNames.at(ClassName).erase(OldIDataName);
  m_IDataNames.at(ClassName).insert(NewIDataName);
}

// =====================================================================
// =====================================================================

