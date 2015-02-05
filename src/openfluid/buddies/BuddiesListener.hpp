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
  @file BuddiesListener.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_BUDDIES_BUDDIESLISTENER_HPP__
#define __OPENFLUID_BUDDIES_BUDDIESLISTENER_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/base/Listener.hpp>
#include <string>
#include <map>

namespace openfluid { namespace buddies {

// =====================================================================
// =====================================================================


class OPENFLUID_API BuddiesListener : public openfluid::base::Listener
{
  private:


  public:

    BuddiesListener() {};

    virtual ~BuddiesListener() {};

    virtual void onInfo(const std::string& /*Message*/) {};

    virtual void onStageCompleted(const std::string& /*Message*/) {};

    virtual void onSubstageCompleted(const std::string& /*Message*/) {};

    virtual void onHelpRequired(const std::map<std::string,std::string>& /*OptionsHelp*/) {};

    virtual void onHelpOthers(const std::map<std::string,std::string>& /*OptionsHelp*/) {};



};


} } //namespaces

#endif /* __OPENFLUID_BUDDIES_BUDDIESLISTENER_HPP__ */
