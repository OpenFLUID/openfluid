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
  \file SourceInfo.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "SourceInfos.hpp"


SourceInfos::SourceInfos():
  SourceGeomType(wkbUnknown),
  IsAreaCompute(false), AreaComputeAttribute("area"),
  IsLengthCompute(false), LengthComputeAttribute("length"),
  IsXCentroidCompute(false), XCentroidComputeAttribute("xcentroid"),
  IsYCentroidCompute(false), YCentroidComputeAttribute("ycentroid"),
  IsZCentroidCompute(false), ZCentroidComputeAttribute("zcentroid"),
  IsDatasetImport(false), IsDatastore(false)
{

}


// =====================================================================
// =====================================================================


int SourceInfos::getGeometryDimension()
{
  int Dim = -1;

  switch (SourceGeomType)
  {
    case wkbUnknown: Dim = -1; break;
    case wkbPoint: Dim = 0; break;
    case wkbLineString: Dim = 1; break;
    case wkbPolygon: Dim = 2; break;
    case wkbMultiPoint: Dim = 0; break;
    case wkbMultiLineString: Dim = 1; break;
    case wkbMultiPolygon: Dim = 2; break;
    case wkbGeometryCollection: Dim = -1; break;
    case wkbNone: Dim = -1; break;
    case wkbLinearRing: Dim = 1; break;
    case wkbPoint25D: Dim = 0; break;
    case wkbLineString25D: Dim = 1; break;
    case wkbPolygon25D: Dim = 2; break;
    case wkbMultiPoint25D: Dim = 0; break;
    case wkbMultiLineString25D: Dim = 1; break;
    case wkbMultiPolygon25D: Dim = 2; break;
    case wkbGeometryCollection25D: Dim = -1; break;
  }

  return Dim;
}
