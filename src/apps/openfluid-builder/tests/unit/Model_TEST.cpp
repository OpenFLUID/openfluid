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
 \file Model_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_Model
#include <boost/test/unit_test.hpp>

#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/fluidx/SimulatorDescriptor.hpp>
#include <openfluid/fluidx/GeneratorDescriptor.hpp>
#include <openfluid/machine/Factory.hpp>
#include "BuilderTestHelper.hpp"
#include "tests-config.hpp"
#include "ModelModule.hpp"
#include "WareSetWidget.hpp"
#include "SimulatorParamWidget.hpp"
#include "GlobalParamsWidget.hpp"

// =====================================================================
// =====================================================================

class ModelModuleSub: public ModelModule
{
  public:

    ModelModuleSub(openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
        ModelModule(AdvancedDesc)
    {
    }

    WareSetWidget* getWareSetWidget()
    {
      return mp_ModelWidget;
    }

    GlobalParamsWidget* getGlobalParamsWidget()
    {
      return mp_GlobalParamsWidget;
    }

    std::list<SimulatorParamWidget*> getParamWidgets()
    {
      return m_ParamWidgets;
    }

    void whenRemoveSimulatorAsked(std::string ID)
    {
      ModelModule::whenRemoveSimulatorAsked(ID);
    }
};

// =====================================================================
// =====================================================================

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
};

// =====================================================================
// =====================================================================

class WareItemWidgetSub: public WareItemWidget
{
  public:
    WareItemWidgetSub(std::string ID, Gtk::Widget& ParamWidget,
                      Gtk::Widget& InfoWidget, std::string Description) :
        WareItemWidget(ID, ParamWidget, InfoWidget, Description)
    {
    }

    std::string getID()
    {
      return m_ID;
    }

    std::string getIDLabelText()
    {
      return mp_IDLabel->get_text();
    }

    std::string getDescriptionLabelText()
    {
      return mp_DescriptionLabel->get_text();
    }

    BuilderItemButtonBox* getButtonBox()
    {
      return mp_ButtonBox;
    }

    void onRemoveButtonClicked(std::string ID)
    {
      WareItemWidget::onRemoveButtonClicked(ID);
    }

    void onUpButtonClicked(std::string ID)
    {
      WareItemWidget::onUpButtonClicked(ID);
    }

    void onDownButtonClicked(std::string ID)
    {
      WareItemWidget::onDownButtonClicked(ID);
    }
};

// =====================================================================
// =====================================================================

class GlobalParamsWidgetSub: public GlobalParamsWidget
{
  public:

    std::map<std::string, GlobalParamRow*> getRows()
    {
      return m_Rows;
    }

    void onStructureChangeOccured()
    {
      GlobalParamsWidget::onStructureChangeOccured();
    }
};

// =====================================================================
// =====================================================================

class GlobalParamRowSub: public GlobalParamRow
{
  public:

    std::string getName()
    {
      return m_Name;
    }

    Gtk::Entry* getValueEntry()
    {
      return mp_ValueEntry;
    }

    void onRemoveButtonClicked()
    {
      GlobalParamRow::onRemoveButtonClicked();
    }
};

// =====================================================================
// =====================================================================

class SimulatorParamWidgetSub: public SimulatorParamWidget
{
  public:

    std::map<std::string, SimulatorParamRow*> getParamsRows()
    {
      return m_ParamsRows;
    }
};

// =====================================================================
// =====================================================================

class SimulatorParamRowSub: public SimulatorParamRow
{
  public:

    std::string getName()
    {
      return m_Name;
    }

    Gtk::Entry* getValueEntry()
    {
      return mp_ValueEntry;
    }

    std::string getGlobalLabel()
    {
      return mp_GlobalLabel->get_text();
    }
};

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

struct init_Empty
{
    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;
    openfluid::fluidx::AdvancedModelDescriptor* mp_Model;

    ModelModuleSub* mp_Module;
    WareSetWidgetSub* mp_WareSet;

    openfluid::machine::SimulatorSignatureRegistry* mp_Reg;

    init_Empty()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Reg = openfluid::machine::SimulatorSignatureRegistry::getInstance();

      mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(0);
      openfluid::fluidx::AdvancedFluidXDescriptor Desc(*mp_FXDesc);

      mp_Model = &Desc.getModel();

      mp_Module = new ModelModuleSub(Desc);

      mp_WareSet =
          static_cast<WareSetWidgetSub*>(mp_Module->getWareSetWidget());
    }

    ~init_Empty()
    {
      delete mp_Module;
      delete mp_Model;
      delete mp_FXDesc;
    }
};

// =====================================================================
// =====================================================================

struct init_FromFile
{
    openfluid::fluidx::FluidXDescriptor* mp_FXDesc;
    openfluid::fluidx::AdvancedModelDescriptor* mp_Model;

    ModelModuleSub* mp_Module;
    WareSetWidgetSub* mp_WareSet;

    openfluid::machine::SimulatorSignatureRegistry* mp_Reg;

    init_FromFile()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Reg = openfluid::machine::SimulatorSignatureRegistry::getInstance();

      mp_FXDesc = new openfluid::fluidx::FluidXDescriptor(0);
      mp_FXDesc->loadFromDirectory(
          CONFIGTESTS_INPUT_DATASETS_DIR + "/OPENFLUID.IN.AdvancedDescriptors/singlefileForModel");
      openfluid::fluidx::AdvancedFluidXDescriptor Desc(*mp_FXDesc);

      mp_Model = &Desc.getModel();

      mp_Module = new ModelModuleSub(Desc);

      mp_WareSet =
          static_cast<WareSetWidgetSub*>(mp_Module->getWareSetWidget());
    }

    ~init_FromFile()
    {
      delete mp_Module;
      delete mp_Model;
      delete mp_FXDesc;
    }
};

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_SUITE(ModelEmpty, init_Empty)

BOOST_AUTO_TEST_CASE(check_wareset_updateAfterAdd)
{
  std::vector<Gtk::Widget*> Children;
  WareItemWidgetSub* WareItem;

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 0);

  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.prod"));
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 1);

  Children = mp_WareSet->getListBox()->get_children();
  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "examples.primitives.unitsA.prod");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance("examples.primitives.unitsA.prod")->Signature->Name);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.up"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsB.prod"));
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 3);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "examples.primitives.unitsA.prod");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance("examples.primitives.unitsA.prod")->Signature->Name);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "examples.primitives.unitsA.up");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance("examples.primitives.unitsA.up")->Signature->Name);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "examples.primitives.unitsB.prod");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance("examples.primitives.unitsB.prod")->Signature->Name);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_wareset_remove)
{
  std::vector<Gtk::Widget*> Children;
  WareItemWidgetSub* WareItem;
  WareItemWidgetSub* ItemToRemove;

  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.prod"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.up"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsB.prod"));
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 3);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  ItemToRemove = static_cast<WareItemWidgetSub*>(Children[1]);
  ItemToRemove->onRemoveButtonClicked(ItemToRemove->getID());

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 2);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  ItemToRemove = static_cast<WareItemWidgetSub*>(Children[0]);
  ItemToRemove->onRemoveButtonClicked(ItemToRemove->getID());
  ItemToRemove = static_cast<WareItemWidgetSub*>(Children[1]);
  ItemToRemove->onRemoveButtonClicked(ItemToRemove->getID());

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 0);
}
// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_wareset_move)
{
  std::vector<Gtk::Widget*> Children;
  WareItemWidgetSub* WareItem;
  WareItemWidgetSub* ItemToMove;

  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.prod"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.up"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsB.prod"));
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 3);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  ItemToMove = static_cast<WareItemWidgetSub*>(Children[1]);
  ItemToMove->onUpButtonClicked(ItemToMove->getID());

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  ItemToMove = static_cast<WareItemWidgetSub*>(Children[0]);
  ItemToMove->onDownButtonClicked(ItemToMove->getID());

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  ItemToMove = static_cast<WareItemWidgetSub*>(Children[2]);
  ItemToMove->onUpButtonClicked(ItemToMove->getID());

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_globalParameters)
{
  std::map<std::string, GlobalParamRow*> GlobalRows;

  GlobalParamsWidgetSub* GlobalWidget =
      static_cast<GlobalParamsWidgetSub*>(mp_Module->getGlobalParamsWidget());

  std::list<SimulatorParamWidget*> ParamWidgets;
  std::list<SimulatorParamWidget*>::iterator it;

  GlobalRows = GlobalWidget->getRows();
  BOOST_CHECK_EQUAL(GlobalRows.size(), 0);

  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.prod"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsA.up"));
  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsB.prod"));
  mp_Module->update();

  GlobalRows =
      static_cast<GlobalParamsWidgetSub*>(mp_Module->getGlobalParamsWidget())->getRows();
  BOOST_CHECK_EQUAL(GlobalRows.size(), 0);

  ParamWidgets = mp_Module->getParamWidgets();
  BOOST_CHECK_EQUAL(ParamWidgets.size(), 3);

  it = ParamWidgets.begin();
  BOOST_CHECK_EQUAL(
      (static_cast<SimulatorParamWidgetSub*>(*it))->getParamsRows().size(), 0);
  it++;
  SimulatorParamWidgetSub* ParamsUp = static_cast<SimulatorParamWidgetSub*>(*it);
  BOOST_CHECK_EQUAL(ParamsUp->getParamsRows().size(), 1);
  SimulatorParamRowSub* ParamUp =
      static_cast<SimulatorParamRowSub*>(ParamsUp->getParamsRows().begin()->second);
  BOOST_CHECK_EQUAL(ParamUp->getName(), "gmult");
  BOOST_CHECK_EQUAL(ParamUp->getValueEntry()->get_text(), "");
  BOOST_CHECK_EQUAL(ParamUp->getGlobalLabel(), "");

  // change a Param value

  ParamUp->getValueEntry()->set_text("123");
  BOOST_CHECK_EQUAL(mp_Model->getItemAt(1)->getParameters()["gmult"].get(),
                    "123");

  it++;
  BOOST_CHECK_EQUAL(
      (static_cast<SimulatorParamWidgetSub*>(*it))->getParamsRows().size(), 0);

  // add a Global param

  mp_Model->setGlobalParameter("gmult", "");
  GlobalWidget->onStructureChangeOccured();

  GlobalRows = GlobalWidget->getRows();
  BOOST_CHECK_EQUAL(GlobalRows.size(), 1);
  GlobalParamRowSub* GlobalRow =
      static_cast<GlobalParamRowSub*>(GlobalRows.begin()->second);
  BOOST_CHECK_EQUAL(GlobalRow->getName(), "gmult");
  BOOST_CHECK_EQUAL(GlobalRow->getValueEntry()->get_text(), "");

  BOOST_CHECK_EQUAL(mp_Model->getGlobalParameters()["gmult"].get(), "");
  BOOST_CHECK_EQUAL(ParamUp->getValueEntry()->get_text(), "123");
  BOOST_CHECK_EQUAL(ParamUp->getGlobalLabel(), "");

  // change global Param value

  GlobalRow->getValueEntry()->set_text("456");
  BOOST_CHECK_EQUAL(mp_Model->getGlobalParameters()["gmult"].get(), "456");
  BOOST_CHECK_EQUAL(ParamUp->getValueEntry()->get_text(), "123");
  BOOST_CHECK_EQUAL(
      ParamUp->getGlobalLabel(),
      Glib::ustring::compose("(%1 \"%2\")", _("global value:"), "456"));

  // remove Global param

  GlobalRow->onRemoveButtonClicked();
  BOOST_CHECK_EQUAL(mp_Model->getGlobalParameters().size(), 0);
  BOOST_CHECK_EQUAL(ParamUp->getValueEntry()->get_text(), "123");
  BOOST_CHECK_EQUAL(ParamUp->getGlobalLabel(), "");
}
BOOST_AUTO_TEST_SUITE_END();

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

BOOST_FIXTURE_TEST_SUITE(ModelFromFile, init_FromFile)

BOOST_AUTO_TEST_CASE(check_wareset_operations)
{
  std::vector<Gtk::Widget*> Children;
  WareItemWidgetSub* WareItem;

  std::string InterpID = openfluid::machine::Factory::buildGeneratorID(
      "tests.generator.interp", false, "TU");
  std::string FixedID = openfluid::machine::Factory::buildGeneratorID(
      "tests.generator.fixed", true, "TU");
  std::string RandomID = openfluid::machine::Factory::buildGeneratorID(
      "tests.generator.random", false, "TU");

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 5);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), InterpID);
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(), InterpID);
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance(openfluid::fluidx::GeneratorDescriptor::getGeneratorName( openfluid::fluidx::GeneratorDescriptor::Interp))->Signature->Name);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "examples.primitives.unitsA.prod");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance("examples.primitives.unitsA.prod")->Signature->Name);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), FixedID);
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(), FixedID);
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance(openfluid::fluidx::GeneratorDescriptor::getGeneratorName( openfluid::fluidx::GeneratorDescriptor::Fixed))->Signature->Name);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[3]);
  BOOST_CHECK_EQUAL(WareItem->getID(), RandomID);
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(), RandomID);
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance(openfluid::fluidx::GeneratorDescriptor::getGeneratorName( openfluid::fluidx::GeneratorDescriptor::Random))->Signature->Name);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[4]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.up");
  BOOST_CHECK_EQUAL(WareItem->getIDLabelText(),
                    "examples.primitives.unitsA.up");
  BOOST_CHECK_EQUAL(
      WareItem->getDescriptionLabelText(),
      mp_Reg->getSignatureItemInstance("examples.primitives.unitsA.up")->Signature->Name);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem->onRemoveButtonClicked(WareItem->getID());

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 4);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), InterpID);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), FixedID);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[3]);
  BOOST_CHECK_EQUAL(WareItem->getID(), RandomID);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  WareItem->onRemoveButtonClicked(WareItem->getID());

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 3);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), InterpID);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), RandomID);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  mp_Model->appendItem(
      new openfluid::fluidx::SimulatorDescriptor(
          "examples.primitives.unitsB.prod"));
  mp_Module->update();

  BOOST_CHECK_EQUAL(mp_WareSet->getListBox()->get_children().size(), 4);

  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), InterpID);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), RandomID);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[3]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem->onUpButtonClicked(WareItem->getID());
  Children = mp_WareSet->getListBox()->get_children();

  WareItem = static_cast<WareItemWidgetSub*>(Children[0]);
  BOOST_CHECK_EQUAL(WareItem->getID(), InterpID);
  BOOST_CHECK(!WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[1]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsA.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[2]);
  BOOST_CHECK_EQUAL(WareItem->getID(), "examples.primitives.unitsB.prod");
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(WareItem->getButtonBox()->isDownCommandAvailable());

  WareItem = static_cast<WareItemWidgetSub*>(Children[3]);
  BOOST_CHECK_EQUAL(WareItem->getID(), RandomID);
  BOOST_CHECK(WareItem->getButtonBox()->isUpCommandAvailable());
  BOOST_CHECK(!WareItem->getButtonBox()->isDownCommandAvailable());
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_globalParameters)
{
  // globals

  GlobalParamsWidgetSub* GlobalWidget =
      static_cast<GlobalParamsWidgetSub*>(mp_Module->getGlobalParamsWidget());

  std::map<std::string, GlobalParamRow*> GlobalRows = GlobalWidget->getRows();
  BOOST_CHECK_EQUAL(GlobalRows.size(), 2);
  GlobalParamRowSub* Global1 =
      static_cast<GlobalParamRowSub*>(GlobalRows.begin()->second);
  BOOST_CHECK_EQUAL(Global1->getName(), "gparam1");
  BOOST_CHECK_EQUAL(Global1->getValueEntry()->get_text(), "100");
  GlobalParamRowSub* Global2 =
      static_cast<GlobalParamRowSub*>(GlobalRows.begin().operator ++()->second);
  BOOST_CHECK_EQUAL(Global2->getName(), "gparam2");
  BOOST_CHECK_EQUAL(Global2->getValueEntry()->get_text(), "0.1");

  // params

  std::list<SimulatorParamWidget*> ParamWidgets = mp_Module->getParamWidgets();

  BOOST_CHECK_EQUAL(ParamWidgets.size(), 5);

  std::list<SimulatorParamWidget*>::iterator it;
  SimulatorParamWidgetSub* ParamWidget;

  std::map<std::string, SimulatorParamRow*> ParamRows;
  SimulatorParamRowSub* ParamRow;

  // // interp

  it = ParamWidgets.begin();
  ParamWidget = static_cast<SimulatorParamWidgetSub*>(*it);
  ParamRows = ParamWidget->getParamsRows();
  BOOST_CHECK_EQUAL(ParamRows.size(), 4);
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["distribution"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "distribution");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "distri.dat");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["sources"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "sources");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "sources.xml");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["thresholdmax"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "thresholdmax");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["thresholdmin"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "thresholdmin");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");

  // // A.prod

  it++;
  ParamWidget = static_cast<SimulatorParamWidgetSub*>(*it);
  ParamRows = ParamWidget->getParamsRows();
  BOOST_CHECK_EQUAL(ParamRows.size(), 0);

  // // fixed

  it++;
  ParamWidget = static_cast<SimulatorParamWidgetSub*>(*it);
  ParamRows = ParamWidget->getParamsRows();
  BOOST_CHECK_EQUAL(ParamRows.size(), 1);
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["fixedvalue"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "fixedvalue");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "20");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");

  // // random

  it++;
  ParamWidget = static_cast<SimulatorParamWidgetSub*>(*it);
  ParamRows = ParamWidget->getParamsRows();
  BOOST_CHECK_EQUAL(ParamRows.size(), 2);
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["max"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "max");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "50");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["min"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "min");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "20.53");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");

  // // A.up

  it++;
  ParamWidget = static_cast<SimulatorParamWidgetSub*>(*it);
  ParamRows = ParamWidget->getParamsRows();
  BOOST_CHECK_EQUAL(ParamRows.size(), 5);
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["gmult"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "gmult");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["strparam"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "strparam");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "strvalue");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["doubleparam"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "doubleparam");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "1.1");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["longparam"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "longparam");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "11");
  BOOST_CHECK_EQUAL(ParamRow->getGlobalLabel(), "");
  ParamRow = static_cast<SimulatorParamRowSub*>(ParamRows["gparam1"]);
  BOOST_CHECK_EQUAL(ParamRow->getName(), "gparam1");
  BOOST_CHECK_EQUAL(ParamRow->getValueEntry()->get_text(), "50");
  BOOST_CHECK_EQUAL(
      ParamRow->getGlobalLabel(),
      Glib::ustring::compose("(%1 \"%2\")", _("global value:"), "100"));
}
BOOST_AUTO_TEST_SUITE_END();

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

