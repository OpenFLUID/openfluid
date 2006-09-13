/**
  \file HydroObject.cpp
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#include "HydroObject.h"


namespace mhydasdk { namespace core {



// =====================================================================
// =====================================================================


HydroObject::HydroObject()
{
  m_ID = -1;
  m_ProcessOrder = 0;

  m_SimVars.clear();

}


// =====================================================================
// =====================================================================


HydroObject::HydroObject(hoid_t ID, int ProcessOrder)
{
  m_ID = ID;
  m_ProcessOrder = ProcessOrder;
}


// =====================================================================
// =====================================================================


HydroObject::~HydroObject()
{

}



// =====================================================================
// =====================================================================


int HydroObject::getID() const
{
  return m_ID;
};


// =====================================================================
// =====================================================================



int HydroObject::getProcessOrder() const
{
  return m_ProcessOrder;
}


// =====================================================================
// =====================================================================


void HydroObject::setProcessOrder(const int ProcessOrder)
{
  m_ProcessOrder = ProcessOrder;
}



} } // namespace mhydasdk::core

