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
 \file Monitoring_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_Monitoring
#include <boost/test/unit_test.hpp>

#include <openfluid/machine/ObserverSignatureRegistry.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include "BuilderTestHelper.hpp"
#include "tests-config.hpp"
#include "MonitoringModule.hpp"
#include "WareSetWidget.hpp"

// =====================================================================
// =====================================================================

class MonitoringModuleSub: public MonitoringModule
{
  public:

    MonitoringModuleSub(
        openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
        MonitoringModule(AdvancedDesc)
    {
    }

    WareSetWidget* getWareSetWidget()
    {
      return mp_MonitoringWidget;
    }

    void whenRemoveObserverAsked(std::string ID)
    {
      MonitoringModule::whenRemoveObserverAsked(ID);
    }
};

class WareSetWidgetSub: public WareSetWidget
{
  public:
    WareSetWidgetSub(std::string AddText) :
        WareSetWidget(AddText)
    {
    }

    Gtk::VBox* getListBox()
    {
      return mp_ListBox;
    }

  private:
};

class WareItemWidgetSub: public WareItemWidget
{
  public:
    WareItemWidgetSub(std::string ID, Gtk::Widget& ParamWidget,
                      Gtk::Widget& InfoWidget, std::string Description) :
        WareItemWidget(ID, ParamWidget, InfoWidget, Description)
    {
    }

    std::string getIDLabelText()
    {
      return mp_IDLabel->get_text();
    }

    std::string getDescriptionLabelText()
    {
      return mp_DescriptionLabel->get_text();
    }

    void onRemoveButtonClicked(std::string ID)
    {
      WareItemWidget::onRemoveButtonClicked(ID);
    }
};

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

struct init_Empty
{
    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;
    openfluid::fluidx::AdvancedMonitoringDescriptor* mp_Monit;

    MonitoringModuleSub* mp_Module;
    WareSetWidgetSub* mp_WareSet;

    openfluid::machine::ObserverSignatureRegistry* mp_Reg;

    init_Empty()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Reg = openfluid::machine::ObserverSignatureRegistry::getInstance();

      mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(0);
      openfluid::fluidx::AdvancedFluidXDescriptor Desc(*mp_FXDesc);

      mp_Monit = &Desc.getMonitoring();

      mp_Module = new MonitoringModuleSub(Desc);

      mp_WareSet =
          static_cast<WareSetWidgetSub*>(mp_Module->getWareSetWidget());
    }

    ~init_Empty()
    {
      delete mp_Module;
      delete mp_Monit;
      delete mp_FXDesc;
    }
};

// =====================================================================
// =====================================================================

struct init_FromFile
{
    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;
    openfluid::fluidx::AdvancedMonitoringDescriptor* mp_Monit;
    MonitoringModuleSub* mp_Module;
    WareSetWidgetSub* mp_WareSet;

    init_FromFile()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(0);
      mp_FXDesc->loadFromDirectory(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefile");
      openfluid::fluidx::AdvancedFluidXDescriptor Desc(*mp_FXDesc);

      mp_Monit = &Desc.getMonitoring();

      mp_Module = new MonitoringModuleSub(Desc);

      mp_WareSet =
          static_cast<WareSetWidgetSub*>(mp_Module->getWareSetWidget());
    }

    ~init_FromFile()
    {
      delete mp_Module;
      delete mp_Monit;
      delete mp_FXDesc;
    }
};

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_SUITE(MonitoringEmpty, init_Empty)

BOOST_AUTO_TEST_CASE(check_wareset_operations)
{
  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 0);

  mp_Monit->addToObserverList("export.vars.files.kml-plot");
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 1);

  mp_Monit->addToObserverList("export.vars.files.csv");
  mp_Monit->addToObserverList("export.spatial-graph.files.dot");
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 3);

  std::vector<Gtk::Widget*> Children = mp_WareSet->getListBox()->get_children();

  WareItemWidgetSub* WareItem = static_cast<WareItemWidgetSub*>(Children[0]);

  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(), "export.vars.files.kml-plot");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignature("export.vars.files.kml-plot").Signature->Name);

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);

  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(), "export.vars.files.csv");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignature("export.vars.files.csv").Signature->Name);

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);

  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "export.spatial-graph.files.dot");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignature("export.spatial-graph.files.dot").Signature->Name);

  /////////////
  mp_Module->whenRemoveObserverAsked("export.vars.files.csv");

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 2);

  mp_Module->whenRemoveObserverAsked("export.vars.files.kml-plot");
  mp_Module->whenRemoveObserverAsked("export.spatial-graph.files.dot");

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 0);
}
BOOST_AUTO_TEST_SUITE_END();

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_SUITE(MonitoringFromFile, init_FromFile)

BOOST_AUTO_TEST_CASE(check_wareset_operations)
{

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 2);

  mp_Monit->addToObserverList("export.vars.files.kml-plot");
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 3);
}
BOOST_AUTO_TEST_SUITE_END();
