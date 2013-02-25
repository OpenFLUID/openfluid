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
  \file GlobalParams.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/ware/PluggableFunction.hpp>


// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE("tests.globalparams")

  DECLARE_SIGNATURE_NAME("test function for global parameters of models");
  DECLARE_SIGNATURE_DESCRIPTION("tttt");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

END_FUNCTION_SIGNATURE

/**

*/
class GlobalParamsFunction : public openfluid::ware::PluggableFunction
{
  private:

  public:


  GlobalParamsFunction() : PluggableFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  ~GlobalParamsFunction()
  {


  }


  // =====================================================================
  // =====================================================================


  void initParams(const openfluid::ware::WareParams_t& Params)
  {
    std::string StrParam;
    long LongParam;
    double DoubleParam;

    openfluid::ware::WareParams_t::const_iterator itParams;

    for (itParams=Params.begin();itParams!=Params.end();++itParams)
      std::cout << (*itParams).first << " -> " << (*itParams).second.data() << std::endl;

    if (!OPENFLUID_GetFunctionParameter(Params,"gparam1",&LongParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam1 not found");

    if (LongParam != 1)
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam1");


    if (!OPENFLUID_GetFunctionParameter(Params,"lparam1",&LongParam))
      OPENFLUID_RaiseError("tests.globalparams","lparam1 not found");

    if (LongParam != 100)
      OPENFLUID_RaiseError("tests.globalparams","wrong value for lparam1");


    if (!OPENFLUID_GetFunctionParameter(Params,"gparam2",&StrParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam2 not found");

    if (StrParam != "nineteen")
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam2");


    if (!OPENFLUID_GetFunctionParameter(Params,"gparam3",&DoubleParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam3 not found");

    if (!openfluid::tools::IsCloseEnough(DoubleParam,0.1,0.00001))
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam3");
  }

  // =====================================================================
  // =====================================================================


  void prepareData()
  { }


  // =====================================================================
  // =====================================================================


  void checkConsistency()
  { }


  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest initializeRun()
  {
    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  openfluid::base::SchedulingRequest runStep()
  {
    return DefaultDeltaT();
  }

  // =====================================================================
  // =====================================================================


  void finalizeRun()
  { }

};

// =====================================================================
// =====================================================================

DEFINE_FUNCTION_CLASS(GlobalParamsFunction)

