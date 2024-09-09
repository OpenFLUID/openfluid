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
  @file SimulationModeConsistency_HEAVYTEST.cpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_SimulationModeConsistency


#include <iostream>

#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QString>
#include <QTextStream>
#include <QThread>

#include <boost/test/unit_test.hpp>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/base/IOListener.hpp>
#include <openfluid/config.hpp>
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/ui/common/RunSimulationWorker.hpp>

#include "tests-config.hpp"


const std::string TestName = "SimulationModeConsistency";
const std::string IntegratedFolder = "integrated-mode";
const std::string CLIFolder = "CLI-mode";


void runCLISimulation(const std::string& DatasetName, const openfluid::tools::Path& INPath) 
{
  std::unique_ptr<QProcess> Process = std::make_unique<QProcess>();
  const auto OUTPath = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR, TestName, DatasetName, CLIFolder, openfluid::config::PROJECT_OUTPUTDIRPREFIX});
  std::string CmdPath = openfluid::tools::Filesystem::joinPath({openfluid::base::Environment::getInstallPrefix(),
                                                                openfluid::config::INSTALL_BIN_PATH,
                                                                openfluid::config::CMD_APP
                                                               });

  QString Command = QDir::toNativeSeparators(QString::fromStdString(CmdPath));

  QStringList Args;
  Args << "run" << "--clean-output-dir"
                << QDir::toNativeSeparators(QString::fromStdString(INPath.toGeneric())) 
                << QDir::toNativeSeparators(QString::fromStdString(OUTPath.toGeneric()));

  Process->start(Command, Args);
  BOOST_CHECK(Process->waitForFinished(-1));
}


// =====================================================================
// =====================================================================


class TestRunContextManager : public openfluid::base::RunContextManager
{
  public:

    TestRunContextManager(const openfluid::tools::Path& INPath, const openfluid::tools::Path& PrjPath)
    // TODO  find solution without full project generation, not easy: solutions only based on output dir failed
    {
      createProject(PrjPath.toGeneric(), "Test project","This is a test project", "John Doe", false);
      m_InputDir = INPath.toGeneric();
      updateWaresEnvironment();
      setClearOutputDir(true);
    }
};


// =====================================================================
// =====================================================================


void runIntegratedSimulation(const std::string& DatasetName, const openfluid::tools::Path& INPath)
{
  openfluid::base::IOListener Listener;
  openfluid::fluidx::FluidXIO FXIO(&Listener);
  const auto PrjPath = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR, TestName, DatasetName, IntegratedFolder});
  openfluid::fluidx::FluidXDescriptor m_FXDesc = FXIO.loadFromDirectory(INPath.toGeneric());

  openfluid::ui::common::RunSimulationListener* mp_Listener = new openfluid::ui::common::RunSimulationListener();
  openfluid::ui::common::RunSimulationWorker* Worker =
      new openfluid::ui::common::RunSimulationWorker(&m_FXDesc, mp_Listener);

  openfluid::base::RunContextManager::setInstance(new TestRunContextManager(INPath, PrjPath));
  Worker->run();
  openfluid::base::RunContextManager::kill();
}


// =====================================================================
// =====================================================================


bool compareModes(const std::string& DatasetName, const openfluid::tools::Path& DatasetPath)
{
  std::unique_ptr<QProcess> Process = std::make_unique<QProcess>();

  runIntegratedSimulation(DatasetName, DatasetPath);
  runCLISimulation(DatasetName, DatasetPath);

  const auto OUTPathIntegratedMode = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR, TestName, DatasetName, IntegratedFolder, openfluid::config::PROJECT_OUTPUTDIRPREFIX});
  const auto OUTPathCLIMode = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR, TestName, DatasetName, CLIFolder, openfluid::config::PROJECT_OUTPUTDIRPREFIX});

  QString DiffCommand = "diff";
  QStringList Args;
  Args << "-y"
       << "--suppress-common-lines"
       << "--exclude=*.log"
       << "--exclude=*.kmz"
       << "--exclude=*.pdf"
       << "--exclude=*.gnuplot"
       << QDir::toNativeSeparators(QString::fromStdString(OUTPathIntegratedMode.toGeneric()))
       << QDir::toNativeSeparators(QString::fromStdString(OUTPathCLIMode.toGeneric()));

  Process->start(DiffCommand, Args);
  BOOST_CHECK(Process->waitForFinished(-1));
  std::string DiffCommandOutput = Process->readAllStandardOutput().toStdString() + \
                                  Process->readAllStandardError().toStdString();
  if(!DiffCommandOutput.empty())
  {
    std::cout << DiffCommandOutput << std::endl;
  }
  
  return (DiffCommandOutput == "");
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_consistency_Firespread)
{
  const openfluid::tools::Path INPath = openfluid::tools::Path(
    {CONFIGTESTS_INPUT_DATASETS_DIR, "OPENFLUID.IN.FirespreadReduced"});
  BOOST_CHECK(compareModes("FirespreadReduced", INPath));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_consistency_MhydasRoujan)
{
  const openfluid::tools::Path INPath = openfluid::tools::Path(
   {CONFIGTESTS_INPUT_DATASETS_DIR, "OPENFLUID.IN.MhydasReduced"});
  BOOST_CHECK(compareModes("MhydasReduced", INPath));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_consistency_Manhattan)
{
  const openfluid::tools::Path INPath = openfluid::tools::Path(
    {openfluid::config::EXAMPLES_PROJECTS_SOURCE_PATH, "Manhattan", "IN"});
  BOOST_CHECK(compareModes("Manhattan", INPath));
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_consistency_Primitives)
{
  const openfluid::tools::Path INPath = openfluid::tools::Path(
    {openfluid::config::EXAMPLES_PROJECTS_SOURCE_PATH, "Primitives", "IN"});
  BOOST_CHECK(compareModes("Primitives", INPath));
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  
  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
