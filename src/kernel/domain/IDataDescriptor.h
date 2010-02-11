/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file IDataDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __IDATADESCRIPTOR_H___
#define __IDATADESCRIPTOR_H___

#include "openfluid-core.h"

class InputDataDescriptor
{
  private:

    openfluid::core::UnitClass_t m_UnitsClass;

    std::vector<std::string> m_ColumnsOrder;

    std::string m_DataBlob;


  public:

    InputDataDescriptor();

    ~InputDataDescriptor();

    openfluid::core::UnitClass_t& getUnitsClass() { return m_UnitsClass; };

    std::vector<std::string>& getColumnsOrder() { return m_ColumnsOrder; };

    std::string& getDataBlob() { return m_DataBlob; };

};


#endif /* __IDATADESCRIPTOR_H___ */
