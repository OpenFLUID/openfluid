

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


// =====================================================================
// =====================================================================


void TokenizeString(const std::string& StrToTokenize,
                    std::vector<std::string>& Tokens,
                    const std::string& Delimiters);




} } //namespaces


#endif // __OFSWISSTOOLS_H__

