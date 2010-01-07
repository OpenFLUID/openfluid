/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputFilesDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "OutputFilesDescriptor.h"

// =====================================================================
// =====================================================================


OutputFilesDescriptor::OutputFilesDescriptor()
{
  m_Sets.clear();
  m_ColSeparator = "\t";
  m_DateFormat = "%Y%m%dT%H%M%S";
  m_CommentChar = "%";
}


// =====================================================================
// =====================================================================


OutputFilesDescriptor::~OutputFilesDescriptor()
{

}


// =====================================================================
// =====================================================================

