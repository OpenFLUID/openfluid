/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file OutputSetDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTSETDESCRIPTOR_H___
#define __OUTPUTSETDESCRIPTOR_H___

#include <vector>
#include <string>
#include "openfluid-core.h"

class OutputSetDescriptor
{
  private:

    std::string m_Name;
    std::string m_UnitsClass;
    bool m_AllUnits;
    std::vector<openfluid::core::UnitID_t> m_UnitsIDs;
    bool m_AllScalars;
    std::vector<std::string> m_ScalarVariables;
    bool m_AllVectors;
    std::vector<std::string> m_VectorVariables;

  public:

    OutputSetDescriptor();

    ~OutputSetDescriptor();

    void setName(const std::string Name) { m_Name = Name; };

    std::string getName() const { return m_Name; };

    void setUnitsClass(const std::string UnitsClass)  { m_UnitsClass = UnitsClass; };

    std::string getUnitsClass() const { return m_UnitsClass; };

    void setAllUnits(bool AllUnits) { m_AllUnits = AllUnits; };

    bool isAllUnits() const {return m_AllUnits; };

    std::vector<openfluid::core::UnitID_t>& getUnitsIDs() { return m_UnitsIDs; };

    void setAllScalars(bool AllScalars) { m_AllScalars = AllScalars; };

    bool isAllScalars() const {return m_AllScalars; };

    std::vector<std::string>& getScalars() { return m_ScalarVariables; };

    void setAllVectors(bool AllVectors) { m_AllVectors = AllVectors; };

    bool isAllVectors() const {return m_AllVectors; };

    std::vector<std::string>& getVectors() { return m_VectorVariables; };

};


#endif /* __OUTPUTSETDESCRIPTOR_H___ */
