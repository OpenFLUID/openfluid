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
  @file SimParamsPrimitivesSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/core/MapValue.hpp>


class SimParamsPrimitivesSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    long m_ParamLong;

    double m_ParamDouble;

    std::string m_ParamString;


  public:


    SimParamsPrimitivesSimulator() : PluggableSimulator(),m_ParamLong(0),m_ParamDouble(0.0)
    {


    }


    // =====================================================================
    // =====================================================================


    ~SimParamsPrimitivesSimulator()
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


      if (!OPENFLUID_IsWareParameterExist(Params,"strparam"))
      {
        OPENFLUID_RaiseError("missing parameter (strparam)");
      }

      if (OPENFLUID_IsWareParameterExist(Params,"fakestrparam"))
      {
        OPENFLUID_RaiseError("unexpected parameter (fakestrparam)");
      }


      // ====== String param ======

      m_ParamString = "";
      if (!OPENFLUID_GetWareParameter(Params,"strparam",m_ParamString))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam) get by reference");
      }
      if (m_ParamString != "strvalue")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam, value) get by reference");
      }

      m_ParamString = OPENFLUID_GetWareParameter(Params,"strparam");
      if (m_ParamString != "strvalue")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam, value) get by return");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongstrparam",m_ParamString))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongstrparam)");
      }

      if (OPENFLUID_GetWareParameter(Params,"strparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam) into DoubleValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"strparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam) into StringValue");
      }
      if(ParamStrVal.get() != "strvalue")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam, value) into StringValue");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongstrparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongstrparam)  into StringValue");
      }

      if (OPENFLUID_GetWareParameter(Params,"strparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strparam) into VectorValue");
      }


      // ====== Double param ======

      m_ParamDouble = 0.0;
      if (!OPENFLUID_GetWareParameter(Params,"doubleparam",m_ParamDouble))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam) get by reference");
      }
      if(!openfluid::scientific::isCloseEnough(m_ParamDouble,0.1,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam, value) get by reference");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongdoubleparam",m_ParamDouble))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongdoubleparam)");
      }

      if (!OPENFLUID_GetWareParameter(Params,"doubleparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam) into DoubleValue");
      }

      if(!openfluid::scientific::isCloseEnough(ParamDoubleVal.get(),0.1,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam, value) into DoubleValue");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongdoubleparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongdoubleparam)  into DoubleValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"doubleparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam) into StringValue");
      }
      if(ParamStrVal.get() != "0.1")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam, value) into StringValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"doubleparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam) into VectorValue");
      }
      if(ParamVectorVal.size() != 1)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam, size) into VectorValue");
      }
      if(!openfluid::scientific::isCloseEnough(ParamVectorVal[0],0.1,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doubleparam, value) into VectorValue");
      }


      // ====== Long param ======

      m_ParamLong = 0;
      if (!OPENFLUID_GetWareParameter(Params,"longparam",m_ParamLong))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam) get by reference");
      }
      if(m_ParamLong != 10)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam, value) get by reference");
      }

      if (OPENFLUID_GetWareParameter(Params,"wronglongparam",m_ParamLong))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wronglongparam)");
      }

      if (!OPENFLUID_GetWareParameter(Params,"longparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam) into DoubleValue");
      }
      if(!openfluid::scientific::isCloseEnough<double>(ParamDoubleVal.get(),10,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam, value) into DoubleValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"longparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam) into StringValue");
      }
      if(ParamStrVal.get() != "10")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam, value) into StringValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"longparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam) into VectorValue");
      }
      if(ParamVectorVal.size() != 1)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam, size) into VectorValue");
      }
      if(!openfluid::scientific::isCloseEnough<double>(ParamVectorVal[0],10,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longparam, value) into VectorValue");
      }


      // ====== Bool param ======

      bool aBool = false;
      if (!OPENFLUID_GetWareParameter(Params,"boolparam",aBool))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (boolparam) get by reference");
      }
      if(aBool != true)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (boolparam, value) get by reference");
      }


      // ====== String array param ======

      ParamStrArray.clear();
      if (!OPENFLUID_GetWareParameter(Params,"strarrayparam",ParamStrArray))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam) get by reference");
      }
      if (ParamStrArray.size() != 3)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam, size) get by reference");
      }
      if (ParamStrArray[1] != "strvalue2")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam, value) get by reference");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongstrarrayparam",ParamStrArray))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongstrarrayparam)");
      }

      if (OPENFLUID_GetWareParameter(Params,"strarrayparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam) into DoubleValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"strarrayparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam) into StringValue");
      }
      if(ParamStrVal.get() != "strvalue1;strvalue2;strvalue3")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam, value) into StringValue");
      }

      if (OPENFLUID_GetWareParameter(Params,"strarrayparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (strarrayparam) into VectorValue");
      }


      // ====== Double array param ======

      ParamDoubleArray.clear();
      if (!OPENFLUID_GetWareParameter(Params,"doublearrayparam",ParamDoubleArray))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam) get by reference");
      }
      if (ParamDoubleArray.size() != 4)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam, size) get by reference");
      }
      if (!openfluid::scientific::isCloseEnough(ParamDoubleArray[2],1.3,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam, value) get by reference");
      }
      if (openfluid::scientific::isCloseEnough(ParamDoubleArray[3],1.3,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam, wrongvalue) "
                             "get by reference");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongdoublearrayparam",ParamDoubleArray))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongdoublearrayparam)");
      }

      if (OPENFLUID_GetWareParameter(Params,"doublearrayparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam) into DoubleValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"doublearrayparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam) into StringValue");
      }
      if(ParamStrVal.get() != "1.1;1.3;1.3;1.4")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam, value) into StringValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"doublearrayparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam) into VectorValue");
      }
      if(ParamVectorVal.size() != 4)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam, size) into VectorValue");
      }
      if(!openfluid::scientific::isCloseEnough(ParamVectorVal[2],1.3,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (doublearrayparam, value) into VectorValue");
      }


      // ====== Long array param ======

      ParamLongArray.clear();
      if (!OPENFLUID_GetWareParameter(Params,"longarrayparam",ParamLongArray))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam) get by reference");
      }
      if (ParamLongArray.size() != 5)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam, size) get by reference");
      }
      if (ParamLongArray[3] != 14)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam, value) get by reference");
      }
      if (ParamLongArray[4] == 14)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam, wrongvalue) get by reference");
      }

      if (OPENFLUID_GetWareParameter(Params,"wronglongarrayparam",ParamLongArray))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wronglongarrayparam)");
      }

      if (OPENFLUID_GetWareParameter(Params,"longarrayparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam) into DoubleValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"longarrayparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam) into StringValue");
      }
      if(ParamStrVal.get() != "[11,12,13,14,15]")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam, value) into StringValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"longarrayparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam) into VectorValue");
      }
      if(ParamVectorVal.size() != 5)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam, size) into VectorValue");
      }
      if(!openfluid::scientific::isCloseEnough<double>(ParamVectorVal[3],14,0.00001))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (longarrayparam, value) into VectorValue");
      }

      // ====== Matrix param ======

      if (!OPENFLUID_GetWareParameter(Params,"matrixparam",ParamMatrixVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam) get by reference");
      }
      if (ParamMatrixVal.getColsNbr() != 2)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam, cols nb) get by reference");
      }
      if (ParamMatrixVal.getRowsNbr() != 3)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam, rows nb) get by reference");
      }
      if (!openfluid::scientific::isCloseEnough(ParamMatrixVal.get(0,0),1.1))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam, value) get by reference");
      }
      if (!openfluid::scientific::isCloseEnough(ParamMatrixVal.get(0,1),2.1))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam, wrongvalue) get by reference");
      }
      if (!openfluid::scientific::isCloseEnough(ParamMatrixVal.get(1,2),3.2))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam, wrongvalue) get by reference");
      }

      if (OPENFLUID_GetWareParameter(Params,"wrongmatrixparam",ParamMatrixVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (wrongmatrixparam)");
      }

      if (OPENFLUID_GetWareParameter(Params,"matrixparam",ParamDoubleVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam) into DoubleValue");
      }

      if (OPENFLUID_GetWareParameter(Params,"matrixparam",ParamVectorVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam) into VectorValue");
      }

      if (!OPENFLUID_GetWareParameter(Params,"matrixparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam) into StringValue");
      }
      if(ParamStrVal.get() != "[[1.1,1.2],[2.1,2.2],[3.1,3.2]]")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (matrixparam, value) into StringValue");
      }


      // ====== Map param ======

      if (!OPENFLUID_GetWareParameter(Params,"mapparam",ParamStrVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (mapparam) into StringValue");
      }
      if(!ParamStrVal.toMapValue(ParamMapVal))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (mapparam, type)");
      }
      if(ParamMapVal.size() != 3)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (mapparam, size)");
      }
      double aDouble = ParamMapVal.getDouble("key1");
      if(!openfluid::scientific::isCloseEnough(aDouble,1.1))
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (mapparam, value for key key1)");
      }
      if(ParamMapVal.getString("key2") != "a string")
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (mapparam, value for key key2)");
      }
      aBool = ParamMapVal.getBoolean("key3");
      if(aBool != true)
      {
        OPENFLUID_RaiseError("incorrect OPENFLUID_GetWareParameter (mapparam, value for key key3)");
      }


      // ====== Vars replacements in params ======

      std::string InPath;
      std::string OutPath;
      std::string TempPath;

      OPENFLUID_GetRunEnvironment("dir.input",InPath);
      OPENFLUID_GetRunEnvironment("dir.output",OutPath);
      OPENFLUID_GetRunEnvironment("dir.temp",TempPath);

      OPENFLUID_GetWareParameter(Params,"inpath",ParamStrVal);
      if (ParamStrVal.toString() != (InPath+"/path/in"))
      {
        OPENFLUID_RaiseError("incorrect inpath parameter value");
      }

      OPENFLUID_GetWareParameter(Params,"outpath",ParamStrVal);
      if (ParamStrVal.toString() != (OutPath+"/path/out"))
      {
        OPENFLUID_RaiseError("incorrect outpath parameter value");
      }

      OPENFLUID_GetWareParameter(Params,"temppath",ParamStrVal);
      if (ParamStrVal.toString() != (TempPath+"/path/temp"))
      {
        OPENFLUID_RaiseError("incorrect temppath parameter value");
      }
    }


    // =====================================================================
    // =====================================================================


    void prepareData() { }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {

    }


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


DEFINE_SIMULATOR_CLASS(SimParamsPrimitivesSimulator)

