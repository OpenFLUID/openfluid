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
  @file ToolsSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/scientific/FloatingPoint.hpp>
#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/ColumnTextParser.hpp>
#include <openfluid/tools/Filesystem.hpp>


// =====================================================================
// =====================================================================


DECLARE_SIMULATOR_PLUGIN


// =====================================================================
// =====================================================================


/**

*/
class ToolsSimulator : public openfluid::ware::PluggableSimulator
{
  private:


  public:
    /**
      Constructor
    */
    ToolsSimulator();

    /**
      Destructor
    */
    ~ToolsSimulator();

    void initParams(const openfluid::ware::WareParams_t& Params);

    void prepareData();

    void checkConsistency();

    openfluid::base::SchedulingRequest initializeRun();

    openfluid::base::SchedulingRequest runStep();

    void finalizeRun();

};



// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(ToolsSimulator)


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.tools")

  DECLARE_NAME("test simulator for openfluid::tools");
  DECLARE_DESCRIPTION("");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_DOMAIN("");
  DECLARE_PROCESS("");
  DECLARE_METHOD("");
  DECLARE_AUTHOR("","");


END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


ToolsSimulator::ToolsSimulator()
                : PluggableSimulator()
{


}


// =====================================================================
// =====================================================================


ToolsSimulator::~ToolsSimulator()
{


}


// =====================================================================
// =====================================================================


void ToolsSimulator::initParams(const openfluid::ware::WareParams_t& /*Params*/)
{

}

// =====================================================================
// =====================================================================


void ToolsSimulator::prepareData()
{  }


// =====================================================================
// =====================================================================


void ToolsSimulator::checkConsistency()
{  }


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest ToolsSimulator::initializeRun()
{

  openfluid::tools::ColumnTextParser CTParser("#");
  std::string InputDir;
  double DoubleValue;
  std::string StrValue;

  OPENFLUID_GetRunEnvironment("dir.input",InputDir);
  std::string FileName = InputDir + "/" + "columnfile.txt";

  if (openfluid::tools::Filesystem::isFile(FileName))
  {
    CTParser.loadFromFile(FileName);

    if (CTParser.getColsCount() != 5)
      OPENFLUID_RaiseError("wrong columns number in " + FileName);

    if (!CTParser.getDoubleValue(2,1,&DoubleValue))
      OPENFLUID_RaiseError("error reading double value (3,2) in " + FileName);

    if (!openfluid::scientific::isVeryClose(DoubleValue,1.2))
      OPENFLUID_RaiseError("wrong double value for (3,2) in " + FileName);

    if (!CTParser.getStringValue(0,0,&StrValue))
      OPENFLUID_RaiseError("error reading string value (0,0) in " + FileName);

    if (StrValue != "5")
      OPENFLUID_RaiseError("wrong string value for (0,0) in " + FileName);


    if (CTParser.getStringValue(5,5,&StrValue))
      OPENFLUID_RaiseError("error : found value out of range (5,5) in " + FileName);
  }
  else OPENFLUID_RaiseError("file " + FileName + " not found!");

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest ToolsSimulator::runStep()
{

  return Never();
}

// =====================================================================
// =====================================================================


void ToolsSimulator::finalizeRun()
{

}

