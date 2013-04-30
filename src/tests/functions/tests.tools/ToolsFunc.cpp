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
  \file ToolsFunc.cpp
  \brief Implements ...
*/

#include <boost/filesystem/operations.hpp>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/ColTextParser.hpp>

// =====================================================================
// =====================================================================


DECLARE_FUNCTION_PLUGIN


// =====================================================================
// =====================================================================


/**

*/
class ToolsFunction : public openfluid::ware::PluggableSimulator
{
  private:


  public:
    /**
      Constructor
    */
    ToolsFunction();

    /**
      Destructor
    */
    ~ToolsFunction();

    void initParams(const openfluid::ware::WareParams_t& Params);

    void prepareData();

    void checkConsistency();

    openfluid::base::SchedulingRequest initializeRun();

    openfluid::base::SchedulingRequest runStep();

    void finalizeRun();

};



// =====================================================================
// =====================================================================


DEFINE_FUNCTION_CLASS(ToolsFunction)


// =====================================================================
// =====================================================================


BEGIN_FUNCTION_SIGNATURE("tests.tools")

  DECLARE_SIGNATURE_NAME("test function for openfluid::tools");
  DECLARE_SIGNATURE_DESCRIPTION("");

  DECLARE_SIGNATURE_VERSION("1.0");
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::ware::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN("");
  DECLARE_SIGNATURE_PROCESS("");
  DECLARE_SIGNATURE_METHOD("");
  DECLARE_SIGNATURE_AUTHORNAME("");
  DECLARE_SIGNATURE_AUTHOREMAIL("");

END_FUNCTION_SIGNATURE


// =====================================================================
// =====================================================================


ToolsFunction::ToolsFunction()
                : PluggableSimulator()
{


}


// =====================================================================
// =====================================================================


ToolsFunction::~ToolsFunction()
{


}


// =====================================================================
// =====================================================================


void ToolsFunction::initParams(const openfluid::ware::WareParams_t& /*Params*/)
{

}

// =====================================================================
// =====================================================================


void ToolsFunction::prepareData()
{  }


// =====================================================================
// =====================================================================


void ToolsFunction::checkConsistency()
{  }


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest ToolsFunction::initializeRun()
{

  openfluid::tools::ColumnTextParser CTParser("#");
  std::string InputDir;
  double DoubleValue;
  std::string StrValue;

  OPENFLUID_GetRunEnvironment("dir.input",InputDir);
  boost::filesystem::path FileName(InputDir + "/" + "columnfile.txt");

  if (boost::filesystem::exists(FileName))
  {
    CTParser.loadFromFile(FileName.string());

    if (CTParser.getColsCount() != 5)
      OPENFLUID_RaiseError("tests.tools","wrong columns number in " + FileName.string());

    if (!CTParser.getDoubleValue(2,1,&DoubleValue))
      OPENFLUID_RaiseError("tests.tools","error reading double value (3,2) in " + FileName.string());

    if (!openfluid::tools::IsVeryClose(DoubleValue,1.2))
      OPENFLUID_RaiseError("tests.tools","wrong double value for (3,2) in " + FileName.string());

    if (!CTParser.getStringValue(0,0,&StrValue))
      OPENFLUID_RaiseError("tests.tools","error reading string value (0,0) in " + FileName.string());

    if (StrValue != "5")
      OPENFLUID_RaiseError("tests.tools","wrong string value for (0,0) in " + FileName.string());


    if (CTParser.getStringValue(5,5,&StrValue))
      OPENFLUID_RaiseError("tests.tools","error : found value out of range (5,5) in " + FileName.string());
  }
  else OPENFLUID_RaiseError("tests.tools","file " + FileName.string() + " not found!");

  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest ToolsFunction::runStep()
{

  return Never();
}

// =====================================================================
// =====================================================================


void ToolsFunction::finalizeRun()
{

}

