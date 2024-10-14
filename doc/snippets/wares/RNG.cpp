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
  @file RNG.cpp

  @author dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <openfluid/tools/RandomNumberGenerator.hpp>


void snippet_RandomNumberGenerator()
{
//! [rng_decl]
openfluid::tools::RandomNumberGenerator Rng;
//! [rng_decl]


//! [rng_initialize_seed]
Rng.init(50);
//! [rng_initialize_seed]


//! [rng_initialize_random_seed]
Rng.init(-10);
//! [rng_initialize_random_seed]


//! [rng_log]
// Mean of 2
// Standard deviation of 10
int randomizedIntValue = Rng.rnorm(2, 10);
//! [rng_log]


//! [rng_bernoulli]
// Probability of success of 0.7
bool randomizedBoolValue = Rng.bernoulli(0.7);
//! [rng_bernoulli]
}
