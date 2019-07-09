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
  @file DataHelpers.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <boost/algorithm/string.hpp>

#include <openfluid/tools/DataHelpers.hpp>


namespace openfluid { namespace tools {


void tokenizeString(const std::string& StrToTokenize,
                    std::vector<std::string>& Tokens,
                    const std::string& Delimiters = " ")
{
  Tokens.clear();

  std::string::size_type LastPos = StrToTokenize.find_first_not_of(Delimiters, 0);
  std::string::size_type Pos = StrToTokenize.find_first_of(Delimiters, LastPos);

  while (std::string::npos != Pos || std::string::npos != LastPos)
  {
    // Found a token, add it to the vector.
    Tokens.push_back(StrToTokenize.substr(LastPos, Pos - LastPos));
    // Skip delimiters.  Note the "not_of"
    LastPos = StrToTokenize.find_first_not_of(Delimiters, Pos);
    // Find next "non-delimiter"
    Pos = StrToTokenize.find_first_of(Delimiters, LastPos);
  }
}


// =====================================================================
// =====================================================================


std::vector<std::string> splitString(const std::string& StrToSplit,
                                     const std::string& Separators,
                                     bool ReturnsEmpty)
{
  std::vector<std::string> SplitParts;

  boost::algorithm::token_compress_mode_type TokCompress = boost::token_compress_on;
  if (ReturnsEmpty)
  {
    TokCompress = boost::token_compress_off;
  }

  boost::split(SplitParts, StrToSplit, boost::is_any_of(Separators),TokCompress);

  return SplitParts;
}


// =====================================================================
// =====================================================================


std::vector<std::string> OPENFLUID_API splitString(const std::string& StrToSplit,
                                                   char Separator,
                                                   bool ReturnsEmpty)
{
  std::vector<std::string> SplitParts;

  boost::algorithm::token_compress_mode_type TokCompress = boost::token_compress_on;
  if (ReturnsEmpty)
  {
    TokCompress = boost::token_compress_off;
  }
  
  boost::split(SplitParts, StrToSplit, boost::is_any_of(std::string(1,Separator)),TokCompress);

  return SplitParts;
}


} } // namespaces
