/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file UnitDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __UNITDESCRIPTOR_H___
#define __UNITDESCRIPTOR_H___

#include "openfluid-core.h"

class UnitDescriptor
{
  private:

    openfluid::core::UnitID_t m_UnitID;
    openfluid::core::UnitClass_t m_UnitClass;
    openfluid::core::PcsOrd_t m_PcsOrder;
    std::list<openfluid::core::UnitClassID_t> m_UnitsTos;
    std::list<openfluid::core::UnitClassID_t> m_UnitsParents;

  public:

   UnitDescriptor();

    ~UnitDescriptor();


    openfluid::core::UnitID_t& getUnitID() { return m_UnitID; };

    openfluid::core::UnitClass_t& getUnitClass() { return m_UnitClass; };

    openfluid::core::PcsOrd_t& getProcessOrder() { return m_PcsOrder; };

    std::list<openfluid::core::UnitClassID_t>& getUnitsTos() { return m_UnitsTos; };

    std::list<openfluid::core::UnitClassID_t>& getUnitsParents() { return m_UnitsParents; };

};


#endif /* __UNITDESCRIPTOR_H___ */
