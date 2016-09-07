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
 @file CMakeProxy_TEST.cpp

 @author Jean-Christophe Fabre <jean-christophe.fabre@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_cmakeproxy
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <iostream>
#include <openfluid/utils/CMakeProxy.hpp>
#include "tests-config.hpp"


BOOST_AUTO_TEST_CASE(check_init)
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    std::cout << openfluid::utils::CMakeProxy::getVersion().toStdString() << std::endl;

    BOOST_REQUIRE(!openfluid::utils::CMakeProxy::getVersion().isEmpty());
  }
  else
    std::cout << "** Test not run due to failing to find cmake program **" << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_configurecmd)
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    QString Cmd;

    Cmd = openfluid::utils::CMakeProxy::getConfigureCommand("/tmp/build","/tmp/src");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("\"/tmp/src\""));


    Cmd = openfluid::utils::CMakeProxy::getConfigureCommand("/tmp/build","/tmp/src",
                                                   {{"CMAKE_INSTALL_PREFIX","/usr"},{"CMAKE_BUILD_TYPE","release"}});
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("\"/tmp/src\" -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr"));


    Cmd = openfluid::utils::CMakeProxy::getConfigureCommand("/tmp/build","/tmp/src",
                                                   {{"CMAKE_INSTALL_PREFIX","/usr"},{"CMAKE_BUILD_TYPE","release"}},
                                                   "Ninja");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("\"/tmp/src\" -G \"Ninja\" -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr"));


    Cmd = openfluid::utils::CMakeProxy::getConfigureCommand("/tmp/build","/tmp/src",
                                                   {{"CMAKE_INSTALL_PREFIX","/usr"},{"CMAKE_BUILD_TYPE","release"}},
                                                   "Ninja",{"-Wdev","--trace"});
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(
        Cmd.endsWith("\"/tmp/src\" -G \"Ninja\" -DCMAKE_BUILD_TYPE=release -DCMAKE_INSTALL_PREFIX=/usr -Wdev --trace"));
  }
  else
    std::cout << "** Test not run due to failing to find cmake program **" << std::endl;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_buildcmd)
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    QString Cmd;

    Cmd = openfluid::utils::CMakeProxy::getBuildCommand("/tmp/build");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("--build ."));

    Cmd = openfluid::utils::CMakeProxy::getBuildCommand("/tmp/build","install");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("--build . --target install"));

    Cmd = openfluid::utils::CMakeProxy::getBuildCommand("/tmp/build","install","--clean-first");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("--build . --target install --clean-first"));

    Cmd = openfluid::utils::CMakeProxy::getBuildCommand("/tmp/build","install","--clean-first","-j 4 VERBOSE=1");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("--build . --target install --clean-first -- -j 4 VERBOSE=1"));
  }
  else
    std::cout << "** Test not run due to failing to find cmake program **" << std::endl;

}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tarcmd)
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    QString Cmd;
    QStringList FilesToCompress = {"dir1/fileb.txt","dir1/filea.txt","dir3/filey.txt"};

    Cmd = openfluid::utils::CMakeProxy::getTarCompressCommand("/tmp/compress","compressedfile.tar.gz",FilesToCompress);
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(
        Cmd.endsWith("tar cf \"compressedfile.tar.gz\" \"dir1/fileb.txt\" \"dir1/filea.txt\" \"dir3/filey.txt\""));

    Cmd = openfluid::utils::CMakeProxy::getTarCompressCommand("/tmp/compress","compressedfile.tar.gz",
                                                              FilesToCompress,"vz");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(
        Cmd.endsWith("tar cfvz \"compressedfile.tar.gz\" \"dir1/fileb.txt\" \"dir1/filea.txt\" \"dir3/filey.txt\""));

  }
  else
    std::cout << "** Test not run due to failing to find cmake program **" << std::endl;

}



// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_untarcmd)
{
  if (openfluid::utils::CMakeProxy::isAvailable())
  {
    QString Cmd;

    Cmd = openfluid::utils::CMakeProxy::getTarUncompressCommand("/tmp/compress","compressedfile.tar.gz");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("tar xf \"compressedfile.tar.gz\""));

    Cmd = openfluid::utils::CMakeProxy::getTarUncompressCommand("/tmp/compress","compressedfile.tar.gz","vz");
    std::cout << Cmd.toStdString() << std::endl;
    BOOST_REQUIRE(Cmd.endsWith("tar xfvz \"compressedfile.tar.gz\""));

  }
  else
    std::cout << "** Test not run due to failing to find cmake program **" << std::endl;

}
