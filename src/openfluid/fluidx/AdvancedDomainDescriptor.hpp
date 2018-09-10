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
  @file AdvancedDomainDescriptor.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
*/


#ifndef __OPENFLUID_FLUIDX_ADVANCEDDOMAINDESCRIPTOR_HPP__
#define __OPENFLUID_FLUIDX_ADVANCEDDOMAINDESCRIPTOR_HPP__


#include <map>
#include <set>

#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/EventDescriptor.hpp>


namespace openfluid {

namespace core {
class Event;
}

namespace fluidx {

class SpatialDomainDescriptor;
class SpatialUnitDescriptor;
class AttributesDescriptor;


// =====================================================================
// =====================================================================


class OPENFLUID_API AdvancedUnitDescriptor
{

  public:

    openfluid::fluidx::SpatialUnitDescriptor* UnitDescriptor;

    std::map<openfluid::core::AttributeName_t, std::string*> Attributes;

    std::list<openfluid::fluidx::EventDescriptor*> EventsDescriptors;

    AdvancedUnitDescriptor(openfluid::fluidx::SpatialUnitDescriptor& UnitDesc) :
        UnitDescriptor(&UnitDesc)
    {
    }

};


// =====================================================================
// =====================================================================


class OPENFLUID_API AdvancedDomainDescriptor
{
  public:

    typedef std::map<std::string, std::map<int, AdvancedUnitDescriptor> > UnitsByIdByClass_t;

  private:

    openfluid::fluidx::SpatialDomainDescriptor* mp_DomainDesc;

    /**
     * Units by ID by ClassName
     */
    std::map<std::string, std::map<int, AdvancedUnitDescriptor> > m_Units;

    std::map<std::string, std::set<openfluid::core::AttributeName_t> > m_AttrsNames;

    void dispatchUnits();

    void dispatchAttributes();

    void checkUnitRelations() const;

    void checkUnitRelations(openfluid::fluidx::SpatialUnitDescriptor& Unit) const;

    void checkAttributesConsistency() const;

    void dispatchEvents();


  public:

    AdvancedDomainDescriptor(openfluid::fluidx::SpatialDomainDescriptor& DomainDesc);

    ~AdvancedDomainDescriptor();

    bool isSpatialUnitExist(const std::string& ClassName, int ID) const;

    /**
      @return May return an empty map. Never throws.
    */
    const std::map<std::string, std::map<int, AdvancedUnitDescriptor> >& spatialUnitsByIdByClass() const;

    /**
      @throw openfluid::base::FrameworkException if Unit doesn't exist
      @param ClassName
      @param ID
      @return
    */
    const AdvancedUnitDescriptor& spatialUnit(const std::string& ClassName, int ID) const;

    /**
      @throw openfluid::base::FrameworkException if Unit doesn't exist
      @param ClassName
      @param ID
      @return
    */
    const openfluid::fluidx::SpatialUnitDescriptor& spatialUnitDescriptor(const std::string& ClassName,
                                                                          int ID) const;

    /**
      @param ClassName
      @return An empty set if ClassName doesn't exist. Never throws.
    */
    std::set<int> getIDsOfClass(const std::string& ClassName) const;

    bool isClassNameExists(const std::string& ClassName) const;

    /**
      @return May return an empty set. Never throws.
    */
    std::set<std::string> getClassNames() const;

    unsigned int getUnitsCount() const;

    /**
      @details Add this UnitDesc ID to the descriptor for all attributes of this UnitDesc class,
               with a default value of "-"
      @warning Invalidate UnitDesc
      @throw openfluid::base::FrameworkException if Unit already exists or if Unit has a relation
             with a non-existent Unit
      @param UnitDesc
    */
    void addUnit(openfluid::fluidx::SpatialUnitDescriptor* UnitDesc);

    /**
      Does nothing if Unit doesn't exist
      @param ClassName
      @param ID
    */
    void deleteUnit(const std::string& ClassName, int ID);

    /**
      @throw openfluid::base::FrameworkException if Unit or DataName doesn't exist
      @param ClassName
      @param ID
      @param AttrName
      @return
    */
    std::string& attribute(const std::string& ClassName, int ID,
                           const std::string& AttrName);

    /**
      @param ClassName
      @return An empty set if ClassName doesn't exist or has no attribute. Never throws.
    */
    std::set<std::string> getAttributesNames(const std::string& ClassName) const;

    /**
      @throw openfluid::base::FrameworkException if ClassName doesn't exist
             or if AttrName already exists for class ClassName
      @param ClassName
      @param AttrName
      @param DefaultValue
    */
    void addAttribute(const std::string& ClassName, const std::string& AttrName,
                      const std::string& DefaultValue);

    /**
      @throw openfluid::base::FrameworkException if ClassName doesn't exist
             or if AttrName doesn't exist for class ClassName
      @param ClassName
      @param AttrName
    */
    void deleteAttribute(const std::string& ClassName, const std::string& AttrName);

    /**
      Does nothing if NewDataName is the same as OldDataName
      @throw openfluid::base::FrameworkException if ClassName doesn't exist
             or if OldDataName doesn't exist for the class ClassName
      @param ClassName
      @param OldAttrName
      @param NewAttrName
    */
    void renameAttribute(const std::string& ClassName, const std::string& OldAttrName,
                         const std::string& NewAttrName);

    /**
      Adds an event
      @param[in] UnitsClass The units class of the event
      @param[in] UnitID The unit ID of the event
      @param[in] Event The event definition
    */
    void addEvent(const openfluid::core::UnitsClass_t& UnitsClass, const openfluid::core::UnitID_t& UnitID,
                     const openfluid::core::Event& Event);

    /**
      Deletes an event
      @param[in] UnitsClass The units class of the event
      @param[in] UnitID The unit ID of the event
      @param[in] EventID The ID of the event to delete
    */
    void deleteEvent(const openfluid::core::UnitsClass_t& UnitsClass, const openfluid::core::UnitID_t& UnitID,
                     const openfluid::fluidx::EventID_t& EventID);

    /**
      Modify an event
      @param[in] EventID The ID of the event to modify
      @param[in] Event The new content of the event
    */
    void modifyEvent(const openfluid::fluidx::EventID_t& EventID,
                     const openfluid::core::Event& Event);

    /**
      Returns a pointer to the event descriptor corresponding to the event ID
      @param[in] ID The ID of the event to delete
    */
    openfluid::fluidx::EventDescriptor* eventDescriptor(const openfluid::fluidx::EventID_t& ID);


    const std::list<openfluid::core::UnitClassID_t>& toSpatialUnits(const openfluid::core::UnitClassID_t Unit) const;

    const std::list<openfluid::core::UnitClassID_t>&
      parentSpatialUnits(const openfluid::core::UnitClassID_t Unit) const;

    std::list<openfluid::core::UnitClassID_t> getFromSpatialUnits(const openfluid::core::UnitClassID_t Unit) const;

    std::list<openfluid::core::UnitClassID_t> getChildSpatialUnits(const openfluid::core::UnitClassID_t Unit) const;

    /**
      @brief Add ToUnit to the list of "Tos" of FromUnit
      @details Does nothing if the relation already exists
      @throw openfluid::base::FrameworkException if FromUnit or ToUnit doesn't exist
    */
    void addFromToRelation(const openfluid::core::UnitClassID_t FromUnit,
                           const openfluid::core::UnitClassID_t ToUnit);

    /**
      @brief Remove ToUnit from the list of "Tos" of FromUnit
      @throw openfluid::base::FrameworkException if FromUnit or ToUnit doesn't exist or if the relation doesn't exists
    */
    void removeFromToRelation(const openfluid::core::UnitClassID_t FromUnit,
                              const openfluid::core::UnitClassID_t ToUnit);

    /**
      @brief Add ChildUnit to the list of "Children" of ParentUnit
      @details Does nothing if the relation already exists
      @throw openfluid::base::FrameworkException if ParentUnit or ChildUnit doesn't exist
    */
    void addParentChildRelation(const openfluid::core::UnitClassID_t ParentUnit,
                                const openfluid::core::UnitClassID_t ChildUnit);

    /**
      @brief Remove ChildUnit from the list of "Children" of ParentUnit
      @throw openfluid::base::FrameworkException if ParentUnit or ChildUnit doesn't exist
      or if the relation doesn't exists
    */
    void removeParentChildRelation(
        const openfluid::core::UnitClassID_t ParentUnit,
        const openfluid::core::UnitClassID_t ChildUnit);

    /**
      @brief Clear the list of "Tos" and "Parents" of Unit,
      and remove Unit from the list of "Tos" and "Parents" of all other Units of the Domain
      @throw openfluid::base::FrameworkException if Unit doesn't exist
    */
    void clearRelations(const openfluid::core::UnitClassID_t Unit);

    void clearDomain();
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_ADVANCEDDOMAINDESCRIPTOR_HPP__ */

