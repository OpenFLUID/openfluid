/***copyright>=========================================================

  OpenFLUID - Software environment for modelling Fluxes In Landscapes

  This file is a part of OpenFLUID software
  http://www.umr-lisah.fr/openfluid
  Copyright (c) INRA
  See the file "copyright" for the full license governing this code.

=========================================================<copyright***/


/**
  \file SwissTools.h
  \brief Header for general purpose tools.
  \author JC.Fabre <fabrejc@supagro.inra.fr>
*/


#ifndef __OFSWISSTOOLS_H__
#define __OFSWISSTOOLS_H__

#include <vector>
#include <string>
#include <sstream>
#include <cmath>

namespace openfluid { namespace tools {

/**
  Template function for string to other type conversion
  \param[in] StrToConvert the string to convert
  \param[out] Converted the result of the conversion
  \return true if the conversion is correct
*/
template<typename T>
inline bool ConvertString(const std::string& StrToConvert, T* Converted)
{
  std::istringstream iss(StrToConvert);
  return !(iss >> (*Converted)).fail();
}

// =====================================================================
// =====================================================================

/**
  Template function for value to string conversion
  \param[in] ValueToConvert the value to convert
  \param[out] StrConverted the result of the conversion
  \return true if the conversion is correct
*/
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

/**
  Function for tokenizing string into a vector of tokens
  \param[in] StrToTokenize the string to tokenize
  \param[out] Tokens the resulting tokens
  \param[in] Delimiters the string delimiter
*/
void TokenizeString(const std::string& StrToTokenize,
                    std::vector<std::string>& Tokens,
                    const std::string& Delimiters);

// =====================================================================
// =====================================================================


/**
  Function for testing equality between two double precision floats,
  using the "close enough" method.
  \param[in] A the firts term of the equality
  \param[in] B the firts term of the equality
  \param[in] Epsilon the comparison tolerance factor

  \see http://www.ensta.fr/~diam/c++/online/c++-faq-lite/newbie.html#faq-29.17
  \see http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/testing-tools/floating_point_comparison.html
*/
inline bool IsCloseEnough(double A, double B, double Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return ((std::abs(A - B)) <= (Epsilon * std::abs(A)));
}


// =====================================================================
// =====================================================================


/**
  Function for testing equality between two double precision floats,
  using the "very close" method.
  \param[in] A the firts term of the equality
  \param[in] B the firts term of the equality
  \param[in] Epsilon the comparison tolerance factor

  \see http://www.ensta.fr/~diam/c++/online/c++-faq-lite/newbie.html#faq-29.17
  \see http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/testing-tools/floating_point_comparison.html
*/
inline bool IsVeryClose(double A, double B, double Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return (((std::abs(A - B)) <= (Epsilon * std::abs(A))) && ((std::abs(A - B)) <= (Epsilon * std::abs(B))));
}


// =====================================================================
// =====================================================================


bool WildcardMatching(const std::string Pattern, const std::string Str);



} } //namespaces


#endif // __OFSWISSTOOLS_H__

