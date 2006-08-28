/**
  \file ReachSegment.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "ReachSegment.h"

// =====================================================================
// =====================================================================



namespace mhydasdk { namespace core {


ReachSegment::ReachSegment()
     : HydroObject()
{
  m_UpstreamNode = -1;
  m_DownstreamNode = -1;
  m_LowReachID = -1;
  m_UsrSlope = 0;
  m_UsrLength = 0;
  m_UsrWidth = 0;
  m_UsrHeight = 0;
  m_GUExchangeID = -1;
  mp_GUExchange = NULL;
  mp_LowReach = NULL;
}


// =====================================================================
// =====================================================================


ReachSegment::ReachSegment(int ID, int ProcessOrder,
                     nodeid_t UpstreamNode, nodeid_t DownstreamNode, hoid_t LowReachID,
                     double UsrSlope, double UsrLength, double UsrWidth, double UsrHeight,
                     hoid_t GUExchangeID)
         : HydroObject(ID, ProcessOrder)
{
  m_UpstreamNode = UpstreamNode;
  m_DownstreamNode = DownstreamNode;
  m_LowReachID = LowReachID;
  m_UsrSlope = UsrSlope;
  m_UsrLength = UsrLength;
  m_UsrWidth = UsrWidth;
  m_UsrHeight = UsrHeight;
	m_GUExchangeID = GUExchangeID;

  mp_GUExchange = NULL;
  mp_LowReach = NULL;

}

// =====================================================================
// =====================================================================


ReachSegment::~ReachSegment()
{

}

// =====================================================================
// =====================================================================


nodeid_t ReachSegment::getUpstreamNode() const
{
  return m_UpstreamNode;
}

// =====================================================================
// =====================================================================

nodeid_t ReachSegment::getDownstreamNode() const
{
  return m_DownstreamNode;
}

// =====================================================================
// =====================================================================

hoid_t ReachSegment::getLowReachID() const
{
  return m_LowReachID;
}


// =====================================================================
// =====================================================================


double ReachSegment::getUsrSlope() const
{
  return m_UsrSlope;
}


// =====================================================================
// =====================================================================


double ReachSegment::getUsrLength() const
{
  return m_UsrLength;
}


// =====================================================================
// =====================================================================


double ReachSegment::getUsrWidth() const
{
  return m_UsrWidth;
}


// =====================================================================
// =====================================================================


double ReachSegment::getUsrHeight() const
{
  return m_UsrHeight;
}


// =====================================================================
// =====================================================================


hoid_t ReachSegment::getGUExchangeID() const
{
  return m_GUExchangeID;
}



// =====================================================================
// =====================================================================


GroundwaterUnit *ReachSegment::getGUExchange() const
{
  return mp_GUExchange;
}


// =====================================================================
// =====================================================================


ReachSegment *ReachSegment::getLowReach() const
{
  return mp_LowReach;

}


// =====================================================================
// =====================================================================


void ReachSegment::setUpstreamNode(nodeid_t Node)
{
  m_UpstreamNode = Node;
}


// =====================================================================
// =====================================================================


void ReachSegment::setDownstreamNode(nodeid_t Node)
{
  m_DownstreamNode = Node;
}


// =====================================================================
// =====================================================================


void ReachSegment::setLowReachID(hoid_t ID)
{
  m_LowReachID = ID;
}


// =====================================================================
// =====================================================================


void ReachSegment::setUsrSlope(double Slope)
{
  m_UsrSlope = Slope;
}


// =====================================================================
// =====================================================================


void ReachSegment::setUsrLength(double Length)
{
  m_UsrLength = Length;
}


// =====================================================================
// =====================================================================


void ReachSegment::setUsrWidth(double Width)
{
  m_UsrWidth = Width;
}


// =====================================================================
// =====================================================================


void ReachSegment::setUsrHeight(double Height)
{
  m_UsrHeight = Height;
}


// =====================================================================
// =====================================================================


void ReachSegment::setGUExchangeID(hoid_t ID)
{
  m_GUExchangeID = ID;
}


// =====================================================================
// =====================================================================


void ReachSegment::setGUExchange(GroundwaterUnit *GU)
{
  mp_GUExchange = GU;
}


// =====================================================================
// =====================================================================


void ReachSegment::setLowReach(ReachSegment *RS)
{
  mp_LowReach = RS;

}




} } // namespace mhydasdk::core




