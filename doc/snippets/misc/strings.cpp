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
  @file strings.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


// OpenFLUID:stylecheck:!incs


#include <openfluid/tools/StringHelpers.hpp>

void snippet_Strings()
{
{
//! [str_lower]
std::string Str = openfluid::tools::toLowerCase("UpPeRaNdLoWeR");
// Str = "upperandlower"
//! [str_lower]
}

{
//! [str_upper]
std::string Str = openfluid::tools::toUpperCase("UpPeRaNdLoWeR");
// Str = "UPPERANDLOWER"
//! [str_upper]
}

{
//! [str_triml]
std::string Str = openfluid::tools::trimLeft(" The string to trim    \n  ");
// Str = "The string to trim    \n  "
//! [str_triml]
}

{
//! [str_trimr]
std::string Str = openfluid::tools::trimRight(" The string to trim    \n  ");
// Str = " The string to trim"
//! [str_trimr]
}

{
//! [str_trim]
std::string Str = openfluid::tools::trim(" The string to trim    \n  ");
// Str = "The string to trim"
//! [str_trim]
}

{
//! [str_numbool]
double Val;
if (openfluid::tools::toNumeric("42.01",Val))
{
  // conversion succeeded
}
else
{
  // conversion failed
}
//! [str_numbool]
}

{
//! [str_num]
double Val = openfluid::tools::toNumeric<double>("42.01");
// Val = 42.01
//! [str_num]
}

{
//! [str_starts]
openfluid::tools::startsWith("Some words in a string","Some");  // true
//! [str_starts]
}

{
//! [str_ends]
openfluid::tools::endsWith("Some words in a string","Some");  // false
//! [str_ends]
}

{
//! [str_contains]
openfluid::tools::contains("Some words in a string","Some");  // true
//! [str_contains]
}

{
//! [str_repl]
std::string Str = openfluid::tools::replace("Oranges are orange","orange","blue");
// Str = "Oranges are blue"
//! [str_repl]
}

{
//! [str_format]
const double Pi = 3.1415926;
std::string TwoPiStr = openfluid::tools::format("%dxPi equals %.5f",2,2*Pi);

const std::string Name = "Marty";
const unsigned int Year = 1955;
std::string BTTFStr = openfluid::tools::format("%s went back to %d!",Name.c_str(),Year);
// BTTFStr = "Marty went back to 1955!"
//! [str_format]
}


{
//! [str_split]
std::vector<std::string> VectStr = openfluid::tools::split("Hello World!"," ");
// VectStr = ["Hello","World!"]
//! [str_split]
}

{
//! [str_join]
std::string Str = openfluid::tools::join({"Apples","Oranges","Strawberries"},", ");
// Str = "Apples, Oranges, Strawberries"
//! [str_join]
}

{
//! [vectstr_opstr]
std::vector<std::string> VectStr;
std::string Word = "strings";

VectStr << "Add" << Word << "to";
// VectStr = ["Add","strings","to"]
VectStr << "vectors";
// VectStr = ["Add","strings","to","vectors"]
//! [vectstr_opstr]
}

{
//! [vectstr_opvect]
std::vector<std::string> VectStr = {"My","favourite","color"};
std::vector<std::string> OtherVectStr = {"is","green"};

VectStr << OtherVectStr;
// VectStr = ["My","favourite","color","is","green"]
//! [vectstr_opvect]
}

}

