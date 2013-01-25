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
  \file VTimeProdFunc.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include <openfluid/ware/PluggableFunction.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE
  DECLARE_SIGNATURE_ID(("tests.variabletime.prod"));
  DECLARE_SIGNATURE_NAME(("test function for variable time production"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));

  DECLARE_PRODUCED_VAR("tests.double","TestUnits","double for tests","");
  DECLARE_PRODUCED_VAR("tests.string","TestUnits","string for tests","");
  DECLARE_PRODUCED_VAR("tests.null","TestUnits","null for tests","");
  DECLARE_PRODUCED_VAR("tests.vector","TestUnits","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.matrix","TestUnits","matrix for tests","");

END_FUNCTION_SIGNATURE

/**

*/
class VariableTimeProdFunction : public openfluid::ware::PluggableFunction
{
  private:

    bool m_DefaultDT;

  public:


  VariableTimeProdFunction() : PluggableFunction(), m_DefaultDT(false)
  {


  }


  // =====================================================================
  // =====================================================================


  ~VariableTimeProdFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  void initParams(const openfluid::ware::WareParams_t& Params)
  {
    long TmpLong = 0;
    OPENFLUID_GetFunctionParameter(Params,"usedefaultdeltat",&TmpLong);

    m_DefaultDT = TmpLong > 0;

  }

  // =====================================================================
  // =====================================================================


  void prepareData() { }


  // =====================================================================
  // =====================================================================


  void checkConsistency() { }


  // =====================================================================
  // =====================================================================


  openfluid::core::Duration_t initializeRun()
  {
    openfluid::core::Unit* TU;

    openfluid::core::NullValue TheNull;
    long VectorSize = 40;
    openfluid::core::VectorValue TheVector;
    long MatrixColsNb = 4;
    long MatrixRowsNb = 3;
    openfluid::core::MatrixValue TheMatrix;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_InitializeVariable(TU,"tests.double",0.0);
      OPENFLUID_InitializeVariable(TU,"tests.string","empty");

      OPENFLUID_InitializeVariable(TU,"tests.null",TheNull);

      TheVector = openfluid::core::VectorValue(VectorSize,0.0);
      OPENFLUID_InitializeVariable(TU,"tests.vector",TheVector);
      TheMatrix = openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,0.0);
      OPENFLUID_InitializeVariable(TU,"tests.matrix",TheMatrix);
    }
    return DefaultDeltaT();
  }


  // =====================================================================
  // =====================================================================


  openfluid::core::Duration_t computeDeltaT() const
  {
    openfluid::core::Duration_t D = (std::rand() % DefaultDeltaT()) + 1;
    return D;
  }


  // =====================================================================
  // =====================================================================


  openfluid::core::Duration_t runStep()
  {
    openfluid::core::Unit* TU;

    openfluid::core::NullValue TheNull;
    long VectorSize = 40;
    openfluid::core::VectorValue TheVector;
    long MatrixColsNb = 4;
    long MatrixRowsNb = 3;
    openfluid::core::MatrixValue TheMatrix;

    OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
    {
      OPENFLUID_AppendVariable(TU,"tests.double",double(OPENFLUID_GetCurrentTimeIndex()));
      OPENFLUID_AppendVariable(TU,"tests.string",openfluid::core::IntegerValue(OPENFLUID_GetCurrentTimeIndex()).toString());

      OPENFLUID_AppendVariable(TU,"tests.null",TheNull);

      OPENFLUID_AppendVariable(TU,"tests.vector",openfluid::core::VectorValue(VectorSize,double(OPENFLUID_GetCurrentTimeIndex())));
      OPENFLUID_AppendVariable(TU,"tests.matrix",openfluid::core::MatrixValue(MatrixColsNb,MatrixRowsNb,double(OPENFLUID_GetCurrentTimeIndex())));
    }

    if (m_DefaultDT)
      return DefaultDeltaT();
    else
      return computeDeltaT();
  }

  // =====================================================================
  // =====================================================================


  void finalizeRun()
  {


  }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_CLASS(VariableTimeProdFunction)

