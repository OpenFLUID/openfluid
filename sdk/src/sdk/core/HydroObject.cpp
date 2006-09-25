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

  mp_SimVars = new SimulatedVarsMap();
  mp_Properties = new ParamsMap();
  mp_IniConditions = new ParamsMap();

}


// =====================================================================
// =====================================================================


HydroObject::HydroObject(hoid_t ID, int ProcessOrder)
{

  mp_SimVars = new SimulatedVarsMap();
  mp_Properties = new ParamsMap();
  mp_IniConditions = new ParamsMap();

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

