/**
  \file FunctionEnv.h
  \brief Header of function environment management class

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __FUNCTIONENV_H__
#define __FUNCTIONENV_H__

#include <wx/string.h>
#include <wx/hashmap.h>

namespace openfluid { namespace base {

/**
  Class containing execution environment information for funtions
*/
class FunctionEnvironment
{
  private:
    WX_DECLARE_STRING_HASH_MAP(bool,KeyBoolMap);
    WX_DECLARE_STRING_HASH_MAP(wxString,KeyStringMap);

    KeyBoolMap m_BoolValues;
    KeyStringMap m_StringValues;

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
    bool setValue(wxString Key, bool Value);

    /**
      Assigns a string value to the corresponding key

      \return true if the assignment is correct
    */
    bool setValue(wxString Key, wxString Value);

    /**
      Gets the boolean value assigned to the key

      \return true if the value exists as a boolean
    */
    bool getValue(wxString Key, bool *Value) const;

    /**
      Gets the string value assigned to the key

      \return true if the value exists as a string
    */
    bool getValue(wxString Key, wxString *Value) const;


};

} } // namespace

#endif /* __FUNCTIONENV_H__ */
