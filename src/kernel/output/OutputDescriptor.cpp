/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "OutputDescriptor.h"

// =====================================================================
// =====================================================================


OutputDescriptor::OutputDescriptor()
{
  m_FileSets.clear();
}


// =====================================================================
// =====================================================================


OutputDescriptor::~OutputDescriptor()
{

}


// =====================================================================
// =====================================================================


void OutputDescriptor::fillUnitsPtr()
{
  for (unsigned int i = 0; i < m_FileSets.size() ; i++) m_FileSets[i].fillUnitsPtr();
}

// =====================================================================
// =====================================================================

