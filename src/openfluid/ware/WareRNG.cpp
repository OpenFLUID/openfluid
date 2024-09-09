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
  @file WareRNG.cpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
 */


#include <openfluid/ware/WareRNG.hpp>
#include <openfluid/tools/StringHelpers.hpp>
#include <openfluid/ware/SimulationDrivenWare.hpp>


namespace openfluid { namespace ware {


WareRNG::WareRNG(SimulationDrivenWare* Ware) : openfluid::tools::RandomNumberGenerator(), m_Ware(Ware)
{
}


// =====================================================================
// =====================================================================


void WareRNG::init(const WareParams_t& Params)
{
  m_DisplaySeed = true;

  std::string SeedStr;
  int Seed;

  WareParams_t::const_iterator It = Params.find("seed");

  if (It != Params.end())
  {
    SeedStr = It->second;
    if(!openfluid::tools::toNumeric(SeedStr, Seed))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"wrong value for seed");
    }

    openfluid::tools::RandomNumberGenerator::init(Seed);
  }
  else 
  {
    openfluid::tools::RandomNumberGenerator::init();
  }

  displaySeedInfo();
}


// =====================================================================
// =====================================================================


void WareRNG::displaySeedInfo()
{
  if(!m_DisplaySeed)
  {
    return;
  }

  // Detection of non-deterministic weird ware state here
  // cf https://github.com/OpenFLUID/openfluid/issues/1232
  m_Ware->appendToLog(openfluid::tools::FileLogger::LogType::INFO_MSG, m_DisplayNameInfo +
                                                                       std::to_string(m_SelectedSeed));
  std::cout << "\n[Info] " << m_DisplayNameInfo << std::to_string(m_SelectedSeed) << std::endl;
  m_DisplaySeed = false;
}


} } // namespaces
