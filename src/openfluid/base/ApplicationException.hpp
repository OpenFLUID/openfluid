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
  \file FrameworkException.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BASE_APPLICATIONEXCEPTION_HPP__
#define __OPENFLUID_BASE_APPLICATIONEXCEPTION_HPP__

#include <openfluid/base/Exception.hpp>


namespace openfluid { namespace base {


class ApplicationException : public Exception
{
  protected:

    void buildFullMessage()
    {
      m_FullMessage = m_Message;
      m_FullMessage += " (sent by " + m_Sender + " application";
      if (m_Source != "")
        m_FullMessage += ", from " + m_Source;
      m_FullMessage += ")";
    }


  public:

    ApplicationException(const std::string& AppName, const std::string& Msg) :
      Exception(AppName,Msg)
    {
      buildFullMessage();
    }

    ApplicationException(const std::string& AppName, const std::string& Source, const std::string& Msg) :
      Exception(AppName,Source,Msg)
    {
      buildFullMessage();
    }

};


} } // namespaces

#endif /* __OPENFLUID_BASE_APPLICATIONEXCEPTION_HPP__ */
