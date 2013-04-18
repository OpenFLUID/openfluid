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
  \file PluggableFunction.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/config.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/ware/PluggableFunction.hpp>


// =====================================================================
// =====================================================================


namespace openfluid { namespace ware {



PluggableFunction::PluggableFunction()
  : SimulationContributorWare(FUNCTION),
    m_MaxThreads(openfluid::config::FUNCTIONS_MAXNUMTHREADS)
{

}


// =====================================================================
// =====================================================================


PluggableFunction::~PluggableFunction()
{
  finalizeWare();
}


// =====================================================================
// =====================================================================


void PluggableFunction::initializeWare(const WareID_t& ID,const unsigned int& MaxThreads)
{
  if (m_Initialized) return;

  SimulationContributorWare::initializeWare(ID);

  m_MaxThreads = MaxThreads;
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      openfluid::core::StringValue& Val) const
{
  try
  {
    Val = openfluid::core::StringValue(Params.get<std::string>(ParamName));
    return true;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      openfluid::core::DoubleValue& Val) const
{
  try
  {
    return openfluid::core::StringValue(Params.get<std::string>(ParamName)).toDoubleValue(Val);
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      openfluid::core::VectorValue& Val) const
{
  try
  {
    return openfluid::core::StringValue(Params.get<std::string>(ParamName)).toVectorValue(Val.getStreamSeparator(1),Val);
  }
  catch (...)
  {
    return false;
  }
}



// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                    const openfluid::ware::WareParamKey_t& ParamName,
                                    openfluid::core::MatrixValue& Val) const
{
  try
  {
    return openfluid::core::StringValue(Params.get<std::string>(ParamName)).toMatrixValue(Val.getStreamSeparator(1),Val.getStreamSeparator(2),Val);
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      double& Val) const
{
  try
  {
    return openfluid::core::StringValue(Params.get<std::string>(ParamName)).toDouble(Val);
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      long& Val) const
{
  try
  {
    return openfluid::core::StringValue(Params.get<std::string>(ParamName)).toInteger(Val);
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      float& Val) const
{
  try
  {
    double TmpDbl;
    bool IsOK;

    IsOK = openfluid::core::StringValue(Params.get<std::string>(ParamName)).toDouble(TmpDbl);
    if (IsOK) Val = static_cast<float>(TmpDbl);
    return IsOK;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      int& Val) const
{
  try
  {
    long TmpInt;
    bool IsOK;

    IsOK = openfluid::core::StringValue(Params.get<std::string>(ParamName)).toInteger(TmpInt);
    if (IsOK) Val = static_cast<int>(TmpInt);
    return IsOK;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::string& Value) const
{
  try
  {
    Value = Params.get<std::string>(ParamName);
    return true;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::vector<std::string>& Vals) const
{
  try
  {
    std::vector<std::string> Tokens;

    openfluid::tools::TokenizeString(Params.get<std::string>(ParamName),Tokens,";");

    Vals.clear();
    for (unsigned int i=0;i<Tokens.size();i++)
          Vals.push_back(Tokens[i]);

    return true;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::vector<double>& Vals) const
{
  try
  {
    bool IsOK = false;

    openfluid::core::StringValue TmpStr = openfluid::core::StringValue(Params.get<std::string>(ParamName));
    openfluid::core::VectorValue Vect;
    IsOK = TmpStr.toVectorValue(";",Vect);

    if(IsOK)
    {
      Vals.clear();

      for (unsigned long i=0;i<Vect.size();i++)
        Vals.push_back(Vect[i]);
    }

    return IsOK;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


bool PluggableFunction::OPENFLUID_GetFunctionParameter(const openfluid::ware::WareParams_t& Params,
                                      const openfluid::ware::WareParamKey_t& ParamName,
                                      std::vector<long>& Vals) const
{
  try
  {
    bool IsOK = false;

    openfluid::core::StringValue TmpStr = openfluid::core::StringValue(Params.get<std::string>(ParamName));
    openfluid::core::VectorValue Vect;
    IsOK = TmpStr.toVectorValue(";",Vect);

    if(IsOK)
    {
      Vals.clear();

      for (unsigned long i=0;i<Vect.size();i++)
        Vals.push_back(static_cast<int>(Vect[i]));
    }

    return IsOK;
  }
  catch (...)
  {
    return false;
  }
}


// =====================================================================
// =====================================================================


void PluggableFunction::OPENFLUID_SetFunctionMaxThreads(const unsigned int& MaxNumThreads)
{
  if (MaxNumThreads > 0) m_MaxThreads = MaxNumThreads;
}


} } // namespaces
