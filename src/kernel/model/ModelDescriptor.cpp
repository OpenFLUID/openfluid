/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelDescriptor.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ModelDescriptor.h"



// =====================================================================
// =====================================================================


ModelDescriptor::ModelDescriptor()
{

}


// =====================================================================
// =====================================================================


ModelDescriptor::ModelDescription_t& ModelDescriptor::getItems()
{
  return (m_ModelDescription);
}


// =====================================================================
// =====================================================================


void ModelDescriptor::appendItem(ModelItemDescriptor *Item)
{
  m_ModelDescription.push_back(Item);
}

