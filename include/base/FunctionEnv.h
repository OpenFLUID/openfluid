/**
  \file FunctionEnv.h
  \brief Header of function environment management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __FUNCTIONENV_H__
#define __FUNCTIONENV_H__


#include <map>
#include <string>

namespace openfluid { namespace base {

/**
  \if DOCFORDEV
  Class containing execution environment information for funtions
  \endif
*/
class FunctionEnvironment
{
  private:

    typedef std::map<std::string,bool> KeyBoolMap_t;
    typedef std::map<std::string,std::string> KeyStringMap_t;

    KeyBoolMap_t m_BoolValues;
    KeyStringMap_t m_StringValues;

  public:

    /**
      Constructor
    */
    FunctionEnvironment();

    /**
      Destructor
    */
    virtual ~FunctionEnvironment();

    /**
      Assigns a boolean value to the corresponding key

      \return true if the assignment is correct
    */
    bool setValue(std::string Key, bool Value);

    /**
      Assigns a string value to the corresponding key

      \return true if the assignment is correct
    */
    bool setValue(std::string Key, std::string Value);

    /**
      Gets the boolean value assigned to the key

      \return true if the value exists as a boolean
    */
    bool getValue(std::string Key, bool *Value) const;

    /**
      Gets the string value assigned to the key

      \return true if the value exists as a string
    */
    bool getValue(std::string Key, std::string *Value) const;


};


} } // namespace

#endif /* __FUNCTIONENV_H__ */
