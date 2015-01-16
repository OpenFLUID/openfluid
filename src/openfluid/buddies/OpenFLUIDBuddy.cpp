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
  @file
  @brief Implements ...

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/buddies/OpenFLUIDBuddy.hpp>
#include <openfluid/tools/DataHelpers.hpp>
#include <vector>



namespace openfluid { namespace buddies {


OpenFLUIDBuddy::OpenFLUIDBuddy(openfluid::buddies::BuddiesListener* Listener)
{
  mp_Listener = Listener;

  if (mp_Listener == NULL) mp_Listener = new openfluid::buddies::BuddiesListener();
}


// =====================================================================
// =====================================================================


OpenFLUIDBuddy::~OpenFLUIDBuddy()
{

}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::setOptionIfNotSet(std::string OptionName, std::string OptionValue)
{
  if (m_Options.find(OptionName) == m_Options.end())
  {
    m_Options[OptionName] = OptionValue;
  }
}

// =====================================================================
// =====================================================================


std::string OpenFLUIDBuddy::getYesNoFromOneZero(std::string VStr)
{
  if (VStr == "1") return "yes";
  else return "no";
}


// =====================================================================
// =====================================================================


bool OpenFLUIDBuddy::parseOptions(std::string OptsStr)
{
  // example inputdir=/usr/lib,outputdir=./toto,withfoo=1

  std::vector<std::string> OptsItems;
  std::vector<std::string> OptKeyValue;

  OptsItems = openfluid::tools::splitString(OptsStr,",");

  for (unsigned int i = 0; i<OptsItems.size();i++)
  {
    OptKeyValue = openfluid::tools::splitString(OptsItems[i],"=");

    if (OptKeyValue.size() == 2 )
    {
      m_Options[OptKeyValue[0]] = OptKeyValue[1];
    }
    else
    {
      m_Options.clear();
      return false;
    }
  }

  return true;
}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::streamOptions(std::ostream& OStream)
{

  std::map<std::string,std::string>::iterator it;

  for (it = m_Options.begin();it != m_Options.end();++it)
  {
    OStream << it->first << " : " << it->second << std::endl;
  }

}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::invokeHelp()
{
  mp_Listener->onHelpRequired(m_RequiredOptionsHelp);
  mp_Listener->onHelpOthers(m_OtherOptionsHelp);
}


} } //namespaces

