/**
  \file Unit.h
  \brief Header for spatial unit definition
  \author JC.Fabre <fabrejc@supagro.inra.fr>
*/




#ifndef __UNIT_H__
#define __UNIT_H__


#include <map>
#include <string>

#include "UnitsColl.h"
#include "InputData.h"
#include "Variables.h"
#include "EventCollection.h"

namespace openfluid { namespace core {

class UnitsCollection;



/**
  Type for a hashmap of lists of units, indexed by UnitClass
*/
typedef std::map<UnitClass_t,UnitsCollection> UnitsListByClassMap_t;

class Unit;

/**
  Type for a list of pointers on Unit
*/
typedef std::list<Unit*> UnitsPtrList_t;

typedef std::map<UnitClass_t,UnitsPtrList_t> LinkedUnitsListByClassMap_t;

/**
  Class defining a spatial unit

  example of use:
  \code
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
  \endcode
*/
class Unit
{
  private:
    UnitID_t m_ID;
    UnitClass_t m_Class;
    unsigned int m_PcsOrder;

    LinkedUnitsListByClassMap_t m_FromUnits;
    LinkedUnitsListByClassMap_t m_ToUnits;
//    UnitsListByClassMap_t m_AssociatedUnits;

    InputData<ScalarValue> m_InputData;

    Variables<ScalarValue> m_ScalarVariables;
    Variables<VectorValue> m_VectorVariables;

    EventCollection m_Events;


  public:

    /*
      \if DOCFORDEV
      Constructor
      \param[in] aClass the class of the unit
      \param[in] anID the ID of the unit
      \param[in] aPcsOrder the process order of the unit
      \endif
    */
    Unit(const UnitClass_t aClass, const UnitID_t anID, const PcsOrd_t aPcsOrder);

    /*
      \if DOCFORDEV
      Destructor
      \endif
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

    /**
      Returns a list of units, of the requested class, connected to this unit.
      Returns NULL if no units of the requested class are connected to this unit.
      \param[in] aClass the requested class
    */
    UnitsPtrList_t* getToUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getToUnits(const UnitClass_t aClass) const;

    /**
      Returns a list of units, of the requested class, connected from this unit.
      Returns NULL if no units of the requested class are connected from this unit.
      \param[in] aClass the requested class
    */
    UnitsPtrList_t* getFromUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getFromUnits(const UnitClass_t aClass) const;

    InputData<ScalarValue>* getInputData()  { return &m_InputData; };

    const InputData<ScalarValue>* getInputData() const { return &m_InputData; };

    Variables<ScalarValue>* getScalarVariables() { return &m_ScalarVariables; };

    const Variables<ScalarValue>* getScalarVariables() const { return &m_ScalarVariables; };

    Variables<VectorValue>* getVectorVariables() { return &m_VectorVariables; };

    const Variables<VectorValue>* getVectorVariables() const { return &m_VectorVariables; };

    EventCollection* getEvents() { return &m_Events; };

    const EventCollection* getEvents() const { return &m_Events; };

    void printSTDOUT();

};


} } // namespaces


#endif /* __UNIT_H__ */

