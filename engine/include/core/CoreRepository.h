
#ifndef __COREREPOSITORY_H__
#define __COREREPOSITORY_H__


#include "Unit.h"
#include "UnitsColl.h"
#include "MemMonitor.h"



namespace openfluid { namespace core {




class CoreRepository
{
  private:

    UnitsListByClassMap_t m_PcsOrderedUnitsByClass;


    static CoreRepository* mp_Singleton;

    MemoryMonitor* mp_MemMonitor;

    CoreRepository();

    bool releaseMemory(TimeStep_t Step);

  public:

    static CoreRepository* getInstance();

    void setMemoryMonitor(MemoryMonitor* MemMonitor) { mp_MemMonitor = MemMonitor; };

    bool addUnit(const Unit aUnit);

    bool sortUnitsByProcessOrder();

    Unit* getUnit(UnitClass_t UnitClass, UnitID_t UnitID);

    UnitsCollection* getUnits(UnitClass_t UnitClass);

    const UnitsCollection* getUnits(UnitClass_t UnitClass) const;

    const UnitsListByClassMap_t* getUnits() const { return &m_PcsOrderedUnitsByClass; };

    bool isUnitsClassExists(UnitClass_t UnitClass) const;

    void printSTDOUT();

//    bool isMemReleaseStep(TimeStep_t Step);

//    bool getMemReleaseRange(TimeStep_t* BeginStep, TimeStep_t* EndStep);

    bool doMemRelease(TimeStep_t Step, bool WithoutKeep);

};


} } // namespaces


#endif /* COREREPOSITORY_H_ */

