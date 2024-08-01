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
  @file main.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <iostream>
#include <typeinfo>

#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

#include <openfluid/base/Init.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/common/OpenFLUIDSplashScreen.hpp>
#include <openfluid/tools/MiscHelpers.hpp>
#include <openfluid/utils/InternalLogger.hpp>

#include "BuilderApp.hpp"
#include "builderconfig.hpp"


int main(int argc, char** argv)
{

  try
  {

    int ExitCode = 0;

    INIT_OPENFLUID_QT_APPLICATION_WITH_GUI(argc,argv);

    openfluid::utils::log::setup();

    do
    {
      openfluid::ui::common::OpenFLUIDSplashScreen Splash(
        openfluid::ui::common::getImage("openfluid_splash_builder","builder")
      );

      Splash.show();

      Splash.setMessage("i18n");
      // translations management
      QString Lang = QString::fromStdString(openfluid::base::PreferencesManager::instance()->getUILanguage());

      QTranslator QtTranslator;
      QTranslator OpenFLUIDTranslator;
      if (Lang != "default")
      {
        // load provided default translations
#if (QT_VERSION_MAJOR < 6)
        if (!QtTranslator.load("qt_"+Lang.left(2)+".qm",QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        {
#else
        if (!QtTranslator.load("qt_"+Lang.left(2)+".qm",QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
        {
#endif
          std::cerr << "Default translation load error" << std::endl;
        }
        // load provided OpenFLUID translations
        if (!OpenFLUIDTranslator.load(QString(openfluid::config::TRANSLATIONS_FILEROOT.c_str()) + "-" + Lang + ".qm",
                                 QString(openfluid::base::Environment::getTranslationsDir().c_str())))
        {
          std::cerr << "OpenFLUID translation load error" << std::endl;
        }
      }
      OPENFLUID_QT_APPLICATION.installTranslator(&QtTranslator);
      OPENFLUID_QT_APPLICATION.installTranslator(&OpenFLUIDTranslator);

      OPENFLUID_QT_APPLICATION.setStyleSheet(QString("QToolTip { color: #FFFFFF; background-color: %1; "
                                                  "border: 1px solid %2; }")
                                          .arg(openfluid::ui::config::TOOLTIP_BGCOLOR,
                                               openfluid::ui::config::TOOLTIP_BORDERCOLOR));

      // Application launching
      BuilderApp App(&Splash);

      App.initialize();
      App.run();

      Splash.finish(App.mainWindow());

      ExitCode = CLOSE_OPENFLUID_QT_APPLICATION_WITH_GUI;

#if (QT_VERSION_MAJOR < 6)
      OPENFLUID_QT_APPLICATION.flush();
#else
      QCoreApplication::sendPostedEvents();
#endif

    }
    while (ExitCode == openfluid::ui::config::EXIT_CODE_FOR_RESTART);

    return ExitCode;

  }
  catch (std::bad_alloc & E)
  {
    std::cerr << "bad_alloc ERROR: " << E.what()
             << ". Possibly not enough memory available" << std::endl;
  }
  catch (std::bad_exception & E)
  {
    std::cerr << "bad_exception ERROR: " << E.what() << std::endl;
  }
  catch (std::bad_cast & E)
  {
    std::cerr << "bad_cast ERROR: " << E.what() << std::endl;
  }
  catch (std::exception & E)
  {
    std::cerr << "std ERROR: " << E.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "ERROR: " << "Unknown Error" << std::endl;
  }
}
