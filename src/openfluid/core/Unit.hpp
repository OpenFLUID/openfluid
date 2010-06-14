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
  @file

  @author JC.Fabre <fabrejc@supagro.inra.fr>
*/




#ifndef __UNIT_H__
#define __UNIT_H__


#include <map>
#include <string>

#include <openfluid/core/UnitsColl.hpp>
#include <openfluid/core/InputData.hpp>
#include <openfluid/core/Variables.hpp>
#include <openfluid/core/EventsColl.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

class DLLEXPORT UnitsCollection;



/**
  Type for a hashmap of lists of units, indexed by UnitClass
*/
typedef std::map<UnitClass_t,UnitsCollection> UnitsListByClassMap_t;

class DLLEXPORT Unit;

/**
  Type for a list of pointers on Unit
*/
typedef std::list<Unit*> UnitsPtrList_t;

/**
  Type for a map associating a unit class to a list of pointers on Unit
*/
typedef std::map<UnitClass_t,UnitsPtrList_t> LinkedUnitsListByClassMap_t;

/**
  Class defining a spatial unit

  example of use:
  @code
  openfluid::core::Unit aUnit;
  openfluid::core::Unit* aUnitPtr;
  openfluid::core::UnitClass_t aUnitClass;
  openfluid::core::UnitID_t aUnitID;
  openfluid::core::UnitsPtrList_t* aUnitListPtr;


  // *** get unit ID ***
  aUnitID = aUnit.getID();
  // or (pointed unit)
  aUnitID = aUnitPtr->getID();

  // *** get unit class ***
  aUnitClass = aUnit.getClass();
  // or (pointed unit)
  aUnitClass = aUnitPtr->getClass();

  // *** get connected units (to and from) ***
  aUnitListPtr = aUnit.getToUnits("foo");
  // or
  aUnitListPtr = aUnit.getFromUnits("bar");
  @endcode
*/
class DLLEXPORT Unit
{
  private:
    UnitID_t m_ID;
    UnitClass_t m_Class;
    unsigned int m_PcsOrder;

    LinkedUnitsListByClassMap_t m_FromUnits;
    LinkedUnitsListByClassMap_t m_ToUnits;
    LinkedUnitsListByClassMap_t m_ParentUnits;
    LinkedUnitsListByClassMap_t m_ChildrenUnits;

    InputData<InputDataValue> m_InputData;

    Variables<ScalarValue> m_ScalarVariables;
    Variables<VectorValue> m_VectorVariables;

    EventsCollection m_Events;

  public:

    /*
          Constructor
      @param[in] aClass the class of the unit
      @param[in] anID the ID of the unit
      @param[in] aPcsOrder the process order of the unit
        */
    Unit(const UnitClass_t aClass, const UnitID_t anID, const PcsOrd_t aPcsOrder);

    /*
          Destructor
        */
    ~Unit();

    /**
      Returns the process order of the unit
    */
    unsigned int getProcessOrder() const { return m_PcsOrder; };

    /**
      Returns the ID of the unit
    */
    UnitID_t getID() const { return m_ID; };


    /**
      Returns the class of the unit
    */
    UnitClass_t getClass() const { return m_Class; };

    bool addToUnit(Unit* aUnit);

    bool addFromUnit(Unit* aUnit);

    bool addParentUnit(Unit* aUnit);

    bool addChildUnit(Unit* aUnit);

    /**
      Returns a list of units, of the requested class, connected to this unit.
      Returns NULL if no units of the requested class are connected to this unit.
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* getToUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getToUnits(const UnitClass_t aClass) const;

    /**
      Returns a list of units, of the requested class, connected from this unit.
      Returns NULL if no units of the requested class are connected from this unit.
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* getFromUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getFromUnits(const UnitClass_t aClass) const;

    /**
      Returns a list of parent units of the requested class.
      Returns NULL if this unit has no parent
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* getParentUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getParentUnits(const UnitClass_t aClass) const;

    /**
      Returns a list of children units of the requested class.
      Returns NULL if this unit has no child
      @param[in] aClass the requested class
    */
    UnitsPtrList_t* getChildrenUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getChildrenUnits(const UnitClass_t aClass) const;




    InputData<InputDataValue>* getInputData()  { return &m_InputData; };

    const InputData<InputDataValue>* getInputData() const { return &m_InputData; };

    Variables<ScalarValue>* getScalarVariables() { return &m_ScalarVariables; };

    const Variables<ScalarValue>* getScalarVariables() const { return &m_ScalarVariables; };

    Variables<VectorValue>* getVectorVariables() { return &m_VectorVariables; };

    const Variables<VectorValue>* getVectorVariables() const { return &m_VectorVariables; };

    EventsCollection* getEvents() { return &m_Events; };

    const EventsCollection* getEvents() const { return &m_Events; };

    void streamContents(std::ostream& OStream);

};


} } // namespaces


#endif /* __UNIT_H__ */

