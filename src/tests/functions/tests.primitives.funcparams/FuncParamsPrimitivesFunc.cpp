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


#include <openfluid/ware/PluggableFunction.hpp>
#include <openfluid/core.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_FUNC_ID "tests.primitives.funcparams"

BEGIN_FUNCTION_SIGNATURE(THIS_FUNC_ID)

  DECLARE_SIGNATURE_NAME("test function for function parameters primitives");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

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


class FuncParamsPrimitivesFunction : public openfluid::ware::PluggableFunction
{
  private:

    long m_ParamLong;

    double m_ParamDouble;

    std::string m_ParamString;


  public:


    FuncParamsPrimitivesFunction() : PluggableFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    ~FuncParamsPrimitivesFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& Params)
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


    void prepareData() { }


    // =====================================================================
    // =====================================================================


    void checkConsistency() { }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      return Never();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      return Never();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_CLASS(FuncParamsPrimitivesFunction)

