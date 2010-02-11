/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file EventDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __EVENTDESCRIPTOR_H___
#define __EVENTDESCRIPTOR_H___

#include "openfluid-core.h"

class EventDescriptor
{
  private:
    openfluid::core::UnitClass_t m_UnitClass;
    openfluid::core::UnitID_t m_UnitID;
    openfluid::core::Event m_Event;
  public:

    EventDescriptor();

    ~EventDescriptor();

    openfluid::core::UnitID_t& getUnitID() { return m_UnitID; };

    openfluid::core::UnitClass_t& getUnitClass() { return m_UnitClass; };

    openfluid::core::Event& getEvent() { return m_Event; };

};

#endif /* __EVENTDESCRIPTOR_H___ */
