/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file FloatingPoint.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_SCIENTIFIC_FLOATINGPOINT_HPP__
#define __OPENFLUID_SCIENTIFIC_FLOATINGPOINT_HPP__


#include <cmath>

#include <openfluid/deprecation.hpp>


namespace openfluid { namespace scientific {


/**
  Tests equality between two floating point values, using the "close enough" method.
  @param[in] A the first term of the equality
  @param[in] B the second term of the equality
  @param[in] Epsilon the comparison tolerance factor

  @see http://www.parashift.com/c++-faq-lite/floating-point-arith.html
  @see http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/testing-tools/floating_point_comparison.html
*/
template<typename T1, typename T2>
inline bool isCloseEnough(const T1& A, const T2& B, const double& Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return ((std::fabs(A - B)) <= (Epsilon * std::fabs(A)));
}


/**
  Tests equality between two floating point values, using the "very close" method.
  @param[in] A the first term of the equality
  @param[in] B the seond term of the equality
  @param[in] Epsilon the comparison tolerance factor

  @see http://www.parashift.com/c++-faq-lite/floating-point-arith.html
  @see http://www.boost.org/doc/libs/1_38_0/libs/test/doc/html/utf/testing-tools/floating_point_comparison.html
*/
template<typename T1, typename T2>
inline bool isVeryClose(const T1& A, const T2& B, const double& Epsilon = 0.00001)
{
  // see Knuth section 4.2.2 pages 217-218
  return (((std::fabs(A - B)) <= (Epsilon * std::fabs(A))) && ((std::fabs(A - B)) <= (Epsilon * std::fabs(B))));
}


/**
  @deprecated Since version 2.1.0. Use openfluid::scientific::isCloseEnough instead
*/
inline bool IsCloseEnough(double A, double B, double Epsilon = 0.00001) OPENFLUID_DEPRECATED;
inline bool IsCloseEnough(double A, double B, double Epsilon)
{
  return isCloseEnough(A,B,Epsilon);
}


/**
  @deprecated Since version 2.1.0.Use openfluid::scientific::isVeryClose instead
*/
inline bool IsVeryClose(double A, double B, double Epsilon = 0.00001) OPENFLUID_DEPRECATED;
inline bool IsVeryClose(double A, double B, double Epsilon)
{
  return isVeryClose(A,B,Epsilon);
}


} }  // namespaces


#endif /* __OPENFLUID_SCIENTIFIC_FLOATINGPOINT_HPP__ */
