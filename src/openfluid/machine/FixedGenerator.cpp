/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file FixedGenerator.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/machine/FixedGenerator.hpp>
#include <openfluid/tools/StringHelpers.hpp>


namespace openfluid { namespace machine {

template <class T>
FixedGenerator<T>::FixedGenerator() : MonoGenerator(), m_DeltaT(0)
{

}


// =====================================================================
// =====================================================================


template <>
FixedGenerator<double>::FixedGenerator() : MonoGenerator(), m_VarValueInit(0), m_DeltaT(0)
{
  m_VarValue = 0;
}


// =====================================================================
// =====================================================================


template <>
FixedGenerator<long int>::FixedGenerator() : MonoGenerator(), m_VarValueInit(0), m_DeltaT(0)
{
  m_VarValue = 0;
}


// =====================================================================
// =====================================================================


template <>
FixedGenerator<bool>::FixedGenerator() : MonoGenerator(), m_VarValueInit(false), m_DeltaT(0)
{
  m_VarValue = false;
}


// =====================================================================
// =====================================================================


template <>
FixedGenerator<std::string>::FixedGenerator() : MonoGenerator(), m_VarValueInit(""), m_DeltaT(0)
{
  m_VarValue = "";
}


// =====================================================================
// =====================================================================


template <>
LinearFixedGenerator<openfluid::core::VectorValue>::LinearFixedGenerator() : 
  FixedGenerator<openfluid::core::VectorValue>(), LinearGeneratorMixin()
{
  m_VarValueInit = openfluid::core::VectorValue(1);
  m_VarValue = openfluid::core::VectorValue(1);
}


// =====================================================================
// =====================================================================


template <>
LinearFixedGenerator<openfluid::core::MatrixValue>::LinearFixedGenerator() : 
  FixedGenerator<openfluid::core::MatrixValue>(), LinearGeneratorMixin()
{
  m_VarValueInit = openfluid::core::MatrixValue();
  m_VarValue = openfluid::core::MatrixValue();
}


// =====================================================================
// =====================================================================


template <class T>
void FixedGenerator<T>::processVarValue(const openfluid::ware::WareParams_t& Params)
{
  std::string VarSizeStr;
  if (OPENFLUID_GetWareParameter(Params,"varsize", VarSizeStr))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION, s_VarsizeNotAcceptedString);
  }
  if (!this->OPENFLUID_GetWareParameter(Params,"fixedvalue",m_VarValue))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing fixed value for generator");
  }
}


// =====================================================================
// =====================================================================


template <>
void LinearFixedGenerator<openfluid::core::VectorValue>::processVarValue(const openfluid::ware::WareParams_t& Params)
{
  std::string VarSizeStr;
  bool ExplicitSize = false;
  if (OPENFLUID_GetWareParameter(Params,"varsize", VarSizeStr))
  {
    LinearGeneratorMixin::processVarValue(VarSizeStr);
    ExplicitSize = true;
  }
  std::string StringVarValue;
  if (!OPENFLUID_GetWareParameter(Params,"fixedvalue",StringVarValue))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing fixed vector value for generator");
  }
  if (openfluid::core::StringValue(StringVarValue).guessTypeConversion() == openfluid::core::Value::VECTOR)
  {
    openfluid::core::VectorValue VV;
    if (!OPENFLUID_GetWareParameter(Params,"fixedvalue",VV))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "badly formatted fixed vector value for generator");
    }
    if (ExplicitSize && VV.size() != m_VarDimensions.Rows)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "wrong size for fixed vector value for generator");
    }
    m_VarValue = VV;
  }
  else
  {
    double d;
    try {
      d = std::stod(StringVarValue);
    }
    catch(const std::invalid_argument&)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "unknown format for fixed vector value for generator");
    }
    openfluid::core::VectorValue VV(m_VarDimensions.Rows, d);
    m_VarValue = VV;
  }
}


// =====================================================================
// =====================================================================


template <>
void LinearFixedGenerator<openfluid::core::MatrixValue>::processVarValue(const openfluid::ware::WareParams_t& Params)
{
  std::string VarSizeStr;
  if (OPENFLUID_GetWareParameter(Params,"varsize", VarSizeStr))
  {
    LinearGeneratorMixin::processVarValue(VarSizeStr);
  }
  std::string StringVarValue;
  if (!OPENFLUID_GetWareParameter(Params,"fixedvalue",StringVarValue))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"missing fixed vector value for generator");
  }
  if (openfluid::core::StringValue(StringVarValue).guessTypeConversion() == openfluid::core::Value::MATRIX)
  {
    openfluid::core::MatrixValue MV;
    if (!OPENFLUID_GetWareParameter(Params,"fixedvalue",MV))
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "badly formatted fixed vector value for generator");
    }
    if (MV.getRowsNbr() != m_VarDimensions.Rows || MV.getColsNbr() != m_VarDimensions.Cols)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "wrong size for fixed vector value for generator");
    }
    m_VarValue = MV;
  }
  else
  {
    double d;
    try {
      d = std::stod(StringVarValue);
    }
    catch(const std::invalid_argument&)
    {
      throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                                "unknown format for fixed vector value for generator");
    }
    openfluid::core::MatrixValue MV(m_VarDimensions.Cols, m_VarDimensions.Rows, d);
    m_VarValue = MV;
  }
}


// =====================================================================
// =====================================================================


template <class T>
void FixedGenerator<T>::initParams(const openfluid::ware::WareParams_t& Params)
{
  processVarValue(Params);

  std::string DeltaTStr;
  if (OPENFLUID_GetWareParameter(Params,"deltat",DeltaTStr) &&
      !openfluid::tools::toNumeric(DeltaTStr,m_DeltaT))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"wrong value for deltat");
  }
}


// =====================================================================
// =====================================================================


template <>
void FixedGenerator<double>::applyValue(openfluid::core::SpatialUnit* LU, bool init)
{
  if (init)
  {
    OPENFLUID_InitializeVariable(LU,m_VarName,0.0);
  }
  else
  {
    OPENFLUID_AppendVariable(LU,m_VarName,m_VarValue);
  }
}


// =====================================================================
// =====================================================================


template <>
void FixedGenerator<std::string>::applyValue(openfluid::core::SpatialUnit* LU, bool init)
{
  if (init)
  {
    std::string InitVar = "";
    OPENFLUID_InitializeVariable(LU,m_VarName, InitVar);
  }
  else
  {
    OPENFLUID_AppendVariable(LU,m_VarName,m_VarValue);
  }
}


// =====================================================================
// =====================================================================


template <>
void LinearFixedGenerator<openfluid::core::VectorValue>::applyValue(openfluid::core::SpatialUnit* LU, bool init)
{
  if (init)
  {
    openfluid::core::VectorValue VV(m_VarDimensions.Rows,0.0);
    OPENFLUID_InitializeVariable(LU,m_VarName,VV);
  }
  else
  {
    OPENFLUID_AppendVariable(LU,m_VarName,m_VarValue);
  }
}


// =====================================================================
// =====================================================================


template <>
void LinearFixedGenerator<openfluid::core::MatrixValue>::applyValue(openfluid::core::SpatialUnit* LU, bool init)
{
  if (init)
  {
    openfluid::core::MatrixValue MV(m_VarDimensions.Cols, m_VarDimensions.Rows,0.0);
    OPENFLUID_InitializeVariable(LU,m_VarName,MV);
  }
  else
  {
    OPENFLUID_AppendVariable(LU,m_VarName,m_VarValue);
  }
}


// =====================================================================
// =====================================================================


template <class T>
openfluid::base::SchedulingRequest FixedGenerator<T>::initializeRun()
{
  openfluid::core::SpatialUnit* LU;
  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    applyValue(LU, true);
  }

  if (m_DeltaT > 0)
  {
    return Duration(m_DeltaT);
  }
  else
  {
    return DefaultDeltaT();
  }
}


// =====================================================================
// =====================================================================


template <class T>
openfluid::base::SchedulingRequest FixedGenerator<T>::runStep()
{

  openfluid::core::SpatialUnit* LU;

  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitsClass,LU)
  {
    applyValue(LU);
  }

  if (m_DeltaT > 0)
  {
    return Duration(m_DeltaT);
  }
  else
  {
    return DefaultDeltaT();
  }
}


} } //namespaces

