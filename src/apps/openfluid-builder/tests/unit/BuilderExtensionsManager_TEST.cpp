/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
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


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  BOOST_REQUIRE_EQUAL(BEM->getRegisteredExtensions()->size(),0);
  BOOST_REQUIRE_EQUAL(BEM->getExtensionsSearchPaths().size(),0);

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
      std::cerr << "File : " << (*ECMit).second.Filename << std::endl;
      std::cerr << " * ID " << (*ECMit).second.Infos.ID << std::endl;
      std::cerr << " * Type " << BuilderExtensionsManager::getExtensionTypeAsString((*ECMit).second.Extension->getType()) << std::endl;
      std::cerr << " * Ready? " << (*ECMit).second.Extension->isReadyForShowtime() << std::endl;
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

  openfluid::machine::SimulationBlob TheBlob;


  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  BEM->prependExtensionsSearchPaths(TESTSBUILDERCONFIG_OUTPUT_BINARY_DIR+";"+openfluid::base::RuntimeEnvironment::getInstance()->getUserDataPath(BUILDER_EXTSDIR));

  BEM->registerExtensions();

  BOOST_REQUIRE_GT(BEM->getRegisteredExtensions()->size(),0);
  BOOST_REQUIRE_GE(BEM->getRegisteredExtensionsCount(),10);

  BOOST_REQUIRE_GE(BEM->getRegisteredExtensions(openfluid::builderext::PluggableBuilderExtension::WorkspaceTab)->size(),1);
  BOOST_REQUIRE_GE(BEM->getRegisteredExtensions(openfluid::builderext::PluggableBuilderExtension::SimulationListener)->size(),1);
  BOOST_REQUIRE_GE(BEM->getRegisteredExtensions(openfluid::builderext::PluggableBuilderExtension::ModelessWindow)->size(),1);

  BEM->linkRegisteredExtensionsWithSimulationBlobAndModel(&TheBlob, NULL);

  DisplayRegisteredExtensions(BEM->getRegisteredExtensions());

  BEM->unlinkRegisteredExtensionsWithSimulationBlobAndModel();

  DisplayRegisteredExtensions(BEM->getRegisteredExtensions());

}
