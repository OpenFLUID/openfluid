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
 \file SimulOutFilesPresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_SimulOutFilesPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "SimulOutFilesComponent.hpp"
#include "SimulOutFilesModel.hpp"
#include "SimulOutFilesView.hpp"
#include "SimulOutFilesAdapterModel.hpp"
#include "EngineProjectFactory.hpp"
#include "../../../tests/tests-config.hpp"

// =====================================================================
// =====================================================================

struct init_Presenter
{
    SimulOutFilesComponent* mp_Component;

    SimulOutFilesModelSub* mp_Model;
    SimulOutFilesViewSub* mp_View;
    SimulOutFilesAdapterModelSub * mp_AdapterModel;

    EngineProject* mp_EngProject;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new SimulOutFilesComponent();
      mp_Model = (SimulOutFilesModelSub*) mp_Component->getModel();
      mp_View = (SimulOutFilesViewSub*) (mp_Component->getView());
      mp_AdapterModel
          = (SimulOutFilesAdapterModelSub*) (mp_Component->getAdapterModel());

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = EngineProjectFactory::createEngineProject(Path);

      mp_Model->setEngineRequirements(mp_EngProject->getOutputDescriptor());
    }

    ~init_Presenter()
    {
      delete mp_Component;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(SimulOutFilesPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

//BOOST_AUTO_TEST_CASE(test_replacedBlankStr)
//{
//  std::string Blank = mp_AdapterModel->getBlankSubstitute();
//  BOOST_CHECK_EQUAL(mp_AdapterModel->replacedBlankStr("smth"),"smth");
//  BOOST_CHECK_EQUAL(mp_AdapterModel->replacedBlankStr("  smth  "),Blank+Blank+"smth"+Blank+Blank);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->replacedBlankStr("  "),Blank+Blank);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->replacedBlankStr(" "),Blank);
//  BOOST_CHECK_EQUAL(mp_AdapterModel->replacedBlankStr(""),"");
//}

BOOST_AUTO_TEST_CASE(test_SetOutDescriptor)
{
  Gtk::TreeView* TreeView =(Gtk::TreeView*)mp_View->asWidget();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),1);

  std::string Value;

  TreeView->get_model()->children().begin()->get_value(1,Value);
  BOOST_CHECK_EQUAL(Value,"Format #1");

  TreeView->get_model()->children().begin()->get_value(2,Value);
  BOOST_CHECK_EQUAL(Value,mp_EngProject->getOutputDescriptor().getFileSets().begin()->getColSeparator());

  TreeView->get_model()->children().begin()->get_value(3,Value);
  BOOST_CHECK_EQUAL(Value,mp_EngProject->getOutputDescriptor().getFileSets().begin()->getDateFormat());

  TreeView->get_model()->children().begin()->get_value(4,Value);
  BOOST_CHECK_EQUAL(Value,mp_EngProject->getOutputDescriptor().getFileSets().begin()->getCommentChar());
}

BOOST_AUTO_TEST_CASE(test_selectFileFormat)
{
  BOOST_CHECK_EQUAL(mp_Model->getSelectedFileFormatIndex(),-1);

  mp_View->selectRowWithIndex(0);

  BOOST_CHECK_EQUAL(mp_Model->getSelectedFileFormatIndex(),0);
}

BOOST_AUTO_TEST_CASE(test_deleteFileFormat)
{
  mp_View->selectRowWithIndex(0);

  mp_Model->deleteSelectedFileFormatConfirmed();

  Gtk::TreeView* TreeView =(Gtk::TreeView*)mp_View->asWidget();
  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),0);
}

BOOST_AUTO_TEST_CASE(test_addFileFormatToEmptyFilesSet)
{
  // clear
  mp_View->selectRowWithIndex(0);
  mp_Model->deleteSelectedFileFormatConfirmed();

  Gtk::TreeView* TreeView =(Gtk::TreeView*)mp_View->asWidget();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),0);

  openfluid::base::OutputFilesDescriptor FileDesc;
  FileDesc.setColSeparator("---");
  FileDesc.setDateFormat("abcd");
  FileDesc.setCommentChar("***");

  mp_Model->addFileFormat(&FileDesc,"A Name");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),1);

  std::string Value;
  Gtk::TreeModel::Children::iterator it = TreeView->get_model()->children().begin();

  it->get_value(1,Value);
  BOOST_CHECK_EQUAL(Value,"A Name");

  it->get_value(2,Value);
  BOOST_CHECK_EQUAL(Value,"---");

  it->get_value(3,Value);
  BOOST_CHECK_EQUAL(Value,"abcd");

  it->get_value(4,Value);
  BOOST_CHECK_EQUAL(Value,"***");
}

BOOST_AUTO_TEST_CASE(test_addFileFormatToNotEmptyFilesSet)
{
  Gtk::TreeView* TreeView =(Gtk::TreeView*)mp_View->asWidget();

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),1);

  openfluid::base::OutputFilesDescriptor FileDesc;
  FileDesc.setColSeparator("---");
  FileDesc.setDateFormat("abcd");
  FileDesc.setCommentChar("***");

  mp_Model->addFileFormat(&FileDesc,"A Name");

  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),2);

  std::string Value;

  Gtk::TreeModel::Children::iterator it = TreeView->get_model()->children().begin();
  it++;

  it->get_value(1,Value);
  BOOST_CHECK_EQUAL(Value,"A Name");

  it->get_value(2,Value);
  BOOST_CHECK_EQUAL(Value,"---");

  it->get_value(3,Value);
  BOOST_CHECK_EQUAL(Value,"abcd");

  it->get_value(4,Value);
  BOOST_CHECK_EQUAL(Value,"***");
}

BOOST_AUTO_TEST_CASE(test_updateSelectedFileFormat)
{
  mp_View->selectRowWithIndex(0);

  mp_EngProject->getOutputDescriptor().getFileSets().begin()->setColSeparator("---");
  mp_EngProject->getOutputDescriptor().getFileSets().begin()->setDateFormat("abcd");
  mp_EngProject->getOutputDescriptor().getFileSets().begin()->setCommentChar("***");

  mp_Model->updateSelectedFileFormat("New Name");

  Gtk::TreeView* TreeView =(Gtk::TreeView*)mp_View->asWidget();


  BOOST_CHECK_EQUAL(TreeView->get_model()->children().size(),1);

  std::string Value;
  Gtk::TreeModel::Children::iterator it = TreeView->get_model()->children().begin();

  it->get_value(1,Value);
  BOOST_CHECK_EQUAL(Value,"New Name");

  it->get_value(2,Value);
  BOOST_CHECK_EQUAL(Value,"---");

  it->get_value(3,Value);
  BOOST_CHECK_EQUAL(Value,"abcd");

  it->get_value(4,Value);
  BOOST_CHECK_EQUAL(Value,"***");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()
