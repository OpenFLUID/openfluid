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
  @file timer.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#include <string>

#include <openfluid/tools/Timer.hpp>


void snippets_usage()
{
//! [usage]
openfluid::tools::Timer MyTimer;

MyTimer.start();
// here is the code we want to measure the duration of execution
MyTimer.stop();

auto Duration = MyTimer.elapsed();  // e.g. 13536
std::string DurationStr = MyTimer.elapsedAsPrettyString();  // e.g. 13.536s
//! [usage]
}


void snippets_getDurationAsPrettyString()
{
//! [durationstr]
std::string DurationStr;

DurationStr = openfluid::tools::Timer::getDurationAsPrettyString(123456789); // "1d 10h 17m 36.789s"
DurationStr = openfluid::tools::Timer::getDurationAsPrettyString(12345678);  // "3h 25m 45.678s"
DurationStr = openfluid::tools::Timer::getDurationAsPrettyString(123456);    // "2m 3.456s"
DurationStr = openfluid::tools::Timer::getDurationAsPrettyString(1234);      // "1.234s"
DurationStr = openfluid::tools::Timer::getDurationAsPrettyString(12);        // "0.012s"
//! [durationstr]
}
