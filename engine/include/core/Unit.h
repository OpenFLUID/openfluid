



#ifndef __UNIT_H__
#define __UNIT_H__


#include <map>
#include <string>

#include "UnitsColl.h"
#include "InputData.h"
#include "Variables.h"

namespace openfluid { namespace core {

class UnitsCollection;



/**
  Type for a hashmap of lists of units, indexed by UnitClass
*/
typedef std::map<UnitClass_t,UnitsCollection> UnitsListByClassMap_t;

class Unit;
typedef std::list<Unit*> UnitsPtrList_t;
typedef std::map<UnitClass_t,UnitsPtrList_t> LinkedUnitsListByClassMap_t;

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



  public:

    Unit(const UnitClass_t aClass, const UnitID_t anID, const PcsOrd_t aPcsOrder);

    ~Unit();

    unsigned int getProcessOrder() const { return m_PcsOrder; };

    UnitID_t getID() const { return m_ID; };

    UnitClass_t getClass() const { return m_Class; };

    bool addToUnit(Unit* aUnit);

    bool addFromUnit(Unit* aUnit);

    UnitsPtrList_t* getToUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getToUnits(const UnitClass_t aClass) const;

    UnitsPtrList_t* getFromUnits(const UnitClass_t aClass);

    const UnitsPtrList_t* getFromUnits(const UnitClass_t aClass) const;

    InputData<ScalarValue>* getInputData()  { return &m_InputData; };

    const InputData<ScalarValue>* getInputData() const { return &m_InputData; };

    Variables<ScalarValue>* getScalarVariables() { return &m_ScalarVariables; };

    const Variables<ScalarValue>* getScalarVariables() const  { return &m_ScalarVariables; };

    Variables<VectorValue>* getVectorVariables() { return &m_VectorVariables; };

    const Variables<VectorValue>* getVectorVariables() const { return &m_VectorVariables; };

    void printSTDOUT();

/*    bool getInputData(const InputDataName_t aName, ScalarValue* aValue) const;

    bool setInputData(const InputDataName_t aName, const ScalarValue aValue);*/

};


} } // namespaces


#endif /* __COREREPOSITORY_H__ */

