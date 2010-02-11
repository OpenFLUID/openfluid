/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file DomainDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __DOMAINDESCRIPTOR_H___
#define __DOMAINDESCRIPTOR_H___

#include "UnitDescriptor.h"
#include "IDataDescriptor.h"
#include "EventDescriptor.h"

class DomainDescriptor
{
  private:

    std::list<UnitDescriptor> m_DefDescriptor;

    std::list<InputDataDescriptor> m_IDataDescriptor;

    std::list<EventDescriptor> m_EventsDescriptor;


  public:

    DomainDescriptor();

    ~DomainDescriptor();

    std::list<UnitDescriptor>& getUnits() { return m_DefDescriptor; };

    std::list<InputDataDescriptor>& getInputData() { return m_IDataDescriptor; };

    std::list<EventDescriptor>& getEvents() { return m_EventsDescriptor; };


};


#endif /* __DOMAINDESCRIPTOR_H___ */
