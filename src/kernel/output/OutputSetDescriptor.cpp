/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputSetDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "OutputSetDescriptor.h"
#include "openfluid-tools.h"

// =====================================================================
// =====================================================================


OutputSetDescriptor::OutputSetDescriptor()
{
  m_Name = "";
  m_UnitsClass = "";
  m_AllUnits = false;
  m_UnitsIDs.clear();
  m_UnitsPtr.clear();
  m_AllScalars = false;
  m_ScalarVariables.clear();
  m_AllVectors = false;
  m_VectorVariables.clear();
}


// =====================================================================
// =====================================================================


OutputSetDescriptor::~OutputSetDescriptor()
{

}


// =====================================================================
// =====================================================================


void OutputSetDescriptor::fillUnitsPtr()
{
  openfluid::core::Unit* UPtr;

  m_UnitsPtr.resize(m_UnitsIDs.size(), NULL);

  for (unsigned int i = 0; i < m_UnitsIDs.size() ; i++)
  {
    UPtr = openfluid::core::CoreRepository::getInstance()->getUnit(m_UnitsClass,m_UnitsIDs[i]);

    if (UPtr == NULL)
    {
      std::string UnitIDStr;
      openfluid::tools::ConvertValue(m_UnitsIDs[i],&UnitIDStr);
      throw openfluid::base::OFException("kernel","OutputSetDescriptor::fillUnitsPtr()","unit #" + UnitIDStr + " not found in class " + m_UnitsClass);
    }
    else
      m_UnitsPtr[i] = UPtr;

  }

}
