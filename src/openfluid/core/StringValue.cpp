/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file StringValue.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <sstream>

#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/core/ArrayValue.hpp>

#include <openfluid/base/OFException.hpp>

#include <openfluid/tools/SwissTools.hpp>


namespace openfluid { namespace core {


void StringValue::writeToStream(std::ostream& OutStm) const
{
  OutStm << m_Value;
}


// =====================================================================
// =====================================================================


bool StringValue::convertStringToDouble(const std::string& Str, double& Dbl)
{
  std::istringstream iss(Str);
  char c;
  return ((iss >> Dbl) && !iss.get(c));
}


// =====================================================================
// =====================================================================


bool StringValue::toDouble(double& Val) const
{
  return convertStringToDouble(m_Value,Val);
}


// =====================================================================
// =====================================================================


bool StringValue::toDoubleValue(DoubleValue& Val) const
{
  double PODVal;

  if (!toDouble(PODVal))
    return false;

  Val = DoubleValue(PODVal);

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toBoolean(bool& Val) const
{

  if (m_Value == "true")
  {
    Val = true;
    return true;
  }

  if (m_Value == "false")
  {
    Val = false;
    return true;
  }


  std::istringstream iss(m_Value);
  char c;
  return ((iss >> Val) && !iss.get(c));
}


// =====================================================================
// =====================================================================


bool StringValue::toBooleanValue(BooleanValue& Val) const
{
  bool PODVal;

  if (!toBoolean(PODVal))
    return false;

  Val = BooleanValue(PODVal);

  return true;
}


// =====================================================================
// =====================================================================



bool StringValue::toInteger(long& Val) const
{
  std::istringstream iss(m_Value);
  char c;
  return ((iss >> Val) && !iss.get(c));
}


// =====================================================================
// =====================================================================


bool StringValue::toIntegerValue(IntegerValue& Val) const
{
  long PODVal;

  if (!toInteger(PODVal))
    return false;

  Val = IntegerValue(PODVal);

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toNullValue(NullValue& Val) const
{
  if (m_Value != "null")
    return false;

  Val = NullValue();

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toVectorValue(const std::string& Sep, VectorValue& Val) const
{

  std::vector<std::string> Splitted = openfluid::tools::SplitString(m_Value,Sep);

  openfluid::core::VectorValue TmpVect(Splitted.size());
  double TmpDbl;

  for (unsigned long i=0;i<Splitted.size();i++)
  {
    if (!convertStringToDouble(Splitted[i],TmpDbl)) return false;
    TmpVect.set(i,TmpDbl);
  }

  Val = TmpVect;

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toMatrixValue(const std::string& ColSep, const std::string& RowSep, MatrixValue& Val) const
{

  std::vector<std::string> SplittedRows = openfluid::tools::SplitString(m_Value,RowSep);

  unsigned long TmpRowsNbr = SplittedRows.size();
  unsigned long TmpColsNbr;
  double TmpDbl;

  MatrixValue TmpMatrix(0,0);

  for (unsigned long i=0;i<TmpRowsNbr;i++)
  {
    std::vector<std::string> SplittedCols = openfluid::tools::SplitString(SplittedRows[i],ColSep);

    if (i==0)
    {
      TmpColsNbr = SplittedCols.size();
      if (TmpColsNbr == 0) return false;
      TmpMatrix = MatrixValue(TmpColsNbr,TmpRowsNbr);
    }

    if (TmpColsNbr != SplittedCols.size()) return false;


    for (unsigned long j=0;j<TmpColsNbr;j++)
    {
      if (!convertStringToDouble(SplittedCols[j],TmpDbl)) return false;
      TmpMatrix.set(j,i,TmpDbl);
    }
  }

  Val = TmpMatrix;

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toMatrixValue(const std::string& Sep, const unsigned int& RowLength, MatrixValue& Val) const
{

  std::vector<std::string> Splitted = openfluid::tools::SplitString(m_Value,Sep);

  unsigned long TmpSize = Splitted.size();

  if ( TmpSize % RowLength != 0 ) return false;

  unsigned long TmpRowsNbr = TmpSize / RowLength;
  unsigned long CurrentCol = 0;
  unsigned long CurrentRow = 0;
  double TmpDbl;

  MatrixValue TmpMatrix(RowLength,TmpRowsNbr);

  for (std::vector<std::string>::const_iterator it=Splitted.begin(); it!= Splitted.end(); ++it)
  {
    if (!convertStringToDouble(*it,TmpDbl)) return false;
    TmpMatrix.set(CurrentCol,CurrentRow,TmpDbl);

    CurrentCol++;
    if (CurrentCol >= RowLength)
    {
      CurrentCol = 0;
      CurrentRow++;
    }
  }

  Val = TmpMatrix;

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toArrayValue(const std::string& /*ColSep*/, const std::string& /*RowSep*/, ArrayValue& /*Val*/) const
{
  throw openfluid::base::OFException("under construction");
  return false;
}


// =====================================================================
// =====================================================================


bool StringValue::toArrayValue(const std::string& /*Sep*/, const unsigned int& /*RowLength*/, ArrayValue& /*Val*/) const
{
  throw openfluid::base::OFException("under construction");
  return false;
}


// =====================================================================
// =====================================================================


bool StringValue::toMapValue(const std::string& Sep, MapValue& Val) const
{
  std::vector<std::string> Splitted = openfluid::tools::SplitString(m_Value,Sep);

  unsigned long TmpSize = Splitted.size();


  MapValue TmpMap;

  for (std::vector<std::string>::const_iterator it=Splitted.begin(); it!= Splitted.end(); ++it)
  {
    std::vector<std::string> KeyValue = openfluid::tools::SplitString(*it,"=");

    if (KeyValue.size() != 2) return false;

    TmpMap.setString(KeyValue.front(),KeyValue.back());
  }

  Val = TmpMap;

  return true;
}


// =====================================================================
// =====================================================================



} }  // namespaces


