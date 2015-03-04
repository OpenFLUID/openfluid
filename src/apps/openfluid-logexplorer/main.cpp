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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <iostream>
#include <typeinfo>

#include <QTranslator>
#include <QLibraryInfo>
#include <QDir>
#include <QDesktopWidget>

#include <openfluid/base/Init.hpp>
#include <openfluid/config.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/common/LogExplorerDialog.hpp>



int main(int argc, char** argv)
{
  try
  {
    INIT_OPENFLUID_APPLICATION_WITH_GUI(argc, argv);

    // translations management
    QString Lang = openfluid::base::PreferencesManager::instance()->getLang();

    QTranslator QtTranslator;
    QTranslator OpenFLUIDTranslator;
    if (Lang != "default")
    {
      // load provided default translations
      QtTranslator.load("qt_" + Lang.left(2) + ".qm",
                        QLibraryInfo::location(QLibraryInfo::TranslationsPath));

      // load provided OpenFLUID translations
      OpenFLUIDTranslator.load(
          QString(openfluid::config::TRANSLATIONS_FILEROOT.c_str()) + "-" + Lang
          + ".qm",
          QString(
              openfluid::base::RuntimeEnvironment::instance()
                  ->getTranslationsDir().c_str()));
    }
    OPENFLUID_APPLICATION.installTranslator(&QtTranslator);
    OPENFLUID_APPLICATION.installTranslator(&OpenFLUIDTranslator);

// TODO remove the #if linux when icons problem on win32 will be solved
#if linux
    Q_INIT_RESOURCE(openfluiduicommon);
#endif

    QString LogDir;

    if (argc >= 2)
      LogDir = QString(argv[1]);
    else
      LogDir = QDir::currentPath();

    openfluid::ui::common::LogExplorerDialog LogDlg(LogDir);

    QRect ScreenRect = QApplication::desktop()->screenGeometry();
    LogDlg.resize(ScreenRect.width()*0.9,ScreenRect.height()*0.9);
    LogDlg.move((ScreenRect.width()-LogDlg.width())/2, (ScreenRect.height()-LogDlg.height())/2);

    LogDlg.show();

    return CLOSE_OPENFLUID_APPLICATION_WITH_GUI;
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
