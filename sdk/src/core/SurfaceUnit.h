/**
  \file SurfaceUnit.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __SURFACEUNIT_H__
#define __SURFACEUNIT_H__


#include <list>
#include "HydroObject.h"
#include "GroundwaterUnit.h"
#include "ChronDataSource.h"



namespace mhydasdk { namespace core {


class GroundwaterUnit;



/**
  \brief Enumerates possible flow types from a surface unit

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
enum SUDownstreamCode
{
  SUDownstream,
  RLatDownstream,
  RSrcDownstream,
  UnknownDownstreamCode
};



/**
  \brief Class definition for Surface Units (SUs)

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SurfaceUnit : public HydroObject
{


  private:


  protected:
    double m_UsrArea;
    double m_UsrSlope;
    SUDownstreamCode m_DownstreamCode;
    hoid_t m_DownstreamID;
    HydroObject *mp_DownstreamObject;
    double m_DownstreamDistance;
    hoid_t m_GUExchangeID;
    GroundwaterUnit *mp_GUExchange;
    ChronDataSource *mp_RainSource;
    std::list<SurfaceUnit*>* mp_UpstreamSUs;

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
	            SUDownstreamCode DSCode, hoid_t DownstreamID, double DSDistance, hoid_t GUExchangeID);
    ~SurfaceUnit();


    ChronDataSource* getRainSource();

    double getUsrArea() const;

    double getUsrSlope() const;

    SUDownstreamCode getDownstreamCode() const;

    hoid_t getDownstreamID() const;

    double getDownstreamDistance() const;

    hoid_t getGUExchangeID() const;

    std::list<SurfaceUnit*>* getUpstreamSUs() { return mp_UpstreamSUs; };     

    void setRainSource(ChronDataSource* RainSource);

    void setUsrArea(double Area);

    void setUsrSlope(double Slope);

    void setDownstreamCode(SUDownstreamCode Code);

    void setDownstreamID(hoid_t ID);

    void setDownstreamDistance(double Distance);

    void setGUExchangeID(hoid_t ID);

    void setGUExchange(GroundwaterUnit *GHU);

    void setDownstreamObject(HydroObject *HObject);


};


} } // namespace mhydasdk::core


#endif
