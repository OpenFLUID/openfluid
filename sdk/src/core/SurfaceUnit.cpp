/**
  \file SurfaceUnit.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "SurfaceUnit.h"

namespace mhydasdk { namespace core {


// =====================================================================
// =====================================================================


SurfaceUnit::SurfaceUnit()
           : HydroObject()
{
  m_UsrArea = -1;
  m_UsrSlope = -1;
  m_DownstreamCode = UnknownDownstreamCode;
  m_DownstreamID = -1;
  m_DownstreamDistance = -1;
  m_GUExchangeID = -1;

  mp_DownstreamObject = NULL;
	mp_GUExchange = NULL;
  mp_RainSource = NULL;
  mp_UpstreamSUs = new std::list<SurfaceUnit*>(); 

}


// =====================================================================
// =====================================================================


SurfaceUnit::SurfaceUnit(HOID ID, int ProcessOrder,
		         double UsrArea, double UsrSlope,
		         SUDownstreamCode DSCode, int DownstreamID, double DSDistance, HOID GUExchangeID)
           : HydroObject(ID, ProcessOrder)
{


  m_UsrArea = UsrArea;
  m_UsrSlope = UsrSlope;
	m_DownstreamCode = DSCode;
	m_DownstreamID = DownstreamID;
	m_DownstreamDistance = DSDistance;
	m_GUExchangeID = GUExchangeID;

  mp_DownstreamObject = NULL;
	mp_GUExchange = NULL;
	mp_RainSource = NULL;
  mp_UpstreamSUs = new std::list<SurfaceUnit*>();   

}


// =====================================================================
// =====================================================================


SurfaceUnit::~SurfaceUnit()
{

}


// =====================================================================
// =====================================================================


ChronDataSource* SurfaceUnit::getRainSource()
{

  return mp_RainSource;
}


// =====================================================================
// =====================================================================


double SurfaceUnit::getUsrArea() const
{
  return m_UsrArea;
}


// =====================================================================
// =====================================================================


double SurfaceUnit::getUsrSlope() const
{
  return m_UsrSlope;
}


// =====================================================================
// =====================================================================


SUDownstreamCode SurfaceUnit::getDownstreamCode() const
{
  return m_DownstreamCode;
}


// =====================================================================
// =====================================================================


HOID SurfaceUnit::getDownstreamID() const
{
  return m_DownstreamID;
}


// =====================================================================
// =====================================================================


double SurfaceUnit::getDownstreamDistance() const
{
  return m_DownstreamDistance;
}


// =====================================================================
// =====================================================================


int SurfaceUnit::getGUExchangeID() const
{
  return m_GUExchangeID;
}


// =====================================================================
// =====================================================================



void SurfaceUnit::setRainSource(ChronDataSource* RainSource)
{
  mp_RainSource = RainSource;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setUsrArea(double Area)
{
  m_UsrArea = Area;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setUsrSlope(double Slope)
{
  m_UsrSlope = Slope;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setDownstreamCode(SUDownstreamCode Code)
{
  m_DownstreamCode = Code;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setDownstreamID(HOID ID)
{
  m_DownstreamID = ID;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setDownstreamDistance(double Distance)
{
  m_DownstreamDistance = Distance;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setGUExchangeID(HOID ID)
{
  m_GUExchangeID = ID;
}



// =====================================================================
// =====================================================================


void SurfaceUnit::setGUExchange(GroundwaterUnit *GU)
{
  mp_GUExchange = GU;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setDownstreamObject(HydroObject *HObject)
{
  mp_DownstreamObject = HObject;
}



} } // namespace mhydasdk::core

