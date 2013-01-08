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

#include <glibmm/ustring.h>

#include <openfluid/core.hpp>


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_CLASS(PrimitivesValuesUseFunction)


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE
  DECLARE_SIGNATURE_ID(("tests.primitivesvalues.use"));
  DECLARE_SIGNATURE_NAME(("test function for primitives values (use)"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

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
  DECLARE_REQUIRED_VAR("tests.matrix","TestUnits","matrix for tests","");
  DECLARE_REQUIRED_VAR("tests.map","TestUnits","map for tests","");

  DECLARE_REQUIRED_INPUTDATA("indataDouble","TestUnits","double input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataLong","TestUnits","long input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataBool","TestUnits","boolean input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataString","TestUnits","string input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataVector","TestUnits","vector input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataMatrix","TestUnits","matrix input data for tests","")
  DECLARE_REQUIRED_INPUTDATA("indataMap","TestUnits","map input data for tests","")

  DECLARE_FUNCTION_PARAM ("strparam","=strvalue","-");
  DECLARE_FUNCTION_PARAM ("doubleparam","=0.1","-");
  DECLARE_FUNCTION_PARAM ("longparam","=10","-");
  DECLARE_FUNCTION_PARAM ("boolparam","=true","-");
  DECLARE_FUNCTION_PARAM ("strarrayparam","=strvalue1;strvalue2;strvalue3","-");
  DECLARE_FUNCTION_PARAM ("doublearrayparam","=1.1;1.3;1.3;1.4","-");
  DECLARE_FUNCTION_PARAM ("longarrayparam","=11;12;13;14;15","-");
  DECLARE_FUNCTION_PARAM ("matrixparam","=1.1;1.2|2.1;2.2|3.1;3.2","-");
  DECLARE_FUNCTION_PARAM ("mapparam","=key1=1.1;key2=a string;key3=true","-");

END_FUNCTION_SIGNATURE


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


void PrimitivesValuesUseFunction::initParams(const openfluid::ware::WareParams_t& Params)
{
  openfluid::core::StringValue ParamStrVal;
  openfluid::core::DoubleValue ParamDoubleVal;
  openfluid::core::VectorValue ParamVectorVal;
  openfluid::core::MatrixValue ParamMatrixVal;
  openfluid::core::MapValue ParamMapVal;

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


  // ====== Bool param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"boolparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (boolparam) into StringValue");
  bool aBool = false;
  if(!ParamStrVal.toBoolean(aBool))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (boolparam, type)");
  if(aBool != true)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (boolparam, value)");


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


  // ====== Map param ======

  if (!OPENFLUID_GetFunctionParameter(Params,"mapparam",ParamStrVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam) into StringValue");
  if(!ParamStrVal.toMapValue(";",ParamMapVal))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, type)");
  if(ParamMapVal.size() != 3)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, size)");
  double aDouble = 0.0;
  if(!ParamMapVal["key1"].asStringValue().toDouble(aDouble))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, type for key key1)");
  if(!openfluid::tools::IsCloseEnough(aDouble,1.1))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, value for key key1)");
  if(ParamMapVal.getString("key2") != "a string")
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, value for key key2)");
  aBool = false;
  if(!ParamMapVal["key3"].asStringValue().toBoolean(aBool))
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, type for key key3)");
  if(aBool != true)
    OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetFunctionParameter (mapparam, value for key key3)");
}

// =====================================================================
// =====================================================================


void PrimitivesValuesUseFunction::prepareData()
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
  openfluid::core::MapValue aMap;
  aMap.setDouble("key1",2.1);
  aMap.setString("key2","a string");
  aMap.setBoolean("key3",true);


  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {
    double Dbl = (double)TU->getID() / 10;
    long Lg = TU->getID();
    bool Bl = (TU->getID()%2 == 0);
    std::string Str = Glib::ustring::compose("ID %1",TU->getID());

    OPENFLUID_SetInputData(TU,"indataDouble2",Dbl);
    OPENFLUID_SetInputData(TU,"indataDouble3",openfluid::core::DoubleValue(Dbl));

    OPENFLUID_SetInputData(TU,"indataLong2",Lg);
    OPENFLUID_SetInputData(TU,"indataLong3",openfluid::core::IntegerValue(Lg));

    OPENFLUID_SetInputData(TU,"indataString2",Str);
    OPENFLUID_SetInputData(TU,"indataString3",openfluid::core::StringValue(Str));

    OPENFLUID_SetInputData(TU,"indataBool2",Bl ? "true" : "false");
    OPENFLUID_SetInputData(TU,"indataBool3",openfluid::core::BooleanValue(Bl));

    OPENFLUID_SetInputData(TU,"indataVector2","1.1;1.2;1.3");
    OPENFLUID_SetInputData(TU,"indataVector3",aVector);

    OPENFLUID_SetInputData(TU,"indataMatrix2","0.0;1.0|0.1;1.1|0.2;1.2");
    OPENFLUID_SetInputData(TU,"indataMatrix3",aMatrix);

    OPENFLUID_SetInputData(TU,"indataMap2","key1=2.1;key2=a string;key3=true");
    OPENFLUID_SetInputData(TU,"indataMap3",aMap);

    OPENFLUID_SetInputData(TU,"indataNull2","null");
    OPENFLUID_SetInputData(TU,"indataNull3",openfluid::core::NullValue());

  };
}


// =====================================================================
// =====================================================================


void PrimitivesValuesUseFunction::checkConsistency()
{  }


// =====================================================================
// =====================================================================


openfluid::core::Duration_t PrimitivesValuesUseFunction::initializeRun()
{
  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::core::Duration_t PrimitivesValuesUseFunction::runStep()
{
  openfluid::core::Unit* TU;
  unsigned int TUID;
  unsigned int CurStep;

  double RefDouble;
  long RefLong;
  bool RefBool;
  std::string RefString;
  unsigned long RefVectorSize = 40;
  unsigned long RefMatrixColsNb = 4;
  unsigned long RefMatrixRowsNb = 3;

  double VarDouble;
  long VarLong;
  bool VarBool;
  std::string VarString;
  openfluid::core::DoubleValue VarDoubleVal;
  openfluid::core::IntegerValue VarLongVal;
  openfluid::core::BooleanValue VarBoolVal;
  openfluid::core::StringValue VarStringVal;
  openfluid::core::VectorValue VarVectorVal;
  openfluid::core::MatrixValue VarMatrixVal;
  openfluid::core::MapValue VarMapVal;
  openfluid::core::NullValue VarNullVal;

  double PreDouble;
  long PreLong;
  bool PreBool;
  std::string PreString;

  double NewDouble;
  long NewLong;
  bool NewBool;
  std::string NewString;
  unsigned long NewVectorSize = 5;
  unsigned long NewMatrixColsNb = RefMatrixColsNb + 1;
  unsigned long NewMatrixRowsNb = RefMatrixRowsNb + 1;

  openfluid::core::EventsCollection TheEvents;
  openfluid::core::Event Ev;

  OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",TU)
  {


    TUID = TU->getID();
    CurStep = (OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT());

    RefDouble = (double)TUID/10;
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


    // ====== Variables ======


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

    OPENFLUID_GetVariable(TU,"tests.null",CurStep,&VarNullVal);
    if (VarNullVal.getType() != openfluid::core::Value::NULLL)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect null value (tests.null)");

    OPENFLUID_GetVariable(TU,"tests.null",CurStep,VarNullVal);
    if (VarNullVal.getType() != openfluid::core::Value::NULLL)
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


    // matrix value

    OPENFLUID_GetVariable(TU,"tests.matrix",CurStep,&VarMatrixVal);
    if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix cols nb");
    if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix rows nb");

    VarMatrixVal.clear();
    OPENFLUID_GetVariable(TU,"tests.matrix",CurStep,VarMatrixVal);
    if (VarMatrixVal.getColsNbr() != RefMatrixColsNb)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix cols nb get by reference");
    if (VarMatrixVal.getRowsNbr() != RefMatrixRowsNb)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix rows nb get by reference");

    openfluid::core::MatrixValue NewMatrix(NewMatrixColsNb,NewMatrixRowsNb,NewDouble);
    OPENFLUID_SetVariable(TU,"tests.matrix",CurStep,NewMatrix);

    OPENFLUID_GetVariable(TU,"tests.matrix",CurStep,&VarMatrixVal);
    if (VarMatrixVal.getColsNbr() != NewMatrixColsNb)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix cols nb after update");
    if (VarMatrixVal.getRowsNbr() != NewMatrixRowsNb)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix rows nb after update");

    if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(0,0),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix value at index 0,0 after update");

    if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix value at index 4,3 after update");


    if (CurStep > 0)
    {
      OPENFLUID_GetVariable(TU,"tests.matrix",CurStep-1,&VarMatrixVal);

      if (VarMatrixVal.getSize() != NewMatrixRowsNb*NewMatrixColsNb)
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix size at t-1");

      if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(0,0),PreDouble,0.00001))
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix value at index 0,0 at t-1");

      if (!openfluid::tools::IsCloseEnough(VarMatrixVal.get(4,3),PreDouble,0.00001))
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect matrix value at index 4,3 at t-1");

    }

    if (!OPENFLUID_IsVariableExist(TU,"tests.matrix"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.matrix)");


    // map value

    OPENFLUID_GetVariable(TU,"tests.map",CurStep,&VarMapVal);
    if (VarMapVal.getSize() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map size");

    VarMapVal.clear();
    OPENFLUID_GetVariable(TU,"tests.map",CurStep,VarMapVal);
    if (VarMapVal.getSize() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map size get by reference");

    openfluid::core::MapValue NewMap;
    NewMap.setString("key1",NewString);
    NewMap.setDouble("key2",NewDouble);
    NewMap.setBoolean("key3",NewBool);
    OPENFLUID_SetVariable(TU,"tests.map",CurStep,NewMap);

    OPENFLUID_GetVariable(TU,"tests.map",CurStep,&VarMapVal);
    if (VarMapVal.getSize() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map size after update");

    if (VarMapVal.getString("key1") != NewString)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map value at key key1 after update");

    if (!openfluid::tools::IsCloseEnough(VarMapVal.getDouble("key2"),NewDouble,0.00001))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map value at key key2 after update");

    if (VarMapVal.getBoolean("key3") != NewBool)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map value at key key3 after update");


    if (CurStep > 0)
    {
      OPENFLUID_GetVariable(TU,"tests.map",CurStep-1,&VarMapVal);

      if (VarMapVal.getSize() != 3)
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map size at t-1");

      if (VarMapVal.getString("key1") != PreString)
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map value at key key1 at t-1");

      if (!openfluid::tools::IsCloseEnough(VarMapVal.getDouble("key2"),PreDouble,0.00001))
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map value at key key2 at t-1");

      if (VarMapVal.getBoolean("key3") != PreBool)
        OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect map value at key key3 at t-1");

    }

    if (!OPENFLUID_IsVariableExist(TU,"tests.map"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsVariableExist (tests.map)");



    // ====== Input data ======

    if (!OPENFLUID_IsInputDataExist(TU,"indataDouble"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataDouble2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataDouble3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataDouble3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataLong"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataLong)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataLong2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataLong2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataLong3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataLong3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataBool"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataBool)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataBool2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataBool2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataBool3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataBool3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataString"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataString)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataString2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataString2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataString3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataString3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataVector"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataVector)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataVector2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataVector2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataVector3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataVector3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataMatrix"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMatrix)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataMatrix2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMatrix2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataMatrix3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMatrix3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataMap"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMap)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataMap2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMap2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataMap3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataMap3)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataNull2"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataNull2)");

    if (!OPENFLUID_IsInputDataExist(TU,"indataNull3"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (indataNull3)");

    if (OPENFLUID_IsInputDataExist(TU,"wrongdata"))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_IsInputDataExist (wrongdata)");


    // double

    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble",&VarDouble);
    if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue)");

    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble",VarDouble);
    if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get by reference");

    VarDoubleVal.set(0.0);
    OPENFLUID_GetInputData(TU,"indataDouble",&VarDoubleVal);
    if (!openfluid::tools::IsVeryClose(VarDoubleVal.get(),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with DoubleValue");

    VarDoubleVal.set(0.0);
    OPENFLUID_GetInputData(TU,"indataDouble",VarDoubleVal);
    if (!openfluid::tools::IsVeryClose(VarDoubleVal.get(),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with DoubleValue by reference");

    VarVectorVal.clear();
    OPENFLUID_GetInputData(TU,"indataDouble",&VarVectorVal);
    if (VarVectorVal.size() != 1)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong size) get with VectorValue");
    if (!openfluid::tools::IsVeryClose(VarVectorVal[0],1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with VectorValue");

    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataDouble",VarMatrixVal);
    if (VarMatrixVal.size() != 1)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong size) get with MatrixValue by reference");
    if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with MatrixValue by reference");

    VarStringVal.set("");
    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble",VarStringVal);
    if(!VarStringVal.toDouble(VarDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrong type) get with StringValue by reference");
    if (!openfluid::tools::IsVeryClose(VarDouble,1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble wrongvalue) get with StringValue by reference");

    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble2",VarDouble);
    if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble2 wrongvalue)");

    VarStringVal.set("");
    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble2",VarStringVal);
    if(!VarStringVal.toDouble(VarDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble2 wrong type) get with StringValue by reference");
    if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble2 wrongvalue) get with StringValue by reference");

    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble3",&VarDouble);
    if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble3 wrongvalue)");

    VarStringVal.set("");
    VarDouble = 0.0;
    OPENFLUID_GetInputData(TU,"indataDouble3",VarStringVal);
    if(!VarStringVal.toDouble(VarDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble3 wrong type) get with StringValue by reference");
    if (!openfluid::tools::IsVeryClose(VarDouble,RefDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataDouble3 wrongvalue) get with StringValue by reference");


    // long

    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong",&VarLong);
    if (VarLong != 1)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrongvalue)");

    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong",VarLong);
    if (VarLong != 1)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrongvalue) get by reference");

    VarStringVal.set("");
    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong",VarStringVal);
    if(!VarStringVal.toInteger(VarLong))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrong type) get with StringValue by reference");
    if (VarLong != 1)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong wrongvalue) get with StringValue by reference");

    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong2",&VarLong);
    if (VarLong != RefLong)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong2 wrongvalue)");

    VarStringVal.set("");
    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong2",VarStringVal);
    if(!VarStringVal.toInteger(VarLong))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong2 wrong type) get with StringValue by reference");
    if (VarLong != RefLong)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong2 wrongvalue) get with StringValue by reference");

    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong3",&VarLong);
    if (VarLong != RefLong)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong3 wrongvalue)");

    VarStringVal.set("");
    VarLong = 0;
    OPENFLUID_GetInputData(TU,"indataLong3",VarStringVal);
    if(!VarStringVal.toInteger(VarLong))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong3 wrong type) get with StringValue by reference");
    if (VarLong != RefLong)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataLong3 wrongvalue) get with StringValue by reference");


    // bool

    VarStringVal.set("");
    VarBool = false;
    OPENFLUID_GetInputData(TU,"indataBool",VarStringVal);
    if(!VarStringVal.toBoolean(VarBool))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrong type) get with StringValue by reference");
    if (VarBool != RefBool)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrongvalue) get with StringValue by reference");

    VarStringVal.set("");
    VarBool = false;
    OPENFLUID_GetInputData(TU,"indataBool2",VarStringVal);
    if(!VarStringVal.toBoolean(VarBool))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrong type) get with StringValue by reference");
    if (VarBool != RefBool)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool2 wrongvalue) get with StringValue by reference");

    VarStringVal.set("");
    VarBool = false;
    OPENFLUID_GetInputData(TU,"indataBool3",VarStringVal);
    if(!VarStringVal.toBoolean(VarBool))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool3 wrong type) get with StringValue by reference");
    if (VarBool != RefBool)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataBool3 wrongvalue) get with StringValue by reference");


    //string

    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString",&VarString);
    std::string RefStr;
    openfluid::tools::ConvertValue(TU->getID(),&RefStr);
    RefStr = "CODE"+RefStr;
    if (VarString != RefStr)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString wrongvalue)");

    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString",VarString);
    if (VarString != RefStr)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString wrongvalue) get by reference");

    VarStringVal.set("");
    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString",VarStringVal);
    if (VarStringVal.get() != RefStr)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString wrongvalue) get with StringValue by reference");

    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString2",&VarString);
    if (VarString != RefString)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString2 wrongvalue)");

    VarStringVal.set("");
    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString2",VarStringVal);
    if (VarStringVal.get() != RefString)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString2 wrongvalue) get with StringValue by reference");

    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString3",&VarString);
    if (VarString != RefString)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString3 wrongvalue)");

    VarStringVal.set("");
    VarString = "";
    OPENFLUID_GetInputData(TU,"indataString3",VarStringVal);
    if (VarStringVal.get() != RefString)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataString3 wrongvalue) get with StringValue by reference");


    // vector

    VarStringVal.set("");
    VarVectorVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector",VarStringVal);
    if(!VarStringVal.toVectorValue(";",VarVectorVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong type)");
    if(VarVectorVal.size() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong size)");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[0], TUID + 0.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value)");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[2], TUID + 0.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value)");

    VarStringVal.set("");
    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector",VarStringVal);
    if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong type) get with StringValue by reference");
    if(VarMatrixVal.getColsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong cols nb) get with StringValue by reference");
    if(VarMatrixVal.getRowsNbr() != 1)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong rows nb) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),TUID + 0.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,0),TUID + 0.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector wrong value) get with StringValue by reference");

    VarStringVal.set("");
    VarVectorVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector2",VarStringVal);
    if(!VarStringVal.toVectorValue(";",VarVectorVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong type)");
    if(VarVectorVal.size() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size)");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value)");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value)");

    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector2",VarMatrixVal);
    if (VarMatrixVal.size() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size) get with MatrixValue by reference");
    if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrongvalue) get with MatrixValue by reference");
    if (!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,0),1.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrongvalue) get with MatrixValue by reference");

    VarVectorVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector2",&VarVectorVal);
    if(VarVectorVal.size() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size) get with VectorValue");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value) get with VectorValue");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value) get with VectorValue");

    VarVectorVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector2",VarVectorVal);
    if(VarVectorVal.size() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong size) get with VectorValue by reference");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value) get with VectorValue by reference");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector2 wrong value) get with VectorValue by reference");

    VarStringVal.set("");
    VarVectorVal.clear();
    OPENFLUID_GetInputData(TU,"indataVector3",VarStringVal);
    if(!VarStringVal.toVectorValue(";",VarVectorVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong type) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarVectorVal.size(), 3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong size) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[0], 1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarVectorVal[2], 1.3))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataVector3 wrong value) get with StringValue by reference");

    // matrix

    VarStringVal.set("");
    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataMatrix",VarStringVal);
    if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong type) get with StringValue by reference");
    if(VarMatrixVal.getColsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong cols nb) get with StringValue by reference");
    if(VarMatrixVal.getRowsNbr() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong rows nb) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),TUID * 10))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,1),(TUID * 10)+1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(2,1),(TUID * 10)+1.2))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix wrong value) get with StringValue by reference");

    VarStringVal.set("");
    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataMatrix2",VarStringVal);
    if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong type) get with StringValue by reference");
    if(VarMatrixVal.getColsNbr() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong cols nb) get with StringValue by reference");
    if(VarMatrixVal.getRowsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong rows nb) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong value) get with StringValue by reference");

    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataMatrix2",VarMatrixVal);
    if(VarMatrixVal.getColsNbr() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong cols nb) get with MatrixValue by reference");
    if(VarMatrixVal.getRowsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrong rows nb) get with MatrixValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrongvalue) get with MatrixValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrongvalue) get with MatrixValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix2 wrongvalue) get with MatrixValue by reference");

    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataMatrix3",VarMatrixVal);
    if(VarMatrixVal.getColsNbr() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong cols nb) get with MatrixValue by reference");
    if(VarMatrixVal.getRowsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong rows nb) get with MatrixValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with MatrixValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with MatrixValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with MatrixValue by reference");

    VarStringVal.set("");
    VarMatrixVal.clear();
    OPENFLUID_GetInputData(TU,"indataMatrix3",VarStringVal);
    if(!VarStringVal.toMatrixValue(";","|",VarMatrixVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong type) get with StringValue by reference");
    if(VarMatrixVal.getColsNbr() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong cols nb) get with StringValue by reference");
    if(VarMatrixVal.getRowsNbr() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong rows nb) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,0),0.0))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(0,2),0.2))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarMatrixVal.at(1,1),1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMatrix3 wrong value) get with StringValue by reference");

    // map

    VarStringVal.set("");
    VarMapVal.clear();
    OPENFLUID_GetInputData(TU,"indataMap",VarStringVal);
    if(!VarStringVal.toMapValue(";",VarMapVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap wrong type) get with StringValue by reference");
    if(VarMapVal.getSize() != 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap wrong size) get with StringValue by reference");
    VarDouble = 0.0;
    if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap key1 wrong type) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarDouble,1.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap key1 wrong value) get with StringValue by reference");
    if(VarMapVal.getString("key2") != "aString")
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap key2 wrong value) get with StringValue by reference");

    VarStringVal.set("");
    VarMapVal.clear();
    OPENFLUID_GetInputData(TU,"indataMap2",VarStringVal);
    if(!VarStringVal.toMapValue(";",VarMapVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 wrong type) get with StringValue by reference");
    if(VarMapVal.getSize() != 3)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 wrong size) get with StringValue by reference");
    VarDouble = 0.0;
    if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key1 wrong type) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarDouble,2.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key1 wrong value) get with StringValue by reference");
    if(VarMapVal.getString("key2") != "a string")
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key2 wrong value) get with StringValue by reference");
    VarBool = false;
    if(!VarMapVal["key3"].asStringValue().toBoolean(VarBool))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key3 wrong type) get with StringValue by reference");
    if(VarBool != true)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap2 key3 wrong value) get with StringValue by reference");

    VarStringVal.set("");
    VarMapVal.clear();
    OPENFLUID_GetInputData(TU,"indataMap3",VarStringVal);
    if(!VarStringVal.toMapValue(";",VarMapVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 wrong type) get with StringValue by reference");
    if(VarMapVal.getSize() != 3)
      VarDouble = 0.0;
    if(!VarMapVal["key1"].asStringValue().toDouble(VarDouble))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key1 wrong type) get with StringValue by reference");
    if(!openfluid::tools::IsVeryClose(VarDouble,2.1))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key1 wrong value) get with StringValue by reference");
    if(VarMapVal.getString("key2") != "a string")
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key2 wrong value) get with StringValue by reference");
    VarBool = false;
    if(!VarMapVal["key3"].asStringValue().toBoolean(VarBool))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key3 wrong type) get with StringValue by reference");
    if(VarBool != true)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataMap3 key3 wrong value) get with StringValue by reference");


    // null

    VarStringVal.set("");
    OPENFLUID_GetInputData(TU,"indataNull2",VarStringVal);
    if(!VarStringVal.toNullValue(VarNullVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataNull2 wrong type)");

    VarStringVal.set("");
    OPENFLUID_GetInputData(TU,"indataNull3",VarStringVal);
    if(!VarStringVal.toNullValue(VarNullVal))
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetInputData (indataNull3 wrong type)");



    // ====== Events ======

    TheEvents.clear();

    OPENFLUID_GetEvents(TU,OPENFLUID_GetCurrentDate(),OPENFLUID_GetCurrentDate() + OPENFLUID_GetDefaultDeltaT(),&TheEvents);

    if (TheEvents.getCount() > 2)
      OPENFLUID_RaiseError("tests.primitivesvalues.use","incorrect OPENFLUID_GetEvents");


    Ev = openfluid::core::Event(openfluid::core::DateTime(OPENFLUID_GetCurrentDate()+(OPENFLUID_GetDefaultDeltaT()*2)));
    OPENFLUID_AppendEvent(TU,Ev);


  }


  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


void PrimitivesValuesUseFunction::finalizeRun()
{  }

