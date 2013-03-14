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
 \file ModelAvailFctPresenter_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE builder_unittest_ModelAvailFctPresenter
#include <boost/test/unit_test.hpp>

#include "BuilderTestHelper.hpp"
#include "ModelAvailFctComponent.hpp"
#include "ModelAvailFctModel.hpp"
#include "ModelAvailFctView.hpp"

#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/FunctionSignatureRegistry.hpp>

// =====================================================================
// =====================================================================

struct init_Presenter
{
    ModelAvailFctComponent* mp_Component;

    ModelAvailFctModel* mp_Model;
    ModelAvailFctViewSub* mp_View;

    init_Presenter()
    {
      BuilderTestHelper::getInstance()->initGtk();

      mp_Component = new ModelAvailFctComponent();

      mp_Model = mp_Component->getModel();
      mp_View = (ModelAvailFctViewSub*) (mp_Component->getView());
    }

    ~init_Presenter()
    {
      delete mp_Component;
    }
};

BOOST_FIXTURE_TEST_SUITE(ModelAvailFctPresenterTest, init_Presenter)

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(test_SetSignatures)
{
  BOOST_CHECK_EQUAL(mp_View->getPluggableBranchRowsNumber(),0);
  BOOST_CHECK_EQUAL(mp_View->getGeneratorBranchRowsNumber(),0);

  // set Generator signatures
  openfluid::ware::FunctionSignatureRegistrySub Signatures;
  int GeneratorCount = Signatures.getGeneratorSignatures().size();
  mp_Model->setSignatures(Signatures);

  BOOST_CHECK_EQUAL(mp_View->getPluggableBranchRowsNumber(),0);
  BOOST_CHECK_EQUAL(mp_View->getGeneratorBranchRowsNumber(),GeneratorCount);

  // set dummy Pluggable signatures
  for(int i=0; i<3; i++)
  {
    openfluid::machine::ModelItemSignatureInstance* Plug = new openfluid::machine::ModelItemSignatureInstance();
    openfluid::ware::FunctionSignature* PlugSignature = new openfluid::ware::FunctionSignature();
    PlugSignature->ID = i;
    Plug->Signature = PlugSignature;
    Signatures.addAPluggableSignature(Plug);
  }
  mp_Model->setSignatures(Signatures);

  BOOST_CHECK_EQUAL(mp_View->getPluggableBranchRowsNumber(),3);
  BOOST_CHECK_EQUAL(mp_View->getGeneratorBranchRowsNumber(),GeneratorCount);

  Signatures.clearPluggableSignatures();
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_SUITE_END()
