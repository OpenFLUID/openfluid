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
  \file BuilderExtensionManager_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_builderextensionsmanager
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>

#include "tests-builderconfig.hpp"
#include "builderconfig.hpp"
#include "BuilderTestHelper.hpp"

#include "BuilderExtensionsManager.hpp"

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/builderext/ModelessWindow.hpp>

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_construction)
{
  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  BOOST_REQUIRE_EQUAL(BEM->getRegisteredExtensions()->size(),0);
  BOOST_REQUIRE_EQUAL(BEM->getExtensionsSearchPaths().size(),2);

  BOOST_REQUIRE_EQUAL(BEM->isRegistrationDone(),false);
}

// =====================================================================
// =====================================================================

void DisplayRegisteredExtensions(CollectionOfExtensions_t* ExtColl)
{
  CollectionOfExtensions_t::iterator COEit;
  ExtensionContainerMap_t::iterator ECMit;

  std::cerr << " #################################### " << std::endl;
  for (COEit = ExtColl->begin(); COEit!= ExtColl->end(); ++COEit)
  {
    for (ECMit = (*COEit).second.begin(); ECMit!= (*COEit).second.end(); ++ECMit)
    {
      bool Instantiated = (*ECMit).second.Extension;
      std::cerr << "File : " << (*ECMit).second.Filename << std::endl;
      std::cerr << " * ID " << (*ECMit).second.Infos.ID << std::endl;
      std::cerr << " * Type from Infos " << BuilderExtensionsManager::getExtensionTypeAsString((*ECMit).second.Infos.Type) << std::endl;
      std::cerr << " * Instantiated? " << Instantiated << std::endl;
      if(Instantiated)
      {
        std::cerr << " ** Type of Extension" << BuilderExtensionsManager::getExtensionTypeAsString((*ECMit).second.Extension->getType()) << std::endl;
        std::cerr << " ** Ready? " << (*ECMit).second.Extension->isReadyForShowtime() << std::endl;
      }
      std::cerr << " * Shortname " << (*ECMit).second.Infos.ShortName << std::endl;
      std::cerr << " * Name " << (*ECMit).second.Infos.Name << std::endl;
      std::cerr << " * Description " << (*ECMit).second.Infos.Description << std::endl;
      std::cerr << " * Authors " << (*ECMit).second.Infos.Authors << std::endl;
      std::cerr << " * Authors contacts " << (*ECMit).second.Infos.AuthorsContact << std::endl;
    }
  }
  std::cerr << " #################################### " << std::endl;
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{
  BuilderTestHelper::getInstance()->initGtk();

  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  BOOST_CHECK_EQUAL(BEM->getExtensionsDefaultSearchPaths().size(),2);
  BOOST_CHECK_EQUAL(BEM->getExtensionsExtraSearchPaths().size(),0);
  BOOST_CHECK_EQUAL(BEM->getExtensionsSearchPaths().size(),2);

  BEM->prependExtensionsSearchPaths(TESTSBUILDERCONFIG_OUTPUT_BINARY_DIR);

  BOOST_CHECK_EQUAL(BEM->getExtensionsDefaultSearchPaths().size(),2);
  BOOST_CHECK_EQUAL(BEM->getExtensionsExtraSearchPaths().size(),1);
  BOOST_REQUIRE_EQUAL(BEM->getExtensionsSearchPaths().size(),3);

  std::list<std::string> ExtPaths = BEM->getExtensionsSearchPaths();
  BOOST_CHECK_EQUAL(*ExtPaths.begin(),TESTSBUILDERCONFIG_OUTPUT_BINARY_DIR);

  BEM->prependExtensionsSearchPaths("aa/bb/cc");

  BOOST_CHECK_EQUAL(BEM->getExtensionsDefaultSearchPaths().size(),2);
  BOOST_CHECK_EQUAL(BEM->getExtensionsExtraSearchPaths().size(),2);
  BOOST_REQUIRE_EQUAL(BEM->getExtensionsSearchPaths().size(),4);

  ExtPaths = BEM->getExtensionsSearchPaths();
  BOOST_CHECK_EQUAL(*ExtPaths.begin(),"aa/bb/cc");

  BEM->registerExtensions();

  BOOST_REQUIRE_GT(BEM->getRegisteredExtensions()->size(),0);
  BOOST_REQUIRE_GE(BEM->getRegisteredExtensionsCount(),9);

  BOOST_REQUIRE_GE(BEM->getRegisteredExtensions(openfluid::builderext::PluggableBuilderExtension::WorkspaceTab)->size(),1);
  BOOST_REQUIRE_GE(BEM->getRegisteredExtensions(openfluid::builderext::PluggableBuilderExtension::ModelessWindow)->size(),1);

  DisplayRegisteredExtensions(BEM->getRegisteredExtensions());

  BOOST_CHECK(!BEM->getExtensionContainer("wrong.extension"));
  BOOST_CHECK(!BEM->instantiatePluggableExtension("wrong.extension"));

  ExtensionContainer* WrongtypeContainer = BEM->getExtensionContainer("tests.builder.wrongtypeextension");
  BOOST_CHECK(WrongtypeContainer);
  BOOST_CHECK(!WrongtypeContainer->Extension);
  BOOST_CHECK_THROW(WrongtypeContainer->instantiateExt(),openfluid::base::OFException);
  BOOST_CHECK_THROW(BEM->instantiatePluggableExtension("tests.builder.wrongtypeextension"),openfluid::base::OFException);
  BOOST_CHECK(!WrongtypeContainer->Extension);

  ExtensionContainer* AssistantContainer = BEM->getExtensionContainer("tests.builder.assistant");
  BOOST_CHECK(AssistantContainer);
  BOOST_CHECK(!AssistantContainer->Extension);
  BOOST_CHECK(BEM->instantiatePluggableExtension("tests.builder.assistant"));
  BOOST_CHECK(!AssistantContainer->instantiateExt());
  BOOST_CHECK(!BEM->instantiatePluggableExtension("tests.builder.assistant"));
  BOOST_CHECK(AssistantContainer->Extension);
  BOOST_CHECK_EQUAL(AssistantContainer->Extension->getType(),AssistantContainer->Infos.Type);

  DisplayRegisteredExtensions(BEM->getRegisteredExtensions());

  BEM->deletePluggableExtension("tests.builder.assistant");
  BOOST_CHECK(!AssistantContainer->Extension);

  DisplayRegisteredExtensions(BEM->getRegisteredExtensions());

  BOOST_CHECK(BEM->instantiatePluggableExtension("tests.builder.assistant"));
  BOOST_CHECK(!AssistantContainer->instantiateExt());
  BOOST_CHECK(!BEM->instantiatePluggableExtension("tests.builder.assistant"));
  BOOST_CHECK(AssistantContainer->Extension);
  BOOST_CHECK_EQUAL(AssistantContainer->Extension->getType(),AssistantContainer->Infos.Type);
  BOOST_CHECK_EQUAL(AssistantContainer->Extension->getConfiguration().size(),0);

  DisplayRegisteredExtensions(BEM->getRegisteredExtensions());

  BEM->deletePluggableExtension("tests.builder.assistant");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_prefsOperations)
{
  BuilderTestHelper::getInstance()->initGtk();

  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  BEM->prependExtensionsSearchPaths(TESTSBUILDERCONFIG_OUTPUT_BINARY_DIR);
  BEM->registerExtensions();

  BOOST_CHECK_EQUAL(BEM->isPreferencesInstantiationDone(), false);
  BOOST_CHECK_EQUAL(BEM->getRegisteredExtensionPreferences()->size(),0);

  BEM->instantiateRegisteredExtensionPreferences();

  BOOST_CHECK_EQUAL(BEM->isPreferencesInstantiationDone(), true);
  BOOST_CHECK_GE(BEM->getRegisteredExtensionPreferences()->size(),1);

  BEM->deleteRegisteredExtensionPreferences();

  BOOST_CHECK_EQUAL(BEM->isPreferencesInstantiationDone(), false);
  BOOST_CHECK_GE(BEM->getRegisteredExtensionPreferences()->size(),0);
}

// =====================================================================
// =====================================================================
