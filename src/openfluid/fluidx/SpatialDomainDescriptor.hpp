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
  @file SpatialDomainDescriptor.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Aline LIBRES <aline.libres@gmail.com>
*/


#ifndef __OPENFLUID_FLUIDX_SPATIALDOMAINDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_SPATIALDOMAINDESCRIPTOR_HPP__


#include <map>
#include <set>

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/SpatialUnitDescriptor.hpp>


namespace openfluid {

namespace core {
class Event;
}

namespace fluidx {


class OPENFLUID_API SpatialDomainDescriptor
{
  public:

    typedef std::map<openfluid::core::UnitID_t, SpatialUnitDescriptor> SpatialUnitsByID_t;

    typedef std::map<openfluid::core::UnitsClass_t,SpatialUnitsByID_t> SpatialUnitsByIDByClass_t;


  private:

    /**
      Units by ID by ClassName
    */
    SpatialUnitsByIDByClass_t m_SpatialUnits;

    /**
      Attributes names by ClassName
    */
    std::map<openfluid::core::UnitsClass_t, std::set<openfluid::core::AttributeName_t>> m_AttributesNames;

    /**
      Checks if the spatial relations (From-To, Parent-Child) are consistent for the given spatial unit
      @throw openfluid::base::FrameworkException if spatial relations are not consistent
    */
    void checkUnitRelations(const openfluid::fluidx::SpatialUnitDescriptor& Unit) const;


  public:

    SpatialDomainDescriptor() = default;

    ~SpatialDomainDescriptor()
    { }

    /**
      Returns a reference to the structure of spatial units descriptors
      @return a reference to the structure
    */
    SpatialUnitsByIDByClass_t& spatialUnits()
    {
      return m_SpatialUnits;
    }

    /**
      Returns a const reference to the structure of spatial units descriptors
      @return a constreference to the structure
    */
    const SpatialUnitsByIDByClass_t& spatialUnits() const
    {
      return m_SpatialUnits;
    }

    /**
      Checks if spatial units relations (From-To, Parent-Child) are consistent.
      @throw openfluid::base::FrameworkException if spatial units relations are not consistent
    */
    void checkUnitsRelations() const;

    /**
      Checks if spatial attributes are consistent.
      @throw openfluid::base::FrameworkException if spatial attributes are not consistent
    */
    void checkAttributesConsistency() const;

    /**
      Returns true if the spatial unit descriptor exists for the given units class and unit ID
      @param[in] ClassName the name of the units class
      @param[in] ID the ID of the spatial
      @return true if the spatial unit descriptor exists
    */
    bool isSpatialUnitExist(const openfluid::core::UnitsClass_t& ClassName,
                            const openfluid::core::UnitID_t& ID) const;

    /**
      Returns a const reference to the spatial unit descriptor identified by a units class and a unit ID
      @param[in] ClassName the name of the units class
      @param[in] ID the ID of the spatial
      @return a const reference to the spatial unit if found
      @throw openfluid::base::FrameworkException if spatial unit is not found
    */
    const SpatialUnitDescriptor& spatialUnit(const openfluid::core::UnitsClass_t& ClassName,
                                             const openfluid::core::UnitID_t& ID) const;

    /**
      Returns the spatial units IDs for a given units class
      @param[in] ClassName the name of the spatial units class
      @return a set of IDs for the given class, an empty set if the units class does not exist.
    */
    std::set<int> getIDsOfClass(const openfluid::core::UnitsClass_t& ClassName) const;

    /**
      Returns true if the spatial units class exists
      @param[in] ClassName the name of the units class
      @return true if the units class exists
    */
    bool isClassNameExists(const openfluid::core::UnitsClass_t& ClassName) const;

    /**
      Returns the names of units classes of the spatial domain
      @return a set of units class names
    */
    std::set<std::string> getClassNames() const;

    /**
      Returns the count of all spatial units in the spatial domain
      @return the count of spatial units
    */
    unsigned int getUnitsCount() const;

    /**
      Returns the count of spatial units for a given units class in the spatial domain
      @return the count of spatial units
    */
    unsigned int getUnitsCount(const openfluid::core::UnitsClass_t& ClassName) const;

    /**
      Adds a spatial unit to the spatial domain
      @param[in] UnitDesc the spatial unit descriptor to add
      @param[in] Check enables internal checks when set to true (default value = true)
      @throw openfluid::base::FrameworkException if the spatial unit already exists, or if Check is true
         and the spatial unit has a wrong spatial relation
    */
    void addUnit(const openfluid::fluidx::SpatialUnitDescriptor& UnitDesc, bool Check = true);

    /**
      Removes a spatial unit
      @param[in] ClassName the units class of the spatial unit to remove
      @param[in] ID the ID of the spatial unit to remove
    */
    void deleteUnit(const openfluid::core::UnitsClass_t& ClassName, const openfluid::core::UnitID_t& ID);

    /**
      Returns a const reference to the list of units class and ID linked with a "To" relation to the given spatial unit
      @param[in] Unit units class and ID of the spatial unit
      @throw openfluid::base::FrameworkException if the spatial unit does not exists
      @return a const reference to the list of the "To" spatial units
    */
    const std::list<openfluid::core::UnitClassID_t>& toSpatialUnits(const openfluid::core::UnitClassID_t& Unit) const;

    /**
      Returns a const reference to the list of units class and ID linked with a "Parent" relation
      to the given spatial unit
      @param[in] Unit units class and ID of the spatial unit
      @throw openfluid::base::FrameworkException if the spatial unit does not exists
      @return a const reference to the list of the "Parent" spatial units
    */
    const std::list<openfluid::core::UnitClassID_t>&
      parentSpatialUnits(const openfluid::core::UnitClassID_t& Unit) const;

    /**
      Builds a list of units class and ID linked with a "From" relation
      to the given spatial unit
      @param[in] Unit units class and ID of the spatial unit
      @throw openfluid::base::FrameworkException if the spatial unit does not exists
      @return the list of the "From" spatial units, may be empty
    */
    std::list<openfluid::core::UnitClassID_t> getFromSpatialUnits(const openfluid::core::UnitClassID_t& Unit) const;

    /**
      Builds a list of units class and ID linked with a "Child" relation
      to the given spatial unit
      @param[in] Unit units class and ID of the spatial unit
      @throw openfluid::base::FrameworkException if the spatial unit does not exists
      @return the list of the "Child" spatial units, may be empty
    */
    std::list<openfluid::core::UnitClassID_t> getChildSpatialUnits(const openfluid::core::UnitClassID_t& Unit) const;

    /**
      Adds the ToUnit to the list of "To" relations of FromUnit. It does nothing if the relation already exists.
      @param[in] FromUnit the source spatial unit of the relation
      @param[in] ToUnit the target spatial unit of the relation
      @throw openfluid::base::FrameworkException if FromUnit or ToUnit does not exist
    */
    void addFromToRelation(const openfluid::core::UnitClassID_t& FromUnit,
                           const openfluid::core::UnitClassID_t& ToUnit);

    /**
      Removes the ToUnit from the list of "To" relations of FromUnit. It does nothing if the relation does not exists.
      @param[in] FromUnit the source spatial unit of the relation
      @param[in] ToUnit the target spatial unit of the relation
      @throw openfluid::base::FrameworkException if FromUnit does not exist
    */
    void removeFromToRelation(const openfluid::core::UnitClassID_t& FromUnit,
                              const openfluid::core::UnitClassID_t& ToUnit);

    /**
      Adds the ParentUnit to the list of "Parent" relations of ChildUnit.
      It does nothing if the relation already exists.
      @param[in] ParentUnit the source spatial unit of the relation
      @param[in] ChildUnit the target spatial unit of the relation
      @throw openfluid::base::FrameworkException if ParentUnit or ChildUnit does not exist
    */
    void addParentChildRelation(const openfluid::core::UnitClassID_t& ParentUnit,
                                const openfluid::core::UnitClassID_t& ChildUnit);

    /**
      Removes the ParentUnit from the list of "Parent" relations of ChildUnit.
      It does nothing if the relation does not exists.
      @param[in] ParentUnit the source spatial unit of the relation
      @param[in] ChildUnit the target spatial unit of the relation
      @throw openfluid::base::FrameworkException if ChildUnit does not exist
    */
    void removeParentChildRelation(const openfluid::core::UnitClassID_t& ParentUnit,
                                   const openfluid::core::UnitClassID_t& ChildUnit);

    /**
      Removes all relations related to the given spatial unit.
      @param[in] Unit units class and ID of the spatial unit
      @throw openfluid::base::FrameworkException if the spatial unit does not exist
    */
    void clearRelations(const openfluid::core::UnitClassID_t& Unit);

    /**
      Gets the value of a spatial attribute
      @param[in] ClassName the units class of the spatial unit
      @param[in] ID the ID of the spatial unit
      @param[in] AttrName the name of the attribute
      @return the value of the attribute as a string
      @throw openfluid::base::FrameworkException if the spatial unit or the attribute does not exist
    */
    std::string getAttribute(const openfluid::core::UnitsClass_t& ClassName,
                             const openfluid::core::UnitID_t& ID,
                             const openfluid::core::AttributeName_t& AttrName) const;

    /**
      Sets the value of an attribute on a spatial unit
      @param[in] ClassName the units class of the spatial unit
      @param[in] ID the ID of the spatial unit
      @param[in] AttrName the name of the attribute
      @param[in] AttrValue the value of the attribute
      @throw openfluid::base::FrameworkException if the spatial unit or the attribute does not exist
    */
    void setAttribute(const openfluid::core::UnitsClass_t& ClassName,
                      const openfluid::core::UnitID_t& ID,
                      const openfluid::core::AttributeName_t& AttrName,
                      const std::string& AttrValue);

    /**
      Returns the attributes names for a given units class
      @param ClassName the units class of the spatial unit
      @return a set of attributes names for the given class, an empty set if the units class does not exist.
    */
    std::set<std::string> getAttributesNames(const openfluid::core::UnitsClass_t& ClassName) const;

    /**
      @param[in] ClassName the units class of the spatial unit
      @param[in] AttrName the name of the attribute
      @param[in] DefaultValue the value of the attribute
      @param[in] Check enables internal checks when set to true (default value = true)
      @throw openfluid::base::FrameworkException if the spatial unit does not exist,
        or if Check is true and the attribute already exists
    */
    void addAttribute(const openfluid::core::UnitsClass_t& ClassName, const std::string& AttrName,
                      const std::string& DefaultValue,
                      bool Check = true);

    /**
      Deletes an attribute for the given units class
      @param[in] ClassName the units class of the spatial unit
      @param[in] AttrName the name of the attribute
      @throw openfluid::base::FrameworkException if the units class or attribute does not exist
    */
    void deleteAttribute(const openfluid::core::UnitsClass_t& ClassName, const std::string& AttrName);

    /**
      Renames an attribute
      @param[in] ClassName the units class of the spatial unit
      @param[in] OldAttrName the old name of the attribute
      @param[in] NewAttrName the new name of the attribute
      @throw openfluid::base::FrameworkException if the units class or attribute does not exist
    */
    void renameAttribute(const openfluid::core::UnitsClass_t& ClassName, const std::string& OldAttrName,
                         const std::string& NewAttrName);

    /**
      Adds an event from an event descriptor. If the spatial unit of the event does not exist, the event is ignored
      @param[in] EvDesc the event descriptor to add
    */
    void addEvent(const openfluid::fluidx::EventDescriptor& EvDesc);

    /**
      Adds an event from an event definition. If the spatial unit of the event does not exist, the event is ignored
      @param[in] UnitsClass the spatial units class
      @param[in] UnitID the spatial unit ID
      @param[in] Event the event definition
      @return the ID assigned to the added event
    */
    EventID_t addEvent(const openfluid::core::UnitsClass_t& UnitsClass, const openfluid::core::UnitID_t& UnitID,
                       const openfluid::core::Event& Event);

    /**
      Deletes an event
      @param[in] UnitsClass the spatial units class
      @param[in] UnitID the spatial unit ID
      @param[in] EventID the ID of the event to delete
      @throw openfluid::base::FrameworkException if the spatial units does not exist
    */
    void deleteEvent(const openfluid::core::UnitsClass_t& UnitsClass, const openfluid::core::UnitID_t& UnitID,
                     const openfluid::fluidx::EventID_t& EventID);

    /**
      Modify an event
      @param[in] EventID the ID of the event to modify
      @param[in] Event the new definition of the event
    */
    void modifyEvent(const openfluid::fluidx::EventID_t& EventID,
                     const openfluid::core::Event& Event);

    /**
      Returns a reference to the event descriptor corresponding to the event ID
      @param[in] ID The ID of the event
      @throw openfluid::base::FrameworkException if the event ID does not exist
    */
    openfluid::fluidx::EventDescriptor& event(const openfluid::fluidx::EventID_t& ID);

    /**
      Returns a const reference to the event descriptor corresponding to the event ID
      @param[in] ID The ID of the event
      @throw openfluid::base::FrameworkException if the event ID does not exist
    */
    const openfluid::fluidx::EventDescriptor& event(const openfluid::fluidx::EventID_t& ID) const;

    /**
      Clears the spatial domain
    */
    void clearDomain();
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_SPATIALDOMAINDESCRIPTOR_HPP__ */
