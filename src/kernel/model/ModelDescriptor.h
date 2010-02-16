/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file ModelDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __MODELDESCRIPTOR_H___
#define __MODELDESCRIPTOR_H___

#include <list>
#include "ModelItemDescriptor.h"

class ModelDescriptor
{
  public:
    typedef std::list<ModelItemDescriptor*> ModelDescription_t;

  private:
    ModelDescription_t m_ModelDescription;

  public:
    ModelDescriptor();

    ModelDescription_t& getItems() { return (m_ModelDescription); };

    const ModelDescription_t& getItems() const { return (m_ModelDescription); };

    void appendItem(ModelItemDescriptor *Item);

};



#endif /* __MODELDESCRIPTOR_H___ */
