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


#include <map>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>


namespace openfluid { namespace tools {


class OPENFLUID_API ClassIDVar
{
  public:
    
    openfluid::core::UnitsClass_t UnitsClassesStr;

    std::string UnitsIDsStr;

    openfluid::core::VariableName_t VariablesStr;
    
    ClassIDVar(): UnitsClassesStr(""), UnitsIDsStr(""), VariablesStr("")
    {
    }
    
    ClassIDVar(openfluid::core::UnitsClass_t Classes, std::string IDs, openfluid::core::VariableName_t Vars) : 
        UnitsClassesStr(Classes), UnitsIDsStr(IDs), VariablesStr(Vars)
    {
    }
    
    std::string getClassIDVarString();
};


// =====================================================================
// =====================================================================


class OPENFLUID_API ClassIDVarPrecision : public ClassIDVar
{

  public:
    
    bool HasPrecision;
    unsigned int Precision;
    
    std::string FloatFormat;
    
    ClassIDVarPrecision() : ClassIDVar()
    {
    }
    
    ClassIDVarPrecision(openfluid::core::UnitsClass_t Classes, std::string IDs, openfluid::core::VariableName_t Vars, 
              unsigned int Precision, std::string FloatFormat) : 
        ClassIDVar(Classes, IDs, Vars), Precision(Precision), FloatFormat(FloatFormat)
    {
    }
    
    std::string getClassIDVarString(bool WithPrecision);
};


std::vector<std::pair<std::string, std::string>> OPENFLUID_API parseVars(const std::string& Selection);


} }


#endif /* __OPENFLUID_TOOLS_VARHELPERS_HPP__ */
