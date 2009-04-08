

#ifndef __OFSWISSTOOLS_H__
#define __OFSWISSTOOLS_H__

#include <vector>
#include <string>
#include <sstream>
#include <cmath>

namespace openfluid { namespace tools {


template<typename T>
inline bool ConvertString(const std::string& StrToConvert, T* Converted)
{
  std::istringstream iss(StrToConvert);
  return !(iss >> (*Converted)).fail();
}

// =====================================================================
// =====================================================================

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

// =====================================================================
// =====================================================================

inline bool IsCloseEnough(double A, double B, double Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return ((std::abs(A - B)) <= (Epsilon * std::abs(A)));
}

// =====================================================================
// =====================================================================

inline bool IsVeryClose(double A, double B, double Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return (((std::abs(A - B)) <= (Epsilon * std::abs(A))) && ((std::abs(A - B)) <= (Epsilon * std::abs(B))));
}



} } //namespaces


#endif // __OFSWISSTOOLS_H__

