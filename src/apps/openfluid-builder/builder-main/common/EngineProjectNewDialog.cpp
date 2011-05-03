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
 \file EngineProjectNewDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "EngineProjectNewDialog.hpp"

#include <glibmm/i18n.h>

#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>

#include "BuilderFrame.hpp"

#include <iostream>
#include <boost/system/error_code.hpp>

// =====================================================================
// =====================================================================


EngineProjectNewDialog::EngineProjectNewDialog() :
  m_ProjectFolder(""), m_ImportFolder(""), m_ProjectInputDir("")
{
  mp_MsgLabel = Gtk::manage(new Gtk::Label());
  mp_MsgLabel->set_markup(Glib::ustring::compose("<span color='red'>%1</span>",
      _("This Project already exists!")));

  mp_ProjectFileChooserButton
      = Gtk::manage(new Gtk::FileChooserButton(_("Choose the Project folder"),
          Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER));
  mp_ProjectFileChooserButton->set_show_hidden(false);

  /* ! signal_file_set doesn't emit with FILE_CHOOSER_ACTION_SELECT_FOLDER
   * if the change comes from combo */
  mp_ProjectFileChooserButton->signal_selection_changed().connect(
      sigc::mem_fun(*this,
          &EngineProjectNewDialog::onProjectFolderSelectionChanged));

  mp_NameEntry = Gtk::manage(new Gtk::Entry());

  mp_DescriptionTextView = Gtk::manage(new Gtk::TextView());
  Gtk::ScrolledWindow* DescriptionWin = Gtk::manage(new Gtk::ScrolledWindow());
  DescriptionWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  DescriptionWin->set_shadow_type(Gtk::SHADOW_ETCHED_OUT);
  DescriptionWin->add(*mp_DescriptionTextView);
  DescriptionWin->set_visible(true);

  mp_AuthorsEntry = Gtk::manage(new Gtk::Entry());

  Gtk::Label* ProjectFolderLabel = Gtk::manage(new Gtk::Label(
      _("Project Folder")+std::string(":"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  ProjectFolderLabel->set_alignment(1.0,0.5);

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(_("Project Name")+std::string(":"),
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  NameLabel->set_alignment(1.0,0.5);

  Gtk::Label* DescriptionLabel = Gtk::manage(new Gtk::Label(
      _("Project Description")+std::string(":"), Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP));
  DescriptionLabel->set_alignment(1.0,0.0);

  Gtk::Label* AuthorsLabel = Gtk::manage(new Gtk::Label(_("Project Authors")+std::string(":"),
      Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  AuthorsLabel->set_alignment(1.0,0.5);

  Gtk::Table* MainTable = Gtk::manage(new Gtk::Table());
  MainTable->set_spacings(5);
  MainTable->set_border_width(5);
  MainTable->attach(*mp_MsgLabel, 0, 2, 0, 1);
  MainTable->attach(*ProjectFolderLabel, 0, 1, 1, 2, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_ProjectFileChooserButton, 1, 2, 1, 2);
  MainTable->attach(*NameLabel, 0, 1, 2, 3, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*mp_NameEntry, 1, 2, 2, 3);
  MainTable->attach(*DescriptionLabel, 0, 1, 3, 4, Gtk::FILL, Gtk::FILL);
  MainTable->attach(*DescriptionWin, 1, 2, 3, 4);
  MainTable->attach(*AuthorsLabel, 0, 1, 4, 5, Gtk::FILL, Gtk::SHRINK);
  MainTable->attach(*mp_AuthorsEntry, 1, 2, 4, 5);
  MainTable->set_border_width(8);
  MainTable->set_visible(true);
  MainTable->show_all_children();

  mp_MsgLabel->set_visible(false);

  mp_ImportCheck = Gtk::manage(new Gtk::CheckButton(_("Import dataset from ")+std::string(":")));
  mp_ImportCheck->signal_clicked().connect(sigc::mem_fun(*this,
      &EngineProjectNewDialog::onImportCheckClicked));

  mp_ImportFileChooserButton = Gtk::manage(new Gtk::FileChooserButton(
      _("Choose the folder which contains data to import"),
      Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER));
  mp_ImportFileChooserButton->set_show_hidden(false);
  mp_ImportFileChooserButton->set_create_folders(false);
  mp_ImportFileChooserButton->signal_selection_changed().connect(sigc::mem_fun(
      *this, &EngineProjectNewDialog::onImportFolderSelectionChanged));
  mp_ImportFileChooserButton->set_sensitive(false);

  mref_TreeModel = Gtk::TreeStore::create(m_Columns);

  Gtk::CellRendererToggle* MyCellRend = Gtk::manage(
      new Gtk::CellRendererToggle());
  MyCellRend->set_activatable(true);
  MyCellRend->signal_toggled().connect(sigc::mem_fun(*this,
      &EngineProjectNewDialog::onCheckToggled));

  Gtk::TreeView::Column* MyColumn = Gtk::manage(new Gtk::TreeView::Column(
      "Files to import"));
  MyColumn->pack_start(*MyCellRend, false);
  MyColumn->pack_start(m_Columns.m_FileName);
  MyColumn->add_attribute(*MyCellRend, "active", m_Columns.m_IsSelected);
  MyColumn->add_attribute(*MyCellRend, "inconsistent", m_Columns.m_Inconsistent);

  mp_TreeView = Gtk::manage(new Gtk::TreeView(mref_TreeModel));
  mp_TreeView->append_column(*MyColumn);
  mp_TreeView->set_sensitive(false);
  mp_TreeView->signal_test_expand_row().connect(sigc::mem_fun(*this,
      &EngineProjectNewDialog::onTestExpandRow));

  Gtk::ScrolledWindow* ImportWin = Gtk::manage(new Gtk::ScrolledWindow());
  ImportWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ImportWin->add(*mp_TreeView);
  ImportWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  Gtk::HBox* ImportTopBox = Gtk::manage(new Gtk::HBox());
  ImportTopBox->pack_start(*mp_ImportCheck, Gtk::PACK_SHRINK);
  ImportTopBox->pack_start(*mp_ImportFileChooserButton, Gtk::PACK_EXPAND_WIDGET);

  Gtk::VBox* ImportBox = Gtk::manage(new Gtk::VBox());
  ImportBox->pack_start(*ImportTopBox, Gtk::PACK_SHRINK,6);
  ImportBox->pack_start(*ImportWin, Gtk::PACK_EXPAND_WIDGET,6);
  ImportBox->set_border_width(8);

  BuilderFrame* ImportFrame = new BuilderFrame();
  ImportFrame->setLabelText(_("Data import"));
  ImportFrame->set_visible(true);
  ImportFrame->add(*ImportBox);
  ImportFrame->set_border_width(8);
  ImportFrame->show_all_children();

  mp_Dialog = new Gtk::Dialog(_("Create a new project"));
  mp_Dialog->set_has_separator(true);
  mp_Dialog->get_vbox()->pack_start(*MainTable, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*ImportFrame);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->set_default_size(600, 500);
}

// =====================================================================
// =====================================================================


EngineProjectNewDialog::~EngineProjectNewDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================


bool EngineProjectNewDialog::onTestExpandRow(
    const Gtk::TreeModel::iterator& Iter, const Gtk::TreeModel::Path& /*Path*/)
{
  appendDirectoryContent(Iter->get_value(m_Columns.m_FilePath), Iter);
  return false;
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::onCheckToggled(const Glib::ustring TreePath)
{
  Gtk::TreeRow Row = *mref_TreeModel->get_iter(TreePath);

  bool Checked = !Row[m_Columns.m_IsSelected];

  Row[m_Columns.m_IsSelected] = Checked;
  Row[m_Columns.m_Inconsistent] = false;

  checkRowSiblings(Row, Checked);

  if (boost::filesystem::is_directory(Row.get_value(m_Columns.m_FilePath)))
  {
    setChildrenChecked(Row, Checked);
  }

}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::checkRowSiblings(Gtk::TreeRow Row, bool Checked)
{
  if (Row.parent())
  {
    Gtk::TreeRow ParentRow = *Row.parent();

    bool Homogeneous = true;

    Gtk::TreeModel::Children::iterator it = ParentRow.children().begin();
    while (it != ParentRow.children().end() && Homogeneous)
    {
      if (it->get_value(m_Columns.m_IsSelected) != Checked || it->get_value(
          m_Columns.m_Inconsistent))
        Homogeneous = false;

      ++it;
    }

    if (Homogeneous)
    {
      ParentRow[m_Columns.m_IsSelected] = Checked;
      ParentRow[m_Columns.m_Inconsistent] = false;
      checkRowSiblings(ParentRow, Checked);
    } else
    {
      setParentsUnconsistent(Row);
    }
  }
}

// =====================================================================
// =====================================================================

void EngineProjectNewDialog::setChildrenChecked(Gtk::TreeRow Row, bool Checked)
{
  for (unsigned int i = 0; i < Row.children().size(); i++)
  {
    Gtk::TreeRow ChildRow = Row.children()[i];

    ChildRow[m_Columns.m_IsSelected] = Checked;
    ChildRow[m_Columns.m_Inconsistent] = false;

    setChildrenChecked(ChildRow, Checked);
  }
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::setParentsUnconsistent(Gtk::TreeRow Row)
{
  if (Row.parent())
  {
    Gtk::TreeRow ParentRow = *Row.parent();

    ParentRow[m_Columns.m_IsSelected] = true;
    ParentRow[m_Columns.m_Inconsistent] = true;

    setParentsUnconsistent(ParentRow);
  }
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::onProjectFolderSelectionChanged()
{
  std::string ProjectFolder = Glib::filename_to_utf8(
      mp_ProjectFileChooserButton->get_filename());

  if (openfluid::base::ProjectManager::isProject(ProjectFolder))
  {
    mp_MsgLabel->set_visible(true);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);
  } else
  {
    mp_MsgLabel->set_visible(false);
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, true);
  }
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::onImportFolderSelectionChanged()
{
  std::string ImportFolder = Glib::filename_to_utf8(
      mp_ImportFileChooserButton->get_filename());

  boost::filesystem::path ImportPath(ImportFolder);

  mref_TreeModel->clear();

  Gtk::TreeIter RootIter = mref_TreeModel->get_iter("0");

  appendDirectoryContent(ImportPath, RootIter);

  setChildrenChecked(*RootIter, true);
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::appendDirectoryContent(
    boost::filesystem::path DirectoryPath, Gtk::TreeIter DirectoryIter)
{

  Gtk::TreeRow DirectoryRow;

  // it's not the root directory
  if (DirectoryIter)
  {
    DirectoryRow = *DirectoryIter;

    if (DirectoryRow[m_Columns.m_AlreadyFilled])
      return;

    // erase dummy row
    if (!DirectoryRow.children().empty())
      mref_TreeModel->erase(*DirectoryRow.children().begin());

    DirectoryRow[m_Columns.m_AlreadyFilled] = true;
  }

  try
  {
    boost::filesystem::directory_iterator end_it;
    for (boost::filesystem::directory_iterator it(DirectoryPath); it != end_it; ++it)
    {
      if ((boost::filesystem::is_regular_file(it->status())
          || boost::filesystem::is_directory(it->status())) && !isHidden(
          it->path().filename()))
      {
        Gtk::TreeRow Row = DirectoryIter ? *mref_TreeModel->append(
            DirectoryRow->children()) : *mref_TreeModel->append();
        Row[m_Columns.m_FileName] = it->path().filename();
        Row[m_Columns.m_FilePath] = it->path().string();
        Row[m_Columns.m_IsSelected] = DirectoryIter ? DirectoryRow.get_value(
            m_Columns.m_IsSelected) : true;

        if (boost::filesystem::is_directory(it->status()))
        {
          if (boost::filesystem::is_empty(it->path()))
          {
            Row[m_Columns.m_FileName] = it->path().filename() + _(" (Vide)");
          } else
          {
            // add a dummy row to display expander
            Gtk::TreeRow DummyRow = *mref_TreeModel->append(Row->children());
          }
        }
      }
    }
  } catch (std::exception e)
  {
    std::cerr << "EngineProjectNewDialog::appendDirectoryContent " << e.what()
        << std::endl;
  }
}

// =====================================================================
// =====================================================================

bool EngineProjectNewDialog::isHidden(std::string FileName)
{
  return (FileName[0] == '.');
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::onImportCheckClicked()
{
  mp_ImportFileChooserButton->set_sensitive(mp_ImportCheck->get_active());
  mp_TreeView->set_sensitive(mp_ImportCheck->get_active());
}

// =====================================================================
// =====================================================================


std::string EngineProjectNewDialog::show()
{
  m_ProjectFolder = "";
  m_ImportFolder = "";
  m_ProjectInputDir = "";

  onImportFolderSelectionChanged();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    m_ProjectFolder = Glib::filename_to_utf8(
        mp_ProjectFileChooserButton->get_filename());

    openfluid::base::ProjectManager* Manager =
        openfluid::base::ProjectManager::getInstance();

    Manager->create(m_ProjectFolder, mp_NameEntry->get_text(),
        mp_DescriptionTextView->get_buffer()->get_text(),
        mp_AuthorsEntry->get_text(), false);

    m_ProjectInputDir = Manager->getInputDir();

    if (mp_ImportCheck->get_active())
    {
      m_ImportFolder = Glib::filename_to_utf8(
          mp_ImportFileChooserButton->get_filename());

      for (unsigned int i = 0; i < mref_TreeModel->children().size(); i++)
        copyFilePathAndChildren(*mref_TreeModel->children()[i]);
    }
  }

  mp_Dialog->hide();

  return m_ProjectFolder;
}

// =====================================================================
// =====================================================================


void EngineProjectNewDialog::copyFilePathAndChildren(Gtk::TreeRow Row)
{
  if (Row[m_Columns.m_IsSelected])
  {
    std::string SrcPath = Row[m_Columns.m_FilePath];

    copyOnDisk(SrcPath);

    if (boost::filesystem::is_directory(SrcPath)
        && !Row[m_Columns.m_AlreadyFilled])
    {
      boost::filesystem::recursive_directory_iterator end_it;
      for (boost::filesystem::recursive_directory_iterator it(SrcPath); it
          != end_it; ++it)
      {
        if (!isHidden(it->path().filename()))
          copyOnDisk(it->path().string());
      }
    }

    for (unsigned int i = 0; i < Row.children().size(); i++)
    {
      copyFilePathAndChildren(Row.children()[i]);
    }
  }
}

// =====================================================================
// =====================================================================

void EngineProjectNewDialog::copyOnDisk(std::string SrcPath)
{
  std::string DestPath = SrcPath;

  DestPath.replace(0, m_ImportFolder.size(), m_ProjectInputDir);

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
                _("File %1 still exists,\ndo you want to overwrite it ?"),
                DestPath)))
        {
          boost::filesystem::remove(DestPath);
          boost::filesystem::copy_file(SrcPath, DestPath);
        }
      } else
        boost::filesystem::copy_file(SrcPath, DestPath);
    }
  } catch (boost::filesystem::basic_filesystem_error<boost::filesystem::path> e)
  {
    std::cerr
        << "EngineProjectNewDialog::copy boost::filesystem::basic_filesystem_error: "
        << e.what() << std::endl;
  }
}

// =====================================================================
// =====================================================================


std::string EngineProjectNewDialog::getImportDir()
{
  return m_ImportFolder;
}
