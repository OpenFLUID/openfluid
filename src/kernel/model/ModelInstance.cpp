/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelInstance.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "ModelInstance.h"

// =====================================================================
// =====================================================================


ModelInstance::ModelInstance()
{

}


// =====================================================================
// =====================================================================


ModelInstance::~ModelInstance()
{

}


// =====================================================================
// =====================================================================


void ModelInstance::appendItem(ModelItemInstance* ItemInstance)
{
  m_ModelItems.push_back(ItemInstance);
}


// =====================================================================
// =====================================================================


void ModelInstance::clear()
{
  m_ModelItems.clear();
}



// =====================================================================
// =====================================================================


void ModelInstance::deleteItemsAndClear()
{

  std::list<ModelItemInstance*>::iterator it;

  for (it=m_ModelItems.begin();it!=m_ModelItems.end();++it)
  {
    if ((*it)->Function != NULL) delete (*it)->Function;
    if ((*it)->Signature != NULL) delete (*it)->Signature;
  }

  clear();
}



