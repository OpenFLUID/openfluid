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
 @file GrassGISProxy_TEST.cpp

 @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_grassgisproxy
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <QApplication>
#include <openfluid/utils/GrassGISProxy.hpp>
#include <openfluid/base/Environment.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include "tests-config.hpp"


QString BaseWorkDir = QString::fromStdString(CONFIGTESTS_OUTPUT_DATA_DIR+"/GrassGISProxy");


BOOST_AUTO_TEST_CASE(check_init)
{
  BOOST_REQUIRE(!openfluid::utils::GrassGISProxy::getVersion().isEmpty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_singletask)
{
  openfluid::utils::GrassGISProxy GRASS(BaseWorkDir+"/data","single");

  GRASS.setOutputFile(BaseWorkDir+"/check_singletask.out");
  GRASS.setErrorFile(BaseWorkDir+"/check_singletask.err");

  BOOST_REQUIRE_EQUAL(GRASS.runSingleTask("g.gisenv"),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_job)
{
  openfluid::utils::GrassGISProxy GRASS(BaseWorkDir+"/data","job");

  GRASS.setOutputFile(BaseWorkDir+"/check_job.out");
  GRASS.setErrorFile(BaseWorkDir+"/check_job.err");

  BOOST_REQUIRE_EQUAL(GRASS.jobLines().size(),0);

  GRASS.appendTask("g.version",{},{"-e"});
  GRASS.appendTask("g.gisenv");

  std::cout << GRASS.jobLines().join("\n").toStdString() << std::endl;

  BOOST_REQUIRE_EQUAL(GRASS.jobLines().size(),2);

  BOOST_REQUIRE_EQUAL(GRASS.runJob(),0);

  BOOST_REQUIRE_EQUAL(GRASS.jobLines().size(),0);
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_process1)
{
  openfluid::utils::GrassGISProxy GRASS(BaseWorkDir+"/data","process1");

  GRASS.setOutputFile(BaseWorkDir+"/check_process1.out");
  GRASS.setErrorFile(BaseWorkDir+"/check_process1.err");

  BOOST_REQUIRE_EQUAL(GRASS.jobLines().size(),0);

  GRASS.appendTask("g.mapset",{{"mapset","somewhere"}},{"-c"});
  GRASS.appendTask("v.import",
                   {{"input",QString::fromStdString(CONFIGTESTS_INPUT_MISCDATA_DIR+"/GeoVectorValue/SU.shp")},
                    {"output","importedSU"}},
                   {});
  GRASS.appendTask("g.mapsets",{},{"-l"});

  std::cout << GRASS.jobLines().join("\n").toStdString() << std::endl;

  BOOST_REQUIRE_EQUAL(GRASS.jobLines().size(),3);

  BOOST_REQUIRE_EQUAL(GRASS.runJob(),0);

  BOOST_REQUIRE_EQUAL(GRASS.jobLines().size(),0);
}



// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  openfluid::base::Environment::init();


  if (openfluid::utils::GrassGISProxy::isAvailable())
  {
    std::cout << openfluid::utils::GrassGISProxy::getVersion().toStdString() << std::endl;
    openfluid::tools::Filesystem::removeDirectory(BaseWorkDir.toStdString());
    openfluid::tools::Filesystem::makeDirectory(BaseWorkDir.toStdString());

    return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
  }
  else
    std::cout << "** Test not run due to failing to find GRASS GIS program **" << std::endl;

  return 0;
}
