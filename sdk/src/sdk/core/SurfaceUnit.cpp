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
	m_FlowCode = UnknownFlowCode;
	m_FlowID = -1;
	m_FlowDistance = -1;
	m_GUExchangeID = -1;

  mp_FlowObject = NULL;
	mp_GUExchange = NULL;
  mp_RainSource = NULL;

}


// =====================================================================
// =====================================================================


SurfaceUnit::SurfaceUnit(hoid_t ID, int ProcessOrder,
		         double UsrArea, double UsrSlope,
		         SUFlowCode FlowCode, int FlowID, double FlowDistance, hoid_t GUExchangeID)
           : HydroObject(ID, ProcessOrder)
{


  m_UsrArea = UsrArea;
  m_UsrSlope = UsrSlope;
	m_FlowCode = FlowCode;
	m_FlowID = FlowID;
	m_FlowDistance = FlowDistance;
	m_GUExchangeID = GUExchangeID;

  mp_FlowObject = NULL;
	mp_GUExchange = NULL;
	mp_RainSource = NULL;

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


SUFlowCode SurfaceUnit::getFlowCode() const
{
  return m_FlowCode;
}


// =====================================================================
// =====================================================================


hoid_t SurfaceUnit::getFlowID() const
{
  return m_FlowID;
}


// =====================================================================
// =====================================================================


double SurfaceUnit::getFlowDistance() const
{
  return m_FlowDistance;
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


void SurfaceUnit::setFlowCode(SUFlowCode Code)
{
  m_FlowCode = Code;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setFlowID(hoid_t ID)
{
  m_FlowID = ID;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setFlowDistance(double Distance)
{
  m_FlowDistance = Distance;
}


// =====================================================================
// =====================================================================


void SurfaceUnit::setGUExchangeID(hoid_t ID)
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


void SurfaceUnit::setFlowObject(HydroObject *HObject)
{
  mp_FlowObject = HObject;
}



} } // namespace mhydasdk::core

