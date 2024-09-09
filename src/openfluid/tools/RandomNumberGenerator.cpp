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
  @file RandomNumberGenerator.cpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
 */


#include <openfluid/tools/RandomNumberGenerator.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace tools {


RandomNumberGenerator::RandomNumberGenerator() : m_DisplaySeed(true), m_DisplayNameInfo("Seed for random: ")
{
  init();
}


// =====================================================================
// =====================================================================


void RandomNumberGenerator::init(long int CustomSeed)
{
  if (CustomSeed >= 0)
  {
    m_SelectedSeed = CustomSeed;
    m_Generator.seed(m_SelectedSeed);
  }
  else
  {
    std::random_device rd;
    m_SelectedSeed = rd();
    m_Generator.seed(m_SelectedSeed);
  }
}


// =====================================================================
// =====================================================================


uint64_t RandomNumberGenerator::getSelectedSeed() const
{
  return m_SelectedSeed;
}


// =====================================================================
// =====================================================================


std::mt19937_64& RandomNumberGenerator::getGenerator()
{
  displaySeedInfo();
  return m_Generator;
}


// =====================================================================
// =====================================================================


void RandomNumberGenerator::displaySeedInfo()
{
  if(!m_DisplaySeed)
  {
    return;
  }
  std::cout << "[Info] " << m_DisplayNameInfo << m_SelectedSeed << std::endl;
  m_DisplaySeed = false;
}


} } // namespaces
