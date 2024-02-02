/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/

/**
  @file VarHelpers.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_TOOLS_VARHELPERS_HPP__
#define __OPENFLUID_TOOLS_VARHELPERS_HPP__


#include <algorithm>
#include <map>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace tools {


struct OPENFLUID_API UnitVarPair
{
  // simple structure to describe a Variable associated with a Unit class
    openfluid::core::UnitsClass_t UnitsClass;

    openfluid::core::VariableName_t VariableName;

    UnitVarPair(): UnitsClass(""), VariableName("")
    {
    }

    UnitVarPair(openfluid::core::UnitsClass_t Classes, openfluid::core::VariableName_t Vars) : 
        UnitsClass(Classes), VariableName(Vars)
    {
    }

    UnitVarPair(std::initializer_list<std::string> Data)
    {
      std::vector<std::string> TmpV(Data);
      if (TmpV.size() == 2)
      {
        UnitsClass = TmpV[0];
        VariableName = TmpV[1];
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Bad unit class/variable pair format");
      }
    }

    bool operator==(const UnitVarPair& P)
    {
      return (P.UnitsClass == UnitsClass && P.VariableName == VariableName);
    }
};


// =====================================================================
// =====================================================================


struct OPENFLUID_API ClassIDVar : UnitVarPair
{

    std::string UnitsIDsStr;
    
    ClassIDVar(): UnitVarPair(), UnitsIDsStr("")
    {
    }
    
    ClassIDVar(openfluid::core::UnitsClass_t Classes, std::string IDs, openfluid::core::VariableName_t Vars) : 
        UnitVarPair(Classes, Vars), UnitsIDsStr(IDs)
    {
    }

    ClassIDVar(std::initializer_list<std::string> Data)
    {
      std::vector<std::string> TmpV(Data);
      if (TmpV.size() == 2)
      {
        UnitsClass = TmpV[0];
        UnitsIDsStr = "*";
        VariableName = TmpV[1];
      }
      else if (Data.size() == 3)
      {
        UnitsClass = TmpV[0];
        UnitsIDsStr = TmpV[1]; 
        VariableName = TmpV[2];
      }
      else
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                      "Bad unit class/variable pair format");
      }
    }
    
    std::string getClassIDVarString() const;

    bool operator==(const ClassIDVar& P)
    {
      return (UnitVarPair::operator==(P) && UnitsIDsStr==P.UnitsIDsStr);
    }
};


// =====================================================================
// =====================================================================


struct OPENFLUID_API ClassIDVarPrecision : ClassIDVar
{
    
    bool HasPrecision = false;
    unsigned int Precision;
    
    std::string FloatFormat;
    
    ClassIDVarPrecision() : ClassIDVar()
    {
    }
    
    ClassIDVarPrecision(openfluid::core::UnitsClass_t Classes, std::string IDs, openfluid::core::VariableName_t Vars, 
              unsigned int P, std::string FFormat) : 
        ClassIDVar(Classes, IDs, Vars), Precision(P), FloatFormat(FFormat)
    {
    }
    
    std::string getClassIDVarString(bool WithPrecision) const;
};


typedef std::vector<UnitVarPair> UnitVarPairs_t;
typedef std::vector<ClassIDVar> UnitVarTriplets_t;

UnitVarTriplets_t OPENFLUID_API deserializeVarTriplets(const std::string& Selection);

UnitVarPairs_t OPENFLUID_API deduceVarPairs(const UnitVarTriplets_t& Triplets);

std::string OPENFLUID_API serializeVarTriplets(const UnitVarTriplets_t& Triplets);

} }


#endif /* __OPENFLUID_TOOLS_VARHELPERS_HPP__ */
