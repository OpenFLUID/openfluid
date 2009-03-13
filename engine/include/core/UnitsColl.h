
#ifndef __UNITSCOLL_H__
#define __UNITSCOLL_H__




#include "TypeDefs.h"
#include "Unit.h"


namespace openfluid { namespace core {


class Unit;

/**
  Type definition for a list of units
*/
typedef std::list<Unit> UnitsList_t;


class UnitsCollection
{
  private :

    UnitsList_t m_Data;


  public :

    UnitsCollection();

    ~UnitsCollection();

    Unit* getUnit(UnitID_t aUnitID);

    bool addUnit(Unit aUnit);

    void sortByProcessOrder();

    const UnitsList_t* getList() const { return &m_Data; };

    UnitsList_t* getList() { return &m_Data; };

};

} } // namespaces


#endif /* __UNITSCOLL_H__ */
