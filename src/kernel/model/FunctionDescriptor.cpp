/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file FunctionDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "FunctionDescriptor.h"

// =====================================================================
// =====================================================================


FunctionDescriptor::FunctionDescriptor(std::string FileID) : ModelItemDescriptor()
{
  m_ModelItemType = PluggedFunction;
  m_FileID = FileID;
}


// =====================================================================
// =====================================================================


std::string FunctionDescriptor::getFileID() const
{
  return m_FileID;
}
