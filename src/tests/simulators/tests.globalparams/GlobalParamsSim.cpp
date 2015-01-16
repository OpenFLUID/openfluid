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
  @file GlobalParamsSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN

// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.globalparams")

  DECLARE_NAME("test simulator for global parameters of models");
  DECLARE_DESCRIPTION("tttt");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


END_SIMULATOR_SIGNATURE

/**

*/
class GlobalParamsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


  GlobalParamsSimulator() : PluggableSimulator()
  {


  }


  // =====================================================================
  // =====================================================================


  ~GlobalParamsSimulator()
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
      std::cout << (*itParams).first << " -> " << (*itParams).second << std::endl;

    if (!OPENFLUID_GetSimulatorParameter(Params,"gparam1",LongParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam1 not found");

    if (LongParam != 1)
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam1");


    if (!OPENFLUID_GetSimulatorParameter(Params,"lparam1",LongParam))
      OPENFLUID_RaiseError("tests.globalparams","lparam1 not found");

    if (LongParam != 100)
      OPENFLUID_RaiseError("tests.globalparams","wrong value for lparam1");


    if (!OPENFLUID_GetSimulatorParameter(Params,"gparam2",StrParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam2 not found");

    if (StrParam != "nineteen")
      OPENFLUID_RaiseError("tests.globalparams","wrong value for gparam2");


    if (!OPENFLUID_GetSimulatorParameter(Params,"gparam3",DoubleParam))
      OPENFLUID_RaiseError("tests.globalparams","gparam3 not found");

    if (!openfluid::scientific::isCloseEnough(DoubleParam,0.1,0.00001))
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

DEFINE_SIMULATOR_CLASS(GlobalParamsSimulator)

