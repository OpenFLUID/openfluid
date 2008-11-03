/**
  \file GroundwaterUnit.h
  \brief header of GHU management class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __GROUNDWATERUNIT_H__
#define __GROUNDWATERUNIT_H__

//#include <list>
#include "HydroObject.h"
#include "SurfaceUnit.h"


namespace openfluid { namespace core {

class SurfaceUnit;
class ReachSegment;

/**
  \brief Definition class for Groundwater Hydrological Units

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class GroundwaterUnit : public HydroObject
{

  private:
    double m_UsrArea;
    UnitID m_GUExchangeID;
		GroundwaterUnit *mp_GUExchange;
		double m_SubstratumLevel;
    std::list<SurfaceUnit*> *mp_SUsExchange;
    std::list<ReachSegment*>* mp_RSsExchange;
    std::list<GroundwaterUnit*> *mp_GUsExchange;
    
    

  public:
	  /**
	    Default constructor
	  */
	  GroundwaterUnit();

	  /**
	    Constructor
	  */
		GroundwaterUnit(UnitID ID, int ProcessOrder,
                    UnitID GUExchangeID, double SubstratumLevel);

	  /**
	    Destructor
	  */
		~GroundwaterUnit();

    /**
      Returns user defined area
      \return a double
    */
    double getUsrArea() const;
    
    UnitID getGUExchangeID() const;

    /**
      Returns substratum level
      \return a double
    */
		double getSubstratumLevel() const;

    void setGUExchange(GroundwaterUnit *GU);

		void setSubstratumLevel(double Level);
    
    void setUsrArea(double Area) { m_UsrArea = Area; };

    std::list<SurfaceUnit*>* getSUsExchange() { return mp_SUsExchange; };  
    
    std::list<ReachSegment*>* getRSsExchange() { return mp_RSsExchange; };  

    std::list<GroundwaterUnit*>* getGUsExchange() { return mp_GUsExchange; };   
    
    GroundwaterUnit* getGUExchange() { return mp_GUExchange; };

};


} } // namespace openfluid::core

#endif
