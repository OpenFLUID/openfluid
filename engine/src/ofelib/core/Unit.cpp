
#include "Unit.h"

namespace openfluid { namespace core {


// =====================================================================
// =====================================================================


Unit::Unit(const UnitClass_t aClass, const UnitID_t anID, const PcsOrd_t aPcsOrder)
{
  m_Class = aClass;
  m_ID = anID;
  m_PcsOrder = aPcsOrder;

  // TODO clean this

//  m_ScalarVariables = Variables<ScalarValue>();

//  std:: cout << "Unit constructor " << aClass << " " << anID << std::endl;
}


// =====================================================================
// =====================================================================


Unit::~Unit()
{

}

// =====================================================================
// =====================================================================


bool Unit::addToUnit(Unit* aUnit)
{
  m_ToUnits[aUnit->getClass()].push_back(aUnit);
  return true;
}

// =====================================================================
// =====================================================================


bool Unit::addFromUnit(Unit* aUnit)
{
  m_FromUnits[aUnit->getClass()].push_back(aUnit);
  return true;

}

// =====================================================================
// =====================================================================


const UnitsPtrList_t* Unit::getToUnits(const UnitClass_t aClass) const
{
  LinkedUnitsListByClassMap_t::const_iterator it = m_ToUnits.find(aClass);

  if (it != m_ToUnits.end())
    return &(it->second);
  else return NULL;


}

// =====================================================================
// =====================================================================


const UnitsPtrList_t* Unit::getFromUnits(const UnitClass_t aClass) const
{
  LinkedUnitsListByClassMap_t::const_iterator it = m_FromUnits.find(aClass);

  if (it != m_FromUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================


UnitsPtrList_t* Unit::getToUnits(const UnitClass_t aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ToUnits.find(aClass);

  if (it != m_ToUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================


UnitsPtrList_t* Unit::getFromUnits(const UnitClass_t aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_FromUnits.find(aClass);

  if (it != m_FromUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================


void Unit::printSTDOUT()
{
  UnitsPtrList_t::iterator IDIt;
  LinkedUnitsListByClassMap_t::iterator ClassIt;
  UnitsPtrList_t UnitsList;

  std::cout << m_Class << " #"<< m_ID << " (order: " << m_PcsOrder << ")";

  if (m_ToUnits.size() > 0 )
  {
    std::cout << " To[";

    for (ClassIt=m_ToUnits.begin();ClassIt!=m_ToUnits.end();++ClassIt)
    {

      UnitsList = ClassIt->second;

     for (IDIt=UnitsList.begin();IDIt!=UnitsList.end();++IDIt)
      {
        std::cout << "(" << (*IDIt)->getClass() << "," <<  (*IDIt)->getID() << ") ";
      }

    }
    std::cout << "] ";
  }

  if (m_FromUnits.size() > 0 )
  {
    std::cout << " From[";

    for (ClassIt=m_FromUnits.begin();ClassIt!=m_FromUnits.end();++ClassIt)
    {

      UnitsList = ClassIt->second;

     for (IDIt=UnitsList.begin();IDIt!=UnitsList.end();++IDIt)
      {
        std::cout << "(" << (*IDIt)->getClass() << "," <<  (*IDIt)->getID() << ") ";
      }

    }
    std::cout << "] ";
  }

  std::cout << std::endl;
}



} } // namespaces
