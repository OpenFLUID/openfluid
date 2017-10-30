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
  @file GhostSimulatorFileIO_TEST.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_ghostsimulatorfileio
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <openfluid/machine/GhostSimulatorFileIO.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <tests-config.hpp>


// =====================================================================
// =====================================================================


void CheckOriginalSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  BOOST_REQUIRE_EQUAL(Signature.ID,"simA");
  BOOST_REQUIRE_EQUAL(Signature.Name,"Simulator A");
  BOOST_REQUIRE_EQUAL(Signature.Description,"This is simulator A");

  BOOST_REQUIRE_EQUAL(Signature.Authors.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.Authors[0].first,"John Doe");
  BOOST_REQUIRE_EQUAL(Signature.Authors[0].second,"doe@foobar.org");
  BOOST_REQUIRE_EQUAL(Signature.Authors[1].first,"Tony Stark");
  BOOST_REQUIRE_EQUAL(Signature.Authors[1].second,"iron@shield.org");

  BOOST_REQUIRE_EQUAL(Signature.Status,openfluid::ware::BETA);
  BOOST_REQUIRE_EQUAL(Signature.Domain,"tests");
  BOOST_REQUIRE_EQUAL(Signature.Process,"process1");
  BOOST_REQUIRE_EQUAL(Signature.Method,"method1");
  BOOST_REQUIRE_EQUAL(Signature.Version,"1.0");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredParams.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredParams[0].DataName,"param1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredParams[0].Description,"parameter 1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredParams[0].DataUnit,"m/s");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams[0].DataName,"param2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams[0].Description,"parameter 2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams[0].DataUnit,"");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredExtraFiles.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredExtraFiles[0],"file1.dat");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedExtraFiles.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedExtraFiles[0],"file2.dat");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedEventsOnUnits.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedEventsOnUnits[0],"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedEventsOnUnits[1],"UB");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].DataName,"var1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].DataType,openfluid::core::Value::NONE);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].Description,"variable 1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].DataUnit,"");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedVars.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedVars[0].DataName,"var2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedVars[0].DataType,openfluid::core::Value::DOUBLE);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedVars[0].UnitsClass,"UB");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedVars[0].Description,"variable 2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedVars[0].DataUnit,"m");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].DataName,"var3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].DataType,openfluid::core::Value::VECTOR);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].Description,"variable 3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].DataUnit,"m3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].DataName,"var4");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].DataType,openfluid::core::Value::NONE);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].Description,"variable 4");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].DataUnit,"m");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars[0].DataName,"var5");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars[0].DataType,openfluid::core::Value::NONE);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars[0].UnitsClass,"UB");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars[0].Description,"variable 5");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars[0].DataUnit,"");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].DataName,"attr1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].Description,"attribute 1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].DataUnit,"");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].DataName,"attr2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].Description,"");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].DataUnit,"");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedAttribute.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedAttribute[0].DataName,"attr3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedAttribute[0].UnitsClass,"UB");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedAttribute[0].Description,"attribute 3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedAttribute[0].DataUnit,"m");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedAttribute.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedAttribute[0].DataName,"attr4");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedAttribute[0].UnitsClass,"UB");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedAttribute[0].Description,"attribute 4");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedAttribute[0].DataUnit,"m2");


  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsGraph,"modifications on UA & UB");
  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsClass.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsClass[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsClass[0].Description,"update of UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsClass[1].UnitsClass,"UB");
  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsClass[1].Description,"");


  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::DEFAULT);
}


// =====================================================================
// =====================================================================


void CheckModifiedSignature(const openfluid::ware::SimulatorSignature& Signature)
{
  BOOST_REQUIRE_EQUAL(Signature.ID,"simB");
  BOOST_REQUIRE_EQUAL(Signature.Name,"Simulator A");
  BOOST_REQUIRE_EQUAL(Signature.Description,"This is simulator A");

  BOOST_REQUIRE_EQUAL(Signature.Authors.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.Authors[0].first,"John Doe");
  BOOST_REQUIRE_EQUAL(Signature.Authors[0].second,"doe@foobar.org");
  BOOST_REQUIRE_EQUAL(Signature.Authors[1].first,"Tony Stark");
  BOOST_REQUIRE_EQUAL(Signature.Authors[1].second,"iron@shield.org");

  BOOST_REQUIRE_EQUAL(Signature.Status,openfluid::ware::BETA);
  BOOST_REQUIRE_EQUAL(Signature.Process,"process1");
  BOOST_REQUIRE_EQUAL(Signature.Method,"method1");
  BOOST_REQUIRE_EQUAL(Signature.Version,"1.0");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredParams.size(),0);

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams[0].DataName,"param2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams[0].Description,"parameter 2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedParams[0].DataUnit,"");


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredExtraFiles.size(),0);

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedExtraFiles.size(),0);


  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedEventsOnUnits.size(),0);


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars.size(),1);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].DataName,"var1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].DataType,openfluid::core::Value::NONE);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].Description,"variable 1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredVars[0].DataUnit,"");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].DataName,"var3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].DataType,openfluid::core::Value::VECTOR);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].Description,"variable 3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[0].DataUnit,"m3");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].DataName,"var4");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].DataType,openfluid::core::Value::NONE);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].Description,"variable 4");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedVars[1].DataUnit,"m");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UpdatedVars.size(),0);


  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute.size(),2);
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].DataName,"attr1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].Description,"attribute 1");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[0].DataUnit,"");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].DataName,"attr2");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].UnitsClass,"UA");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].Description,"");
  BOOST_REQUIRE_EQUAL(Signature.HandledData.RequiredAttribute[1].DataUnit,"");

  BOOST_REQUIRE_EQUAL(Signature.HandledData.UsedAttribute.size(),0);

  BOOST_REQUIRE_EQUAL(Signature.HandledData.ProducedAttribute.size(),0);


  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsGraph,"");
  BOOST_REQUIRE_EQUAL(Signature.HandledUnitsGraph.UpdatedUnitsClass.size(),0);


  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::DEFAULT);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_operations)
{
  openfluid::ware::SimulatorSignature Signature;

  std::string OriginalFilePath = CONFIGTESTS_INPUT_MISCDATA_DIR+"/"+"GhostSimulators/simA_ofghost-sim.xml";
  std::string ModifiedDirPath = CONFIGTESTS_OUTPUT_DATA_DIR+"/"+"GhostSimulators";

  openfluid::tools::Filesystem::makeDirectory(ModifiedDirPath);

  std::cout << "Ghost file: " << OriginalFilePath << std::endl;

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::loadFromFile(OriginalFilePath,Signature));

  CheckOriginalSignature(Signature);


  // ================================


  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::saveToFile(Signature,ModifiedDirPath));

  Signature.clear();

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::loadFromFile(OriginalFilePath,Signature));

  CheckOriginalSignature(Signature);


  // ================================


  Signature.ID = "simB";
  Signature.HandledData.RequiredParams.clear();
  Signature.HandledData.UsedEventsOnUnits.clear();
  Signature.HandledData.UsedExtraFiles.clear();
  Signature.HandledData.RequiredExtraFiles.clear();
  Signature.HandledData.UsedVars.clear();
  Signature.HandledData.UpdatedVars.clear();
  Signature.HandledData.UsedAttribute.clear();
  Signature.HandledData.ProducedAttribute.clear();
  Signature.HandledUnitsGraph.clear();

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::saveToFile(Signature,ModifiedDirPath));

  Signature.clear();

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::loadFromFile(ModifiedDirPath+"/simB_ofghost-sim.xml",
                                                                       Signature));

  CheckModifiedSignature(Signature);


  // ================================


  // scheduling fixed
  Signature.ID = "simB.fixed";
  Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::FIXED;
  Signature.TimeScheduling.Min = 1955;
  Signature.TimeScheduling.Max = 1955;

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::saveToFile(Signature,ModifiedDirPath));

  Signature.clear();
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::UNDEFINED);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Min,0);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Max,0);

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::loadFromFile(ModifiedDirPath+"/simB.fixed_ofghost-sim.xml",
                                                                       Signature));

  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::FIXED);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Min,1955);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Max,1955);


  // ================================


  // scheduling range
  Signature.ID = "simB.range";
  Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::RANGE;
  Signature.TimeScheduling.Min = 1955;
  Signature.TimeScheduling.Max = 1976;

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::saveToFile(Signature,ModifiedDirPath));

  Signature.clear();
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::UNDEFINED);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Min,0);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Max,0);

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::loadFromFile(ModifiedDirPath+"/simB.range_ofghost-sim.xml",
                                                                       Signature));

  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::RANGE);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Min,1955);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Max,1976);


  // ================================


  // scheduling undefined

  Signature.ID = "simB.undefined";
  Signature.TimeScheduling.Type = openfluid::ware::SignatureTimeScheduling::UNDEFINED;
  Signature.TimeScheduling.Min = 0;
  Signature.TimeScheduling.Max = 0;

  BOOST_REQUIRE(openfluid::machine::GhostSimulatorFileIO::saveToFile(Signature,ModifiedDirPath));

  Signature.clear();
  Signature.TimeScheduling.setAsFixed(1955);

  BOOST_REQUIRE(
      openfluid::machine::GhostSimulatorFileIO::loadFromFile(ModifiedDirPath+"/simB.undefined_ofghost-sim.xml",
                                                             Signature));

  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Type,openfluid::ware::SignatureTimeScheduling::UNDEFINED);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Min,0);
  BOOST_REQUIRE_EQUAL(Signature.TimeScheduling.Max,0);

}
