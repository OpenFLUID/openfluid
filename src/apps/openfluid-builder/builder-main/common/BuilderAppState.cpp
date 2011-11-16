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
  \file BuilderAppState.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#include "BuilderAppState.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/aboutdialog.h>

#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/config.hpp>

#include "builderconfig.hpp"
#include "BuilderGraphicsHelper.hpp"


// =====================================================================
// =====================================================================


void BuilderAppState::whenAboutAsked()
{
  Gtk::AboutDialog About;

  About.set_modal(true);
  About.set_size_request(400,-1);

  About.set_logo(BuilderGraphicsHelper::createPixbufFromFile("openfluid_official.png"));
  About.set_program_name(openfluid::config::FULL_VERSION);
  About.set_comments("OpenFLUID-Builder application");
  About.set_copyright("LISAH - INRA/SupAgro - Montpellier - France");
  About.set_website(BUILDER_URL);

  About.set_license(BUILDER_LICENSE_TEXT);
  About.set_authors(openfluid::tools::SplitString(BUILDER_AUTHORS_TEXT,"\n",true));

  About.set_title(_("About OpenFLUID..."));

  About.run();
}


// =====================================================================
// =====================================================================


void BuilderAppState::whenOnlineAsked(const std::string& URL)
{
  if (!openfluid::tools::OpenURLInBrowser(URL))
    openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(_("Unable to open URL:")+std::string("\n")+URL);
}




