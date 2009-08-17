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
