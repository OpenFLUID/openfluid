/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file RunDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "RunDescriptor.h"

// =====================================================================
// =====================================================================


RunDescriptor::RunDescriptor()
{
  m_DeltaT = -1;
  m_BeginDate = openfluid::core::DateTime();
  m_EndDate = openfluid::core::DateTime();

  m_SimID = "";
  m_ProgOutKeep = 0;
  m_ProgOutPacket = 0;
}


// =====================================================================
// =====================================================================


RunDescriptor::RunDescriptor(int DeltaT,
                             openfluid::core::DateTime BeginDate,
                             openfluid::core::DateTime EndDate)
{
  RunDescriptor();

  m_DeltaT = DeltaT;
  m_BeginDate = BeginDate;
  m_EndDate = EndDate;

}


// =====================================================================
// =====================================================================

RunDescriptor::~RunDescriptor()
{

}


// =====================================================================
// =====================================================================


void RunDescriptor::setProgressiveOutput(const unsigned int Packet, const unsigned int Keep)
{
  m_ProgOutPacket = Packet;
  m_ProgOutKeep = Keep;
}


// =====================================================================
// =====================================================================


bool RunDescriptor::isProgressiveOutput() const
{
  return (m_ProgOutKeep > 0 && m_ProgOutPacket > 0);
}

