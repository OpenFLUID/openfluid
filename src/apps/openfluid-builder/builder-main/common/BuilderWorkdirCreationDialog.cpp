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
 \file BuilderWorkdirCreationDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderWorkdirCreationDialog.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include <glibmm/i18n.h>

#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/guicommon/PreferencesManager.hpp>

// =====================================================================
// =====================================================================


BuilderWorkdirCreationDialog::BuilderWorkdirCreationDialog()
{
  mp_Dialog = new Gtk::Dialog(_("OpenFLUID working directory"), true, false);

  mp_Label = Gtk::manage(new Gtk::Label(_(
      "Choose the working directory to use for OpenFLUID Projects :")));

  mp_Entry = Gtk::manage(new Gtk::Entry());
  mp_Entry->set_activates_default(true);

  mp_FileButton = Gtk::manage(new Gtk::Button(_("Browse...")));
  mp_FileButton->signal_clicked().connect(sigc::mem_fun(*this,
      &BuilderWorkdirCreationDialog::onFileButtonClicked));

  mp_FileBox = Gtk::manage(new Gtk::HBox());
  mp_FileBox->pack_start(*mp_Entry, Gtk::PACK_EXPAND_WIDGET,5);
  mp_FileBox->pack_start(*mp_FileButton, Gtk::PACK_SHRINK,5);

  mp_Dialog->get_vbox()->pack_start(*mp_Label,Gtk::PACK_SHRINK,10);
  mp_Dialog->get_vbox()->pack_start(*mp_FileBox,Gtk::PACK_SHRINK,10);

  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  mp_Dialog->set_border_width(8);
  mp_Dialog->set_size_request(500,-1);

  mp_Dialog->show_all_children();
}

// =====================================================================
// =====================================================================


bool BuilderWorkdirCreationDialog::show()
{
  mp_Entry->set_text(Glib::ustring::compose("%1/OpenFLUID-Projects",
      Glib::get_home_dir()));

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    std::string Workdir = mp_Entry->get_text();

    if (!boost::filesystem::exists(Workdir))
    {
      try
      {
        boost::filesystem::create_directory(Workdir);

      } catch (boost::filesystem::basic_filesystem_error<
          boost::filesystem::path> e)
      {
        openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(e.what());

        mp_Dialog->hide();
        return false;
      }
    }

    openfluid::guicommon::PreferencesManager::getInstance()->setWorkdir(Workdir);
    openfluid::guicommon::PreferencesManager::getInstance()->save();

    mp_Dialog->hide();
    return true;

  }

  mp_Dialog->hide();
  return false;
}


// =====================================================================
// =====================================================================


void BuilderWorkdirCreationDialog::onFileButtonClicked()
{
  Gtk::FileChooserDialog Dialog(_("Choose OpenFLUID working directory"),Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  Dialog.add_button(Gtk::Stock::OK,Gtk::RESPONSE_OK);
  Dialog.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);

  Dialog.set_current_folder(Glib::get_home_dir());

  if(Dialog.run() == Gtk::RESPONSE_OK)
    mp_Entry->set_text(Dialog.get_filename());

}
