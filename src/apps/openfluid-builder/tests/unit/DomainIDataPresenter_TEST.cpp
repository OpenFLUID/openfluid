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
 \file DomainIDataPresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_DomainIDataPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "DomainIDataComponent.hpp"
#include "DomainIDataModel.hpp"
#include "DomainIDataView.hpp"
#include "EngineProject.hpp"
#include "tests-config.hpp"
#include <openfluid/guicommon/BuilderDescriptor.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    DomainIDataComponent* mp_Component;

    DomainIDataModelImpl* mp_Model;
    DomainIDataViewSub* mp_View;

    EngineProject* mp_EngProject;
    openfluid::guicommon::BuilderDomain* mp_Domain;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      std::string Path = CONFIGTESTS_INPUT_DATASETS_DIR
          + "/OPENFLUID.IN.Primitives";
      mp_EngProject = new EngineProject(Path);

      mp_Domain = &mp_EngProject->getBuilderDesc().getDomain();

      mp_Component = new DomainIDataComponent(*mp_Domain);
      mp_Model = (DomainIDataModelImpl*) (mp_Component->getModel());
      mp_View = (DomainIDataViewSub*) (mp_Component->getView());
    }

    ~init_Presenter()
    {
      delete mp_Component;
      delete mp_EngProject;
    }
};

BOOST_FIXTURE_TEST_SUITE(DomainIDataPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_constructors)
{
  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 4);
  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_model()->children().size(),
                    mp_Domain->getIDsOfClass("TestUnits").size());

  int Id;
  std::string DataValFromView;

  mp_View->getTreeView()->get_model()->children()[0]->get_value(0, Id);
  mp_View->getTreeView()->get_model()->children()[0]->get_value(
      1, DataValFromView);

  BOOST_CHECK_EQUAL(mp_Domain->getInputData("TestUnits",Id,"indataA"),
                    DataValFromView);

  mp_View->getTreeView()->get_model()->children()[5]->get_value(0, Id);
  mp_View->getTreeView()->get_model()->children()[5]->get_value(
      3, DataValFromView);

  BOOST_CHECK_EQUAL(mp_Domain->getInputData("TestUnits",Id,"indataC"),
                    DataValFromView);

  mp_Model->setClass("ParentTestUnits");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 0);
  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_model()->children().size(), 2);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_removeData)
{
  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 4);

  mp_Model->removeData("indataB");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 3);

  mp_Model->removeData("indataA");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 2);

  BOOST_CHECK_THROW(mp_Model->removeData("wrongIndata"),
                    openfluid::base::OFException);

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 2);

  mp_Model->removeData("indataC");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 0);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_addData)
{
  mp_Model->setClass("TestUnits");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 4);

  mp_Model->addData("NewIndata", "DefaultVal");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 5);
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_changeDataName)
{
  mp_Model->setClass("TestUnits");

  mp_Model->changeDataName("indataB", "newDataB");

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 4);

  std::vector<Gtk::TreeViewColumn*> Columns =
      mp_View->getTreeView()->get_columns();

  std::set<std::string> Titles;

  for (unsigned int i = 0; i < Columns.size(); i++)
  {
    if (Columns[i]->get_widget())
      Titles.insert(
          (static_cast<Gtk::Label*>(Columns[i]->get_widget()))->get_text());
  }

  BOOST_CHECK_EQUAL(mp_View->getTreeView()->get_columns().size(), 4);
  BOOST_CHECK_EQUAL(Titles.find("indataB") != Titles.end(), false);
  BOOST_CHECK_EQUAL(Titles.find("newDataB") != Titles.end(), true);
}
// =====================================================================
// =====================================================================
BOOST_AUTO_TEST_SUITE_END()
