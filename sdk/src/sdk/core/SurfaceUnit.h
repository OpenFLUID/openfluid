/**
  \file SurfaceUnit.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __SURFACEUNIT_H__
#define __SURFACEUNIT_H__


#include "HydroObject.h"
#include "GroundwaterUnit.h"
#include "ChronDataSource.h"




namespace mhydasdk { namespace core {



/**
  \brief Enumération des types possibles d'écoulement en sortie d'une unité de surface

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
enum SUFlowCode
{
  SU,
  RLat,
  RSrc,
  UnknownFlowCode
};



/**
  \brief Class definition for Surface HYdrological Units (SHUs)

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SurfaceUnit : public HydroObject
{


	private:


	protected:
    double m_UsrArea;
    double m_UsrSlope;
		SUFlowCode m_FlowCode;
		hoid_t m_FlowID;
		HydroObject *mp_FlowObject;
		double m_FlowDistance;
		hoid_t m_GUExchangeID;
		GroundwaterUnit *mp_GUExchange;
		ChronDataSource *mp_RainSource;

	public:

	  /**
	    Default constructor
	  */
	  SurfaceUnit();

	  /**
	    Constructor
	  */
		SurfaceUnit(hoid_t ID, int ProcessOrder,
		            double UsrArea, double UsrSlope,
		            SUFlowCode FlowCode, hoid_t FlowID, double FlowDistance, hoid_t GUExchangeID);
		~SurfaceUnit();


    ChronDataSource* getRainSource();

    double getUsrArea() const;

    double getUsrSlope() const;

		SUFlowCode getFlowCode() const;

		hoid_t getFlowID() const;

		double getFlowDistance() const;

		hoid_t getGUExchangeID() const;

    void setRainSource(ChronDataSource* RainSource);

    void setUsrArea(double Area);

    void setUsrSlope(double Slope);

		void setFlowCode(SUFlowCode Code);

		void setFlowID(hoid_t ID);

		void setFlowDistance(double Distance);

		void setGUExchangeID(hoid_t ID);

    void setGUExchange(GroundwaterUnit *GHU);

    void setFlowObject(HydroObject *HObject);


};


} } // namespace mhydasdk::core


#endif
