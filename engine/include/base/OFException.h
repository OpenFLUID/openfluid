/**
  \file OFException.h
  \brief Header of class managing exception

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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
      std::string m_Msg;

  public:
    OFException(const char * Msg)
    {
      m_Msg = Msg;
    }

    OFException(std::string Msg)
    {
      m_Msg = Msg;
    }

    OFException(std::string Sender, std::string Msg)
    {
      m_Msg = Msg + " (sent by " + Sender + ")" ;
    }

    OFException(std::string Sender, int TimeStep, std::string Msg)
    {
      std::stringstream StrStream;
      StrStream << TimeStep;

      m_Msg = Msg + " (sent by " + Sender + ", time step #" + StrStream.str() +")" ;
    }


    virtual ~OFException() throw()
    {

    }

    virtual const char * what() const throw()
    {
      return this->m_Msg.c_str();
    }


};




} } // namespace

#endif
