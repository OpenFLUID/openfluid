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
  @file datetime.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <iostream>

#include <openfluid/core/DateTime.hpp>


void snippet_DateTime()
{
//! [dt_decl]
openfluid::core::DateTime aDate(2009,01,25,12,05,00);
//! [dt_decl]


//! [dt_add]
// adding 45 seconds
aDate.addSeconds(45);

// adding 5 minutes
aDate.addSeconds(openfluid::core::DateTime::Minutes(5));

// adding an hour
aDate.addSeconds(openfluid::core::DateTime::Hour());

// adding 60 days
aDate.addSeconds(openfluid::core::DateTime::Days(60));
//! [dt_add]


//! [dt_sub]
// subtracting 10 seconds
aDate.subtractSeconds(10);

// subtracting 30 days
aDate.subtractSeconds(openfluid::core::DateTime::Days(30));
//! [dt_sub]


//! [dt_diff]
openfluid::core::DateTime FirstDate(2009,01,25,12,05,00);
openfluid::core::DateTime SecondDate(2009,02,28,00,00,00);

openfluid::core::RawTime_t Diff;

Diff = SecondDate.diffInSeconds(FirstDate);

if (Diff > openfluid::core::DateTime::Days(60))
{
  std::cout << "The difference between the two dates is greater than 60 days" << std::endl;
}
else
{
  std::cout << "The difference between the two dates is lesser than 60 days" << std::endl;
}
//! [dt_diff]

}
