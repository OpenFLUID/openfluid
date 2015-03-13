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
  @file StringValue.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */



#include <sstream>

#include <boost/algorithm/string.hpp>

#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>

#include <openfluid/base/FrameworkException.hpp>


namespace openfluid { namespace core {


StringValue::StringValue(bool Val) :
  SimpleValue(), m_Value(openfluid::core::BooleanValue(Val).toString())
{

}


// =====================================================================
// =====================================================================


StringValue::StringValue(int Val) :
  SimpleValue(), m_Value(openfluid::core::IntegerValue(Val).toString())
{

}


// =====================================================================
// =====================================================================


StringValue::StringValue(double Val) :
  SimpleValue(), m_Value(openfluid::core::DoubleValue(Val).toString())
{

}


// =====================================================================
// =====================================================================


Value& StringValue::operator =(const Value& Other)
{
  const StringValue* CastedValue = dynamic_cast<const StringValue*> (&Other);

  if (CastedValue)
  {
    m_Value = CastedValue->m_Value;
  }

  return *this;
}


// =====================================================================
// =====================================================================


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

std::vector<std::string> StringValue::splitString(const std::string& StrToSplit,
                                                  const std::string& Separators,
                                                  bool ReturnsEmpty)
{
  std::vector<std::string> SplitParts;

  boost::algorithm::token_compress_mode_type TokCompress = boost::token_compress_on;
  if (ReturnsEmpty) TokCompress = boost::token_compress_off;

  boost::split(SplitParts, StrToSplit, boost::is_any_of(Separators),TokCompress);

  return SplitParts;
}


// =====================================================================
// =====================================================================


std::vector<std::string> StringValue::split(const std::string& Separators,
                                            bool ReturnsEmpty) const
{
  return splitString(m_Value,Separators,ReturnsEmpty);
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

  if (m_Value == "true" || m_Value == "1")
  {
    Val = true;
    return true;
  }

  if (m_Value == "false" || m_Value == "0")
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


bool StringValue::toInteger(int& Val) const
{
  std::istringstream iss(m_Value);
  char c;
  return ((iss >> Val) && !iss.get(c));
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

  std::vector<std::string> Splitted = split(Sep);

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

  std::vector<std::string> SplittedRows = split(RowSep);

  unsigned long TmpRowsNbr = SplittedRows.size();
  unsigned long TmpColsNbr;
  double TmpDbl;

  MatrixValue TmpMatrix(0,0);

  for (unsigned long i=0;i<TmpRowsNbr;i++)
  {
    std::vector<std::string> SplittedCols = splitString(SplittedRows[i],ColSep);

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

  std::vector<std::string> Splitted = split(Sep);

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


bool StringValue::toMapValue(const std::string& Sep, MapValue& Val) const
{
  std::vector<std::string> Splitted = split(Sep);

//  unsigned long TmpSize = Splitted.size();


  MapValue TmpMap;

  for (std::vector<std::string>::const_iterator it=Splitted.begin(); it!= Splitted.end(); ++it)
  {
    std::vector<std::string> KeyValue = splitString(*it,"=");

    if (KeyValue.size() != 2) return false;

    TmpMap.setString(KeyValue.front(),KeyValue.back());
  }

  Val = TmpMap;

  return true;
}


// =====================================================================
// =====================================================================



} }  // namespaces


