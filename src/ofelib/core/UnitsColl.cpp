

#include "Unit.h"
#include "UnitsColl.h"

namespace openfluid { namespace core {



struct SortByProcessOrder
{
  bool operator ()(Unit& U1,Unit& U2) const
  {
    return (U1.getProcessOrder() <= U2.getProcessOrder());
  }

};


// =====================================================================
// =====================================================================


UnitsCollection::UnitsCollection()
{

}


// =====================================================================
// =====================================================================


UnitsCollection::~UnitsCollection()
{

}

// =====================================================================
// =====================================================================


Unit* UnitsCollection::getUnit(UnitID_t aUnitID)
{
  UnitsList_t::iterator it;

  for (it=m_Data.begin();it!=m_Data.end();++it)
  {
    if (it->getID() == aUnitID) return &(*it);
  }

  return NULL;
}


// =====================================================================
// =====================================================================



bool UnitsCollection::addUnit(Unit aUnit)
{

  if (getUnit(aUnit.getID()) == NULL)
  {
    m_Data.push_back(aUnit);
    return true;
  }
  else return false;
}

// =====================================================================
// =====================================================================


void UnitsCollection::sortByProcessOrder()
{
  m_Data.sort(SortByProcessOrder());
}





} } // namespaces

