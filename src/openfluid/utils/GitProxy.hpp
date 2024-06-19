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
  @file GitProxy.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_UTILS_GITPROXY_HPP__
#define __OPENFLUID_UTILS_GITPROXY_HPP__


#include <openfluid/base/OtherException.hpp>
#include <openfluid/utils/ProgramProxy.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace utils {


class OPENFLUID_API GitProxy : public ProgramProxy<GitProxy>
{
  private:

    static void findGitProgram();

  protected: 
  
    std::string m_LocalGitProgram;

  public:

    GitProxy();

    virtual ~GitProxy();

    static bool isAvailable();

    static bool isPathGitRepo(const std::string& Path);

    static const std::string getCurrentBranchName(const std::string& Path);   

};

class GitOperationException : public openfluid::base::Exception
{
  protected:

    void buildFullMessage()
    {
      m_FullMessage = m_Message;
    }


  public:

    GitOperationException(const std::string& Msg) :
      Exception(Msg)
    {
      buildFullMessage();
    }
};


} } // namespaces


#endif /* __OPENFLUID_UTILS_GITPROXY_HPP__ */
