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
 \file EngineProjectOpenDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "EngineProjectOpenDialog.hpp"

#include <glibmm/i18n.h>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "BuilderGraphicsHelper.hpp"

// =====================================================================
// =====================================================================


EngineProjectOpenDialog::EngineProjectOpenDialog() :
  m_ProjectFolder("")
{
  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(_("Name")/*,
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER*/));
  Gtk::Label* DescLabel = Gtk::manage(new Gtk::Label(_("Description")/*,
      Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP*/));
  Gtk::Label* AuthorsLabel = Gtk::manage(new Gtk::Label(_("Authors")/*,
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER*/));
  Gtk::Label* CreationDateLabel = Gtk::manage(new Gtk::Label(
      _("Creation Date")/*, Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER*/));
  Gtk::Label* LastModDateLabel = Gtk::manage(new Gtk::Label(
      _("Last modification Date")/*, Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER*/));
  mp_ProjectName = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER, 5));
  mp_ProjectDesc = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER, 5));
  mp_ProjectAuthors = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER, 5));
  mp_ProjectCreationDate = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER, 5));
  mp_ProjectLastModDate = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER, 5));

  mp_PreviewInfoBox = Gtk::manage(new Gtk::VBox());
  mp_PreviewInfoBox->pack_start(*NameLabel, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*mp_ProjectName, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*DescLabel, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*mp_ProjectDesc, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*AuthorsLabel, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*mp_ProjectAuthors, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*CreationDateLabel, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*mp_ProjectCreationDate, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*Gtk::manage(new Gtk::HSeparator()), Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*LastModDateLabel, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->pack_start(*mp_ProjectLastModDate, Gtk::PACK_SHRINK);
  mp_PreviewInfoBox->set_visible(true);
  mp_PreviewInfoBox->show_all_children();

  Gtk::VBox* PreviewBox = Gtk::manage(new Gtk::VBox());
  Gtk::Image* OFImage = BuilderGraphicsHelper::createImageFromFileName(
      "openfluid_icon.png");
  OFImage->set_visible(true);
  PreviewBox->pack_start(*OFImage);
  PreviewBox->pack_start(*mp_PreviewInfoBox, Gtk::PACK_SHRINK, 5);

  mp_Dialog = new Gtk::FileChooserDialog(_("Select existing project folder"),
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
  mp_Dialog->set_create_folders(false);

  mp_Dialog->signal_selection_changed().connect(sigc::mem_fun(*this,
      &EngineProjectOpenDialog::onProjectFolderSelectionChanged));

  //TODO set work directory from preferences
  mp_Dialog->set_current_folder(
      openfluid::base::RuntimeEnvironment::getInstance()->getInputDir() + "/..");

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  mp_Dialog->set_preview_widget(*PreviewBox);
  mp_Dialog->set_preview_widget_active(true);
}

// =====================================================================
// =====================================================================


void EngineProjectOpenDialog::onProjectFolderSelectionChanged()
{
  std::string FileName = Glib::filename_to_utf8(mp_Dialog->get_filename());

  if (openfluid::base::ProjectManager::isProject(FileName))
  {
    openfluid::base::ProjectManager* Manager =
        openfluid::base::ProjectManager::getInstance();

    Manager->open(FileName);

    mp_ProjectName->set_text(replaceEmpty(Manager->getName()));
    mp_ProjectDesc->set_text(replaceEmpty(Manager->getDescription()));
    mp_ProjectAuthors->set_text(replaceEmpty(Manager->getAuthors()));
    mp_ProjectCreationDate->set_text(boost::posix_time::to_simple_string(
        boost::posix_time::from_iso_string(Manager->getCreationDate())));
    mp_ProjectLastModDate->set_text(boost::posix_time::to_simple_string(
        boost::posix_time::from_iso_string(Manager->getLastModDate())));

    Manager->close();

    mp_PreviewInfoBox->set_visible(true);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, true);
    mp_Dialog->get_preview_widget()->set_sensitive(true);
  } else
  {
    mp_PreviewInfoBox->set_visible(false);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);
    mp_Dialog->get_preview_widget()->set_sensitive(false);
  }
}


// =====================================================================
// =====================================================================

std::string EngineProjectOpenDialog::replaceEmpty(std::string StringToCheck)
{
  if(StringToCheck.empty())
    return _("<none>");

  return StringToCheck;
}


std::string EngineProjectOpenDialog::show()
{
  m_ProjectFolder = "";

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    m_ProjectFolder = mp_Dialog->get_filename();
  }

  mp_Dialog->hide();

  return m_ProjectFolder;
}

// =====================================================================
// =====================================================================
