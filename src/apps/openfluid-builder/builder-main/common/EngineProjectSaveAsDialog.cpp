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
 \file EngineProjectSaveAsDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "EngineProjectSaveAsDialog.hpp"

#include <iostream>

#include <glibmm/i18n.h>

#include <gtkmm/table.h>
#include <gtkmm/stock.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/regex.h>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>

// =====================================================================
// =====================================================================


EngineProjectSaveAsDialog::EngineProjectSaveAsDialog()
{
  // InfoBar

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label(""));
  mp_InfoBarLabel->set_visible(true);

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  // Project Properties

  mp_WorkdirFileChooserButton = Gtk::manage(new Gtk::FileChooserButton(
      _("Choose the directory to use"),
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER));
  mp_WorkdirFileChooserButton->set_show_hidden(false);

  /* ! signal_file_set doesn't emit with FILE_CHOOSER_ACTION_SELECT_FOLDER
   * if the change comes from combo */
  mp_WorkdirFileChooserButton->signal_selection_changed().connect(
      sigc::mem_fun(*this,
          &EngineProjectSaveAsDialog::onProjectFolderSelectionChanged));

  mp_NameEntry = Gtk::manage(new Gtk::Entry());
  mp_NameEntry->signal_changed().connect(sigc::mem_fun(*this,
      &EngineProjectSaveAsDialog::fillFolderName));
  mp_NameEntry->set_activates_default(true);

  mp_ProjectFolderName = Gtk::manage(new Gtk::Label("", Gtk::ALIGN_LEFT,
      Gtk::ALIGN_CENTER));

  Gtk::Label* WorkdirLabel = Gtk::manage(new Gtk::Label(_("Working directory")
      + std::string(":"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  WorkdirLabel->set_alignment(1.0, 0.5);

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(_("Project name")
      + std::string(":"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  NameLabel->set_alignment(1.0, 0.5);

  Gtk::Label* FolderNameLabel = Gtk::manage(new Gtk::Label(_("Project directory")
      + std::string(":"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  FolderNameLabel->set_alignment(1.0, 0.5);

  Gtk::Table* MainTable = Gtk::manage(new Gtk::Table());
  MainTable->set_spacings(5);
  MainTable->set_border_width(5);
  MainTable->attach(*WorkdirLabel, 0, 1, 0, 1, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_WorkdirFileChooserButton, 1, 2, 0, 1);
  MainTable->attach(*NameLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_NameEntry, 1, 2, 1, 2);
  MainTable->attach(*FolderNameLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_ProjectFolderName, 1, 2, 2, 3);
  MainTable->set_border_width(8);
  MainTable->set_visible(true);
  MainTable->show_all_children();

  // Main dialog

  mp_Dialog = new Gtk::Dialog(_("Save the current project as..."));
  mp_Dialog->set_has_separator(true);
  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*MainTable, Gtk::PACK_SHRINK, 5);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_response(Gtk::RESPONSE_OK);

  checkProject();
}

// =====================================================================
// =====================================================================


EngineProjectSaveAsDialog::~EngineProjectSaveAsDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================


void EngineProjectSaveAsDialog::onProjectFolderSelectionChanged()
{
  checkProject();
}

// =====================================================================
// =====================================================================


void EngineProjectSaveAsDialog::checkProject()
{
  Glib::ustring ProjectWorkDir = Glib::filename_to_utf8(
      mp_WorkdirFileChooserButton->get_filename());

  Glib::ustring ProjectPath = Glib::ustring::compose("%1/%2", ProjectWorkDir,
      mp_ProjectFolderName->get_text());

  bool IsValid = false;

  if (mp_NameEntry->get_text().empty())
  {
    mp_InfoBarLabel->set_text(_("Project name cannot be empty"));
  }
  else if (boost::filesystem::exists(boost::filesystem::path(std::string(ProjectPath))))
  {
    mp_InfoBarLabel->set_text(_("This project directory already exists"));
  }
  else
  {
    IsValid = true;
  }

  mp_InfoBar->set_visible(!IsValid);
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, IsValid);
  mp_Dialog->resize(1, 1);
}

// =====================================================================
// =====================================================================

void EngineProjectSaveAsDialog::fillFolderName()
{
  std::string Str = mp_NameEntry->get_text();

  boost::algorithm::replace_all_regex(Str,boost::regex("[^[:alnum:]]"),std::string("_"));

  mp_ProjectFolderName->set_text(Str);

  checkProject();
}

// =====================================================================
// =====================================================================


Glib::ustring EngineProjectSaveAsDialog::show()
{
  openfluid::base::ProjectManager* p_Manager =
      openfluid::base::ProjectManager::getInstance();

  boost::filesystem::path CurrentProjectPath = boost::filesystem::path(
      std::string(p_Manager->getPath()));

  mp_WorkdirFileChooserButton->set_current_folder(
      CurrentProjectPath.parent_path().string());

  mp_NameEntry->set_text(p_Manager->getName());

  mp_ProjectFolderName->set_text(CurrentProjectPath.filename().string());

  mp_Dialog->set_focus(*mp_NameEntry);

  Glib::ustring NewProjectPathStr = "";

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    NewProjectPathStr = Glib::ustring::compose("%1/%2", Glib::filename_to_utf8(
        mp_WorkdirFileChooserButton->get_filename()),
        mp_ProjectFolderName->get_text());

    // create the new project directory

    try
    {
      boost::filesystem::create_directory(Glib::filename_from_utf8(
          NewProjectPathStr));
    }
    catch (boost::filesystem::filesystem_error e)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
          Glib::ustring::compose(
              _("Impossible to create directory %1\n(%2)\n\nProject not created."),
              NewProjectPathStr, e.what()));

      mp_Dialog->hide();
      return "";
    }

    // copy content to the new project directory

    boost::filesystem::recursive_directory_iterator end_it;
    for (boost::filesystem::recursive_directory_iterator it(CurrentProjectPath); it
        != end_it; ++it)
    {
      std::string SrcPath = it->path().string();

      std::string DestPath = SrcPath;

      DestPath.replace(0,
          Glib::filename_from_utf8(CurrentProjectPath.string()).size(),
          Glib::filename_from_utf8(NewProjectPathStr));

      try
      {
        if (boost::filesystem::is_directory(SrcPath))
          boost::filesystem::create_directory(DestPath);
        else if (boost::filesystem::is_regular_file(SrcPath))
        {
          if (boost::filesystem::exists(DestPath))
          {
            if (openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
                Glib::ustring::compose(
                    _("File %1 already exists.\nDo you really want to overwrite it?"),
                    DestPath)))
            {
              boost::filesystem::remove(DestPath);
              boost::filesystem::copy_file(SrcPath, DestPath);
            }
          }
          else
            boost::filesystem::copy_file(SrcPath, DestPath);
        }
      }
      catch (boost::filesystem::filesystem_error e)
      {
        std::cerr
            << "EngineProjectSaveAsDialog::copy boost::filesystem::basic_filesystem_error: "
            << e.what() << std::endl;
      }
    }

    // fill new project manager information

    p_Manager->close();

    p_Manager->open(NewProjectPathStr);
    p_Manager->setName(mp_NameEntry->get_text());
    p_Manager->setCreationDateAsNow();
    p_Manager->save();

  }

  mp_Dialog->hide();

  return NewProjectPathStr;
}
