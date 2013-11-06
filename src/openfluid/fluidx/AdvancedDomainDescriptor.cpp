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
#include <openfluid/fluidx/AttributesDescriptor.hpp>
#include <stdexcept>
#include <algorithm>

namespace openfluid { namespace fluidx {


AdvancedDomainDescriptor::AdvancedDomainDescriptor(
    openfluid::fluidx::DomainDescriptor& DomainDesc) :
    mp_DomainDesc(&DomainDesc)
{
  dispatchUnits();
  checkUnitRelations();

  dispatchAttributes();
  checkAttributesConsistency();

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
        std::make_pair(it->getUnitID(), AdvancedUnitDescriptor(*it))).second)
      throw openfluid::base::FrameworkException("AdvancedDomainDescriptor::dispatchUnits",
                                                "trying to add a Unit that already exists");
  }
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::checkUnitRelations() const
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
    openfluid::fluidx::UnitDescriptor& Unit) const
{
  std::list<openfluid::core::UnitClassID_t>::iterator it;

  std::list<openfluid::core::UnitClassID_t>& Tos = Unit.getUnitsTos();

  for (it = Tos.begin(); it != Tos.end(); ++it)
  {
    try
    {
      getUnit(it->first, it->second);
    }
    catch (openfluid::base::FrameworkException& e)
    {
      std::ostringstream ss;
      ss << "Unit " << it->second << " of class " << it->first
         << " in \"To\" relation of unit " << Unit.getUnitID() << " of class "
         << Unit.getUnitClass() << " doesn't exist";
      throw openfluid::base::FrameworkException(
          "AdvancedDomainDescriptor::checkUnitRelation",
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
    catch (openfluid::base::FrameworkException& e)
    {
      std::ostringstream ss;
      ss << "Unit " << it->second << " of class " << it->first
         << " in \"Parent\" relation of unit " << Unit.getUnitID()
         << " of class " << Unit.getUnitClass() << " doesn't exist";
      throw openfluid::base::FrameworkException(
          "AdvancedDomainDescriptor::checkUnitRelations",
          ss.str());
    }
  }

}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::dispatchAttributes()
{
  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->getAttributes());

  for (std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin(); it != Attrs->end(); ++it)
  {
    std::map<openfluid::core::UnitID_t,
        openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* Data =
        &(it->getAttributes());

    for (std::map<openfluid::core::UnitID_t,
        openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>::iterator it2 =
        Data->begin(); it2 != Data->end(); ++it2)
    {
      std::map<openfluid::core::AttributeName_t, std::string>* Data =
          &(it2->second);
      for (std::map<openfluid::core::AttributeName_t, std::string>::iterator it3 =
          Data->begin(); it3 != Data->end(); ++it3)
      {
        m_AttrsNames[it->getUnitsClass()].insert(it3->first);

        try
        {
          if (!m_Units.at(it->getUnitsClass()).at(it2->first).Attributes.insert(
              std::make_pair(it3->first, &it3->second)).second)
            throw openfluid::base::FrameworkException(
                "AdvancedDomainDescriptor::dispatchAttributes",
                "trying to add an attribute (" + it3->first
                + ") that already exists");
        }
        catch (std::out_of_range& e)
        {
          std::ostringstream ss;
          ss << "trying to add an attribute (" << it3->first
             << ") to a Unit that doesn't exist (class " << it->getUnitsClass()
             << " - ID " << it2->first << ")";
          throw openfluid::base::FrameworkException(
              "AdvancedDomainDescriptor::dispatchAttributes",
              ss.str());
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::checkAttributesConsistency() const
{
  for (std::map<std::string, std::set<std::string> >::const_iterator it =
      m_AttrsNames.begin(); it != m_AttrsNames.end(); ++it)
  {
    std::string ClassName = it->first;
    std::set<std::string> Names = it->second;

    if (!isClassNameExists(ClassName))
      throw openfluid::base::FrameworkException(
          "AdvancedDomainDescriptor::checkAttributesConsistency",
          "class " + ClassName + " doesn't exist");

    const std::map<int, AdvancedUnitDescriptor>* Units = &(m_Units.at(ClassName));

    for (std::set<std::string>::iterator itName = Names.begin();
        itName != Names.end(); ++itName)
    {
      std::string AttrName = *itName;

      for (std::map<int, AdvancedUnitDescriptor>::const_iterator itU = Units->begin();
          itU != Units->end(); ++itU)
      {
        int ID = itU->first;

        if (!itU->second.Attributes.count(AttrName))
        {
          std::ostringstream ss;
          ss << "Attribute " << AttrName << " doesn't exist for Unit " << ID
             << " of class " << ClassName;
          throw openfluid::base::FrameworkException(
              "AdvancedDomainDescriptor::checkAttributesConsistency", ss.str());
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
      m_Units.at(it->getUnitClass()).at(it->getUnitID()).Events.push_back(
          &(it->getEvent()));
    }
    catch (std::out_of_range& e)
    {
      // Factory does nothing in this case
//      throw openfluid::base::FrameworkException(
//          "OpenFLUID-Framework",
//          "AdvancedDomainDescriptor::dispatchEvents",
//          "trying to add an event to a Unit that doesn't exist");
    }
  }
}


// =====================================================================
// =====================================================================


bool AdvancedDomainDescriptor::isUnitExist(const std::string& ClassName, int ID) const
{
  std::map<std::string, std::map<int, AdvancedUnitDescriptor> >::const_iterator it = m_Units.find(ClassName);

  return (it != m_Units.end() && (*it).second.find(ID) != (*it).second.end());
}


// =====================================================================
// =====================================================================


const std::map<std::string, std::map<int, AdvancedUnitDescriptor> >& AdvancedDomainDescriptor::getUnitsByIdByClass() const
{
  return m_Units;
}


// =====================================================================
// =====================================================================


const AdvancedUnitDescriptor& AdvancedDomainDescriptor::getUnit(const std::string& ClassName,
                                                     int ID) const
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
    throw openfluid::base::FrameworkException(
                                       "AdvancedDomainDescriptor::getUnit",
                                       ss.str());
  }
}


// =====================================================================
// =====================================================================

const openfluid::fluidx::UnitDescriptor& AdvancedDomainDescriptor::getUnitDescriptor(const std::string& ClassName,
                                                                                     int ID) const
{
  return *(getUnit(ClassName, ID).UnitDescriptor);
}


// =====================================================================
// =====================================================================


std::set<int> AdvancedDomainDescriptor::getIDsOfClass(const std::string& ClassName) const
{
  std::set<int> IDs;

  if (isClassNameExists(ClassName))
  {
    for (std::map<int, AdvancedUnitDescriptor>::const_iterator it =
        m_Units.at(ClassName).begin(); it != m_Units.at(ClassName).end(); ++it)
      IDs.insert(it->first);
  }

  return IDs;
}


// =====================================================================
// =====================================================================


bool AdvancedDomainDescriptor::isClassNameExists(const std::string& ClassName) const
{
  return m_Units.count(ClassName);
}


// =====================================================================
// =====================================================================


std::set<std::string> AdvancedDomainDescriptor::getClassNames() const
{
  std::set<std::string> Classes;

  for (std::map<std::string, std::map<int, AdvancedUnitDescriptor> >::const_iterator it =
      m_Units.begin(); it != m_Units.end(); ++it)
    Classes.insert(it->first);

  return Classes;
}


// =====================================================================
// =====================================================================


unsigned int AdvancedDomainDescriptor::getUnitsCount() const
{
  return mp_DomainDesc->getUnits().size();
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::addUnit(openfluid::fluidx::UnitDescriptor* UnitDesc)
{
  std::string ClassName = UnitDesc->getUnitClass();
  int ID = UnitDesc->getUnitID();

  // add in DomainDesc - to do first to fetch the newly added pointer
  mp_DomainDesc->getUnits().push_back(*UnitDesc);
  openfluid::fluidx::UnitDescriptor* NewUnitDesc =
      &mp_DomainDesc->getUnits().back();

  // add in m_Units
  if (!m_Units[ClassName].insert(std::make_pair(ID, AdvancedUnitDescriptor(*NewUnitDesc))).second)
  {
    // if fails, remove the newly added from DomainDesc
    mp_DomainDesc->getUnits().erase(
        mp_DomainDesc->getUnits().end().operator --());

    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addUnit",
        "trying to add a Unit that already exists");
  }

  //check relations
  try
  {
    checkUnitRelations(*NewUnitDesc);
  }
  catch (openfluid::base::FrameworkException& e)
  {
    // if fails, remove the newly added from DomainDesc and from Units
    mp_DomainDesc->getUnits().erase(
        mp_DomainDesc->getUnits().end().operator --());
    m_Units[ClassName].erase(ID);
    if (m_Units.at(ClassName).empty())
      m_Units.erase(ClassName);
    throw;
  }

  // add attributes
  std::set<std::string> AttrsNames = getAttributesNames(ClassName);

  if (AttrsNames.empty())
    return;

  openfluid::fluidx::AttributesDescriptor AttrsDesc;

  AttrsDesc.getUnitsClass() = ClassName;
  AdvancedUnitDescriptor& BUnit = m_Units.at(ClassName).at(ID);

  for (std::set<std::string>::iterator it = AttrsNames.begin();
      it != AttrsNames.end(); ++it)
  {
    AttrsDesc.getColumnsOrder().push_back(*it);
    AttrsDesc.getAttributes()[ID][*it] = "-";
  }

  mp_DomainDesc->getAttributes().push_back(AttrsDesc);

  // to do after pushing back descriptor (which is *not* a pointer!), to get the right address
  openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t& Data =
      mp_DomainDesc->getAttributes().end().operator --()->getAttributes().at(ID);
  for (std::set<std::string>::iterator it = AttrsNames.begin();
      it != AttrsNames.end(); ++it)
  {
    BUnit.Attributes[*it] = &Data.at(*it);
  }

}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::deleteUnit(const std::string& ClassName, int ID)
{
  // delete in m_Units and in m_AttrsNames
  m_Units[ClassName].erase(ID);
  if (m_Units.at(ClassName).empty())
  {
    m_Units.erase(ClassName);
    m_AttrsNames.erase(ClassName);
  }

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

  // delete in attributes list
  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->getAttributes());
  std::list<openfluid::fluidx::AttributesDescriptor>::iterator itAttrs =
      Attrs->begin();
  while (itAttrs != Attrs->end())
  {
    if (itAttrs->getUnitsClass() == ClassName)
    {
      itAttrs->getAttributes().erase(ID);

      if (itAttrs->getAttributes().empty())
        itAttrs = Attrs->erase(itAttrs);
      else
        itAttrs++;
    }
    else
      itAttrs++;
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


std::string& AdvancedDomainDescriptor::getAttribute(const std::string& ClassName,
                                                    int ID,
                                                    const std::string& AttrName)
{
  try
  {
    return *(m_Units.at(ClassName).at(ID).Attributes.at(AttrName));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::getAttributes",
        "trying to get an attribute that doesn't exist");
  }
}


// =====================================================================
// =====================================================================


std::set<std::string> AdvancedDomainDescriptor::getAttributesNames(const std::string& ClassName) const
{
  std::set<std::string> Names;

  if (m_AttrsNames.count(ClassName))
    Names = m_AttrsNames.at(ClassName);

  return Names;
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::addAttribute(const std::string& ClassName,
                                            const std::string& AttrName,
                                            const std::string& DefaultValue)
{
  if (!isClassNameExists(ClassName))
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addAttribute",
        "trying to add an attribute to a Class that doesn't exist");

  if (getAttributesNames(ClassName).count(AttrName))
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addAttribute",
        "trying to add an attribute that already exists");

  // add in DomainDesc
  openfluid::fluidx::AttributesDescriptor AttrsDesc;

  AttrsDesc.getUnitsClass() = ClassName;
  AttrsDesc.getColumnsOrder().push_back(AttrName);

  std::set<int> IDs = getIDsOfClass(ClassName);
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
    AttrsDesc.getAttributes()[*it][AttrName] = DefaultValue;

  mp_DomainDesc->getAttributes().push_back(AttrsDesc);

  // add in m_Units
  std::map<openfluid::core::UnitID_t,
      openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* Data =
      &(mp_DomainDesc->getAttributes().back().getAttributes());

  for (std::map<openfluid::core::UnitID_t,
      openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>::iterator it =
      Data->begin(); it != Data->end(); ++it)
  {
    m_Units.at(ClassName).at(it->first).Attributes[AttrName] = &(it->second.at(
        AttrName));
  }

  // add in m_AttrsNames
  m_AttrsNames[ClassName].insert(AttrName);

}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::deleteAttribute(const std::string& ClassName,
                                               const std::string& AttrName)
{
  if (!isClassNameExists(ClassName))
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::deleteAttribute",
        "trying to delete an attribute from a Class that doesn't exist");

  if (!getAttributesNames(ClassName).count(AttrName))
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::deleteAttribute",
        "trying to delete an attribute that doesn't exist");

  // delete in m_Units
  for (std::map<int, AdvancedUnitDescriptor>::iterator it = m_Units.at(ClassName).begin();
      it != m_Units.at(ClassName).end(); ++it)
    it->second.Attributes.erase(AttrName);

  // delete in DomainDesc
  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->getAttributes());
  std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin();
  while (it != Attrs->end())
  {
    if (it->getUnitsClass() == ClassName)
    {
      std::vector<std::string>& ColOrd = it->getColumnsOrder();
      std::vector<std::string>::iterator Found = std::find(ColOrd.begin(),
                                                           ColOrd.end(),
                                                           AttrName);
      if (Found != ColOrd.end())
        ColOrd.erase((Found));

      if (ColOrd.empty())
      {
        it = Attrs->erase(it);
      }
      else
      {
        std::map<openfluid::core::UnitID_t,
            openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* DataById =
            &(it->getAttributes());

        for (std::map<openfluid::core::UnitID_t,
            openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>::iterator it2 =
            DataById->begin(); it2 != DataById->end(); ++it2)
        {
          if (it2->second.count(AttrName))
            it2->second.erase(AttrName);
        }

        ++it;
      }
    }
    else
      ++it;
  }

  // delete in m_AttrsNames
  m_AttrsNames.at(ClassName).erase(AttrName);
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::renameAttribute(const std::string& ClassName,
                                               const std::string& OldAttrName,
                                               const std::string& NewAttrName)
{
  if (OldAttrName == NewAttrName)
    return;

  if (!isClassNameExists(ClassName))
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::renameAttribute",
        "trying to rename an attribute of a Class that doesn't exist");

  if (!getAttributesNames(ClassName).count(OldAttrName))
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::renameAttribute",
        "trying to rename an attribute that doesn't exist");

  //rename in DomainDesc
  std::map<unsigned int, std::string*> DomainDescAttrs;

  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->getAttributes());

  for (std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin(); it != Attrs->end(); ++it)
  {
    if (it->getUnitsClass() == ClassName)
    {
      std::vector<std::string>& ColOrd = it->getColumnsOrder();
      std::replace(ColOrd.begin(), ColOrd.end(), OldAttrName, NewAttrName);

      std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* DataById =
          &(it->getAttributes());

      for (std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>::iterator it2 =
          DataById->begin(); it2 != DataById->end(); ++it2)
      {
        if (it2->second.count(OldAttrName))
        {
          std::string Value = it2->second.at(OldAttrName);
          it2->second.erase(OldAttrName);
          it2->second[NewAttrName] = Value;
          DomainDescAttrs[it2->first] = &(it2->second[NewAttrName]);
        }
      }
    }
  }

// rename in m_Units
  for (std::map<int, AdvancedUnitDescriptor>::iterator it = m_Units.at(ClassName).begin();
      it != m_Units.at(ClassName).end(); ++it)
  {
    it->second.Attributes.erase(OldAttrName);
    it->second.Attributes[NewAttrName] = DomainDescAttrs[it->first];
  }

  //rename in m_AttrsNames
  m_AttrsNames.at(ClassName).erase(OldAttrName);
  m_AttrsNames.at(ClassName).insert(NewAttrName);
}


// =====================================================================
// =====================================================================


const std::list<openfluid::core::UnitClassID_t>& AdvancedDomainDescriptor::getUnitsToOf(
    const openfluid::core::UnitClassID_t Unit) const
{
  try
  {
    return m_Units.at(Unit.first).at(Unit.second).UnitDescriptor->getUnitsTos();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::getUnitsToOf",
        "trying to get relations of a Unit that doesn't exist");
  }
}


// =====================================================================
// =====================================================================


const std::list<openfluid::core::UnitClassID_t>& AdvancedDomainDescriptor::getUnitsParentsOf(
    const openfluid::core::UnitClassID_t Unit) const
{
  try
  {
    return m_Units.at(Unit.first).at(Unit.second).UnitDescriptor->getUnitsParents();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::getUnitsParentsOf",
        "trying to get relations of a Unit that doesn't exist");
  }
}


// =====================================================================
// =====================================================================


std::list<openfluid::core::UnitClassID_t> AdvancedDomainDescriptor::getUnitsFromOf(
    const openfluid::core::UnitClassID_t Unit) const
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
    const openfluid::core::UnitClassID_t Unit) const
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
    UFrom = m_Units.at(FromUnit.first).at(FromUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a From/To relation from a Unit that doesn't exist (UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << ")";
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addFromToRelation",
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
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addFromToRelation",
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
    UFrom = m_Units.at(FromUnit.first).at(FromUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a From/To relation from a Unit that doesn't exist (UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << ")";
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::removeFromToRelation",
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
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::removeFromToRelation",
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
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::removeFromToRelation",
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
    UChild = m_Units.at(ChildUnit.first).at(ChildUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a Parent/Child relation to a Unit that doesn't exist (UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addParentChildRelation", ss.str());
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
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::addParentChildRelation", ss.str());
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
    UChild = m_Units.at(ChildUnit.first).at(ChildUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a Parent/Child relation to a Unit that doesn't exist (UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::FrameworkException(
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
    throw openfluid::base::FrameworkException(
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
    throw openfluid::base::FrameworkException(
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
    U = m_Units.at(Unit.first).at(Unit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove relations to a Unit that doesn't exist (UnitClass "
       << Unit.first << " ID " << Unit.second << ")";
    throw openfluid::base::FrameworkException(
        "AdvancedDomainDescriptor::clearRelations",
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
  mp_DomainDesc->getAttributes().clear();
  mp_DomainDesc->getEvents().clear();
  m_Units.clear();
  m_AttrsNames.clear();
}


// =====================================================================
// =====================================================================


} } // namespaces
