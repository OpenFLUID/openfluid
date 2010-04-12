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
  \if DOCFORDEV
  \file OFException.h
  \brief Header of class managing exception

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
  \endif
*/


#ifndef __OFEXCEPTION_H__
#define __OFEXCEPTION_H__

#include <exception>
#include <string>
#include <sstream>

namespace openfluid { namespace base {



class OFException : public std::exception
{
  private:
      std::string m_FullMsg;
      int m_TimeStep;
      std::string m_Sender;
      std::string m_Source;
      std::string m_Msg;

      void initValues()
      {
        m_Sender = "";
        m_Source = "";
        m_Msg = "";
        m_TimeStep = -1;
      }

      void buildFullMsg()
      {
        m_FullMsg = "";
        if (m_Msg != "")
        {
          m_FullMsg = m_Msg;
          if (m_Sender != "" )
          {
            m_FullMsg = m_FullMsg + " (sent by " + m_Sender;

            if (m_Source != "")
            {
              m_FullMsg = m_FullMsg + ", from " + m_Source;
            }

            if (m_TimeStep >= 0)
            {
              std::stringstream StrStream;
              StrStream << m_TimeStep;

              m_FullMsg = m_FullMsg + ", at time step #" + StrStream.str();
            }

            m_FullMsg = m_FullMsg + ")";
          }
        }
      }

  public:
    OFException(const char * Msg)
    {
      initValues();
      m_Msg = std::string(Msg);
      buildFullMsg();
    }

    OFException(std::string Msg)
    {
      initValues();
      m_Msg = Msg;
      buildFullMsg();
    }

    OFException(std::string Sender, std::string Msg)
    {
      initValues();
      m_Msg = Msg;
      m_Sender = Sender;
      buildFullMsg();
    }

    OFException(std::string Sender, int TimeStep, std::string Msg)
    {
      initValues();
      m_Msg = Msg;
      m_Sender = Sender;
      m_TimeStep = TimeStep;
      buildFullMsg();
    }

    OFException(std::string Sender, std::string Source, int TimeStep, std::string Msg)
    {
      initValues();
      m_Msg = Msg;
      m_Sender = Sender;
      m_Source = Source;
      m_TimeStep = TimeStep;
      buildFullMsg();
    }

    OFException(std::string Sender, std::string Source, std::string Msg)
    {
      initValues();
      m_Msg = Msg;
      m_Sender = Sender;
      m_Source = Source;
      buildFullMsg();
    }


    virtual ~OFException() throw()
    {

    }

    virtual const char * what() const throw()
    {
      return this->m_FullMsg.c_str();
    }


};




} } // namespace

#endif
