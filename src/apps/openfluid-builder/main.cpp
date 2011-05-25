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
 \file main.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <iostream>

#include "builderconfig.hpp"

#include "BuilderGtkInit.hpp"
#include "BuilderAppModule.hpp"
#include <openfluid/guicommon/PreferencesManager.hpp>

// =====================================================================
// =====================================================================


int main(int argc, char** argv)
{
  try
  {
    // Native Language Support setup
    if (BUILDER_NLS_ENABLE)
    {
      std::string PrefLang =
          openfluid::guicommon::PreferencesManager::getInstance()->getLang();

      if (PrefLang != "")
      {
        try
        {
          std::locale::global(std::locale(PrefLang.c_str()));

          Glib::ustring Language = Glib::ustring::compose("LANGUAGE=%1",
              PrefLang);
          Glib::ustring Lang = Glib::ustring::compose("LANG=%1", PrefLang);

          char* LanguageC = const_cast<char*> (Language.c_str());
          char* LangC = const_cast<char*> (Lang.c_str());

          putenv(LanguageC);
          putenv(LangC);

        } catch (std::runtime_error const& e)
        {
          std::locale::global(std::locale::classic());
        }
      }

      bindtextdomain(BUILDER_NLS_PACKAGE, BUILDER_NLS_LOCALEDIR);
      bind_textdomain_codeset(BUILDER_NLS_PACKAGE, "UTF-8");
      textdomain(BUILDER_NLS_PACKAGE);
    }

    BuilderGtkInit GtkInitObjet(argc, argv);

    BuilderAppModule AppModule;
    if (AppModule.initialize())
      GtkInitObjet.run(AppModule.composeAndGetAsWindow());
    else
      return -1;

  }
  /* catch (openfluid::base::OFException & E) // TODO pass it in Project Coordinator
   {
   std::cerr << "ERROR: " << E.what() << std::endl;
   } */
  catch (std::bad_alloc & E)
  {
    std::cerr << "bad_alloc ERROR: " << E.what()
        << ". Possibly not enough memory available" << std::endl;
  } catch (std::bad_exception & E)
  {
    std::cerr << "bad_exception ERROR: " << E.what() << std::endl;
  } catch (std::bad_cast & E)
  {
    std::cerr << "bad_cast ERROR: " << E.what() << std::endl;
  } catch (Glib::Error & E)
  {
    std::cerr << "Glib ERROR: " << E.what() << std::endl;
  } catch (std::exception & E)
  {
    std::cerr << "exception ERROR: " << E.what() << std::endl;
  } catch (...)
  {
    std::cerr << "ERROR: " << "Unknown Error" << std::endl;
  }
  return 0;
}

