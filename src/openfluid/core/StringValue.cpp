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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <sstream>


#include <boost/algorithm/string.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <openfluid/core/StringValue.hpp>
#include <openfluid/core/DoubleValue.hpp>
#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/IntegerValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/MapValue.hpp>
#include <openfluid/core/VectorValue.hpp>
#include <openfluid/core/MatrixValue.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/tools/JSONHelpers.hpp>


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


StringValue& StringValue::operator=(const Value& Other)
{
  if (this != &Other)
  {
    const StringValue* CastedValue = dynamic_cast<const StringValue*>(&Other);

    if (CastedValue)
    {
      m_Value = CastedValue->m_Value;
    }
  }

  return *this;
}


// =====================================================================
// =====================================================================


StringValue& StringValue::operator=(Value&& Other)
{
  if (this != &Other)
  {
    StringValue* CastedValue = dynamic_cast<StringValue*>(&Other);

    if (CastedValue)
    {
      m_Value = std::move(CastedValue->m_Value);
      CastedValue->m_Value.clear();
    }
  }

  return *this;
}


// =====================================================================
// =====================================================================


bool StringValue::convert(Value& Val) const
{
  if (Val.getType() == Value::BOOLEAN)
  {
    return toBooleanValue(Val.asBooleanValue());
  }
  if (Val.getType() == Value::DOUBLE)
  {
    return toDoubleValue(Val.asDoubleValue());
  }
  else if (Val.getType() == Value::INTEGER)
  {
    return toIntegerValue(Val.asIntegerValue());
  }
  else if (Val.getType() == Value::VECTOR)
  {
    return toVectorValue(Val.asVectorValue());
  }
  else if (Val.getType() == Value::MATRIX)
  {
    return toMatrixValue(Val.asMatrixValue());
  }
  else if (Val.getType() == Value::MAP)
  {
    return toMapValue(Val.asMapValue());
  }
  else if (Val.getType() == Value::NULLL)
  {
    return toNullValue(Val.asNullValue());
  }

  return false;
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
  if (Str == "true")
  {
    Dbl = 1.0;
    return true;
  }
  else if (Str == "false")
  {
    Dbl = 0.0;
    return true;
  }
  else
  {
    std::istringstream iss(Str);
    char c;
    return ((iss >> Dbl) && !iss.get(c));
  }
}


// =====================================================================
// =====================================================================


std::vector<std::string> StringValue::splitString(const std::string& StrToSplit,
                                                  const std::string& Separators,
                                                  bool ReturnsEmpty)
{
  std::vector<std::string> SplitParts;

  boost::algorithm::token_compress_mode_type TokCompress = boost::token_compress_on;
  if (ReturnsEmpty)
  {
    TokCompress = boost::token_compress_off;
  }

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


unsigned int StringValue::replaceAll(const std::string& FindStr,const std::string& ReplaceStr)
{
  unsigned int Occurrences = 0;
  std::string::size_type StartPos = 0;

  while ((StartPos = m_Value.find(FindStr,StartPos)) != std::string::npos)
  {
    m_Value.replace(StartPos, FindStr.length(), ReplaceStr);
    StartPos += ReplaceStr.length();
    Occurrences++;
  }

  return Occurrences;
}


// =====================================================================
// =====================================================================


Value::Type StringValue::guessTypeConversion() const
{
  if (m_Value.empty())
  {
    return Value::NONE;
  }
  else if (m_Value.front() == '\"')  // explicit string
  {
    return Value::STRING;
  }
  else if (boost::xpressive::regex_match(
             m_Value,boost::xpressive::sregex::compile("((\\+|-)?[[:digit:]]+)")))  // integer
  {
    return Value::INTEGER;
  }
  else if (boost::xpressive::regex_match(
             m_Value,boost::xpressive::sregex::compile(
               "((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?")))  // double
  {
    return Value::DOUBLE;
  }
  else if (m_Value == "true" || m_Value == "false")  // boolean
  {
    return Value::BOOLEAN;
  }
  else if (m_Value == "null")  // null
  {
    return Value::NULLL;
  }
  else if (m_Value.size() >=4 &&
           m_Value.substr(0,2) == "[[" && m_Value.substr(m_Value.size()-2,2) == "]]")   // matrix
  {
    return Value::MATRIX;
  }
  else if (m_Value.size() >=2 &&
           m_Value.front() == '[' && m_Value.back() == ']')  // vector
  {
    return Value::VECTOR;
  }
  else if (m_Value.size() >=2 &&
           m_Value.front() == '{' && m_Value.back() == '}')  // map
  {
    return Value::MAP;
  }

  return Value::STRING;  // fallback if type cannot be automatically guessed
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
  {
    return false;
  }

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
  {
    return false;
  }

  Val = BooleanValue(PODVal);

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toInteger(int& Val) const
{
  if (m_Value == "true")
  {
    Val = 1;
    return true;
  }
  else if (m_Value == "false")
  {
    Val = 0;
    return true;
  }
  else
  {
    std::istringstream iss(m_Value);
    char c;
    return ((iss >> Val) && !iss.get(c));
  }
}


// =====================================================================
// =====================================================================


bool StringValue::toInteger(long& Val) const
{
  if (m_Value == "true")
  {
    Val = 1;
    return true;
  }
  else if (m_Value == "false")
  {
    Val = 0;
    return true;
  }
  else
  {
    std::istringstream iss(m_Value);
    char c;
    return ((iss >> Val) && !iss.get(c));
  }
}


// =====================================================================
// =====================================================================


bool StringValue::toIntegerValue(IntegerValue& Val) const
{
  long PODVal;

  if (!toInteger(PODVal))
  {
    return false;
  }

  Val = IntegerValue(PODVal);

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toNullValue(NullValue& Val) const
{
  if (m_Value != "null")
  {
    return false;
  }

  Val = NullValue();

  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toVectorValue(VectorValue& Val) const
{
  if (m_Value.size() >=2 &&
      m_Value.front() == '[' && m_Value.back() == ']')
  {
    std::string TmpStr = m_Value.substr(1,m_Value.size()-2);


    if (TmpStr.empty())
    {
      Val = openfluid::core::VectorValue();
      return true;
    }

    std::vector<std::string> Splitted = splitString(TmpStr,",");
    openfluid::core::VectorValue TmpVect(Splitted.size());

    double TmpDbl;

    for (unsigned long i=0;i<Splitted.size();i++)
    {
      if (!convertStringToDouble(Splitted[i],TmpDbl))
      {
        return false;
      }
      TmpVect.set(i,TmpDbl);
    }

    Val = TmpVect;

    return true;
  }
  else
  {
    // old deprecated format, e.g. "1.2;2.5;19.6;7"

    std::vector<std::string> Splitted = split(";");

    openfluid::core::VectorValue TmpVect(Splitted.size());
    double TmpDbl;

    for (unsigned long i=0;i<Splitted.size();i++)
    {
      if (!convertStringToDouble(Splitted[i],TmpDbl))
      {
        return false;
      }
      TmpVect.set(i,TmpDbl);
    }

    Val = TmpVect;
    return true;
  }
}


// =====================================================================
// =====================================================================


bool StringValue::toMatrixValue(MatrixValue& Val) const
{
  std::string TmpStr = m_Value;

  if (TmpStr.size() >=4 &&
      TmpStr.substr(0,2) == "[[" && TmpStr.substr(TmpStr.size()-2,2) == "]]")
  {
    TmpStr = TmpStr.substr(2,TmpStr.size()-4);

    if (TmpStr.empty())
    {
      Val = openfluid::core::MatrixValue();
      return true;
    }

    boost::replace_all(TmpStr,"],[","|");
    boost::replace_all(TmpStr,",",";");
  }
  else if (m_Value.size() >=2 && m_Value.front() == '[' && m_Value.back() == ']')
  {
    TmpStr = m_Value.substr(1,m_Value.size()-2);

    if (TmpStr.empty())
    {
      Val = openfluid::core::MatrixValue();
      return true;
    }

    boost::replace_all(TmpStr,",",";");
  }

  // now, process like old deprecated format, e.g. "1.2;2.5;19.6;7|18.7;5.0;6.2"

  std::vector<std::string> SplittedRows = splitString(TmpStr,"|");

  unsigned long TmpRowsNbr = SplittedRows.size();
  unsigned long TmpColsNbr;
  double TmpDbl;

  MatrixValue TmpMatrix(0,0);

  for (unsigned long i=0;i<TmpRowsNbr;i++)
  {
    std::vector<std::string> SplittedCols = splitString(SplittedRows[i],";");

    if (i==0)
    {
      TmpColsNbr = SplittedCols.size();
      if (TmpColsNbr == 0)
      {
        return false;
      }
      TmpMatrix = MatrixValue(TmpColsNbr,TmpRowsNbr);
    }

    if (TmpColsNbr != SplittedCols.size())
    {
      return false;
    }

    for (unsigned long j=0;j<TmpColsNbr;j++)
    {
      if (!convertStringToDouble(SplittedCols[j],TmpDbl))
      {
        return false;
      }
      TmpMatrix.set(j,i,TmpDbl);
    }
  }

  Val = TmpMatrix;

  return true;

}


// =====================================================================
// =====================================================================


bool StringValue::toMatrixValue(const unsigned int& RowLength, MatrixValue& Val) const
{
  std::string TmpStr = m_Value;

  if (TmpStr.size() >=2 &&
      TmpStr.front() == '[' && TmpStr.back() == ']')
  {
    TmpStr = TmpStr.substr(1,m_Value.size()-2);
    boost::replace_all(TmpStr,",",";");
  }

  // now, process like old deprecated format, e.g. "1.2;2.5;19.6;7;255.42;19.77"

  std::vector<std::string> Splitted = splitString(TmpStr,";");

  unsigned long TmpSize = Splitted.size();

  if ( TmpSize % RowLength != 0 )
  {
    return false;
  }

  unsigned long TmpRowsNbr = TmpSize / RowLength;
  unsigned long CurrentCol = 0;
  unsigned long CurrentRow = 0;
  double TmpDbl;

  MatrixValue TmpMatrix(RowLength,TmpRowsNbr);

  for (std::vector<std::string>::const_iterator it=Splitted.begin(); it!= Splitted.end(); ++it)
  {
    if (!convertStringToDouble(*it,TmpDbl))
    {
      return false;
    }
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


bool JSONObjectToMapValue(const openfluid::tools::json& Obj, MapValue& Val)
{
  if (!Obj.is_object())
  {
    return false;
  }

  MapValue TmpMap;

  for (const auto& [k,v] : Obj.items())
  {
    if (v.is_number_float())
    {
      TmpMap.setDouble(k,v.get<double>());
    }
    else if (v.is_number_integer())
    {
      TmpMap.setInteger(k,v.get<int>());
    }
    else if (v.is_string())
    {
      TmpMap.setString(k,v.get<std::string>());
    }
    else if (v.is_boolean())
    {
      TmpMap.setBoolean(k,v.get<bool>());
    }
    else if (v.is_array())
    {
      if (v.empty())
      {
        TmpMap.setVectorValue(k,VectorValue(0));
      }
      else if (v[0].is_number())
      {
        // case: simple vector
        VectorValue TmpVectVal(v.size());

        for (unsigned int i=0;i<v.size();i++)
        {
          if (!v[i].is_number())
          {
            return false;
          }
          TmpVectVal.set(i,v[i].get<double>());
        }
        TmpMap.setVectorValue(k,TmpVectVal);
      }
      else if (v[0].is_array())
      {
        // case: matrix (vector of vectors)
        unsigned int ExpectedColCount = v[0].size();

        if (!ExpectedColCount)
        {
          return false;
        }

        MatrixValue TmpMatVal(ExpectedColCount,v.size());

        for (unsigned int iMVR=0;iMVR<v.size();iMVR++)
        {
          if (ExpectedColCount != v[iMVR].size())
          {
            return false;
          }

          for (unsigned int iMVC=0;iMVC<ExpectedColCount;iMVC++)
          {
            TmpMatVal.set(iMVC,iMVR,v[iMVR][iMVC].get<double>());
          }
        }
        TmpMap.setMatrixValue(k,TmpMatVal);
      }
      else
      {
        return false;
      }
    }
    else if (v.is_object())
    {
      MapValue TmpMapVal;

      if (!JSONObjectToMapValue(v,TmpMapVal))
      {
        return false;
      }
      else
      {
        TmpMap.setMapValue(k,TmpMapVal);
      }
    }
    else
    {
      return false;
    }
  }

  Val = TmpMap;
  return true;
}


// =====================================================================
// =====================================================================


bool StringValue::toMapValue(MapValue& Val) const
{
  if (m_Value.front() == '{' && m_Value.back() == '}')
  {
    try
    {
      return JSONObjectToMapValue(openfluid::tools::json::parse(m_Value),Val);
    }
    catch (openfluid::tools::json::parse_error&)
    {
      return false;
    }
  }

  return false;
}


// =====================================================================
// =====================================================================


bool StringValue::toTreeValue(TreeValue& /*Val*/) const
{
  throw openfluid::base::FrameworkException(
      openfluid::base::FrameworkException::computeContext(OPENFLUID_CODE_LOCATION),"not implemented");

  return false;
}


} }  // namespaces

