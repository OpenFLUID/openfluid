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
  \file PrimitivesValuesUseFunc.cpp
  \brief Implements ...
*/


#include "PrimitivesValuesUseFunc.h"

#include <openfluid/core/BooleanValue.hpp>
#include <openfluid/core/NullValue.hpp>
#include <openfluid/core/MatrixValue.hpp>

// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(PrimitivesValuesUseFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.primitivesvalues.use"));
  DECLARE_SIGNATURE_NAME(("test function for primitives values (use)"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));


  DECLARE_USED_EVENTS("TestUnits");


  DECLARE_REQUIRED_VAR("tests.double","TestUnits","double for tests","");
  DECLARE_REQUIRED_VAR("tests.integer","TestUnits","integer for tests","");
  DECLARE_REQUIRED_VAR("tests.bool","TestUnits","boolean for tests","");
  DECLARE_REQUIRED_VAR("tests.string","TestUnits","string for tests","");
  DECLARE_REQUIRED_VAR("tests.null","TestUnits","null for tests","");
  DECLARE_REQUIRED_VAR("tests.doubleval","TestUnits","double value for tests","");
  DECLARE_REQUIRED_VAR("tests.integerval","TestUnits","integer value for tests","");
  DECLARE_REQUIRED_VAR("tests.boolval","TestUnits","boolean value for tests","");
  DECLARE_REQUIRED_VAR("tests.stringval","TestUnits","string value for tests","");
  DECLARE_REQUIRED_VAR("tests.vector","TestUnits","vector for tests","");

  DECLARE_REQUIRED_INPUTDATA("indataA","TestUnits","input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataB","TestUnits","input data for tests","")

  DECLARE_FUNCTION_PARAM ("strparam","=strvalue","-");
  DECLARE_FUNCTION_PARAM ("doubleparam","=0.1","-");
  DECLARE_FUNCTION_PARAM ("longparam","=10","-");
  DECLARE_FUNCTION_PARAM ("strarrayparam","=strvalue1;strvalue2;strvalue3","-");
  DECLARE_FUNCTION_PARAM ("doublearrayparam","=1.1;1.3;1.3;1.4","-");
  DECLARE_FUNCTION_PARAM ("longarrayparam","=11;12;13;14;15","-");
  DECLARE_FUNCTION_PARAM ("matrixparam","=1.1;1.2|2.1;2.2|3.1;3.2","-");

END_SIGNATURE_HOOK


// =====================================================================
// =====================================================================


PrimitivesValuesUseFunction::PrimitivesValuesUseFunction()
                : PluggableFunction()
{

}


// =====================================================================
// =====================================================================


PrimitivesValuesUseFunction::~PrimitivesValuesUseFunction()
{


}


// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{

  openfluid::core::DoubleValue ParamDoubleVal;
  openfluid::core::StringValue ParamStrVal;
  openfluid::core::VectorValue ParamVectorVal;
  openfluid::core::MatrixValue ParamMatrixVal;

  std::vector<std::string> ParamStrArray;
  std::vector<long> ParamLongArray;
  std::vector<double> ParamDoubleArray;


  // ====== String param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"strparam",&m_ParamString))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam)");
  if(m_ParamString != "strvalue")
     OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam, value)");

  m_ParamString = "";
  if (!OPENFLUID_GetFunctionParameter(Params,"strparam",m_ParamString))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam) get by reference");
  if(m_ParamString != "strvalue")
     OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam, value) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrparam",&m_ParamString))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"strparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam) into DoubleValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"strparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam) into StringValue");
  if(ParamStrVal.get() != "strvalue")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam, value) into StringValue");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrparam)  into StringValue");

  if (OPENFLUID_GetFunctionParameter(Params,"strparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strparam) into VectorValue");


  // ====== Double param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",&m_ParamDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam)");
  if(!openfluid::tools::IsCloseEnough(m_ParamDouble,0.1,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam, value)");

  m_ParamDouble = 0.0;
  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",m_ParamDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam) get by reference");
  if(!openfluid::tools::IsCloseEnough(m_ParamDouble,0.1,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam, value) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoubleparam",&m_ParamDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoubleparam)");

  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam) into DoubleValue");

  if(!openfluid::tools::IsCloseEnough(ParamDoubleVal.get(),0.1,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam, value) into DoubleValue");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoubleparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoubleparam)  into DoubleValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam) into StringValue");
  if(ParamStrVal.get() != "0.1")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam, value) into StringValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"doubleparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam) into VectorValue");
  if(ParamVectorVal.size() != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam, size) into VectorValue");
  if(!openfluid::tools::IsCloseEnough(ParamVectorVal[0],0.1,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doubleparam, value) into VectorValue");


  // ====== Long param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",&m_ParamLong))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam)");
  if(m_ParamLong != 10)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam, value)");

  m_ParamLong = 0;
  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",m_ParamLong))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam) get by reference");
  if(m_ParamLong != 10)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam, value) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wronglongparam",&m_ParamLong))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wronglongparam)");

  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam) into DoubleValue");
  if(!openfluid::tools::IsCloseEnough(ParamDoubleVal.get(),10,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam, value) into DoubleValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam) into StringValue");
  if(ParamStrVal.get() != "10")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam, value) into StringValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"longparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam) into VectorValue");
  if(ParamVectorVal.size() != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam, size) into VectorValue");
  if(!openfluid::tools::IsCloseEnough(ParamVectorVal[0],10,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longparam, value) into VectorValue");


  // ====== String array param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"strarrayparam",&ParamStrArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam)");
  if (ParamStrArray.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, size)");
  if (ParamStrArray[1] != "strvalue2")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, value)");

  ParamStrArray.clear();
  if (!OPENFLUID_GetFunctionParameter(Params,"strarrayparam",ParamStrArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam) get by reference");
  if (ParamStrArray.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, size) get by reference");
  if (ParamStrArray[1] != "strvalue2")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, value) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongstrarrayparam",&ParamStrArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongstrarrayparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"strarrayparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam) into DoubleValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"strarrayparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam) into StringValue");
  if(ParamStrVal.get() != "strvalue1;strvalue2;strvalue3")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam, value) into StringValue");

  if (OPENFLUID_GetFunctionParameter(Params,"strarrayparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (strarrayparam) into VectorValue");


  // ====== Double array param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",&ParamDoubleArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam)");
  if (ParamDoubleArray.size() != 4)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, size)");
  if (!openfluid::tools::IsCloseEnough(ParamDoubleArray[2],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, value)");
  if (openfluid::tools::IsCloseEnough(ParamDoubleArray[3],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, wrongvalue)");

  ParamDoubleArray.clear();
  if (!OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",ParamDoubleArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam) get by reference");
  if (ParamDoubleArray.size() != 4)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, size) get by reference");
  if (!openfluid::tools::IsCloseEnough(ParamDoubleArray[2],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, value) get by reference");
  if (openfluid::tools::IsCloseEnough(ParamDoubleArray[3],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, wrongvalue) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongdoublearrayparam",&ParamDoubleArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongdoublearrayparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam) into DoubleValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam) into StringValue");
  if(ParamStrVal.get() != "1.1;1.3;1.3;1.4")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, value) into StringValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"doublearrayparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam) into VectorValue");
  if(ParamVectorVal.size() != 4)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, size) into VectorValue");
  if(!openfluid::tools::IsCloseEnough(ParamVectorVal[2],1.3,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (doublearrayparam, value) into VectorValue");


  // ====== Long array param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"longarrayparam",&ParamLongArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam)");
  if (ParamLongArray.size() != 5)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, size)");
  if (ParamLongArray[3] != 14)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, value)");
  if (ParamLongArray[4] == 14)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, wrongvalue)");

  ParamLongArray.clear();
  if (!OPENFLUID_GetFunctionParameter(Params,"longarrayparam",ParamLongArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam) get by reference");
  if (ParamLongArray.size() != 5)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, size) get by reference");
  if (ParamLongArray[3] != 14)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, value) get by reference");
  if (ParamLongArray[4] == 14)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, wrongvalue) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wronglongarrayparam",&ParamLongArray))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wronglongarrayparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"longarrayparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam) into DoubleValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"longarrayparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam) into StringValue");
  if(ParamStrVal.get() != "11;12;13;14;15")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, value) into StringValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"longarrayparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam) into VectorValue");
  if(ParamVectorVal.size() != 5)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, size) into VectorValue");
  if(!openfluid::tools::IsCloseEnough(ParamVectorVal[3],14,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (longarrayparam, value) into VectorValue");

  // ====== Matrix param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"matrixparam",ParamMatrixVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam) get by reference");
  if (ParamMatrixVal.getColsNbr() != 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam, cols nb) get by reference");
  if (ParamMatrixVal.getRowsNbr() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam, rows nb) get by reference");
  if (!openfluid::tools::IsCloseEnough(ParamMatrixVal.get(0,0),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam, value) get by reference");
  if (!openfluid::tools::IsCloseEnough(ParamMatrixVal.get(0,1),2.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam, wrongvalue) get by reference");
  if (!openfluid::tools::IsCloseEnough(ParamMatrixVal.get(1,2),3.2))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam, wrongvalue) get by reference");

  if (OPENFLUID_GetFunctionParameter(Params,"wrongmatrixparam",ParamMatrixVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (wrongmatrixparam)");

  if (OPENFLUID_GetFunctionParameter(Params,"matrixparam",ParamDoubleVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam) into DoubleValue");

  if (OPENFLUID_GetFunctionParameter(Params,"matrixparam",ParamVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam) into VectorValue");

  if (!OPENFLUID_GetFunctionParameter(Params,"matrixparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam) into StringValue");
  if(ParamStrVal.get() != "1.1;1.2|2.1;2.2|3.1;3.2")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (matrixparam, value) into StringValue");


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::prepareData()
{
  openfluid::core::Unit* TU;
  openfluid::core::VectorValue aVector(3);
  aVector.set(0,1.1);
  aVector.set(1,1.2);
  aVector.set(2,1.3);
  openfluid::core::MatrixValue aMatrix(2,3);
  aMatrix.set(0,0,0.0);
  aMatrix.set(0,1,0.1);
  aMatrix.set(0,2,0.2);
  aMatrix.set(1,0,1.0);
  aMatrix.set(1,1,1.1);
  aMatrix.set(1,2,1.2);

  DECLARE_UNITS_ORDERED_LOOP(1);

  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU);

    double Dbl = TU->getID() / 10;
    OPENFLUID_SetInputData(TU,"indataA2",Dbl);
    OPENFLUID_SetInputData(TU,"indataA3",openfluid::core::DoubleValue(Dbl));

    std::string Str = Glib::ustring::compose("ID %1",TU->getID());
    OPENFLUID_SetInputData(TU,"indataB2",Str);
    OPENFLUID_SetInputData(TU,"indataB3",openfluid::core::StringValue(Str));

    long Lg = TU->getID();
    OPENFLUID_SetInputData(TU,"indataC2",Lg);
    OPENFLUID_SetInputData(TU,"indataC3",openfluid::core::IntegerValue(Lg));

    bool Bl = (TU->getID()%2 == 0);
    OPENFLUID_SetInputData(TU,"indataD2",Bl ? "true" : "false");
    OPENFLUID_SetInputData(TU,"indataD3",openfluid::core::BooleanValue(Bl));

    OPENFLUID_SetInputData(TU,"indataE2","null");
    OPENFLUID_SetInputData(TU,"indataE3",openfluid::core::NullValue());

    OPENFLUID_SetInputData(TU,"indataF2","1.1;1.2;1.3");
    OPENFLUID_SetInputData(TU,"indataF3",aVector);

    OPENFLUID_SetInputData(TU,"indataG2","0.0;1.0|0.1;1.1|0.2;1.2");
    OPENFLUID_SetInputData(TU,"indataG3",aMatrix);

  END_LOOP;

  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::initializeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  openfluid::core::Unit* TU;
  unsigned int TUID;
  unsigned int CurStep;

  double RefDouble;
  long RefLong;
  bool RefBool;
  std::string RefString;
  unsigned long RefVectorSize = 40;

  double VarDouble;
  long VarLong;
  bool VarBool;
  std::string VarString;
  openfluid::core::DoubleValue VarDoubleVal;
  openfluid::core::IntegerValue VarLongVal;
  openfluid::core::BooleanValue VarBoolVal;
  openfluid::core::StringValue VarStringVal;
  openfluid::core::VectorValue VarVectorVal;

  double PreDouble;
  long PreLong;
  bool PreBool;
  std::string PreString;

  double NewDouble;
  long NewLong;
  bool NewBool;
  std::string NewString;
  openfluid::core::NullValue NewNullVal;
  unsigned long NewVectorSize = 5;

  double InputDouble;
  long InputLong;
  std::string InputString;
  bool InputBool;
  openfluid::core::StringValue InputStringVal;
  openfluid::core::DoubleValue InputDoubleVal;
  openfluid::core::NullValue InputNullVal;
  openfluid::core::VectorValue InputVectorVal;
  openfluid::core::MatrixValue InputMatrixVal;

  openfluid::core::EventsCollection TheEvents;
  openfluid::core::Event Ev;

  DECLARE_UNITS_ORDERED_LOOP(1);


  BEGIN_UNITS_ORDERED_LOOP(1,"TestUnits",TU)


  // ====== Variables ======

  TUID = TU->getID();
  CurStep = SimStatus->getCurrentStep();

  RefDouble = TUID/10;
  RefLong = TUID;
  RefBool = (TUID%2 == 0);
  RefString = Glib::ustring::compose("ID %1",TUID);

  PreDouble = TUID*(CurStep-1)*m_ParamDouble;
  PreLong = TUID*(CurStep-1)*m_ParamLong;
  PreBool = ((TUID*(CurStep-1))%2 == 0);
  PreString = Glib::ustring::compose("%1 %2x%3",m_ParamString,TUID,(CurStep-1));

  NewDouble = TUID*CurStep*m_ParamDouble;
  NewLong = TUID*CurStep*m_ParamLong;
  NewBool = ((TUID*CurStep)%2 == 0);
  NewString = Glib::ustring::compose("%1 %2x%3",m_ParamString,TUID,CurStep);



  // double

  OPENFLUID_GetVariable(TU,"tests.double",CurStep,&VarDouble);
  if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value (tests.double)");

  VarDouble = 0.0;
  OPENFLUID_GetVariable(TU,"tests.double",CurStep,VarDouble);
  if (!openfluid::tools::IsCloseEnough(VarDouble,RefDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value (tests.double) get by reference");

  OPENFLUID_SetVariable(TU,"tests.double",CurStep,NewDouble);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.double",CurStep-1,&VarDouble);
    if (!openfluid::tools::IsCloseEnough(VarDouble,PreDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value at t-1 (tests.double)");
  }

  OPENFLUID_GetVariable(TU,"tests.double",CurStep,&VarDouble);

  if (!openfluid::tools::IsCloseEnough(VarDouble,NewDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value after update (tests.double)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.double"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.double)");


  // double value

  OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep,&VarDoubleVal);
  if (VarDoubleVal.get() != RefDouble)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value (tests.doubleval)");

  VarDoubleVal.set(0.0);
  OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep,VarDoubleVal);
  if (VarDoubleVal.get() != RefDouble)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value (tests.doubleval) get by reference");

  OPENFLUID_SetVariable(TU,"tests.doubleval",CurStep,NewDouble);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep-1,&VarDoubleVal);
    if (!openfluid::tools::IsCloseEnough(VarDoubleVal.get(),PreDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value at t-1 (tests.doubleval)");
  }

  OPENFLUID_GetVariable(TU,"tests.doubleval",CurStep,&VarDoubleVal);

  if (!openfluid::tools::IsCloseEnough(VarDoubleVal.get(),NewDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect double value after update (tests.doubleval)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.doubleval"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.doubleval)");


  // long

  OPENFLUID_GetVariable(TU,"tests.integer",CurStep,&VarLong);
  if (VarLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integer)");

  VarLong = 0;
  OPENFLUID_GetVariable(TU,"tests.integer",CurStep,VarLong);
  if (VarLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integer) get by reference");

  OPENFLUID_GetVariable(TU,"tests.integer",CurStep,&VarLong);
  if (VarLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integer)");

  OPENFLUID_SetVariable(TU,"tests.integer",CurStep,NewLong);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.integer",CurStep-1,&VarLong);
    if (VarLong != PreLong)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value at t-1 (tests.integer)");
  }

  OPENFLUID_GetVariable(TU,"tests.integer",CurStep,&VarLong);

  if (VarLong != NewLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value after update (tests.integer)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.integer"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.integer)");


  // long value

  OPENFLUID_GetVariable(TU,"tests.integerval",CurStep,&VarLongVal);
  if (VarLongVal.get() != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integerval)");

  VarLongVal.set(0);
  OPENFLUID_GetVariable(TU,"tests.integerval",CurStep,VarLongVal);
  if (VarLongVal.get() != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value (tests.integerval) get by reference");
  OPENFLUID_SetVariable(TU,"tests.integerval",CurStep,NewLong);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.integerval",CurStep-1,&VarLongVal);
    if (VarLongVal.get() != PreLong)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value at t-1 (tests.integerval)");
  }

  OPENFLUID_GetVariable(TU,"tests.integerval",CurStep,&VarLongVal);

  if (VarLongVal.get() != NewLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect long value after update (tests.integerval)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.integerval"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.integerval)");


  // bool

  OPENFLUID_GetVariable(TU,"tests.bool",CurStep,&VarBool);
  if (VarBool != RefBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value (tests.bool)");

  VarBool = false;
  OPENFLUID_GetVariable(TU,"tests.bool",CurStep,VarBool);
  if (VarBool != RefBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value (tests.bool) get by reference");

  OPENFLUID_SetVariable(TU,"tests.bool",CurStep,NewBool);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.bool",CurStep-1,&VarBool);
    if (VarBool != PreBool)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value at t-1 (tests.bool)");
  }

  OPENFLUID_GetVariable(TU,"tests.bool",CurStep,&VarBool);

  if (VarBool != NewBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value after update (tests.bool)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.bool"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.bool)");


  // bool value

  OPENFLUID_GetVariable(TU,"tests.boolval",CurStep,&VarBoolVal);
  if (VarBoolVal.get() != RefBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value (tests.boolval)");

  VarBoolVal.set(false);
  OPENFLUID_GetVariable(TU,"tests.boolval",CurStep,VarBoolVal);
  if (VarBoolVal.get() != RefBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value (tests.boolval) get by reference");

  OPENFLUID_SetVariable(TU,"tests.boolval",CurStep,NewBool);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.boolval",CurStep-1,&VarBoolVal);
    if (VarBoolVal.get() != PreBool)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value at t-1 (tests.boolval)");
  }

  OPENFLUID_GetVariable(TU,"tests.boolval",CurStep,&VarBoolVal);

  if (VarBoolVal.get() != NewBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect bool value after update (tests.boolval)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.boolval"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.boolval)");


  // string

  OPENFLUID_GetVariable(TU,"tests.string",CurStep,&VarString);
  if (VarString != RefString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value (tests.string)");

  VarString = "";
  OPENFLUID_GetVariable(TU,"tests.string",CurStep,VarString);
  if (VarString != RefString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value (tests.string) get by reference");

  OPENFLUID_SetVariable(TU,"tests.string",CurStep,NewString);

  if (CurStep > 0 )
  {
    OPENFLUID_GetVariable(TU,"tests.string",CurStep-1,&VarString);
    if (VarString != PreString)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value at t-1 (tests.string)");
  }

  OPENFLUID_GetVariable(TU,"tests.string",CurStep,&VarString);

  if (VarString != NewString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect string value after update (tests.string)");


  if (!OPENFLUID_IsVariableExist(TU,"tests.string"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.string)");


  // null value

  OPENFLUID_GetVariable(TU,"tests.null",CurStep,&NewNullVal);
  if (NewNullVal.getType() != openfluid::core::Value::NULLL)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect null value (tests.null)");

  OPENFLUID_GetVariable(TU,"tests.null",CurStep,NewNullVal);
    if (NewNullVal.getType() != openfluid::core::Value::NULLL)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect null value (tests.null) get by reference");


  // vector value

  OPENFLUID_GetVariable(TU,"tests.vector",CurStep,&VarVectorVal);
  if (VarVectorVal.getSize() != RefVectorSize)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size");

  VarVectorVal.clear();
  OPENFLUID_GetVariable(TU,"tests.vector",CurStep,VarVectorVal);
    if (VarVectorVal.getSize() != RefVectorSize)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size get by reference");

  openfluid::core::VectorValue NewVect(NewVectorSize,NewDouble);
  OPENFLUID_SetVariable(TU,"tests.vector",CurStep,NewVect);

  OPENFLUID_GetVariable(TU,"tests.vector",CurStep,&VarVectorVal);
  if (VarVectorVal.getSize() != NewVectorSize)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size after update");

  if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],NewDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 0 after update");

  if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],NewDouble,0.00001))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 4 after update");


  if (CurStep > 0)
  {
    OPENFLUID_GetVariable(TU,"tests.vector",CurStep-1,&VarVectorVal);

    if (VarVectorVal.getSize() != NewVectorSize)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector size at t-1");

    if (!openfluid::tools::IsCloseEnough(VarVectorVal[0],PreDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 0 at t-1");

    if (!openfluid::tools::IsCloseEnough(VarVectorVal[4],PreDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect vector value at index 4 at t-1");

  }

  if (!OPENFLUID_IsVariableExist(TU,"tests.vector"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.vector)");



  // ====== Input data ======

  if (!OPENFLUID_IsInputDataExist(TU,"indataA"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataA)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataA2"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataA2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataA3"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataA3)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataB"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataB)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataB2"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataB2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataB3"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataB3)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataC"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataC)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataC2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataC2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataC3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataC3)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataD2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataD2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataD3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataD3)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataE2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataE2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataE3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataE3)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataF"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataF)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataF2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataF2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataF3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataF3)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataG"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataG)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataG2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataG2)");

  if (!OPENFLUID_IsInputDataExist(TU,"indataG3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataG3)");

  if (OPENFLUID_IsInputDataExist(TU,"wrongdata"))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (wrongdata)");


  // double

  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA",&InputDouble);
  if (!openfluid::tools::IsVeryClose(InputDouble,1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue)");

  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA",InputDouble);
  if (!openfluid::tools::IsVeryClose(InputDouble,1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue) get by reference");

  InputDoubleVal.set(0.0);
  OPENFLUID_GetInputData(TU,"indataA",&InputDoubleVal);
  if (!openfluid::tools::IsVeryClose(InputDoubleVal.get(),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue) get with DoubleValue");

  InputDoubleVal.set(0.0);
  OPENFLUID_GetInputData(TU,"indataA",InputDoubleVal);
  if (!openfluid::tools::IsVeryClose(InputDoubleVal.get(),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue) get with DoubleValue by reference");

  InputVectorVal.clear();
  OPENFLUID_GetInputData(TU,"indataA",&InputVectorVal);
  if (InputVectorVal.size() != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrong size) get with VectorValue");
  if (!openfluid::tools::IsVeryClose(InputVectorVal[0],1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue) get with VectorValue");

  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataA",InputMatrixVal);
  if (InputMatrixVal.size() != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrong size) get with MatrixValue by reference");
  if (!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue) get with MatrixValue by reference");

  InputStringVal.set("");
  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA",InputStringVal);
  if(!InputStringVal.toDouble(InputDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrong type)");
  if (!openfluid::tools::IsVeryClose(InputDouble,1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue)");

  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA2",&InputDouble);
  if (!openfluid::tools::IsVeryClose(InputDouble,RefDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA2 wrongvalue)");

  InputStringVal.set("");
  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA2",InputStringVal);
  if(!InputStringVal.toDouble(InputDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA2 wrong type)");
  if (!openfluid::tools::IsVeryClose(InputDouble,RefDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA2 wrongvalue)");

  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA3",&InputDouble);
  if (!openfluid::tools::IsVeryClose(InputDouble,RefDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA3 wrongvalue)");

  InputStringVal.set("");
  InputDouble = 0.0;
  OPENFLUID_GetInputData(TU,"indataA3",InputStringVal);
  if(!InputStringVal.toDouble(InputDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA3 wrong type)");
  if (!openfluid::tools::IsVeryClose(InputDouble,RefDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataA wrongvalue)");


  //string

  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB",&InputString);
  std::string RefStr;
  openfluid::tools::ConvertValue(TU->getID(),&RefStr);
  RefStr = "CODE"+RefStr;
  if (InputString != RefStr)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB wrongvalue)");

  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB",InputString);
  if (InputString != RefStr)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB wrongvalue) get by reference");

  InputStringVal.set("");
  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB",InputStringVal);
  if (InputStringVal.get() != RefStr)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB wrongvalue)");

  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB2",&InputString);
  if (InputString != RefString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB2 wrongvalue)");

  InputStringVal.set("");
  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB2",InputStringVal);
  if (InputStringVal.get() != RefString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB2 wrongvalue)");

  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB3",&InputString);
  if (InputString != RefString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB3 wrongvalue)");

  InputStringVal.set("");
  InputString = "";
  OPENFLUID_GetInputData(TU,"indataB3",InputStringVal);
  if (InputStringVal.get() != RefString)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataB3 wrongvalue)");


  // long

  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC",&InputLong);
  if (InputLong != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC wrongvalue)");

  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC",InputLong);
  if (InputLong != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC wrongvalue) get by reference");

  InputStringVal.set("");
  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC",InputStringVal);
  if(!InputStringVal.toInteger(InputLong))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC wrong type)");
  if (InputLong != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC wrongvalue)");

  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC2",&InputLong);
  if (InputLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC2 wrongvalue)");

  InputStringVal.set("");
  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC2",InputStringVal);
  if(!InputStringVal.toInteger(InputLong))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC2 wrong type)");
  if (InputLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC2 wrongvalue)");

  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC3",&InputLong);
  if (InputLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC3 wrongvalue)");

  InputStringVal.set("");
  InputLong = 0;
  OPENFLUID_GetInputData(TU,"indataC3",InputStringVal);
  if(!InputStringVal.toInteger(InputLong))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC3 wrong type)");
  if (InputLong != RefLong)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataC3 wrongvalue)");

  
  // bool
  
  InputStringVal.set("");
  InputBool = false;
  OPENFLUID_GetInputData(TU,"indataD2",InputStringVal);
  if(!InputStringVal.toBoolean(InputBool))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataD2 wrong type)");
  if (InputBool != RefBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataD2 wrongvalue)");

  InputStringVal.set("");
  InputBool = false;
  OPENFLUID_GetInputData(TU,"indataD3",InputStringVal);
  if(!InputStringVal.toBoolean(InputBool))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataD3 wrong type)");
  if (InputBool != RefBool)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataD3 wrongvalue)");

  
  // null

  InputStringVal.set("");
  OPENFLUID_GetInputData(TU,"indataE2",InputStringVal);
  if(!InputStringVal.toNullValue(InputNullVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataE2 wrong type)");

  InputStringVal.set("");
  OPENFLUID_GetInputData(TU,"indataE3",InputStringVal);
  if(!InputStringVal.toNullValue(InputNullVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataE3 wrong type)");


  // vector

  InputStringVal.set("");
  InputVectorVal.clear();
  OPENFLUID_GetInputData(TU,"indataF",InputStringVal);
  if(!InputStringVal.toVectorValue(";",InputVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong type)");
  if(InputVectorVal.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong size)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[0], TUID + 0.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong value)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[2], TUID + 0.3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong value)");

  InputStringVal.set("");
  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataF",InputStringVal);
  if(!InputStringVal.toMatrixValue(";","|",InputMatrixVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong type) get with MatrixValue");
  if(InputMatrixVal.getColsNbr() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong cols nb) get with MatrixValue");
  if(InputMatrixVal.getRowsNbr() != 1)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong rows nb) get with MatrixValue");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),TUID + 0.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong value) get with MatrixValue");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(2,0),TUID + 0.3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF wrong value) get with MatrixValue");

  InputStringVal.set("");
  InputVectorVal.clear();
  OPENFLUID_GetInputData(TU,"indataF2",InputStringVal);
  if(!InputStringVal.toVectorValue(";",InputVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong type)");
  if(InputVectorVal.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong size)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[0], 1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[2], 1.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong value)");

  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataF2",InputMatrixVal);
  if (InputMatrixVal.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong size) get with MatrixValue by reference");
  if (!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrongvalue) get with MatrixValue by reference");
  if (!openfluid::tools::IsVeryClose(InputMatrixVal.at(2,0),1.3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrongvalue) get with MatrixValue by reference");

  InputVectorVal.clear();
  OPENFLUID_GetInputData(TU,"indataF2",&InputVectorVal);
  if(InputVectorVal.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong size) get with VectorValue");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[0], 1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong value) get with VectorValue");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[2], 1.3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong value) get with VectorValue");

  InputVectorVal.clear();
  OPENFLUID_GetInputData(TU,"indataF2",InputVectorVal);
  if(InputVectorVal.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong size) get with VectorValue by reference");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[0], 1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong value) get with VectorValue by reference");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[2], 1.3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF2 wrong value) get with VectorValue by reference");

  InputStringVal.set("");
  InputVectorVal.clear();
  OPENFLUID_GetInputData(TU,"indataF3",InputStringVal);
  if(!InputStringVal.toVectorValue(";",InputVectorVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF3 wrong type)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal.size(), 3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF3 wrong size)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[0], 1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF3 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputVectorVal[2], 1.3))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataF3 wrong value)");

  // matrix

  InputStringVal.set("");
  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataG",InputStringVal);
  if(!InputStringVal.toMatrixValue(";","|",InputMatrixVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG wrong type)");
  if(InputMatrixVal.getColsNbr() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG wrong cols nb)");
  if(InputMatrixVal.getRowsNbr() != 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG wrong rows nb)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),TUID * 10))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,1),(TUID * 10)+1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(2,1),(TUID * 10)+1.2))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG wrong value)");

  InputStringVal.set("");
  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataG2",InputStringVal);
  if(!InputStringVal.toMatrixValue(";","|",InputMatrixVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong type)");
  if(InputMatrixVal.getColsNbr() != 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong cols nb)");
  if(InputMatrixVal.getRowsNbr() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong rows nb)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),0.0))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,2),0.2))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(1,1),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong value)");

  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataG2",InputMatrixVal);
  if(InputMatrixVal.getColsNbr() != 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong cols nb) get with MatrixValue by reference");
  if(InputMatrixVal.getRowsNbr() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrong rows nb) get with MatrixValue by reference");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),0.0))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrongvalue) get with MatrixValue by reference");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,2),0.2))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrongvalue) get with MatrixValue by reference");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(1,1),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG2 wrongvalue) get with MatrixValue by reference");

  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataG3",InputMatrixVal);
  if(InputMatrixVal.getColsNbr() != 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong cols nb)");
  if(InputMatrixVal.getRowsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong rows nb)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),0.0))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,2),0.2))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(1,1),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong value)");

  InputStringVal.set("");
  InputMatrixVal.clear();
  OPENFLUID_GetInputData(TU,"indataG3",InputStringVal);
  if(!InputStringVal.toMatrixValue(";","|",InputMatrixVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong type)");
  if(InputMatrixVal.getColsNbr() != 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong cols nb)");
  if(InputMatrixVal.getRowsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong rows nb)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,0),0.0))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(0,2),0.2))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong value)");
  if(!openfluid::tools::IsVeryClose(InputMatrixVal.at(1,1),1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataG3 wrong value)");



  // ====== Events ======

  TheEvents.clear();

  OPENFLUID_GetEvents(TU,SimStatus->getCurrentTime(),SimStatus->getCurrentTime() + SimStatus->getTimeStep(),&TheEvents);

  if (TheEvents.getCount() > 2)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetEvents");


  Ev = openfluid::core::Event(openfluid::core::DateTime(SimStatus->getCurrentTime()+(SimStatus->getTimeStep()*2)));
  OPENFLUID_AppendEvent(TU,Ev);


  END_LOOP


  return true;
}

// =====================================================================
// =====================================================================


bool PrimitivesValuesUseFunction::finalizeRun(const openfluid::base::SimulationInfo* /*SimInfo*/)
{
  return true;
}

