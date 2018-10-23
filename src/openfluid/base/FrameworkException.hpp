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
  @file FrameworkException.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BASE_FRAMEWORKEXCEPTION_HPP__
#define __OPENFLUID_BASE_FRAMEWORKEXCEPTION_HPP__


#include <openfluid/base/Exception.hpp>
#include <openfluid/deprecation.hpp>


namespace openfluid { namespace base {


class FrameworkException : public Exception
{
  protected:

    void buildFullMessage()
    {
      m_FullMessage = m_Message;
      m_FullMessage += " (sent by OpenFLUID framework";

      // framework part
      if (m_Context.find("part") != m_Context.end())
      {
        m_FullMessage += ", from " + m_Context["part"];
      }

      // code location
      if (m_Context.find("codeloc") != m_Context.end())
      {
        m_FullMessage += ", located at " + m_Context["codeloc"];
      }

      m_FullMessage += ")";
    }


  public:

    FrameworkException() = delete;

    FrameworkException(const ExceptionContext& Context, const std::string& Msg) :
      Exception(Context,Msg)
    {
      buildFullMessage();
    }


    // =====================================================================
    // =====================================================================


    FrameworkException(const std::string& CodeLoc, const std::string& Msg) :
      Exception(computeContext(CodeLoc),Msg)
    {
      buildFullMessage();
    }


    // =====================================================================
    // =====================================================================


    static ExceptionContext computeContext()
    {
      ExceptionContext Context;
      Context["source"] = "framework";
      return Context;
    }


    // =====================================================================
    // =====================================================================


    static ExceptionContext computeContext(const std::string& CodeLoc)
    {
      ExceptionContext Context = computeContext();
      Context["codeloc"] = CodeLoc;
      return Context;
    }


    // =====================================================================
    // =====================================================================


    FrameworkException(const std::string& Msg) :
      Exception(computeContext(),Msg)
    {
      buildFullMessage();
    }

};


} } // namespaces

#endif /* __OPENFLUID_BASE_FRAMEWORKEXCEPTION_HPP__ */
