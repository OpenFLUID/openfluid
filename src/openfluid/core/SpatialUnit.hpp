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
  @file SpatialUnit.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/



#ifndef __OPENFLUID_CORE_SPATIALUNIT_HPP__
#define __OPENFLUID_CORE_SPATIALUNIT_HPP__


#include <map>
#include <string>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/Attributes.hpp>
#include <openfluid/core/EventsCollection.hpp>
#include <openfluid/core/Variables.hpp>
#include <openfluid/core/UnitsCollection.hpp>



namespace openfluid { namespace core {

//class OPENFLUID_API UnitsCollection;



/**
  Type for a hashmap of lists of units, indexed by UnitClass
*/
typedef std::map<UnitClass_t,UnitsCollection> UnitsListByClassMap_t;


/**
  Type for a list of pointers on Unit
*/
typedef std::list<SpatialUnit*> UnitsPtrList_t;


/**
  Type for a map associating a unit class to a list of pointers on Unit
*/
typedef std::map<UnitClass_t,UnitsPtrList_t> LinkedUnitsListByClassMap_t;


/**
  Class defining a spatial unit

  example of use:
  @code
  openfluid::core::SpatialUnit aUnit;
  openfluid::core::SpatialUnit* aUnitPtr;
  openfluid::core::UnitClass_t aUnitClass;
  openfluid::core::UnitID_t aUnitID;
  openfluid::core::UnitsPtrList_t* aUnitListPtr;


  // get unit ID
  aUnitID = aUnit.getID();
  // or (pointed unit)
  aUnitID = aUnitPtr->getID();

  // get unit class
  aUnitClass = aUnit.getClass();
  // or (pointed unit)
  aUnitClass = aUnitPtr->getClass();

  // get connected units (to and from)
  aUnitListPtr = aUnit.toSpatialUnits("foo");
  // or
  aUnitListPtr = aUnit.fromSpatialUnits("bar");
  @endcode
*/
class OPENFLUID_API SpatialUnit
{
  private:

    UnitID_t m_ID;

    UnitClass_t m_Class;

    // TODO use openfluid::core::PcsOrd_t instead
    unsigned int m_PcsOrder;

    LinkedUnitsListByClassMap_t m_FromUnits;

    LinkedUnitsListByClassMap_t m_ToUnits;

    LinkedUnitsListByClassMap_t m_ParentUnits;

    LinkedUnitsListByClassMap_t m_ChildrenUnits;

    Attributes m_Attributes;

    Variables m_Variables;

    EventsCollection m_Events;


  public:

    /*
      Constructor
      @param[in] aClass the class of the unit
      @param[in] anID the ID of the unit
      @param[in] aPcsOrder the process order of the unit
     */
    SpatialUnit(const UnitClass_t& aClass, const UnitID_t anID,
         const PcsOrd_t aPcsOrder);

    /*
      Destructor
    */
    ~SpatialUnit();

    /**
      Returns the process order of the unit
    */
    inline PcsOrd_t getProcessOrder() const { return m_PcsOrder; };

    /**
      Returns the ID of the unit
    */
    inline UnitID_t getID() const { return m_ID; };


    /**
      Returns the class of the unit
    */
    inline UnitClass_t getClass() const { return m_Class; };

    bool addToUnit(SpatialUnit* aUnit);

    bool addFromUnit(SpatialUnit* aUnit);

    bool addParentUnit(SpatialUnit* aUnit);

    bool addChildUnit(SpatialUnit* aUnit);

    /**
      Returns a list of units, of the requested class, connected to this unit.
      Returns NULL if no units of the requested class are connected to this unit.
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* toSpatialUnits(const UnitClass_t& aClass);

    const UnitsPtrList_t* toSpatialUnits(const UnitClass_t& aClass) const;

    /**
      Returns a list of units, of the requested class, connected from this unit.
      Returns NULL if no units of the requested class are connected from this unit.
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* fromSpatialUnits(const UnitClass_t& aClass);

    const UnitsPtrList_t* fromSpatialUnits(const UnitClass_t& aClass) const;

    /**
      Returns a list of parent units of the requested class.
      Returns NULL if this unit has no parent
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* parentSpatialUnits(const UnitClass_t& aClass);

    const UnitsPtrList_t* parentSpatialUnits(const UnitClass_t& aClass) const;

    /**
      Returns a list of children units of the requested class.
      Returns NULL if this unit has no child
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* childSpatialUnits(const UnitClass_t& aClass);

    const UnitsPtrList_t* childSpatialUnits(const UnitClass_t& aClass) const;

    inline Attributes* attributes()  { return &m_Attributes; };

    inline const Attributes* attributes() const { return &m_Attributes; };

    Variables* variables() { return &m_Variables; };

    const Variables* variables() const { return &m_Variables; };

    inline EventsCollection* events() { return &m_Events; };

    inline const EventsCollection* events() const { return &m_Events; };

    void streamContents(std::ostream& OStream);

    void setProcessOrder(unsigned int PcsOrder) { m_PcsOrder = PcsOrder; };

};


typedef SpatialUnit Unit OPENFLUID_DEPRECATED;


} } // namespace openfluid::core


#endif /* __OPENFLUID_CORE_SPATIALUNIT_HPP__ */

