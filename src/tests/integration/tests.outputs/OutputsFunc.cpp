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
  \file OutputsFunc.cpp
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
  DECLARE_SIGNATURE_ID("tests.outputs");
  DECLARE_SIGNATURE_NAME("test function for outputs");
  DECLARE_SIGNATURE_SDKVERSION;

  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_PRODUCED_VAR("tests.vector[]","UnitsA","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","UnitsA","scalar for tests","");
  DECLARE_PRODUCED_VAR("tests.vector[]","UnitsB","vector for tests","");
  DECLARE_PRODUCED_VAR("tests.scalar","UnitsB","scalar for tests","");

END_FUNCTION_SIGNATURE


class OutputsFunction : public openfluid::ware::PluggableFunction
{
  private:

  public:


  OutputsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~OutputsFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  void initParams(const openfluid::core::FuncParamsMap_t& /*Params*/)
  { }

  // =====================================================================
  // =====================================================================


  void prepareData()
  { }


  // =====================================================================
  // =====================================================================


  void checkConsistency()
  {

  }


  // =====================================================================
  // =====================================================================


  openfluid::core::Duration_t initializeRun()
  {
    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  openfluid::core::Duration_t runStep()
  {
    openfluid::core::Unit *TUA, *TUB;
    long VectorSize = (OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT()) % 40;
    openfluid::core::VectorValue TheVector;
    std::string IDStr, TSStr;

    openfluid::tools::ConvertValue((OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT()),&TSStr);

    OPENFLUID_UNITS_ORDERED_LOOP("UnitsA",TUA)
    {

      TheVector = openfluid::core::VectorValue(VectorSize,double(TUA->getID()));
      OPENFLUID_AppendVariable(TUA,"tests.vector",TheVector);
      OPENFLUID_AppendVariable(TUA,"tests.scalar",double(TUA->getID()));

      openfluid::tools::ConvertValue(TUA->getID(),&IDStr);
      OPENFLUID_RaiseWarning("tests.messages",(OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT()),"["+TUA->getClass()+"|"+IDStr+"|"+TSStr+"] Message from tests.outputs function");

    }

    OPENFLUID_UNITS_ORDERED_LOOP("UnitsB",TUB)
    {

      TheVector = openfluid::core::VectorValue(VectorSize,double(TUB->getID()));
      OPENFLUID_AppendVariable(TUB,"tests.vector",TheVector);
      OPENFLUID_AppendVariable(TUB,"tests.scalar",double(TUB->getID()));

      openfluid::tools::ConvertValue(TUB->getID(),&IDStr);
      OPENFLUID_RaiseWarning("tests.messages",(OPENFLUID_GetCurrentTimeIndex()/OPENFLUID_GetDefaultDeltaT()),"["+TUB->getClass()+"|"+IDStr+"|"+TSStr+"] Message from tests.outputs function");

    }


    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  void finalizeRun()
  { }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_CLASS(OutputsFunction)

