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
  mp_SimVectorVars = new SimulatedVectorVarsMap();  
  mp_Properties = new PropertiesMap();
  mp_IniConditions = new PropertiesMap();

}


// =====================================================================
// =====================================================================


HydroObject::HydroObject(HOID ID, int ProcessOrder)
{

  mp_SimVars = new SimulatedVarsMap();
  mp_SimVectorVars = new SimulatedVectorVarsMap();  
  
  mp_Properties = new PropertiesMap();
  mp_IniConditions = new PropertiesMap();

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

