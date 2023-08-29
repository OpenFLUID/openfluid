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
  @file SignatureEditor_TEST.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_SignatureEditor


#include <iostream>

#include <QCoreApplication>
#include <QString>
#include <QDir>
#include <QTextStream>

#include <boost/test/unit_test.hpp>

#include <openfluid/ui/common/WareshubIssueDialog.hpp>

#include "tests-config.hpp"
#include "SimulatorSignatureUtils.hpp"
#include "SignatureCommon.hpp"


BOOST_AUTO_TEST_CASE(check_consistency)
{
  auto Sign = getRefSignature();
  auto WareIssue = Sign.Issues.get(101);
  auto QtIssue = openfluid::ui::common::WareshubIssueDialog::Issue(WareIssue);
  auto WareIssueFromQt = QtIssue.toWareIssue();

  compareIssues(WareIssue, WareIssueFromQt);
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  
  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
