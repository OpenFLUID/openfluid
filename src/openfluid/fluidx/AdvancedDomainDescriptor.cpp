/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file AdvancedDomainDescriptor.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
*/


#include <stdexcept>
#include <algorithm>
#include <sstream>

#include <openfluid/fluidx/AdvancedDomainDescriptor.hpp>
#include <openfluid/fluidx/AttributesDescriptor.hpp>
#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <openfluid/fluidx/SpatialUnitDescriptor.hpp>


namespace openfluid { namespace fluidx {


AdvancedDomainDescriptor::AdvancedDomainDescriptor(
    openfluid::fluidx::SpatialDomainDescriptor& DomainDesc) :
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
  std::list<openfluid::fluidx::SpatialUnitDescriptor>* Units =
      &(mp_DomainDesc->spatialUnits());

  for (std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    if (!m_Units[it->getUnitsClass()].insert(
        std::make_pair(it->getID(), AdvancedUnitDescriptor(*it))).second)
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "trying to add a Unit that already exists");
  }
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::checkUnitRelations() const
{
  std::list<openfluid::fluidx::SpatialUnitDescriptor>* Units =
      &(mp_DomainDesc->spatialUnits());

  for (std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    checkUnitRelations(*it);
  }

}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::checkUnitRelations(
    openfluid::fluidx::SpatialUnitDescriptor& Unit) const
{
  std::list<openfluid::core::UnitClassID_t>::iterator it;

  std::list<openfluid::core::UnitClassID_t>& Tos = Unit.toSpatialUnits();

  for (it = Tos.begin(); it != Tos.end(); ++it)
  {
    try
    {
      spatialUnit(it->first, it->second);
    }
    catch (openfluid::base::FrameworkException& e)
    {
      std::ostringstream ss;
      ss << "Unit " << it->second << " of class " << it->first
         << " in \"To\" relation of unit " << Unit.getID() << " of class "
         << Unit.getUnitsClass() << " doesn't exist";
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
    }
  }

  std::list<openfluid::core::UnitClassID_t>& Parents = Unit.parentSpatialUnits();

  for (it = Parents.begin(); it != Parents.end(); ++it)
  {
    try
    {
      spatialUnit(it->first, it->second);
    }
    catch (openfluid::base::FrameworkException& e)
    {
      std::ostringstream ss;
      ss << "Unit " << it->second << " of class " << it->first
         << " in \"Parent\" relation of unit " << Unit.getID()
         << " of class " << Unit.getUnitsClass() << " doesn't exist";
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
    }
  }

}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::dispatchAttributes()
{
  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->attributes());

  for (std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin(); it != Attrs->end(); ++it)
  {
    std::map<openfluid::core::UnitID_t,
        openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* Data =
        &(it->attributes());

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
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "trying to add an attribute (" + it3->first
                                                      + ") that already exists");
        }
        catch (std::out_of_range& e)
        {
          std::ostringstream ss;
          ss << "trying to add an attribute (" << it3->first
             << ") to a Unit that doesn't exist (class " << it->getUnitsClass()
             << " - ID " << it2->first << ")";
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
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
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
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
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, ss.str());
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
      &(mp_DomainDesc->events());

  for (std::list<openfluid::fluidx::EventDescriptor>::iterator it =
      Events->begin(); it != Events->end(); ++it)
  {
    try
    {
      m_Units.at(it->getUnitsClass()).at(it->getUnitID()).EventsDescriptors.push_back(&(*it));
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


bool AdvancedDomainDescriptor::isSpatialUnitExist(const std::string& ClassName, int ID) const
{
  std::map<std::string, std::map<int, AdvancedUnitDescriptor> >::const_iterator it = m_Units.find(ClassName);

  return (it != m_Units.end() && (*it).second.find(ID) != (*it).second.end());
}


// =====================================================================
// =====================================================================


const std::map<std::string, std::map<int, AdvancedUnitDescriptor> >&
  AdvancedDomainDescriptor::spatialUnitsByIdByClass() const
{
  return m_Units;
}


// =====================================================================
// =====================================================================


const AdvancedUnitDescriptor& AdvancedDomainDescriptor::spatialUnit(const std::string& ClassName,
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
  }
}


// =====================================================================
// =====================================================================


const openfluid::fluidx::SpatialUnitDescriptor&
  AdvancedDomainDescriptor::spatialUnitDescriptor(const std::string& ClassName,
                                                  int ID) const
{
  return *(spatialUnit(ClassName, ID).UnitDescriptor);
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
  return mp_DomainDesc->spatialUnits().size();
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::addUnit(openfluid::fluidx::SpatialUnitDescriptor* UnitDesc)
{
  std::string ClassName = UnitDesc->getUnitsClass();
  int ID = UnitDesc->getID();

  // add in DomainDesc - to do first to fetch the newly added pointer
  mp_DomainDesc->spatialUnits().push_back(*UnitDesc);
  openfluid::fluidx::SpatialUnitDescriptor* NewUnitDesc =
      &mp_DomainDesc->spatialUnits().back();

  // add in m_Units
  if (!m_Units[ClassName].insert(std::make_pair(ID, AdvancedUnitDescriptor(*NewUnitDesc))).second)
  {
    // if fails, remove the newly added from DomainDesc
    mp_DomainDesc->spatialUnits().erase(
        mp_DomainDesc->spatialUnits().end().operator --());

    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"trying to add a Unit that already exists");
  }

  //check relations
  try
  {
    checkUnitRelations(*NewUnitDesc);
  }
  catch (openfluid::base::FrameworkException& e)
  {
    // if fails, remove the newly added from DomainDesc and from Units
    mp_DomainDesc->spatialUnits().erase(
        mp_DomainDesc->spatialUnits().end().operator --());
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

  AttrsDesc.setUnitsClass(ClassName);
  AdvancedUnitDescriptor& BUnit = m_Units.at(ClassName).at(ID);

  for (std::set<std::string>::iterator it = AttrsNames.begin();
      it != AttrsNames.end(); ++it)
  {
    AttrsDesc.columnsOrder().push_back(*it);
    AttrsDesc.attributes()[ID][*it] = "-";
  }

  mp_DomainDesc->attributes().push_back(AttrsDesc);

  // to do after pushing back descriptor (which is *not* a pointer!), to get the right address
  openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t& Data =
      mp_DomainDesc->attributes().end().operator --()->attributes().at(ID);
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
  std::list<openfluid::fluidx::SpatialUnitDescriptor>& Units =
      mp_DomainDesc->spatialUnits();
  openfluid::core::UnitClassID_t Unit = std::make_pair(ClassName, ID);
  std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator it = Units.begin();
  while (it != Units.end())
  {
    if (it->getUnitsClass() == ClassName && (int) it->getID() == ID)
    {
      it = Units.erase(it);
    }
    else
    {
      std::list<openfluid::core::UnitClassID_t>& Tos = it->toSpatialUnits();
      std::list<openfluid::core::UnitClassID_t>::iterator Found = std::find(
          Tos.begin(), Tos.end(), Unit);
      if (Found != Tos.end())
        Tos.remove(Unit);

      std::list<openfluid::core::UnitClassID_t>& Parents =
          it->parentSpatialUnits();
      Found = std::find(Parents.begin(), Parents.end(), Unit);
      if (Found != Parents.end())
        Parents.remove(Unit);

      ++it;
    }
  }

  // delete in attributes list
  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->attributes());
  std::list<openfluid::fluidx::AttributesDescriptor>::iterator itAttrs =
      Attrs->begin();
  while (itAttrs != Attrs->end())
  {
    if (itAttrs->getUnitsClass() == ClassName)
    {
      itAttrs->attributes().erase(ID);

      if (itAttrs->attributes().empty())
        itAttrs = Attrs->erase(itAttrs);
      else
        ++itAttrs;
    }
    else
      ++itAttrs;
  }

  // delete in EventDesc list
  std::list<openfluid::fluidx::EventDescriptor>* Events =
      &(mp_DomainDesc->events());
  std::list<openfluid::fluidx::EventDescriptor>::iterator itEv =
      Events->begin();
  while (itEv != Events->end())
  {
    if (itEv->getUnitsClass() == ClassName && (int) itEv->getUnitID() == ID)
      itEv = Events->erase(itEv);
    else
      ++itEv;
  }

}


// =====================================================================
// =====================================================================


std::string& AdvancedDomainDescriptor::attribute(const std::string& ClassName,
                                                    int ID,
                                                    const std::string& AttrName)
{
  try
  {
    return *(m_Units.at(ClassName).at(ID).Attributes.at(AttrName));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to add an attribute to a Class that doesn't exist");

  if (getAttributesNames(ClassName).count(AttrName))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to add an attribute that already exists");

  // add in DomainDesc
  openfluid::fluidx::AttributesDescriptor AttrsDesc;

  AttrsDesc.setUnitsClass(ClassName);
  AttrsDesc.columnsOrder().push_back(AttrName);

  std::set<int> IDs = getIDsOfClass(ClassName);
  for (std::set<int>::iterator it = IDs.begin(); it != IDs.end(); ++it)
    AttrsDesc.attributes()[*it][AttrName] = DefaultValue;

  mp_DomainDesc->attributes().push_back(AttrsDesc);

  // add in m_Units
  std::map<openfluid::core::UnitID_t,
      openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* Data =
      &(mp_DomainDesc->attributes().back().attributes());

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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to delete an attribute from a Class that doesn't exist");

  if (!getAttributesNames(ClassName).count(AttrName))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to delete an attribute that doesn't exist");

  // delete in m_Units
  for (std::map<int, AdvancedUnitDescriptor>::iterator it = m_Units.at(ClassName).begin();
      it != m_Units.at(ClassName).end(); ++it)
    it->second.Attributes.erase(AttrName);

  // delete in DomainDesc
  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->attributes());
  std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin();
  while (it != Attrs->end())
  {
    if (it->getUnitsClass() == ClassName)
    {
      std::vector<std::string>& ColOrd = it->columnsOrder();
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
            &(it->attributes());

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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to rename an attribute of a Class that doesn't exist");

  if (!getAttributesNames(ClassName).count(OldAttrName))
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to rename an attribute that doesn't exist");

  //rename in DomainDesc
  std::map<unsigned int, std::string*> DomainDescAttrs;

  std::list<openfluid::fluidx::AttributesDescriptor>* Attrs =
      &(mp_DomainDesc->attributes());

  for (std::list<openfluid::fluidx::AttributesDescriptor>::iterator it =
      Attrs->begin(); it != Attrs->end(); ++it)
  {
    if (it->getUnitsClass() == ClassName)
    {
      std::vector<std::string>& ColOrd = it->columnsOrder();
      std::replace(ColOrd.begin(), ColOrd.end(), OldAttrName, NewAttrName);

      std::map<openfluid::core::UnitID_t,
          openfluid::fluidx::AttributesDescriptor::AttributeNameValue_t>* DataById =
          &(it->attributes());

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


void AdvancedDomainDescriptor::addEvent(const openfluid::core::UnitsClass_t& UnitsClass,
                                        const openfluid::core::UnitID_t& UnitID,
                                        const openfluid::core::Event& Event)
{
  try
  {
    AdvancedUnitDescriptor* AdvUnitDesc = &m_Units.at(UnitsClass).at(UnitID);

    EventDescriptor EvDesc;
    EvDesc.setUnitsClass(UnitsClass);
    EvDesc.setUnitID(UnitID);
    EvDesc.event() = Event;

    mp_DomainDesc->events().push_back(EvDesc);

    AdvUnitDesc->EventsDescriptors.push_back(&(mp_DomainDesc->events().back()));

   }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error adding event");
  }
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::deleteEvent(const openfluid::core::UnitsClass_t& UnitsClass,
                                           const openfluid::core::UnitID_t& UnitID,
                                           const openfluid::fluidx::EventID_t& EventID)
{
  // remove link from unit
  try
  {
    AdvancedUnitDescriptor* AdvUnitDesc = &m_Units.at(UnitsClass).at(UnitID);

    std::list<openfluid::fluidx::EventDescriptor*>::iterator itu = AdvUnitDesc->EventsDescriptors.begin();
    std::list<openfluid::fluidx::EventDescriptor*>::iterator itue = AdvUnitDesc->EventsDescriptors.end();

    bool Found = false;

    while (!Found && itu != itue)
    {
      if ((*itu)->getID() == EventID)
      {
        AdvUnitDesc->EventsDescriptors.erase(itu);
        Found = true;
      }
      else
        ++itu;
    }

  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Error deleting event");
  }


 // remove event from list

  std::list<openfluid::fluidx::EventDescriptor>* EventsDescs =
      &(mp_DomainDesc->events());

  std::list<openfluid::fluidx::EventDescriptor>::iterator it = EventsDescs->begin();
  std::list<openfluid::fluidx::EventDescriptor>::iterator ite = EventsDescs->end();

  bool Found = false;

  while (!Found && it != ite)
  {
    if ((*it).getID() == EventID)
    {
      EventsDescs->erase(it);
      Found = true;
    }
    else
      ++it;
  }
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::modifyEvent(const openfluid::fluidx::EventID_t& EventID,
                                           const openfluid::core::Event& Event)
{
  openfluid::fluidx::EventDescriptor* EvDesc = eventDescriptor(EventID);

  if (EvDesc)
    EvDesc->event() = Event;
}


// =====================================================================
// =====================================================================


openfluid::fluidx::EventDescriptor* AdvancedDomainDescriptor::eventDescriptor(const openfluid::fluidx::EventID_t& ID)
{
  openfluid::fluidx::EventDescriptor* EvDesc = nullptr;

  std::list<openfluid::fluidx::EventDescriptor>* EventsDescs =
      &(mp_DomainDesc->events());

  std::list<openfluid::fluidx::EventDescriptor>::iterator it = EventsDescs->begin();
  std::list<openfluid::fluidx::EventDescriptor>::iterator ite = EventsDescs->end();


  while (it != ite && EvDesc == nullptr)
  {
    if ((*it).getID() == ID)
    {
      EvDesc = &(*it);
    }
    else
      ++it;
  }

  return EvDesc;
}


// =====================================================================
// =====================================================================


const std::list<openfluid::core::UnitClassID_t>& AdvancedDomainDescriptor::toSpatialUnits(
    const openfluid::core::UnitClassID_t Unit) const
{
  try
  {
    return m_Units.at(Unit.first).at(Unit.second).UnitDescriptor->toSpatialUnits();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to get relations of a Unit that doesn't exist");
  }
}


// =====================================================================
// =====================================================================


const std::list<openfluid::core::UnitClassID_t>& AdvancedDomainDescriptor::parentSpatialUnits(
    const openfluid::core::UnitClassID_t Unit) const
{
  try
  {
    return m_Units.at(Unit.first).at(Unit.second).UnitDescriptor->parentSpatialUnits();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "trying to get relations of a Unit that doesn't exist");
  }
}


// =====================================================================
// =====================================================================


std::list<openfluid::core::UnitClassID_t> AdvancedDomainDescriptor::getFromSpatialUnits(
    const openfluid::core::UnitClassID_t Unit) const
{
  std::list<openfluid::core::UnitClassID_t> Froms;

  std::list<openfluid::fluidx::SpatialUnitDescriptor>* Units =
      &(mp_DomainDesc->spatialUnits());
  std::list<openfluid::core::UnitClassID_t>* Tos;

  for (std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    Tos = &(it->toSpatialUnits());

    if (std::find(Tos->begin(), Tos->end(), Unit) != Tos->end())
    {
      openfluid::core::UnitClassID_t From = std::make_pair(it->getUnitsClass(),
                                                           it->getID());
      Froms.push_back(From);
    }
  }

  return Froms;
}


// =====================================================================
// =====================================================================


std::list<openfluid::core::UnitClassID_t> AdvancedDomainDescriptor::getChildSpatialUnits(
    const openfluid::core::UnitClassID_t Unit) const
{
  std::list<openfluid::core::UnitClassID_t> Children;

  std::list<openfluid::fluidx::SpatialUnitDescriptor>* Units =
      &(mp_DomainDesc->spatialUnits());
  std::list<openfluid::core::UnitClassID_t>* Parents;

  for (std::list<openfluid::fluidx::SpatialUnitDescriptor>::iterator it =
      Units->begin(); it != Units->end(); ++it)
  {
    Parents = &(it->parentSpatialUnits());

    if (std::find(Parents->begin(), Parents->end(), Unit) != Parents->end())
    {
      openfluid::core::UnitClassID_t Child = std::make_pair(it->getUnitsClass(),
                                                            it->getID());
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
  openfluid::fluidx::SpatialUnitDescriptor* UFrom;

  try
  {
    UFrom = m_Units.at(FromUnit.first).at(FromUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a From/To relation from a Unit that doesn't exist (UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Tos = UFrom->toSpatialUnits();
  if (std::find(Tos.begin(), Tos.end(), ToUnit) == Tos.end())
    Tos.push_back(ToUnit);
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::removeFromToRelation(
    const openfluid::core::UnitClassID_t FromUnit,
    const openfluid::core::UnitClassID_t ToUnit)
{
  openfluid::fluidx::SpatialUnitDescriptor* UFrom;

  try
  {
    UFrom = m_Units.at(FromUnit.first).at(FromUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a From/To relation from a Unit that doesn't exist (UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Tos = UFrom->toSpatialUnits();
  if (std::find(Tos.begin(), Tos.end(), ToUnit) != Tos.end())
    Tos.remove(ToUnit);
  else
  {
    std::ostringstream ss;
    ss << "trying to remove a From/To relation that doesn't exist (from UnitClass "
       << FromUnit.first << " ID " << FromUnit.second << " to UnitClass "
       << ToUnit.first << " ID " << ToUnit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
  }
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::addParentChildRelation(
    const openfluid::core::UnitClassID_t ParentUnit,
    const openfluid::core::UnitClassID_t ChildUnit)
{
  openfluid::fluidx::SpatialUnitDescriptor* UChild;

  try
  {
    UChild = m_Units.at(ChildUnit.first).at(ChildUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to add a Parent/Child relation to a Unit that doesn't exist (UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, ss.str());
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Parents =
      UChild->parentSpatialUnits();
  if (std::find(Parents.begin(), Parents.end(), ParentUnit) == Parents.end())
    Parents.push_back(ParentUnit);
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::removeParentChildRelation(
    const openfluid::core::UnitClassID_t ParentUnit,
    const openfluid::core::UnitClassID_t ChildUnit)
{
  openfluid::fluidx::SpatialUnitDescriptor* UChild;

  try
  {
    UChild = m_Units.at(ChildUnit.first).at(ChildUnit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove a Parent/Child relation to a Unit that doesn't exist (UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, ss.str());
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
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, ss.str());
  }

  std::list<openfluid::core::UnitClassID_t>& Parents =
      UChild->parentSpatialUnits();
  if (std::find(Parents.begin(), Parents.end(), ParentUnit) != Parents.end())
    Parents.remove(ParentUnit);
  else
  {
    std::ostringstream ss;
    ss << "trying to remove a Parent/Child relation that doesn't exist (from UnitClass "
       << ParentUnit.first << " ID " << ParentUnit.second << " to UnitClass "
       << ChildUnit.first << " ID " << ChildUnit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
  }

}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::clearRelations(
    const openfluid::core::UnitClassID_t Unit)
{
  openfluid::fluidx::SpatialUnitDescriptor* U;

  try
  {
    U = m_Units.at(Unit.first).at(Unit.second).UnitDescriptor;
  }
  catch (std::out_of_range& e)
  {
    std::ostringstream ss;
    ss << "trying to remove relations to a Unit that doesn't exist (UnitClass "
       << Unit.first << " ID " << Unit.second << ")";
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,ss.str());
  }

  std::list<openfluid::core::UnitClassID_t> Froms = getFromSpatialUnits(Unit);
  for (std::list<openfluid::core::UnitClassID_t>::iterator it = Froms.begin();
      it != Froms.end(); ++it)
  {
    removeFromToRelation(*it, Unit);
  }

  std::list<openfluid::core::UnitClassID_t> Children = getChildSpatialUnits(Unit);
  for (std::list<openfluid::core::UnitClassID_t>::iterator it =
      Children.begin(); it != Children.end(); ++it)
  {
    removeParentChildRelation(Unit, *it);
  }

  U->toSpatialUnits().clear();
  U->parentSpatialUnits().clear();
}


// =====================================================================
// =====================================================================


void AdvancedDomainDescriptor::clearDomain()
{
  mp_DomainDesc->spatialUnits().clear();
  mp_DomainDesc->attributes().clear();
  mp_DomainDesc->events().clear();
  m_Units.clear();
  m_AttrsNames.clear();
}


} } // namespaces
