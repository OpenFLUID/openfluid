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
  \file GeosCompat.hpp
  \brief Header of ...

  \author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_LANDR_GEOSCOMPAT_HPP__
#define __OPENFLUID_LANDR_GEOSCOMPAT_HPP__

#include <openfluid/config.hpp>

namespace openfluid {
namespace landr {

#if GEOS_VERSION_GREATER_OR_EQUAL_3_3_0
  #define GET_DANGLES(P,D) \
    D = P->getDangles();
#else
  #define GET_DANGLES(P,D) \
    std::vector<const geos::geom::LineString*>* TheDangles = P->getDangles(); \
    if (TheDangles) D = *TheDangles;
#endif

#if GEOS_VERSION_GREATER_OR_EQUAL_3_3_2
  #define GEOM_PTR_PAIR geos::geom::GeomPtrPair
#else
  #define GEOM_PTR_PAIR std::pair<std::auto_ptr<geos::geom::Geometry>,std::auto_ptr<geos::geom::Geometry> >
#endif


}}


#endif /* __OPENFLUID_LANDR_GEOSCOMPAT_HPP__ */
