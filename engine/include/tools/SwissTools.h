

#ifndef __OFSWISSTOOLS_H__
#define __OFSWISSTOOLS_H__

#include <vector>
#include <string>
#include <sstream>


namespace openfluid { namespace tools {


template<typename T>
inline bool ConvertString(const std::string& StrToConvert, T* Converted)
{
  std::istringstream iss(StrToConvert);
  return !(iss >> (*Converted)).fail();
}


template<typename T>
inline bool ConvertValue(const T ValueToConvert, std::string * StrConverted)
{
  std::ostringstream oss;
  bool IsOK = !(oss << ValueToConvert).fail();

  if (IsOK) *StrConverted = oss.str();

  return IsOK;
}


// =====================================================================
// =====================================================================


void TokenizeString(const std::string& StrToTokenize,
                    std::vector<std::string>& Tokens,
                    const std::string& Delimiters);




} } //namespaces


#endif // __OFSWISSTOOLS_H__

