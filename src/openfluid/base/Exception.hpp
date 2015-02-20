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
  @file Exception.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_BASE_EXCEPTION_HPP__
#define __OPENFLUID_BASE_EXCEPTION_HPP__

#include <exception>
#include <string>
#include <sstream>
#include <map>


namespace openfluid { namespace base {



class ExceptionContext : public std::map<std::string, std::string>
{
  public:

    std::string toString() const
    {
      std::string Str;


      std::map<std::string, std::string>::const_iterator it;
      std::map<std::string, std::string>::const_iterator itb = begin();
      std::map<std::string, std::string>::const_iterator ite = end();

      for (it = itb; it!= ite; ++it)
      {
        if (it!=itb)
          Str += ",";

        Str += (*it).first + "=" + (*it).second;
      }

      return Str;
    }

};


// =====================================================================
// =====================================================================


class Exception : public std::exception
{
  protected:

      std::string m_Message;

      ExceptionContext m_Context;

      std::string m_FullMessage;


      virtual void buildFullMessage() = 0;


      Exception(const std::string& Msg) :
        m_Message(Msg), m_FullMessage("")
      { }

      Exception(const ExceptionContext& Context, const std::string& Msg) :
        m_Message(Msg), m_Context(Context), m_FullMessage("")
      { }

  public:


    virtual ~Exception() throw()
    {

    }

    const char * what() const throw()
    {
      return m_FullMessage.c_str();
    }

    const std::string getMessage() const { return m_Message; }

    const ExceptionContext getContext() const { return m_Context; }

};


} } // namespace

#endif  /* __OPENFLUID_BASE_EXCEPTION_HPP__ */
