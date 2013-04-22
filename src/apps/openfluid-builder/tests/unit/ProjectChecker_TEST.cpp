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
 \file ProjectChecker_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ProjectChecker
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "tests-config.hpp"
#include "ProjectChecker.hpp"
#include <glibmm/i18n.h>
#include <glibmm/ustring.h>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/FunctionSignatureRegistry.hpp>
#include <openfluid/base/OFException.hpp>
#include <openfluid/machine/FunctionPluginsManager.hpp>
#include <openfluid/fluidx/FunctionDescriptor.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>

// =====================================================================
// =====================================================================

class ProjectCheckerSub: public ProjectChecker
{
  public:
    ProjectCheckerSub(openfluid::fluidx::AdvancedFluidXDescriptor& Desc) :
        ProjectChecker(Desc)
    {
    }

    bool isParamSet(openfluid::fluidx::ModelItemDescriptor* Item,
                    std::string ParamName)
    {
      return ProjectChecker::isParamSet(Item, ParamName);
    }

    bool isParamSetAsDouble(openfluid::fluidx::ModelItemDescriptor* Item,
                            std::string ParamName)
    {
      return ProjectChecker::isParamSetAsDouble(Item, ParamName);
    }

    double getParamAsDouble(openfluid::fluidx::ModelItemDescriptor* Item,
                            std::string ParamName)
    {
      return ProjectChecker::getParamAsDouble(Item, ParamName);
    }
};
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");

  openfluid::fluidx::AdvancedFluidXDescriptor Desc(FXDesc);

  ProjectChecker PC(Desc);

  BOOST_CHECK(!PC.IsProjectOk);
  BOOST_CHECK(!PC.IsModelOk);
  BOOST_CHECK(PC.IsParamsOk);
  BOOST_CHECK(PC.IsGeneratorParamsOk);
  BOOST_CHECK(!PC.IsDomainOk);
  BOOST_CHECK(!PC.IsInputdataOk);
  BOOST_CHECK(!PC.IsExtraFilesOk);
  BOOST_CHECK(!PC.IsRunConfigOk);
  BOOST_CHECK(!PC.IsMonitoringOk);

  BOOST_CHECK(PC.ProjectMsg.empty());
  BOOST_CHECK(PC.ModelMsg.empty());
  BOOST_CHECK(PC.ParamsMsg.empty());
  BOOST_CHECK(PC.DomainMsg.empty());
  BOOST_CHECK(PC.InputdataMsg.empty());
  BOOST_CHECK(PC.ExtraFilesMsg.empty());
  BOOST_CHECK(PC.RunConfigMsg.empty());
  BOOST_CHECK(PC.MonitoringMsg.empty());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_isParamSet)
{
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraFunctionsPluginsPaths(
      CONFIGTESTS_OUTPUT_BINARY_DIR);
  openfluid::machine::FunctionSignatureRegistry::getInstance()->updatePluggableSignatures();
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.ProjectChecker");

  openfluid::fluidx::AdvancedFluidXDescriptor Desc(FXDesc);

  ProjectCheckerSub PC(Desc);

  openfluid::fluidx::ModelItemDescriptor* InterpGen = Desc.getModel().getItemAt(
      3);

  BOOST_CHECK(PC.isParamSet(InterpGen,"sources"));
  BOOST_CHECK(!PC.isParamSetAsDouble(InterpGen,"sources"));
  BOOST_CHECK_THROW(PC.getParamAsDouble(InterpGen,"sources"),
                    openfluid::base::OFException);

  BOOST_CHECK(PC.isParamSet(InterpGen,"thresholdmax"));
  BOOST_CHECK(PC.isParamSetAsDouble(InterpGen,"thresholdmax"));
  BOOST_CHECK_CLOSE(PC.getParamAsDouble(InterpGen,"thresholdmax"), 12.8,
                    0.0001);

  BOOST_CHECK(PC.isParamSet(InterpGen,"thresholdmin"));
  BOOST_CHECK(PC.isParamSetAsDouble(InterpGen,"thresholdmin"));
  BOOST_CHECK_CLOSE(PC.getParamAsDouble(InterpGen,"thresholdmin"), 12.7,
                    0.0001);

  openfluid::fluidx::ModelItemDescriptor* InjectGen = Desc.getModel().getItemAt(
      4);
  BOOST_CHECK(PC.isParamSet(InjectGen,"thresholdmin"));
  BOOST_CHECK(PC.isParamSetAsDouble(InjectGen,"thresholdmin"));
  BOOST_CHECK_CLOSE(PC.getParamAsDouble(InjectGen,"thresholdmin"), 1.7, 0.0001);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_check)
{
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraFunctionsPluginsPaths(
      CONFIGTESTS_OUTPUT_BINARY_DIR);
  openfluid::machine::FunctionSignatureRegistry::getInstance()->updatePluggableSignatures();
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.ProjectChecker");
  openfluid::base::RuntimeEnvironment::getInstance()->setInputDir(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.ProjectChecker");

  openfluid::fluidx::AdvancedFluidXDescriptor Desc(FXDesc);

  ProjectChecker PC(Desc);
  bool GlobalState = PC.check();

  BOOST_CHECK(PC.IsProjectOk);
  BOOST_CHECK(PC.IsModelOk);
  BOOST_CHECK(PC.IsParamsOk);
  BOOST_CHECK(PC.IsGeneratorParamsOk);
  BOOST_CHECK(PC.IsDomainOk);
  BOOST_CHECK(PC.IsInputdataOk);
  BOOST_CHECK(PC.IsExtraFilesOk);
  BOOST_CHECK(PC.IsRunConfigOk);
  BOOST_CHECK(PC.IsMonitoringOk);

  BOOST_CHECK(PC.ProjectMsg.empty());
  BOOST_CHECK(PC.ModelMsg.empty());
  BOOST_CHECK(PC.ParamsMsg.empty());
  BOOST_CHECK(PC.DomainMsg.empty());
  BOOST_CHECK(PC.InputdataMsg.empty());
  BOOST_CHECK(PC.ExtraFilesMsg.empty());
  BOOST_CHECK(PC.RunConfigMsg.empty());
  BOOST_CHECK(PC.MonitoringMsg.empty());

  BOOST_CHECK(GlobalState);

  openfluid::fluidx::ModelItemDescriptor* RandomGen = Desc.getModel().getItemAt(
      2);

  RandomGen->setParameter("min", "12.8");
  PC.check();
  BOOST_CHECK(!PC.IsGeneratorParamsOk);
  BOOST_CHECK_EQUAL(
      PC.ParamsMsg,
      Glib::ustring::compose(_("- %1 >= %2 in %3\n"),"min","max",static_cast<openfluid::fluidx::GeneratorDescriptor*>(RandomGen)->getGeneratedID()));

  RandomGen->setParameter("min", "12.7");
  PC.check();
  BOOST_CHECK(PC.IsGeneratorParamsOk);
  BOOST_CHECK(PC.ParamsMsg.empty());

  Desc.getModel().setGlobalParameter("min", "12.8");
  PC.check();
  BOOST_CHECK(PC.IsGeneratorParamsOk);
  BOOST_CHECK(PC.ParamsMsg.empty());

  RandomGen->setParameter("min", "");
  PC.check();
  BOOST_CHECK(!PC.IsGeneratorParamsOk);
  BOOST_CHECK_EQUAL(
      PC.ParamsMsg,
      Glib::ustring::compose(_("- %1 >= %2 in %3\n"),"min","max",static_cast<openfluid::fluidx::GeneratorDescriptor*>(RandomGen)->getGeneratedID()));

  openfluid::fluidx::GeneratorDescriptor Gen(
      "aVar", "aUnit", openfluid::fluidx::GeneratorDescriptor::Fixed);
  Desc.getModel().appendItem(&Gen);
  PC.check();
  BOOST_CHECK(!PC.IsProjectOk);
  BOOST_CHECK_EQUAL(
      PC.ProjectMsg,
      Glib::ustring::compose(_("- Unit class %1 doesn't exist for %2 variable generated by %3\n"),"aUnit","aVar",Gen.getGeneratedID()));

  openfluid::fluidx::UnitDescriptor U;
  U.getUnitClass() = "aUnit";
  U.getUnitID() = 1;
  Desc.getDomain().addUnit(&U);
  PC.check();
  BOOST_CHECK(PC.IsProjectOk);
  BOOST_CHECK(PC.ProjectMsg.empty());

  Desc.getMonitoring().removeFromObserverList("tests.hopla");
  PC.check();
  BOOST_CHECK(!PC.IsMonitoringOk);
  BOOST_CHECK_EQUAL(PC.MonitoringMsg, _("No observer defined"));

  Desc.getModel().appendItem(
      new openfluid::fluidx::FunctionDescriptor(
          "tests.primitives.inputdata.use"));
  PC.check();
  BOOST_CHECK(!PC.IsInputdataOk);
  BOOST_CHECK(!PC.InputdataMsg.empty());

  Desc.getDomain().addInputData("TestUnits", "indataDouble", "");
  Desc.getDomain().addInputData("TestUnits", "indataLong", "");
  Desc.getDomain().addInputData("TestUnits", "indataString", "");
  Desc.getDomain().addInputData("TestUnits", "indataMatrix", "");
  Desc.getDomain().addInputData("TestUnits", "indataMap", "");
  Desc.getDomain().addInputData("TestUnits", "indataBool", "");
  Desc.getDomain().addInputData("TestUnits", "indataVector", "");
  PC.check();
  BOOST_CHECK(!PC.IsInputdataOk);
  BOOST_CHECK(!PC.InputdataMsg.empty());

  Desc.getModel().appendItem(
      new openfluid::fluidx::FunctionDescriptor(
          "tests.primitives.inputdata.prod"));
  Desc.getModel().moveItem(8, 7);
  PC.check();
  BOOST_CHECK(PC.IsInputdataOk);
  BOOST_CHECK(PC.InputdataMsg.empty());

  Desc.getDomain().deleteInputData("TestUnits", "indataDouble");
  Desc.getDomain().deleteInputData("TestUnits", "indataLong");
  Desc.getDomain().deleteInputData("TestUnits", "indataString");
  Desc.getDomain().deleteInputData("TestUnits", "indataMatrix");
  Desc.getDomain().deleteInputData("TestUnits", "indataMap");
  Desc.getDomain().deleteInputData("TestUnits", "indataBool");
  Desc.getDomain().deleteInputData("TestUnits", "indataVector");

  PC.check();
  BOOST_CHECK(!PC.IsInputdataOk);
  BOOST_CHECK(!PC.InputdataMsg.empty());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_generators_with_files)
{
  openfluid::base::RuntimeEnvironment::getInstance()->addExtraFunctionsPluginsPaths(
      CONFIGTESTS_OUTPUT_BINARY_DIR);
  openfluid::machine::FunctionSignatureRegistry::getInstance()->updatePluggableSignatures();
  openfluid::fluidx::FluidXDescriptor FXDesc(0);
  FXDesc.loadFromDirectory(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.ProjectChecker");
  openfluid::base::RuntimeEnvironment::getInstance()->setInputDir(
      CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.ProjectChecker");

  openfluid::fluidx::AdvancedFluidXDescriptor Desc(FXDesc);

  ProjectChecker PC(Desc);

  bool GlobalState = PC.check();

  BOOST_CHECK(PC.IsParamsOk);
  BOOST_CHECK(PC.IsExtraFilesOk);
  BOOST_CHECK(PC.ExtraFilesMsg.empty());
  BOOST_CHECK(GlobalState);

  openfluid::fluidx::ModelItemDescriptor* InterpGen = Desc.getModel().getItemAt(
      3);

  openfluid::fluidx::ModelItemDescriptor* InjectpGen =
      Desc.getModel().getItemAt(4);

  std::string InterpID =
      static_cast<openfluid::fluidx::GeneratorDescriptor*>(InterpGen)->getGeneratedID();
  std::string InjectID =
      static_cast<openfluid::fluidx::GeneratorDescriptor*>(InjectpGen)->getGeneratedID();

  InterpGen->setParameter("sources", "src2.txt");
  GlobalState = PC.check();

  BOOST_CHECK(PC.IsGeneratorParamsOk);
  BOOST_CHECK(!PC.IsExtraFilesOk);
  BOOST_CHECK_EQUAL(
      PC.ExtraFilesMsg,
      Glib::ustring::compose(_("- File %1 required by %2 not found\n"),"src2.txt",InterpID));
  BOOST_CHECK(!GlobalState);

  InterpGen->setParameter("sources", "");
  GlobalState = PC.check();

  BOOST_CHECK(!PC.IsGeneratorParamsOk);
  BOOST_CHECK(PC.IsExtraFilesOk);
  BOOST_CHECK( PC.ExtraFilesMsg.empty());
  BOOST_CHECK(!GlobalState);

  InterpGen->eraseParameter("sources");
  GlobalState = PC.check();

  BOOST_CHECK(!PC.IsGeneratorParamsOk);
  BOOST_CHECK(PC.IsExtraFilesOk);
  BOOST_CHECK( PC.ExtraFilesMsg.empty());
  BOOST_CHECK(!GlobalState);

  InterpGen->setParameter("sources", "src.txt");
  GlobalState = PC.check();

  BOOST_CHECK(PC.IsParamsOk);
  BOOST_CHECK(PC.IsExtraFilesOk);
  BOOST_CHECK(PC.ExtraFilesMsg.empty());
  BOOST_CHECK(GlobalState);

  InjectpGen->setParameter("distribution", "distrib2.txt");
  GlobalState = PC.check();

  BOOST_CHECK(PC.IsGeneratorParamsOk);
  BOOST_CHECK(!PC.IsExtraFilesOk);
  BOOST_CHECK_EQUAL(
      PC.ExtraFilesMsg,
      Glib::ustring::compose(_("- File %1 required by %2 not found\n"),"distrib2.txt",InjectID));
  BOOST_CHECK(!GlobalState);
}

// =====================================================================
// =====================================================================

