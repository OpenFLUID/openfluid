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
  @file CompletionProvider.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/waresdev/CompletionProvider.hpp>


namespace openfluid { namespace waresdev {


OPENFLUID_SINGLETON_INITIALIZATION(CompletionProvider)


CompletionProvider::CompletionProvider()
{


  #include "CompletionProviderAPIData.cpp"


  initializeCppRules();


}


// =====================================================================
// =====================================================================


void CompletionProvider::addRule(const CompletionProvider::Rule& R)
{
  if (R.DevContext.testFlag(ANYWARE) || R.DevContext.testFlag(SIMULATOR))
  {
    m_Rules[R.ProgLangCode][openfluid::ware::WareType::SIMULATOR].append(R);
  }
  
  if (R.DevContext.testFlag(ANYWARE) || R.DevContext.testFlag(OBSERVER))
  {
    m_Rules[R.ProgLangCode][openfluid::ware::WareType::OBSERVER].append(R);
  }
  
  if (R.DevContext.testFlag(ANYWARE) || R.DevContext.testFlag(BUILDEREXT))
  {
    m_Rules[R.ProgLangCode][openfluid::ware::WareType::BUILDEREXT].append(R);
  }
}


// =====================================================================
// =====================================================================


const CompletionProvider::Rules& CompletionProvider::getRules(QString LangCode, openfluid::ware::WareType WType)
{

  if (m_Rules.contains(LangCode))
  {
    if (m_Rules[LangCode].contains(WType))
    {
      return m_Rules[LangCode][WType];
    }
  }

  return m_NoRule;
}


// =====================================================================
// =====================================================================


void CompletionProvider::initializeCppRules()
{

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Boolean"),
            "bool %%SEL_START%%Val%%SEL_END%%")
         );
  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Boolean"),
            "bool %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Integer (signed)"),
            "int %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Integer (unsigned)"),
            "unsigned int %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Long integer (signed)"),
            "long int %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Long integer (unsigned)"),
            "unsigned long int %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Single precision float (signed)"),
            "float %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Single precision float (unsigned)"),
            "unsigned float %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Double precision float (signed)"),
            "double %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Simple")
            },
            CompletionProvider::tr("Double precision float (unsigned)"),
            "unsigned double %%SEL_START%%Val%%SEL_END%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Containers")
            },
            CompletionProvider::tr("Vector"),
            "std::vector<%%SEL_START%%Type%%SEL_END%%> Vect")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Containers")
            },
            CompletionProvider::tr("List"),
            "std::list<%%SEL_START%%Type%%SEL_END%%> List")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Containers")
            },
            CompletionProvider::tr("Set"),
            "std::set<%%SEL_START%%Type%%SEL_END%%> Set")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Types"),
              CompletionProvider::tr("Containers")
            },
            CompletionProvider::tr("Map"),
            "std::map<%%SEL_START%%KeyType%%SEL_END%%,ValType> Map")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Streams")
            },
            CompletionProvider::tr("Output to console"),
            "std::cout << %%CURSOR%%")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Math")
            },
            CompletionProvider::tr("Include <cmath> header"),
            "#include <cmath>")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Math")
            },
            CompletionProvider::tr("Absolute value"),
            "std::abs(%%SEL_START%%Val%%SEL_END%%)")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Math")
            },
            CompletionProvider::tr("Round value downward"),
            "std::floor(%%SEL_START%%Val%%SEL_END%%)")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Math")
            },
            CompletionProvider::tr("Round value upward"),
            "std::ceil(%%SEL_START%%Val%%SEL_END%%)")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Math")
            },
            CompletionProvider::tr("Square root of value "),
            "std::sqrt(%%SEL_START%%Val%%SEL_END%%)")
          );

  addRule(Rule(
            "cpp",Origin::CPP,ANYWARE,
            {
              CompletionProvider::tr("Math")
            },
            CompletionProvider::tr("Power of value "),
            "std::pow(%%SEL_START%%Val%%SEL_END%%,exponent)")
          );
}


} } // namespaces
