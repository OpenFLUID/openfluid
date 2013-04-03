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
 \file DatasoreAddItemDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "DatasoreAddItemDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/box.h>
#include <gtkmm/table.h>
#include <gtkmm/button.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/alignment.h>
#include <openfluid/fluidx/AdvancedDatastoreDescriptor.hpp>
#include <openfluid/fluidx/DatastoreItemDescriptor.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include "BuilderFrame.hpp"
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

DatasoreAddItemDialog::DatasoreAddItemDialog(
    openfluid::fluidx::AdvancedDatastoreDescriptor& Datastore) :
    mp_Datastore(&Datastore)
{
  mref_InDirFile = Gio::File::create_for_path(
      openfluid::base::ProjectManager::getInstance()->getInputDir());

  mp_InfoBarLabel = Gtk::manage(new Gtk::Label());

  // InfoBar

  mp_InfoBar = Gtk::manage(new Gtk::InfoBar());
  mp_InfoBar->set_message_type(Gtk::MESSAGE_WARNING);
  ((Gtk::Container*) mp_InfoBar->get_content_area())->add(*mp_InfoBarLabel);

  // Item info

  mp_IDEntry = Gtk::manage(new Gtk::Entry());
  mp_IDEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::checkIsValid));

  mp_TypeCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_TypeCombo->append(
      openfluid::core::UnstructuredValue::getStringFromValueType(
          openfluid::core::UnstructuredValue::GeoVectorValue));
  mp_TypeCombo->append(
      openfluid::core::UnstructuredValue::getStringFromValueType(
          openfluid::core::UnstructuredValue::GeoRasterValue));
  mp_TypeCombo->set_active(0);

  Gtk::Table* IDTypeTable = Gtk::manage(new Gtk::Table(2, 2));
  IDTypeTable->set_col_spacings(3);
  IDTypeTable->set_row_spacings(5);
  IDTypeTable->attach(
      *Gtk::manage(new Gtk::Label(_("ID"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER)),
      0, 1, 0, 1, Gtk::SHRINK | Gtk::FILL);
  IDTypeTable->attach(
      *Gtk::manage(
          new Gtk::Label(_("Type"), Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER)),
      0, 1, 1, 2, Gtk::SHRINK | Gtk::FILL);
  IDTypeTable->attach(*mp_IDEntry, 1, 2, 0, 1, Gtk::SHRINK);
  IDTypeTable->attach(*mp_TypeCombo, 1, 2, 1, 2, Gtk::FILL);

  mp_ClassCheck = Gtk::manage(new Gtk::CheckButton(_("Unit class")));
  mp_ClassCheck->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onCheckToggled));
  mp_ClassCheck->set_active(false);

  mp_ClassEntry = Gtk::manage(new Gtk::Entry());
  mp_ClassEntry->set_sensitive(false);

  Gtk::HBox* ClassBox = Gtk::manage(new Gtk::HBox());
  ClassBox->pack_start(*mp_ClassCheck, Gtk::PACK_SHRINK, 5);
  ClassBox->pack_start(*mp_ClassEntry, Gtk::PACK_SHRINK);

  // Data source

  Gtk::RadioButtonGroup SourceGroup;

  //  // File

  mp_FileRadio = Gtk::manage(new Gtk::RadioButton(SourceGroup, _("File")));
  mp_FileRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onSourceToggled));
  Gtk::Alignment* AlignFileRadio = Gtk::manage(new Gtk::Alignment(0.5,0,0,0));
  AlignFileRadio->add(*mp_FileRadio);

  mp_FilePathEntry = Gtk::manage(new Gtk::Entry());
  mp_FilePathEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::checkIsValid));

  Gtk::Button* BrowseFileButton = Gtk::manage(new Gtk::Button(_("Browse...")));
  BrowseFileButton->signal_clicked().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onBrowseFileButtonClicked));

  Gtk::HBox* FilePathBox = Gtk::manage(new Gtk::HBox());
  FilePathBox->pack_start(*mp_FilePathEntry, Gtk::PACK_EXPAND_WIDGET);
  FilePathBox->pack_start(*BrowseFileButton, Gtk::PACK_SHRINK);

  mref_TreeModel = Gtk::ListStore::create(m_Columns);
  mp_TreeView = Gtk::manage(new Gtk::TreeView());
  mp_TreeView->set_model(mref_TreeModel);
  mp_TreeView->set_headers_visible(false);
  mp_TreeView->append_column_editable("", m_Columns.m_Selected);
  mp_TreeView->append_column_editable("", m_Columns.m_FileName);

  mp_TreeView->get_column(0)->add_attribute(
      ((Gtk::CellRendererToggle*) mp_TreeView->get_column_cell_renderer(0))->property_sensitive(),
      m_Columns.m_Sensitive);
  mp_TreeView->get_column(1)->add_attribute(
      ((Gtk::CellRendererText*) mp_TreeView->get_column_cell_renderer(1))->property_sensitive(),
      m_Columns.m_Sensitive);

  mref_TreeModel->set_sort_column(m_Columns.m_FileName, Gtk::SORT_ASCENDING);

  Gtk::ScrolledWindow* ModelWin = Gtk::manage(new Gtk::ScrolledWindow());
  ModelWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ModelWin->set_visible(true);
  ModelWin->add(*mp_TreeView);
  ModelWin->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  mp_Expander = Gtk::manage(new Gtk::Expander("Associated files:"));
  mp_Expander->add(*ModelWin);

  mp_FileDetailBox = Gtk::manage(new Gtk::VBox());
  mp_FileDetailBox->pack_start(*FilePathBox, Gtk::PACK_SHRINK);
  mp_FileDetailBox->pack_start(*mp_Expander, Gtk::PACK_SHRINK);

  Gtk::HBox* FileBox = Gtk::manage(new Gtk::HBox());
  FileBox->pack_start(*AlignFileRadio, Gtk::PACK_SHRINK);
  FileBox->pack_start(*mp_FileDetailBox);

  //  // Directory

  mp_DirRadio = Gtk::manage(new Gtk::RadioButton(SourceGroup, _("Directory")));
  mp_DirRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onSourceToggled));

  mp_DirPathEntry = Gtk::manage(new Gtk::Entry());
  mp_DirPathEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::checkIsValid));

  Gtk::Button* BrowseDirButton = Gtk::manage(new Gtk::Button(_("Browse...")));
  BrowseDirButton->signal_clicked().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onBrowseDirButtonClicked));

  Gtk::HBox* DirPathBox = Gtk::manage(new Gtk::HBox());
  DirPathBox->pack_start(*mp_DirPathEntry, Gtk::PACK_EXPAND_WIDGET);
  DirPathBox->pack_start(*BrowseDirButton, Gtk::PACK_SHRINK);

  mp_DirDetailBox = Gtk::manage(new Gtk::VBox());
  mp_DirDetailBox->pack_start(*DirPathBox, Gtk::PACK_SHRINK);

  Gtk::HBox* DirBox = Gtk::manage(new Gtk::HBox());
  DirBox->pack_start(*mp_DirRadio, Gtk::PACK_SHRINK);
  DirBox->pack_start(*mp_DirDetailBox);

  //  // Resource

  mp_ResourceRadio = Gtk::manage(
      new Gtk::RadioButton(SourceGroup, _("Resource string")));
  mp_ResourceRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onSourceToggled));

  mp_ResourceStringEntry = Gtk::manage(new Gtk::Entry());
  mp_ResourceStringEntry->signal_changed().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::checkIsValid));

  Gtk::HBox* ResourceBox = Gtk::manage(new Gtk::HBox());
  ResourceBox->pack_start(*mp_ResourceRadio, Gtk::PACK_SHRINK);
  ResourceBox->pack_start(*mp_ResourceStringEntry, Gtk::PACK_EXPAND_WIDGET);

  Gtk::VBox* DataSourceBox = Gtk::manage(new Gtk::VBox());
  DataSourceBox->pack_start(*FileBox, Gtk::PACK_SHRINK);
  DataSourceBox->pack_start(*DirBox, Gtk::PACK_SHRINK);
  DataSourceBox->pack_start(*ResourceBox, Gtk::PACK_SHRINK);

  BuilderFrame* SourceFrame = Gtk::manage(new BuilderFrame());
  SourceFrame->setLabelText(_("Data source"));
  SourceFrame->add(*DataSourceBox);

  // Destination

  Gtk::RadioButtonGroup SubDirGroup;

  mp_NoSubDirRadio = Gtk::manage(
      new Gtk::RadioButton(SubDirGroup, _("Copied into dataset directory")));
  mp_NoSubDirRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onSubDirToggled));

  mp_SubDirRadio = Gtk::manage(
      new Gtk::RadioButton(SubDirGroup,
                           _("Copied into dataset subdirectory:")));
  mp_SubDirRadio->signal_toggled().connect(
      sigc::mem_fun(*this, &DatasoreAddItemDialog::onSubDirToggled));
  mp_SubDirEntry = Gtk::manage(new Gtk::Entry());
  mp_SubDirBox = Gtk::manage(new Gtk::HBox());
  mp_SubDirBox->pack_start(*mp_SubDirRadio, Gtk::PACK_SHRINK);
  mp_SubDirBox->pack_start(*mp_SubDirEntry, Gtk::PACK_EXPAND_WIDGET);

  Gtk::VBox* DataDestBox = Gtk::manage(new Gtk::VBox());
  DataDestBox->pack_start(*mp_NoSubDirRadio, Gtk::PACK_SHRINK);
  DataDestBox->pack_start(*mp_SubDirBox);

  BuilderFrame* DestFrame = Gtk::manage(new BuilderFrame());
  DestFrame->setLabelText(_("Data destination"));
  DestFrame->add(*DataDestBox);

  // Dialog box

  mp_Dialog = new Gtk::Dialog(_("Add a Datastore item"));
  mp_Dialog->get_vbox()->pack_start(*mp_InfoBar, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*IDTypeTable, Gtk::PACK_SHRINK);
  mp_Dialog->get_vbox()->pack_start(*ClassBox, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*SourceFrame, Gtk::PACK_SHRINK, 5);
  mp_Dialog->get_vbox()->pack_start(*DestFrame, Gtk::PACK_SHRINK, 5);
//  mp_Dialog->set_default_size(700, 200);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  mp_Dialog->show_all_children();

  mp_NoSubDirRadio->set_active(true);
  onSubDirToggled();
  mp_FileRadio->set_active(true);
  onSourceToggled();
}

// =====================================================================
// =====================================================================

openfluid::fluidx::DatastoreItemDescriptor* DatasoreAddItemDialog::show()
{
  checkIsValid();

  openfluid::fluidx::DatastoreItemDescriptor* NewItem = 0;

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
  {
    std::string FromPath = "";

    if (mp_FileRadio->get_active())
    {
      FromPath = mp_FilePathEntry->get_text();
    }
    else if (mp_DirRadio->get_active())
    {
      FromPath = mp_DirPathEntry->get_text();
    }

    if (!FromPath.empty())
    {
      Glib::RefPtr<Gio::File> FromFile = Gio::File::create_for_path(FromPath);

      if (FromFile->query_exists())
      {
        std::string DSRelativePath = "";

        if (isFileInInputDir(FromFile))
        {
          DSRelativePath = mref_InDirFile->get_relative_path(FromFile);
        }
        else
        {
          Glib::RefPtr<Gio::File> ToFile;

          if(mp_FileRadio->get_active())
          {
            Gtk::TreeModel::Children Children = mref_TreeModel->children();
            for(Gtk::TreeModel::Children::iterator it = Children.begin(); it != Children.end(); ++it)
            {
              if(it->get_value(m_Columns.m_Selected) && it->get_value(m_Columns.m_Sensitive))
                copyFile(it->get_value(m_Columns.m_File),ToFile);
            }
          }

          if (copyFile(FromFile, ToFile))
            DSRelativePath = mref_InDirFile->get_relative_path(ToFile);
        }

        if (!DSRelativePath.empty())
          NewItem = createItem(DSRelativePath);
      }

    }
    else if (mp_ResourceRadio->get_active())
    {
      std::string ResourceStr = mp_ResourceStringEntry->get_text();
      //TODO
    }
  }

  mp_Dialog->hide();

  return NewItem;
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::checkIsValid()
{
  std::string InfoBarTxt = "";

  std::string NewID = mp_IDEntry->get_text();

  if (NewID == "" || EngineHelper::isEmptyString(NewID))
  {
    InfoBarTxt = _("ID can not be empty");
  }
  else if (mp_Datastore->isItemAlreadyExist(NewID))
  {
    InfoBarTxt = _("This ID already exists");
  }

  std::string FilePath = mp_FilePathEntry->get_text();
  std::string DirPath = mp_DirPathEntry->get_text();
  std::string ResourcePath = mp_ResourceStringEntry->get_text();

  if (mp_FileRadio->get_active())
  {
    if (FilePath == "" || EngineHelper::isEmptyString(FilePath))
    {
      if (!InfoBarTxt.empty())
        InfoBarTxt.append("\n");
      InfoBarTxt.append(_("File path can not be empty"));
    }
    else
    {
      Glib::RefPtr<Gio::File> File = Gio::File::create_for_path(FilePath);
      if (!File->query_exists())
      {
        if (!InfoBarTxt.empty())
          InfoBarTxt.append("\n");
        InfoBarTxt.append(_("This file path doesn't exist"));
      }
      else if (File->query_file_type() == Gio::FILE_TYPE_DIRECTORY)
      {
        if (!InfoBarTxt.empty())
          InfoBarTxt.append("\n");
        InfoBarTxt.append(_("This file path is a directory"));
      }
      else
      {
        bool ExistsInIN = isFileInInputDir(File);
        mp_SubDirBox->set_sensitive(!ExistsInIN);
        mp_NoSubDirRadio->set_sensitive(!ExistsInIN);
      }
    }
  }
  else if (mp_DirRadio->get_active())
  {
    if (DirPath == "" || EngineHelper::isEmptyString(DirPath))
    {
      if (!InfoBarTxt.empty())
        InfoBarTxt.append("\n");
      InfoBarTxt.append(_("Directory path can not be empty"));
    }
    else
    {
      Glib::RefPtr<Gio::File> File = Gio::File::create_for_path(DirPath);
      if (!File->query_exists())
      {
        if (!InfoBarTxt.empty())
          InfoBarTxt.append("\n");
        InfoBarTxt.append(_("This directory path doesn't exist"));
      }
      else if (File->query_file_type() != Gio::FILE_TYPE_DIRECTORY)
      {
        if (!InfoBarTxt.empty())
          InfoBarTxt.append("\n");
        InfoBarTxt.append(_("This directory path is a file"));
      }
      else
      {
        bool ExistsInIN = isFileInInputDir(File);
        mp_SubDirBox->set_sensitive(!ExistsInIN);
        mp_NoSubDirRadio->set_sensitive(!ExistsInIN);
      }
    }
  }
  else if (mp_ResourceRadio->get_active()
      && (ResourcePath == "" || EngineHelper::isEmptyString(ResourcePath)))
  {
    if (!InfoBarTxt.empty())
      InfoBarTxt.append("\n");
    InfoBarTxt.append(_("Resource string can not be empty"));

    mp_SubDirBox->set_sensitive(false);
    mp_NoSubDirRadio->set_sensitive(false);
  }

  mp_InfoBarLabel->set_text(InfoBarTxt);
  mp_InfoBar->set_visible(!InfoBarTxt.empty());
  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, InfoBarTxt.empty());
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onCheckToggled()
{
  mp_ClassEntry->set_sensitive(mp_ClassCheck->get_active());
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onSourceToggled()
{
  mp_FileDetailBox->set_sensitive(mp_FileRadio->get_active());
  mp_DirDetailBox->set_sensitive(mp_DirRadio->get_active());
  mp_ResourceStringEntry->set_sensitive(mp_ResourceRadio->get_active());

  checkIsValid();
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onBrowseFileButtonClicked()
{
  Gtk::FileChooserDialog Dialog(_("Choose the file to add"));

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  std::string FilePath = mp_FilePathEntry->get_text();
  Glib::RefPtr<Gio::File> File = Gio::File::create_for_path(FilePath);
  if (FilePath == "" || !File->query_exists())
  {
    Dialog.set_current_folder(
        openfluid::base::ProjectManager::getInstance()->getInputDir());
  }
  else
    Dialog.set_filename(FilePath);

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    mp_FilePathEntry->set_text(Glib::filename_to_utf8(Dialog.get_filename()));
    mp_FilePathEntry->set_width_chars(mp_FilePathEntry->get_text_length());

    mref_TreeModel->clear();

    File = Gio::File::create_for_path(mp_FilePathEntry->get_text());
    if (!File)
      return;

    Glib::RefPtr<Gio::File> Dir = File->get_parent();
    if (!Dir)
      return;

    std::string Root = getRoot(File->get_basename());

    int SelectedCount = 0;

    Glib::RefPtr<Gio::FileEnumerator> Children = Dir->enumerate_children();
    for (Glib::RefPtr<Gio::FileInfo> Child = Children->next_file(); Child != 0;
        Child = Children->next_file())
    {
      if (!Child->is_backup() && !Child->is_hidden() && !Child->is_symlink())
      {
        Glib::RefPtr<Gio::File> ChildFile = Dir->get_child(Child->get_name());

        if (ChildFile->query_file_type() != Gio::FILE_TYPE_DIRECTORY)
        {
          Gtk::TreeRow Row = *mref_TreeModel->append();

          Glib::ustring ChildName = Glib::filename_to_utf8(
              ChildFile->get_basename());

          Row[m_Columns.m_File] = ChildFile;
          Row[m_Columns.m_FileName] = ChildName;

          bool SetSelected = (Root == getRoot(ChildName));
          Row[m_Columns.m_Selected] = SetSelected;

          if (SetSelected)
            SelectedCount++;

          Row[m_Columns.m_Sensitive] = (ChildFile->get_path()
              != File->get_path());
        }
      }
    }
    Children->close();

    mp_Expander->set_expanded(SelectedCount > 1);

    if (mref_TreeModel->children().size() > 5)
      mp_TreeView->set_size_request(-1, 100);
  }
}

// =====================================================================
// =====================================================================

std::string DatasoreAddItemDialog::getRoot(std::string FileName)
{
  std::string Root = FileName;

  std::size_t DotPos = FileName.find_first_of(".");

  if (DotPos != std::string::npos)
    Root.erase(Root.begin() + DotPos, Root.end());

  return Root;
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onBrowseDirButtonClicked()
{
  Gtk::FileChooserDialog Dialog(_("Choose the directory to add"),
                                Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);

  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

  std::string DirPath = mp_DirPathEntry->get_text();
  Glib::RefPtr<Gio::File> File = Gio::File::create_for_path(DirPath);
  if (DirPath == "" || !File->query_exists())
  {
    Dialog.set_current_folder(
        openfluid::base::ProjectManager::getInstance()->getInputDir());
  }
  else
    Dialog.set_current_folder(DirPath);

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    mp_DirPathEntry->set_text(Glib::filename_to_utf8(Dialog.get_filename()));
    mp_DirPathEntry->set_width_chars(mp_DirPathEntry->get_text_length());
  }
}

// =====================================================================
// =====================================================================

void DatasoreAddItemDialog::onSubDirToggled()
{
  mp_SubDirEntry->set_sensitive(mp_SubDirRadio->get_active());
}

// =====================================================================
// =====================================================================

bool DatasoreAddItemDialog::isFileInInputDir(Glib::RefPtr<Gio::File> File)
{
  return !mref_InDirFile->get_relative_path(File).empty();
}

// =====================================================================
// =====================================================================

bool DatasoreAddItemDialog::copyFile(Glib::RefPtr<Gio::File> FromFile,
                                     Glib::RefPtr<Gio::File>& ToFile)
{
  std::string ToPath = mref_InDirFile->get_path();

  if (mp_SubDirRadio->get_active())
  {
    std::string SubDir = mp_SubDirEntry->get_text();

    if (!EngineHelper::isEmptyString(SubDir))
    {
      ToPath.append("/").append(SubDir);

      Glib::RefPtr<Gio::File> SubDir = Gio::File::create_for_path(ToPath);

      if (!SubDir->query_exists())
        SubDir->make_directory_with_parents();
    }
  }

  ToPath.append("/").append(FromFile->get_basename());

  ToFile = Gio::File::create_for_path(ToPath);

  Gio::FileCopyFlags Flag = Gio::FILE_COPY_NONE;

  if (ToFile->query_exists())
  {
    if (!openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
        Glib::ustring::compose(
            _("%1 already exists.\nDo you want to overwrite it ?"), ToPath)))
      return false;

    Flag = Gio::FILE_COPY_OVERWRITE;
  }

  if (FromFile->query_file_type() == Gio::FILE_TYPE_DIRECTORY)
  {
    try
    {
      openfluid::tools::CopyDirectoryRecursively(
          FromFile->get_path(), ToFile->get_parent()->get_path());
    }
    catch (...)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
          Glib::ustring::compose(
              _("Error while copying %1 into %2"),
              FromFile->get_path(), ToFile->get_parent()->get_path()));
      return false;
    }
  }
  else
  {
    try
    {
      FromFile->copy(ToFile, Flag);
    }
    catch (Gio::Error& e)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
          Glib::ustring::compose(
              _("Error while copying %1 to %2"),
              FromFile->get_path(), ToPath));
      return false;
    }
  }

  return true;
}

// =====================================================================
// =====================================================================

openfluid::fluidx::DatastoreItemDescriptor* DatasoreAddItemDialog::createItem(
    std::string RelativePath)
{
  openfluid::core::UnstructuredValue::UnstructuredType Type;
  openfluid::core::UnstructuredValue::getValueTypeFromString(
      mp_TypeCombo->get_active_text(), Type);

  openfluid::fluidx::DatastoreItemDescriptor* NewItem =
      new openfluid::fluidx::DatastoreItemDescriptor(mp_IDEntry->get_text(),
                                                     mref_InDirFile->get_path(),
                                                     RelativePath, Type);

  if (mp_ClassCheck->get_active())
    NewItem->setUnitClass(mp_ClassEntry->get_text());

  return NewItem;
}

// =====================================================================
// =====================================================================
