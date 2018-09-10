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
  @file VectorValue.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/core/VectorValue.hpp>


namespace openfluid { namespace core {


Value& VectorValue::operator =(const Value& Other)
{
  const VectorValue* CastedValue = dynamic_cast<const VectorValue*> (&Other);

  if (CastedValue)
  {
    Vector<double>::operator=(static_cast<const Vector<double>& >(Other.asVectorValue()));
  }

  return *this;
}


// =====================================================================
// =====================================================================


void VectorValue::writeToStream(std::ostream& OutStm) const
{
  const unsigned int s = getSize();

  if (s == 0)
  {
    OutStm << "[]";
  }
  else
  {
    OutStm << "[";
    for (unsigned int i = 0; i<s ;i++ )
    {
      OutStm << m_Data[i];
      if (i != s-1)
        OutStm << ",";
    }
    OutStm << "]";
  }
}


} }  // namespaces
