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
  @file WarePluginsSearchResults.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_WAREPLUGINSSEARCHRESULTS_HPP__
#define __OPENFLUID_MACHINE_WAREPLUGINSSEARCHRESULTS_HPP__


#include <string>
#include <map>
#include <vector>


namespace openfluid { namespace machine {


/**
  Class for management of search results of pluggable wares
  @tparam SignatureType class defining the container for ware signature only
*/
template<class SignatureInstanceType>
class OPENFLUID_API WarePluginsSearchResults
{
  private:

    std::vector<SignatureInstanceType*> m_AvailablePlugins;

    std::map<std::string,std::string> m_ErroredFiles;


  public:

    void appendAvailable(SignatureInstanceType* Item);

    void appendErrored(const std::string& Path,const std::string& Message);

    const std::vector<SignatureInstanceType*> availablePlugins() const;

    const std::map<std::string,std::string> erroredFiles() const;

};


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResults<SignatureInstanceType>::appendAvailable(SignatureInstanceType* Item)
{
  m_AvailablePlugins.push_back(Item);
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
void WarePluginsSearchResults<SignatureInstanceType>::appendErrored(const std::string& Path,const std::string& Message)
{
  m_ErroredFiles[Path] = Message;
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
const std::vector<SignatureInstanceType*> WarePluginsSearchResults<SignatureInstanceType>::availablePlugins() const
{
  return m_AvailablePlugins;
}


// =====================================================================
// =====================================================================


template<class SignatureInstanceType>
const std::map<std::string,std::string> WarePluginsSearchResults<SignatureInstanceType>::erroredFiles() const
{
  return m_ErroredFiles;
}


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_WAREPLUGINSSEARCHRESULTS_HPP__ */
