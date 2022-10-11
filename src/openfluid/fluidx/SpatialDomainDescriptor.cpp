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
  @file SpatialDomainDescriptor.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <aline.libres@gmail.com>
*/


#include <stdexcept>
#include <algorithm>
#include <sstream>

#include <openfluid/fluidx/SpatialDomainDescriptor.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace fluidx {


void SpatialDomainDescriptor::checkUnitsRelations() const
{

  for (const auto& UnitsClass : m_SpatialUnits)
  {
    for (const auto& Unit : UnitsClass.second)
    {
      checkUnitRelations(Unit.second);
    }
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::checkUnitRelations(const openfluid::fluidx::SpatialUnitDescriptor& Unit) const
{
  const std::list<openfluid::core::UnitClassID_t>& Tos = Unit.toSpatialUnits();

  for (const auto& ToUnit : Tos)
  {
    try
    {
      spatialUnit(ToUnit.first, ToUnit.second);
    }
    catch (openfluid::base::FrameworkException& e)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Unit " +
                                                openfluid::tools::classIDToString(ToUnit.first,ToUnit.second) +
                                                " with \"To\" relation to unit " +
                                                openfluid::tools::classIDToString(Unit.getUnitsClass(),Unit.getID()) +
                                                " does not exist");
    }
  }

  const std::list<openfluid::core::UnitClassID_t>& Parents = Unit.parentSpatialUnits();

  for (const auto& ParentUnit : Parents)
  {
    try
    {
      spatialUnit(ParentUnit.first, ParentUnit.second);
    }
    catch (openfluid::base::FrameworkException& e)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "Unit " +
                                                openfluid::tools::classIDToString(ParentUnit.first,ParentUnit.second) +
                                                " with \"Parent\" relation to unit " +
                                                openfluid::tools::classIDToString(Unit.getUnitsClass(),Unit.getID()) +
                                                " does not exist");
    }
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::checkAttributesConsistency() const
{
  for (const auto& UnitsClass : m_SpatialUnits)
  {
    if (m_AttributesNames.count(UnitsClass.first))
    {
      // there are attributes for this units class

      const std::set<openfluid::core::AttributeName_t>& AttrNames = m_AttributesNames.at(UnitsClass.first);

      auto AttrsSize = AttrNames.size();

      for (const auto& Unit : UnitsClass.second)
      {
        const auto& UnitAttrs = Unit.second.attributes();

        // check if current unit attributes list size is equal to the size of the list of units class attributes
        if (UnitAttrs.size() != AttrsSize)
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "Attributes list of unit " +
                                                    openfluid::tools::classIDToString(UnitsClass.first,Unit.first)+
                                                    " does not match attributes list of units class " +
                                                    UnitsClass.first);
        }

        for (const auto& Attr : UnitAttrs)
        {

          // check if current unit attribute is a class attribute
          if (AttrNames.count(Attr.first) != 1)
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Attribute " + Attr.first + " of unit " +
                                                      openfluid::tools::classIDToString(UnitsClass.first,Unit.first)+
                                                      " is not an attribute of units class " + UnitsClass.first);
          }

          // check if current unit attribute is not empty
          if (Attr.second.empty())
          {
            throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Attribute " + Attr.first + " of unit " +
                                                      openfluid::tools::classIDToString(UnitsClass.first,Unit.first)+
                                                      " is empty");
          }
        }
      }
    }
    else
    {
      // there is no attribute for this units class

      for (const auto& Unit : UnitsClass.second)
      {
        if (Unit.second.attributes().size())
        {
          throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                    "unit " +
                                                    openfluid::tools::classIDToString(UnitsClass.first,Unit.first)+
                                                    " contains attribute(s) but units class " + UnitsClass.first +
                                                    "does not");
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool SpatialDomainDescriptor::isSpatialUnitExist(const openfluid::core::UnitsClass_t& ClassName,
                                                 const openfluid::core::UnitID_t& ID) const
{
  SpatialUnitsByIDByClass_t::const_iterator it = m_SpatialUnits.find(ClassName);

  return (it != m_SpatialUnits.end() && (*it).second.find(ID) != (*it).second.end());
}


// =====================================================================
// =====================================================================


const SpatialUnitDescriptor& SpatialDomainDescriptor::spatialUnit(const openfluid::core::UnitsClass_t& ClassName,
                                                                  const openfluid::core::UnitID_t& ID) const
{
  try
  {
    return m_SpatialUnits.at(ClassName).at(ID);
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "spatial unit " +openfluid::tools::classIDToString(ClassName,ID)+
                                              " does not exist");
  }
}


// =====================================================================
// =====================================================================


std::set<int> SpatialDomainDescriptor::getIDsOfClass(const openfluid::core::UnitsClass_t& ClassName) const
{
  std::set<int> IDs;

  if (isClassNameExists(ClassName))
  {
    for (SpatialUnitsByID_t::const_iterator it = m_SpatialUnits.at(ClassName).begin();
         it != m_SpatialUnits.at(ClassName).end(); ++it)
    {
      IDs.insert(it->first);
    }
  }

  return IDs;
}


// =====================================================================
// =====================================================================


bool SpatialDomainDescriptor::isClassNameExists(const openfluid::core::UnitsClass_t& ClassName) const
{
  return m_SpatialUnits.count(ClassName);
}


// =====================================================================
// =====================================================================


std::set<std::string> SpatialDomainDescriptor::getClassNames() const
{
  std::set<std::string> Names;

  for (const auto& UnitsClass : m_SpatialUnits)
  {
    Names.insert(UnitsClass.first);
  }

  return Names;
}


// =====================================================================
// =====================================================================


unsigned int SpatialDomainDescriptor::getUnitsCount() const
{
  unsigned int Counter = 0;

  for (const auto& UnitsClass : m_SpatialUnits)
  {
    Counter += UnitsClass.second.size();
  }

  return Counter;
}


// =====================================================================
// =====================================================================


unsigned int SpatialDomainDescriptor::getUnitsCount(const openfluid::core::UnitsClass_t& ClassName) const
{
  const auto& UnitsClass = m_SpatialUnits.find(ClassName);

  if (UnitsClass ==  m_SpatialUnits.end())
  {
    return 0;
  }
  else
  {
    return (*UnitsClass).second.size();
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::addUnit(const openfluid::fluidx::SpatialUnitDescriptor& UnitDesc, bool Check)
{
  openfluid::core::UnitsClass_t ClassName = UnitDesc.getUnitsClass();
  openfluid::core::UnitID_t ID = UnitDesc.getID();

  if (m_SpatialUnits[ClassName].find(ID) != m_SpatialUnits[ClassName].end())
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "unable to add spatial unit " +
                                              openfluid::tools::classIDToString(ClassName,ID) + "that already exists");
  }

  auto itInserted = (m_SpatialUnits[ClassName].insert(std::make_pair(ID, UnitDesc))).first;


  if (Check)
  {
    //check relations
    try
    {
      checkUnitRelations(UnitDesc);
    }
    catch (openfluid::base::FrameworkException& e)
    {
      // if fails, remove the newly added from DomainDesc and from Units
      m_SpatialUnits[ClassName].erase(ID);

      if (m_SpatialUnits[ClassName].empty())
      {
        m_SpatialUnits.erase(ClassName);
      }
      throw;
    }
  }

  // add attributes
  std::set<std::string> AttrsNames = getAttributesNames(ClassName);

  for (const auto& Attr : AttrsNames)
  {
    (*itInserted).second.attributes()[Attr] = "";
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::deleteUnit(const openfluid::core::UnitsClass_t& ClassName,
                                         const openfluid::core::UnitID_t& ID)
{
  if (!isClassNameExists(ClassName))
  {
    return;
  }

  m_SpatialUnits[ClassName].erase(ID);

  if (m_SpatialUnits.at(ClassName).empty())
  {
    // delete class
    m_SpatialUnits.erase(ClassName);

    // delete class in attributes names
    m_AttributesNames.erase(ClassName);
  }


  openfluid::core::UnitClassID_t UnitLink = std::make_pair(ClassName, ID);

  for (auto& UnitsClass : m_SpatialUnits)
  {
    for (auto& Unit : UnitsClass.second)
    {
      Unit.second.toSpatialUnits().remove(UnitLink);
      Unit.second.parentSpatialUnits().remove(UnitLink);
    }
  }
}


// =====================================================================
// =====================================================================


std::string SpatialDomainDescriptor::getAttribute(const openfluid::core::UnitsClass_t& ClassName,
                                                  const openfluid::core::UnitID_t& ID,
                                                  const openfluid::core::AttributeName_t& AttrName) const
{
  try
  {
    return m_SpatialUnits.at(ClassName).at(ID).attributes().at(AttrName);
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "spatial unit " + openfluid::tools::classIDToString(ClassName,ID) +
                                              " or attribute " + AttrName + " does not exist");
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::setAttribute(const openfluid::core::UnitsClass_t& ClassName,
                                           const openfluid::core::UnitID_t& ID,
                                           const openfluid::core::AttributeName_t& AttrName,
                                           const std::string& AttrValue)
{
  if (!m_AttributesNames.count(ClassName) || !m_AttributesNames[ClassName].count(AttrName))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "attribute "+AttrName+" does not exists for units class "+ClassName );
  }


  try
  {
    m_SpatialUnits.at(ClassName).at(ID).attributes()[AttrName] = AttrValue;
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot set value for attribute " + AttrName +
                                              " of unit " + openfluid::tools::classIDToString(ClassName,ID));
  }
}


// =====================================================================
// =====================================================================


std::set<std::string> SpatialDomainDescriptor::getAttributesNames(const openfluid::core::UnitsClass_t& ClassName) const
{
  std::set<std::string> Names;

  if (m_AttributesNames.count(ClassName))
  {
    Names = m_AttributesNames.at(ClassName);
  }

  return Names;
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::addAttribute(const openfluid::core::UnitsClass_t& ClassName,
                                           const std::string& AttrName,
                                           const std::string& DefaultValue,
                                           bool Check)
{
  if (!isClassNameExists(ClassName))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot add attribute to non-existent units class " + ClassName);
  }

  if (getAttributesNames(ClassName).count(AttrName))
  {
    if (Check)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "cannot add attribute " + AttrName +
                                                " that already exists for units class " + ClassName);
    }
    else
    {
      return;
    }
  }

  // Set default value to each spatial unit
  for (auto& Unit : m_SpatialUnits[ClassName])
  {
    Unit.second.attributes()[AttrName] = DefaultValue;
  }

  // Add in m_AttrsNames
  m_AttributesNames[ClassName].insert(AttrName);

}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::deleteAttribute(const openfluid::core::UnitsClass_t& ClassName,
                                               const std::string& AttrName)
{
  if (!isClassNameExists(ClassName))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot delete attribute from non-existent units class " + ClassName);
  }

  if (!getAttributesNames(ClassName).count(AttrName))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot delete attribute " + AttrName +
                                              " that does not exist for units class " + ClassName);
  }


  for (auto& Unit : m_SpatialUnits[ClassName])
  {
    Unit.second.attributes().erase(AttrName);
  }

  m_AttributesNames.at(ClassName).erase(AttrName);
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::renameAttribute(const openfluid::core::UnitsClass_t& ClassName,
                                              const std::string& OldAttrName,
                                              const std::string& NewAttrName)
{
  if (OldAttrName == NewAttrName)
  {
    return;
  }

  if (!isClassNameExists(ClassName))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot rename attribute from non-existent units class " + ClassName);
  }

  if (!getAttributesNames(ClassName).count(OldAttrName))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot rename non-existent attribute " + OldAttrName +
                                              " on units class " + ClassName);
  }


  for (auto& Unit : m_SpatialUnits[ClassName])
  {
    std::string Value = Unit.second.attributes().at(OldAttrName);
    Unit.second.attributes().erase(OldAttrName);
    Unit.second.attributes()[NewAttrName] = Value;
  }

  m_AttributesNames.at(ClassName).erase(OldAttrName);
  m_AttributesNames.at(ClassName).insert(NewAttrName);
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::addEvent(const openfluid::fluidx::EventDescriptor& EvDesc)
{
  try
  {
    SpatialUnitDescriptor* UnitDesc = &m_SpatialUnits.at(EvDesc.getUnitsClass()).at(EvDesc.getUnitID());

    UnitDesc->events().push_back(EvDesc);
  }
  catch (std::out_of_range& e)
  {
    // events added to non-existent spatial units are ignored
  }
}


// =====================================================================
// =====================================================================


EventID_t SpatialDomainDescriptor::addEvent(const openfluid::core::UnitsClass_t& UnitsClass,
                                            const openfluid::core::UnitID_t& UnitID,
                                            const openfluid::core::Event& Event)
{
  EventDescriptor EvDesc;
  EvDesc.setUnitsClass(UnitsClass);
  EvDesc.setUnitID(UnitID);
  EvDesc.setEvent(Event);

  addEvent(EvDesc);

  return EvDesc.getID();
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::deleteEvent(const openfluid::core::UnitsClass_t& UnitsClass,
                                          const openfluid::core::UnitID_t& UnitID,
                                          const openfluid::fluidx::EventID_t& EventID)
{
  // remove link from unit
  try
  {
    SpatialUnitDescriptor* UnitDesc = &m_SpatialUnits.at(UnitsClass).at(UnitID);

    std::list<openfluid::fluidx::EventDescriptor>::iterator itu = UnitDesc->events().begin();
    std::list<openfluid::fluidx::EventDescriptor>::iterator itue = UnitDesc->events().end();

    bool Found = false;

    while (!Found && itu != itue)
    {
      if ((*itu).getID() == EventID)
      {
        UnitDesc->events().erase(itu);
        Found = true;
      }
      else
      {
        ++itu;
      }
    }
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot delete event " + std::to_string(EventID) +
                                              " on units " + openfluid::tools::classIDToString(UnitsClass,UnitID));
  }

}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::modifyEvent(const openfluid::fluidx::EventID_t& EventID,
                                          const openfluid::core::Event& Event)
{
  openfluid::fluidx::EventDescriptor& EvDesc = event(EventID);

  EvDesc.setEvent(Event);
}


// =====================================================================
// =====================================================================


openfluid::fluidx::EventDescriptor& SpatialDomainDescriptor::event(const openfluid::fluidx::EventID_t& ID)
{
  for (auto& UnitsClass : m_SpatialUnits)
  {
    for (auto& Unit : UnitsClass.second)
    {
      for (auto& Event : Unit.second.events())
      {
        if (Event.getID() == ID)
        {
          return Event;
        }
      }
    }
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "cannot find event with ID " + std::to_string(ID));
}


// =====================================================================
// =====================================================================


/**
  Returns the event descriptor corresponding to the event ID
  @param[in] ID The ID of the event to delete
*/
const openfluid::fluidx::EventDescriptor& SpatialDomainDescriptor::event(const openfluid::fluidx::EventID_t& ID) const
{
  for (const auto& UnitsClass : m_SpatialUnits)
  {
    for (const auto& Unit : UnitsClass.second)
    {
      for (const auto& Event : Unit.second.events())
      {
        if (Event.getID() == ID)
        {
          return Event;
        }
      }
    }
  }

  throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                            "cannot find event with ID " + std::to_string(ID));
}


// =====================================================================
// =====================================================================


const std::list<openfluid::core::UnitClassID_t>& SpatialDomainDescriptor::toSpatialUnits(
    const openfluid::core::UnitClassID_t& Unit) const
{
  try
  {
    return m_SpatialUnits.at(Unit.first).at(Unit.second).toSpatialUnits();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot access \"To\" relations of non-existent unit " +
                                              openfluid::tools::classIDToString(Unit.first,Unit.second));
  }
}


// =====================================================================
// =====================================================================


const std::list<openfluid::core::UnitClassID_t>& SpatialDomainDescriptor::parentSpatialUnits(
    const openfluid::core::UnitClassID_t& Unit) const
{
  try
  {
    return m_SpatialUnits.at(Unit.first).at(Unit.second).parentSpatialUnits();
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot access \"Parent\" relations of non-existent unit " +
                                               openfluid::tools::classIDToString(Unit.first,Unit.second));
  }
}


// =====================================================================
// =====================================================================


std::list<openfluid::core::UnitClassID_t> SpatialDomainDescriptor::getFromSpatialUnits(
    const openfluid::core::UnitClassID_t& Unit) const
{
  std::list<openfluid::core::UnitClassID_t> Froms;

  for (const auto& UnitsClass : m_SpatialUnits)
  {
    for (const auto& UnitID : UnitsClass.second)
    {
      const auto& Tos = UnitID.second.toSpatialUnits();

      if (std::find(Tos.begin(), Tos.end(), Unit) != Tos.end())
      {
        Froms.push_back(std::make_pair(UnitsClass.first,UnitID.first));
      }
    }
  }

  return Froms;
}


// =====================================================================
// =====================================================================


std::list<openfluid::core::UnitClassID_t> SpatialDomainDescriptor::getChildSpatialUnits(
    const openfluid::core::UnitClassID_t& Unit) const
{
  std::list<openfluid::core::UnitClassID_t> Children;

  for (const auto& UnitsClass : m_SpatialUnits)
  {
    for (const auto& UnitID : UnitsClass.second)
    {
      const auto& Parents = UnitID.second.parentSpatialUnits();

      if (std::find(Parents.begin(), Parents.end(), Unit) != Parents.end())
      {
        Children.push_back(std::make_pair(UnitsClass.first,UnitID.first));
      }
    }
  }

  return Children;
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::addFromToRelation(const openfluid::core::UnitClassID_t& FromUnit,
                                                const openfluid::core::UnitClassID_t& ToUnit)
{

  openfluid::fluidx::SpatialUnitDescriptor* UFrom;

  try
  {
    UFrom = &(m_SpatialUnits.at(FromUnit.first).at(FromUnit.second));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot add \"From-To\" relation from non-existent unit " +
                                              openfluid::tools::classIDToString(FromUnit.first,FromUnit.second));
  }

  try
  {
    m_SpatialUnits.at(ToUnit.first).at(ToUnit.second);
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot add \"From-To\" relation to non-existent unit " +
                                               openfluid::tools::classIDToString(ToUnit.first,ToUnit.second));
  }

  std::list<openfluid::core::UnitClassID_t>& Tos = UFrom->toSpatialUnits();

  if (std::find(Tos.begin(), Tos.end(), ToUnit) == Tos.end())
  {
    Tos.push_back(ToUnit);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::removeFromToRelation(const openfluid::core::UnitClassID_t& FromUnit,
                                                   const openfluid::core::UnitClassID_t& ToUnit)
{
  openfluid::fluidx::SpatialUnitDescriptor* UFrom;

  try
  {
    UFrom = &(m_SpatialUnits.at(FromUnit.first).at(FromUnit.second));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot remove \"From-To\" relation from non-existent unit " +
                                              openfluid::tools::classIDToString(FromUnit.first,FromUnit.second));
  }

  UFrom->toSpatialUnits().remove(ToUnit);
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::addParentChildRelation(const openfluid::core::UnitClassID_t& ParentUnit,
                                                     const openfluid::core::UnitClassID_t& ChildUnit)
{
  openfluid::fluidx::SpatialUnitDescriptor* UChild;

  try
  {
    UChild = &(m_SpatialUnits.at(ChildUnit.first).at(ChildUnit.second));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot add \"Parent-Child\" relation with non-existent child unit " +
                                              openfluid::tools::classIDToString(ChildUnit.first,ChildUnit.second));
  }

  try
  {
    m_SpatialUnits.at(ParentUnit.first).at(ParentUnit.second);
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot add \"Parent-Child\" relation with non-existent parent unit " +
                                              openfluid::tools::classIDToString(ParentUnit.first,ParentUnit.second));
  }

  std::list<openfluid::core::UnitClassID_t>& Parents = UChild->parentSpatialUnits();

  if (std::find(Parents.begin(), Parents.end(), ParentUnit) == Parents.end())
  {
    Parents.push_back(ParentUnit);
  }
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::removeParentChildRelation(const openfluid::core::UnitClassID_t& ParentUnit,
                                                        const openfluid::core::UnitClassID_t& ChildUnit)
{

  openfluid::fluidx::SpatialUnitDescriptor* UChild;

  try
  {
    UChild = &(m_SpatialUnits.at(ChildUnit.first).at(ChildUnit.second));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot remove \"Parent-Child\" relation with non-existent child unit " +
                                              openfluid::tools::classIDToString(ChildUnit.first,ChildUnit.second));
  }

  UChild->parentSpatialUnits().remove(ParentUnit);
}


// =====================================================================
// =====================================================================


void SpatialDomainDescriptor::clearRelations(const openfluid::core::UnitClassID_t& Unit)
{

  openfluid::fluidx::SpatialUnitDescriptor* U;

  try
  {
    U = &(m_SpatialUnits.at(Unit.first).at(Unit.second));
  }
  catch (std::out_of_range& e)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "cannot remove relations with non-existent unit " +
                                              openfluid::tools::classIDToString(Unit.first,Unit.second));
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


void SpatialDomainDescriptor::clearDomain()
{
  m_SpatialUnits.clear();
  m_AttributesNames.clear();
}


} } // namespaces

