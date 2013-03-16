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
 \file AdvancedDomainDescriptor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>

#include <openfluid/fluidx/DomainDescriptor.hpp>
#include <openfluid/fluidx/UnitDescriptor.hpp>
#include <openfluid/fluidx/IDataDescriptor.hpp>
#include <stdexcept>
#include <algorithm>

namespace openfluid {
namespace fluidx {

// =====================================================================
// =====================================================================

AdvancedDomainDescriptor::AdvancedDomainDescriptor(
    openfluid::fluidx::DomainDescriptor& DomainDesc) :
    mp_DomainDesc(&DomainDesc)
{
  dispatchUnits();
  checkUnitRelations();

  dispatchIData();
  checkIDataConsistency();

  dispatchEvents();
}

// =====================================================================
// =====================================================================

AdvancedDomainDescriptor::~AdvancedDomainDescriptor()
{
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::dispatchUnits()
{
  std::list<openfluid::fluidx::UnitDescriptor>* Units =
      &(mp_DomainDesc->getUnits());

  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    if (!m_Units[it->getUnitClass()].insert(
        std::make_pair(it->getUnitID(), BuilderUnit(*it))).second)
      throw openfluid::base::OFException(
          "OpenFLUID-Builder", "AdvancedDomainDescriptor::dispatchUnits",
          "trying to add a Unit that already exists");
  }
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::checkUnitRelations()
{
  std::list<openfluid::fluidx::UnitDescriptor>* Units =
      &(mp_DomainDesc->getUnits());

  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    checkUnitRelations(*it);
  }

}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::checkUnitRelations(
    openfluid::fluidx::UnitDescriptor& Unit)
{
  std::list<openfluid::core::UnitClassID_t>::iterator it;

  std::list<openfluid::core::UnitClassID_t>& Tos = Unit.getUnitsTos();

  for (it = Tos.begin(); it != Tos.end(); ++it)
  {
    try
    {
      getUnit(it->first, it->second);
    }
    catch (openfluid::base::OFException& e)
    {
      std::ostringstream ss;
      ss << "Unit " << it->second << " of class " << it->first
         << " in \"To\" relation of unit " << Unit.getUnitID() << " of class "
         << Unit.getUnitClass() << " doesn't exist";
      throw openfluid::base::OFException(
          "OpenFLUID-Builder", "AdvancedDomainDescriptor::checkUnitRelation",
          ss.str());
    }
  }

  std::list<openfluid::core::UnitClassID_t>& Parents = Unit.getUnitsParents();

  for (it = Parents.begin(); it != Parents.end(); ++it)
  {
    try
    {
      getUnit(it->first, it->second);
    }
    catch (openfluid::base::OFException& e)
    {
      std::ostringstream ss;
      ss << "Unit " << it->second << " of class " << it->first
         << " in \"Parent\" relation of unit " << Unit.getUnitID()
         << " of class " << Unit.getUnitClass() << " doesn't exist";
      throw openfluid::base::OFException(
          "OpenFLUID-Builder", "AdvancedDomainDescriptor::checkUnitRelations",
          ss.str());
    }
  }

}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::dispatchIData()
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
                "OpenFLUID-Builder",
                "AdvancedDomainDescriptor::dispatchIData",
                "trying to add an input data (" + it3->first
                + ") that already exists");
        }
        catch (std::out_of_range& e)
        {
          std::ostringstream ss;
          ss << "trying to add an input data (" << it3->first
             << ") to a Unit that doesn't exist (class " << it->getUnitsClass()
             << " - ID " << it2->first << ")";
          throw openfluid::base::OFException(
              "OpenFLUID-Builder", "AdvancedDomainDescriptor::dispatchIData",
              ss.str());
        }
      }
    }
  }
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::checkIDataConsistency()
{
  for (std::map<std::string, std::set<std::string> >::iterator it =
      m_IDataNames.begin(); it != m_IDataNames.end(); ++it)
  {
    std::string ClassName = it->first;
    std::set<std::string> Names = it->second;

    if (!isClassNameExists(ClassName))
      throw openfluid::base::OFException(
          "OpenFLUID-Builder",
          "AdvancedDomainDescriptor::checkIDataConsistency",
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
              "OpenFLUID-Builder",
              "AdvancedDomainDescriptor::checkIDataConsistency", ss.str());
        }
      }
    }
  }

}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::dispatchEvents()
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
//          "AdvancedDomainDescriptor::dispatchEvents",
//          "trying to add an event to a Unit that doesn't exist");
    }
  }
}

// =====================================================================
// =====================================================================

const std::map<std::string, std::map<int, BuilderUnit> >& AdvancedDomainDescriptor::getUnitsByIdByClass()
{
  return m_Units;
}

// =====================================================================
// =====================================================================

const BuilderUnit& AdvancedDomainDescriptor::getUnit(std::string ClassName,
                                                     int ID)
{
  try
  {
    return m_Units.at(ClassName).at(ID);
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to get a Unit that doesn't exist (UnitClass " << ClassName
       << " ID " << ID << ")";
    throw openfluid::base::OFException("OpenFLUID-Builder",
                                       "AdvancedDomainDescriptor::getUnit",
                                       ss.str());
  }
}

// =====================================================================
// =====================================================================

const openfluid::fluidx::UnitDescriptor& AdvancedDomainDescriptor::getUnitDescriptor(
    std::string ClassName, int ID)
{
  return *(getUnit(ClassName, ID).mp_UnitDesc);
}

// =====================================================================
// =====================================================================

std::set<int> AdvancedDomainDescriptor::getIDsOfClass(std::string ClassName)
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

bool AdvancedDomainDescriptor::isClassNameExists(std::string ClassName)
{
  return m_Units.count(ClassName);
}

// =====================================================================
// =====================================================================

std::set<std::string> AdvancedDomainDescriptor::getClassNames()
{
  std::set<std::string> Classes;

  for (std::map<std::string, std::map<int, BuilderUnit> >::iterator it =
      m_Units.begin(); it != m_Units.end(); ++it)
    Classes.insert(it->first);

  return Classes;
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::addUnit(
    openfluid::fluidx::UnitDescriptor* UnitDesc)
{
  std::string ClassName = UnitDesc->getUnitClass();
  int ID = UnitDesc->getUnitID();

  // add in m_Units
  if (!m_Units[ClassName].insert(std::make_pair(ID, BuilderUnit(*UnitDesc))).second)
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addUnit",
        "trying to add a Unit that already exists");

  //check relations
  checkUnitRelations(*UnitDesc);

  // add in DomainDesc
  mp_DomainDesc->getUnits().push_back(*UnitDesc);

  // add Input Data
  std::set<std::string> IDataNames = getInputDataNames(ClassName);

  if (IDataNames.empty())
    return;

  openfluid::fluidx::InputDataDescriptor IDataDesc;

  IDataDesc.getUnitsClass() = ClassName;
  BuilderUnit& BUnit = m_Units.at(ClassName).at(ID);

  for (std::set<std::string>::iterator it = IDataNames.begin();
      it != IDataNames.end(); ++it)
  {
    IDataDesc.getColumnsOrder().push_back(*it);
    IDataDesc.getData()[ID][*it] = "-";
    BUnit.m_IData[*it] = new std::string("-");
  }

  mp_DomainDesc->getInputData().push_back(IDataDesc);
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::deleteUnit(std::string ClassName, int ID)
{
  // delete in m_Units
  m_Units[ClassName].erase(ID);
  if (m_Units.at(ClassName).empty())
    m_Units.erase(ClassName);

  // delete in UnitDesc list and in other units relations
  std::list<openfluid::fluidx::UnitDescriptor>& Units =
      mp_DomainDesc->getUnits();
  openfluid::core::UnitClassID_t Unit = std::make_pair(ClassName, ID);
  std::list<openfluid::fluidx::UnitDescriptor>::iterator it = Units.begin();
  while (it != Units.end())
  {
    if (it->getUnitClass() == ClassName && (int) it->getUnitID() == ID)
    {
      it = Units.erase(it);
    }
    else
    {
      std::list<openfluid::core::UnitClassID_t>& Tos = it->getUnitsTos();
      std::list<openfluid::core::UnitClassID_t>::iterator Found = std::find(
          Tos.begin(), Tos.end(), Unit);
      if (Found != Tos.end())
        Tos.remove(Unit);

      std::list<openfluid::core::UnitClassID_t>& Parents =
          it->getUnitsParents();
      Found = std::find(Parents.begin(), Parents.end(), Unit);
      if (Found != Parents.end())
        Parents.remove(Unit);

      ++it;
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

std::string& AdvancedDomainDescriptor::getInputData(std::string ClassName,
                                                    int ID,
                                                    std::string IDataName)
{
  try
  {
    return *(m_Units.at(ClassName).at(ID).m_IData.at(IDataName));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::getInputData",
        "trying to get an Input data that doesn't exist");
  }
}

// =====================================================================
// =====================================================================

std::set<std::string> AdvancedDomainDescriptor::getInputDataNames(
    std::string ClassName)
{
  std::set<std::string> Names;

  if (m_IDataNames.count(ClassName))
    Names = m_IDataNames.at(ClassName);

  return Names;
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::addInputData(std::string ClassName,
                                            std::string IDataName,
                                            std::string DefaultValue)
{
  if (!isClassNameExists(ClassName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addInputData",
        "trying to add an Input data to a Class that doesn't exist");

  if (getInputDataNames(ClassName).count(IDataName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addInputData",
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

void AdvancedDomainDescriptor::deleteInputData(std::string ClassName,
                                               std::string IDataName)
{
  if (!isClassNameExists(ClassName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::deleteInputData",
        "trying to delete an Input data from a Class that doesn't exist");

  if (!getInputDataNames(ClassName).count(IDataName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::deleteInputData",
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

void AdvancedDomainDescriptor::renameInputData(std::string ClassName,
                                               std::string OldIDataName,
                                               std::string NewIDataName)
{
  if (OldIDataName == NewIDataName)
    return;

  if (!isClassNameExists(ClassName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::renameInputData",
        "trying to rename an Input data of a Class that doesn't exist");

  if (!getInputDataNames(ClassName).count(OldIDataName))
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::renameInputData",
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

const std::list<openfluid::core::UnitClassID_t>& AdvancedDomainDescriptor::getUnitsToOf(
    const openfluid::core::UnitClassID_t Unit)
{
  try
  {
    return m_Units.at(Unit.first).at(Unit.second).mp_UnitDesc->getUnitsTos();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::getUnitsToOfPriv",
        "trying to get relations of a Unit that doesn't exist");
  }
}

// =====================================================================
// =====================================================================

const std::list<openfluid::core::UnitClassID_t>& AdvancedDomainDescriptor::getUnitsParentsOf(
    const openfluid::core::UnitClassID_t Unit)
{
  try
  {
    return m_Units.at(Unit.first).at(Unit.second).mp_UnitDesc->getUnitsParents();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::getUnitsParentsOfPriv",
        "trying to get relations of a Unit that doesn't exist");
  }
}

// =====================================================================
// =====================================================================

std::list<openfluid::core::UnitClassID_t> AdvancedDomainDescriptor::getUnitsFromOf(
    const openfluid::core::UnitClassID_t Unit)
{
  std::list<openfluid::core::UnitClassID_t> Froms;

  std::list<openfluid::fluidx::UnitDescriptor>* Units =
      &(mp_DomainDesc->getUnits());
  std::list<openfluid::core::UnitClassID_t>* Tos;

  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    Tos = &(it->getUnitsTos());

    if (std::find(Tos->begin(), Tos->end(), Unit) != Tos->end())
    {
      openfluid::core::UnitClassID_t From = std::make_pair(it->getUnitClass(),
                                                           it->getUnitID());
      Froms.push_back(From);
    }
  }

  return Froms;
}

// =====================================================================
// =====================================================================

std::list<openfluid::core::UnitClassID_t> AdvancedDomainDescriptor::getUnitsChildrenOf(
    const openfluid::core::UnitClassID_t Unit)
{
  std::list<openfluid::core::UnitClassID_t> Children;

  std::list<openfluid::fluidx::UnitDescriptor>* Units =
      &(mp_DomainDesc->getUnits());
  std::list<openfluid::core::UnitClassID_t>* Parents;

  for (std::list<openfluid::fluidx::UnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    Parents = &(it->getUnitsParents());

    if (std::find(Parents->begin(), Parents->end(), Unit) != Parents->end())
    {
      openfluid::core::UnitClassID_t Child = std::make_pair(it->getUnitClass(),
                                                            it->getUnitID());
      Children.push_back(Child);
    }
  }

  return Children;
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::addFromToRelation(
    const openfluid::core::UnitClassID_t FromUnit,
    const openfluid::core::UnitClassID_t ToUnit)
{
  openfluid::fluidx::UnitDescriptor* UFrom;

  try
  {
    UFrom = m_Units.at(FromUnit.first).at(FromUnit.second).mp_UnitDesc;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a From/To relation from a Unit that doesn't exist (UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addFromToRelation",
        ss.str());
  }

  try
  {
    m_Units.at(ToUnit.first).at(ToUnit.second);
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a From/To relation to a Unit that doesn't exist (UnitClass "
       << ToUnit.first << " ID " << ToUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addFromToRelation",
        ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Tos = UFrom->getUnitsTos();
  if (std::find(Tos.begin(), Tos.end(), ToUnit) == Tos.end())
    Tos.push_back(ToUnit);
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::removeFromToRelation(
    const openfluid::core::UnitClassID_t FromUnit,
    const openfluid::core::UnitClassID_t ToUnit)
{
  openfluid::fluidx::UnitDescriptor* UFrom;

  try
  {
    UFrom = m_Units.at(FromUnit.first).at(FromUnit.second).mp_UnitDesc;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a From/To relation from a Unit that doesn't exist (UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::removeFromToRelation",
        ss.str());
  }

  try
  {
    m_Units.at(ToUnit.first).at(ToUnit.second);
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a From/To relation to a Unit that doesn't exist (UnitClass "
       << ToUnit.first << " ID " << ToUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::removeFromToRelation",
        ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Tos = UFrom->getUnitsTos();
  if (std::find(Tos.begin(), Tos.end(), ToUnit) != Tos.end())
    Tos.remove(ToUnit);
  else
  {
    std::ostringstream ss;
    ss << "trying to remove a From/To relation that doesn't exist (from UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << " to UnitClass "
       << ToUnit.first << " ID " << ToUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::removeFromToRelation",
        ss.str());
  }
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::addParentChildRelation(
    const openfluid::core::UnitClassID_t ParentUnit,
    const openfluid::core::UnitClassID_t ChildUnit)
{
  openfluid::fluidx::UnitDescriptor* UChild;

  try
  {
    UChild = m_Units.at(ChildUnit.first).at(ChildUnit.second).mp_UnitDesc;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a Parent/Child relation to a Unit that doesn't exist (UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addParentChildRelation",
        ss.str());
  }

  try
  {
    m_Units.at(ParentUnit.first).at(ParentUnit.second);
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a Parent/Child relation from a Unit that doesn't exist (UnitClass "
       << ParentUnit.first << " ID " << ParentUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::addParentChildRelation",
        ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Parents =
      UChild->getUnitsParents();
  if (std::find(Parents.begin(), Parents.end(), ParentUnit) == Parents.end())
    Parents.push_back(ParentUnit);
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::removeParentChildRelation(
    const openfluid::core::UnitClassID_t ParentUnit,
    const openfluid::core::UnitClassID_t ChildUnit)
{
  openfluid::fluidx::UnitDescriptor* UChild;

  try
  {
    UChild = m_Units.at(ChildUnit.first).at(ChildUnit.second).mp_UnitDesc;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a Parent/Child relation to a Unit that doesn't exist (UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder",
        "AdvancedDomainDescriptor::removeParentChildRelation", ss.str());
  }

  try
  {
    m_Units.at(ParentUnit.first).at(ParentUnit.second);
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a Parent/Child relation from a Unit that doesn't exist (UnitClass "
       << ParentUnit.first << " ID " << ParentUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder",
        "AdvancedDomainDescriptor::removeParentChildRelation", ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Parents =
      UChild->getUnitsParents();
  if (std::find(Parents.begin(), Parents.end(), ParentUnit) != Parents.end())
    Parents.remove(ParentUnit);
  else
  {
    std::ostringstream ss;
    ss << "trying to remove a Parent/Child relation that doesn't exist (from UnitClass "
       << ParentUnit.first << " ID " << ParentUnit.second << " to UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder",
        "AdvancedDomainDescriptor::removeParentChildRelation", ss.str());
  }

}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::clearRelations(
    const openfluid::core::UnitClassID_t Unit)
{
  openfluid::fluidx::UnitDescriptor* U;

  try
  {
    U = m_Units.at(Unit.first).at(Unit.second).mp_UnitDesc;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove relations to a Unit that doesn't exist (UnitClass "
       << Unit.first << " ID " << Unit.second << ")";
    throw openfluid::base::OFException(
        "OpenFLUID-Builder", "AdvancedDomainDescriptor::removeAllRelations",
        ss.str());
  }

  std::list<openfluid::core::UnitClassID_t> Froms = getUnitsFromOf(Unit);
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Froms.begin();
      it != Froms.end(); ++it)
  {
    removeFromToRelation(*it, Unit);
  }

  std::list<openfluid::core::UnitClassID_t> Children = getUnitsChildrenOf(Unit);
  for (std::list<openfluid::core::UnitClassID_t>::iterator it =
      Children.begin(); it != Children.end(); ++it)
  {
    removeParentChildRelation(Unit, *it);
  }

  U->getUnitsTos().clear();
  U->getUnitsParents().clear();
}

// =====================================================================
// =====================================================================

void AdvancedDomainDescriptor::clearDomain()
{
  mp_DomainDesc->getUnits().clear();
  mp_DomainDesc->getInputData().clear();
  mp_DomainDesc->getEvents().clear();
  m_Units.clear();
  m_IDataNames.clear();
}

// =====================================================================
// =====================================================================

}
} // namespaces
