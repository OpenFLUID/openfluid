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
  @file SimulationModeConsistency_TEST.cpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
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
#include <openfluid/fluidx/FluidXIO.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/tools/Filesystem.hpp>
#include <openfluid/ui/common/RunSimulationWorker.hpp>

#include "tests-config.hpp"


void runCLISimulation() 
{
  std::unique_ptr<QProcess> process = std::make_unique<QProcess>();

  const auto INPath = openfluid::tools::Path(
    {CONFIGTESTS_INPUT_DATASETS_DIR,"OPENFLUID.IN.MhydasReduced"});
  const auto OUTPath = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR,"SimulationModeConsistency/CLI-mode/OUT"});
  std::string CmdPath = openfluid::tools::Filesystem::joinPath({openfluid::base::Environment::getInstallPrefix(),
                                                                openfluid::config::INSTALL_BIN_PATH,
                                                                openfluid::config::CMD_APP
                                                               });

  QString Command = QDir::toNativeSeparators(QString::fromStdString(CmdPath));

  QStringList Args;
  Args << "run" << "--clean-output-dir"
                << QDir::toNativeSeparators(QString::fromStdString(INPath.toGeneric())) 
                << QDir::toNativeSeparators(QString::fromStdString(OUTPath.toGeneric()));

  process->start(Command, Args);
  BOOST_CHECK(process->waitForFinished());
}


// =====================================================================
// =====================================================================


void runIntegratedSimulation()
{
  openfluid::base::IOListener Listener;
  openfluid::fluidx::FluidXIO FXIO(&Listener);
  const auto INPath = openfluid::tools::Path(
    {CONFIGTESTS_INPUT_DATASETS_DIR,"OPENFLUID.IN.MhydasReduced"});
  const auto prjPath = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR,"SimulationModeConsistency/integrated-mode"});
  const auto OUTPath = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR,"SimulationModeConsistency/integrated-mode/OUT"});
  openfluid::fluidx::FluidXDescriptor m_FXDesc = FXIO.loadFromDirectory(INPath.toGeneric());

  openfluid::ui::common::RunSimulationListener* mp_Listener = new openfluid::ui::common::RunSimulationListener();
  openfluid::ui::common::RunSimulationWorker* Worker =
      new openfluid::ui::common::RunSimulationWorker(&m_FXDesc, mp_Listener);

  openfluid::base::RunContextManager::instance()->createProject(prjPath.toGeneric(),
                                                                "Test project","This is a test project",
                                                                "John Doe", false);
  openfluid::base::RunContextManager::instance()->setIODir(INPath.toGeneric(), OUTPath.toGeneric());
  Worker->run();
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_consistency)
{
  std::unique_ptr<QProcess> process = std::make_unique<QProcess>();

  runIntegratedSimulation();
  runCLISimulation();

  const auto OUTPathIntegratedMode = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR,"SimulationModeConsistency/integrated-mode/OUT"});
  const auto OUTPathCLIMode = openfluid::tools::Path(
    {CONFIGTESTS_OUTPUT_DATA_DIR,"SimulationModeConsistency/CLI-mode/OUT"});

  QString diffCommand = "diff";
  QStringList args;
  args << "--exclude=*.log" 
       << "-qr"
       << QDir::toNativeSeparators(QString::fromStdString(OUTPathIntegratedMode.toGeneric()))
       << QDir::toNativeSeparators(QString::fromStdString(OUTPathCLIMode.toGeneric()));

  process->setProcessChannelMode(QProcess::MergedChannels);
  process->start(diffCommand, args);
  BOOST_CHECK(process->waitForFinished());
  QString diffcommandOutput = process->readAllStandardOutput();

  BOOST_CHECK(diffcommandOutput.toStdString() == "");
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  
  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
