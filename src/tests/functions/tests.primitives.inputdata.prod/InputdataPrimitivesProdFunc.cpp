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


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/core.hpp>
#include <glibmm/ustring.h>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN


// =====================================================================
// =====================================================================


#define THIS_SIM_ID "tests.primitives.inputdata.prod"

BEGIN_FUNCTION_SIGNATURE(THIS_SIM_ID)

  DECLARE_SIGNATURE_NAME("test function for inputdata primitives");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

  DECLARE_PRODUCED_INPUTDATA("indataDouble2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataDouble3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataLong2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataLong3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataBool2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataBool3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataString2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataString3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataVector2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataVector3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataMatrix2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataMatrix3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataMap2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataMap3","TestUnits","","")

  DECLARE_PRODUCED_INPUTDATA("indataNull2","TestUnits","","")
  DECLARE_PRODUCED_INPUTDATA("indataNull3","TestUnits","","")

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


class InputdataPrimitivesProdFunction : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    InputdataPrimitivesProdFunction() : PluggableSimulator()
    {


    }


    // =====================================================================
    // =====================================================================


    ~InputdataPrimitivesProdFunction()
    {


    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData()
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
      }

    }


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

DEFINE_FUNCTION_CLASS(InputdataPrimitivesProdFunction)

