/**
  \file GroundwaterUnit.h
  \brief header of GHU management class

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __GROUNDWATERUNIT_H__
#define __GROUNDWATERUNIT_H__

#include <list>
#include "HydroObject.h"
#include "SurfaceUnit.h"

namespace mhydasdk { namespace core {

class SurfaceUnit;

/**
  \brief Definition class for Groundwater Hydrological Units

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class GroundwaterUnit : public HydroObject
{

  private:
    double m_UsrArea;
    hoid_t m_GUExchangeID;
		GroundwaterUnit *mp_GUExchange;
		double m_SubstratumLevel;
    std::list<SurfaceUnit*>* mp_SUsExchange;
    

  public:
	  /**
	    Default constructor
	  */
	  GroundwaterUnit();

	  /**
	    Constructor
	  */
		GroundwaterUnit(hoid_t ID, int ProcessOrder,
                    hoid_t GUExchangeID, double SubstratumLevel);

	  /**
	    Destructor
	  */
		~GroundwaterUnit();

    /**
      Returns user defined area
      \return a double
    */
    double getUsrArea() const;
    
    hoid_t getGUExchangeID() const;

    /**
      Returns substratum level
      \return a double
    */
		double getSubstratumLevel() const;

    void setGUExchange(GroundwaterUnit *GU);

		void setSubstratumLevel(double Level);
    
    void setUsrArea(double Area) { m_UsrArea = Area; };

    std::list<SurfaceUnit*>* getSUsExchange() { return mp_SUsExchange; };     

};


} } // namespace mhydasdk::core

#endif
