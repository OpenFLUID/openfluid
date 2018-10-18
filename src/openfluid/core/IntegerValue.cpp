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
  @file IntegerValue.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>


namespace openfluid { namespace core {


Value& IntegerValue::operator =(const Value& Other)
{
  const IntegerValue* CastedValue = dynamic_cast<const IntegerValue*> (&Other);

  if (CastedValue)
  {
    m_Value = CastedValue->m_Value;
  }

  return *this;
}


// =====================================================================
// =====================================================================


bool IntegerValue::convert(Value& Val) const
{
  if (Val.getType() == Value::DOUBLE)
  {
    Val = DoubleValue(m_Value);
    return true;
  }
  else if (Val.getType() == Value::VECTOR)
  {
    Val = VectorValue(1,m_Value);
    return true;
  }
  else if (Val.getType() == Value::MATRIX)
  {
    Val = MatrixValue(1,1,m_Value);
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void IntegerValue::writeToStream(std::ostream& OutStm) const
{
  OutStm << m_Value;
}


} }  // namespaces

