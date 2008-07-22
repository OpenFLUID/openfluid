/**
  \file PlugFunction.h
  \brief header file for function environment

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/


#ifndef __FUNCTIONENV_H__
#define __FUNCTIONENV_H__

#include <wx/string.h>
#include <wx/hashmap.h>

namespace openfluid { namespace base {

class FunctionEnvironment
{
  private:
    WX_DECLARE_STRING_HASH_MAP(bool,KeyBoolMap);
    WX_DECLARE_STRING_HASH_MAP(wxString,KeyStringMap);

    KeyBoolMap m_BoolValues;
    KeyStringMap m_StringValues;

  public:

    FunctionEnvironment();

    virtual ~FunctionEnvironment();

    bool setValue(wxString Key, bool Value);

    bool setValue(wxString Key, wxString Value);

    bool getValue(wxString Key, bool *Value) const;

    bool getValue(wxString Key, wxString *Value) const;


};

} } // namespace

#endif /* __FUNCTIONENV_H__ */
