/**
  \file GroundwaterUnit.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "GroundwaterUnit.h"




namespace openfluid { namespace core {


GroundwaterUnit::GroundwaterUnit()
           : HydroObject()
{
  m_UsrArea = -1;
  m_GUExchangeID = -1;
	m_SubstratumLevel = -1;

  mp_GUExchange = NULL;
  mp_SUsExchange = new std::list<SurfaceUnit*>(); 
  mp_GUsExchange = new std::list<GroundwaterUnit*>(); 
  mp_RSsExchange = new std::list<ReachSegment*>(); 
  
}


GroundwaterUnit::GroundwaterUnit(UnitID ID,int ProcessOrder,
                                UnitID GUExchangeID, double SubstratumLevel)
           : HydroObject(ID, ProcessOrder)
{
  
  m_UsrArea = -1;

  mp_GUExchange = NULL;
  mp_SUsExchange = new std::list<SurfaceUnit*>();
  mp_GUsExchange = new std::list<GroundwaterUnit*>(); 
  mp_RSsExchange = new std::list<ReachSegment*>();    

    
  m_GUExchangeID = GUExchangeID;
	m_SubstratumLevel = SubstratumLevel;

}

GroundwaterUnit::~GroundwaterUnit()
{

}


// =====================================================================
// =====================================================================


double GroundwaterUnit::getUsrArea() const
{
  return m_UsrArea;
}


// =====================================================================
// =====================================================================



UnitID GroundwaterUnit::getGUExchangeID() const
{
  return m_GUExchangeID;
}


// =====================================================================
// =====================================================================


double GroundwaterUnit::getSubstratumLevel() const
{
  return m_SubstratumLevel;
}


// =====================================================================
// =====================================================================


void GroundwaterUnit::setGUExchange(GroundwaterUnit *GU)
{
  mp_GUExchange = GU;
}


// =====================================================================
// =====================================================================



void GroundwaterUnit::setSubstratumLevel(double Level)
{
  m_SubstratumLevel = Level;
}


} } // namespace openfluid::core

