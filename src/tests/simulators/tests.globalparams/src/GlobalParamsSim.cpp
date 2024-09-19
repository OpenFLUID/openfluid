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

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>


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
    {
      std::cout << (*itParams).first << " -> " << (*itParams).second << std::endl;
    }

    if (!OPENFLUID_GetWareParameter(Params,"gparam1",LongParam))
    {
      OPENFLUID_RaiseError("gparam1 not found");
    }

    if (LongParam != 1)
    {
      OPENFLUID_RaiseError("wrong value for gparam1");
    }


    if (!OPENFLUID_GetWareParameter(Params,"lparam1",LongParam))
    {
      OPENFLUID_RaiseError("lparam1 not found");
    }

    if (LongParam != 100)
    {
      OPENFLUID_RaiseError("wrong value for lparam1");
    }


    if (!OPENFLUID_GetWareParameter(Params,"gparam2",StrParam))
    {
      OPENFLUID_RaiseError("gparam2 not found");
    }

    if (StrParam != "nineteen")
    {
      OPENFLUID_RaiseError("wrong value for gparam2");
    }


    if (!OPENFLUID_GetWareParameter(Params,"gparam3",DoubleParam))
    {
      OPENFLUID_RaiseError("gparam3 not found");
    }

    if (!openfluid::scientific::isCloseEnough(DoubleParam,0.1,0.00001))
    {
      OPENFLUID_RaiseError("wrong value for gparam3");
    }
  }


  // =====================================================================
  // =====================================================================


  void prepareData()
  {

  }


  // =====================================================================
  // =====================================================================


  void checkConsistency()
  {

  }


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

