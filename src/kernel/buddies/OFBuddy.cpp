/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file OFBuddy.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "OFBuddy.h"
#include "AppTools.h"


OpenFLUIDBuddy::OpenFLUIDBuddy()
{

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

  OptsItems = SplitString(OptsStr,",");

  for (unsigned int i = 0; i<OptsItems.size();i++)
  {
    OptKeyValue = SplitString(OptsItems[i],"=");

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


void OpenFLUIDBuddy::printlnOptions()
{

  std::map<std::string,std::string>::iterator it;

  for (it = m_Options.begin();it != m_Options.end();++it)
  {
    std::cout << it->first << " : " << it->second << std::endl;
  }

}


// =====================================================================
// =====================================================================


void OpenFLUIDBuddy::displayHelp()
{
  std::map<std::string,std::string>::iterator it;

  std::cout << "  Required options:" << std::endl;
  if (m_RequiredOptionsHelp.size() ==  0)
    std::cout << "    (none)" << std::endl;
  else
  {
    for (it = m_RequiredOptionsHelp.begin();it != m_RequiredOptionsHelp.end();++it)
    {
      std::cout << "    " << it->first << " : " << it->second << std::endl;
    }
  }

  std::cout << "  Other options:" << std::endl;
  if (m_OtherOptionsHelp.size() ==  0)
    std::cout << "    (none)" << std::endl;
  else
  {
    for (it = m_OtherOptionsHelp.begin();it != m_OtherOptionsHelp.end();++it)
    {
      std::cout << "    " << it->first << " : " << it->second << std::endl;
    }
  }

}

