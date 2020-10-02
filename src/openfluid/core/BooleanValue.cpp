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
  @file BooleanValue.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>


namespace openfluid { namespace core {


BooleanValue& BooleanValue::operator=(const Value& Other)
{
  if (this != &Other)
  {
    m_Value = Other.asBooleanValue().m_Value;
  }

  return *this;
}


// =====================================================================
// =====================================================================


BooleanValue& BooleanValue::operator=(Value&& Other)
{
  if (this != &Other)
  {
    m_Value = Other.asBooleanValue().m_Value;
    Other.asBooleanValue().m_Value = false;
  }
  
  return *this;
}


// =====================================================================
// =====================================================================


bool BooleanValue::convert(Value& Val) const
{
  if (Val.getType() == Value::DOUBLE)
  {
    Val = DoubleValue(m_Value);
    return true;
  }
  else if (Val.getType() == Value::INTEGER)
  {
    Val = IntegerValue(m_Value);
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


void BooleanValue::writeToStream(std::ostream& OutStm) const
{
  OutStm << std::boolalpha << m_Value << std::noboolalpha;
}


} }  // namespaces

