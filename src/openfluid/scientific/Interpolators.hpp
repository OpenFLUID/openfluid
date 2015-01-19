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
  @file Interpolators.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#ifndef __OPENFLUID_SCIENTIFIC_INTERPOLATORS_HPP__
#define __OPENFLUID_SCIENTIFIC_INTERPOLATORS_HPP__


namespace openfluid { namespace scientific {


/**
  Performs a linear interpolation between (x0,y0) and (x1,y1),
  giving the y value for the x coordinate
  @param[in] X0 the x0 coordinate
  @param[in] Y0 the y0 coordinate
  @param[in] X1 the x1 coordinate
  @param[in] Y1 the y1 coordinate
  @param[in] X the x coordinate
  @return the y coordinate
*/
template<typename T>
inline T linearInterpolation(const T& X0, const T& Y0,
                             const T& X1, const T& Y1,
                             const T& X)
{
  return Y0 + ((Y1-Y0) * (X-X0) / (X1-X0));
}


// =====================================================================
// =====================================================================


/**
  Performs a linear interpolation between (0,y0) and (x1,y1),
  giving the y value for the origin x coordinate
  @param[in] Y0 the y0 coordinate
  @param[in] X1 the x1 coordinate
  @param[in] Y1 the y1 coordinate
  @param[in] X the x coordinate
  @return the y coordinate
*/
template<typename T>
inline T linearInterpolationFromXOrigin(const T& Y0,
                                        const T& X1, const T& Y1,
                                        const T& X)
{
  return Y0 + ((Y1-Y0) * X / X1);
}


} }  // namespaces


#endif /* __OPENFLUID_SCIENTIFIC_INTERPOLATORS_HPP__ */
