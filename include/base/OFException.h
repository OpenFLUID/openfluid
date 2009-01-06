/**
  \file OFException.h
  \brief Header of class managing exception

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __OFEXCEPTION_H__
#define __OFEXCEPTION_H__

#include <exception>
#include <string>


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

    virtual ~my_exception() throw()
    {

    }

    virtual const char * what() const throw
    {
      return this->m_Msg.c_str();
    }


};


}


} } // namespace


}
