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
 \file FunctionParamWidget
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "FunctionParamWidget.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/table.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/separator.h>
#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/base/ProjectManager.hpp>
#include "EngineHelper.hpp"

// =====================================================================
// =====================================================================

FunctionParamRow::FunctionParamRow(
    openfluid::fluidx::ModelItemDescriptor& FctDesc, std::string ParamName,
    std::string ParamValue, std::string ParamUnit, std::string ParamDescription,
    bool WithRemoveBt) :
    m_FctDesc(FctDesc), m_Name(ParamName)
{
  m_ColumnCount = 5;

  std::string DescriptionTooltip =
      ParamDescription.empty() ? _("no description") : ParamDescription;

  Gtk::Label* NameLabel = Gtk::manage(new Gtk::Label(m_Name, 0, 0.5));
  NameLabel->set_tooltip_text(DescriptionTooltip);
  m_RowWidgets.push_back(NameLabel);

  mp_ValueEntry = Gtk::manage(new Gtk::Entry());
  mp_ValueEntry->set_text(ParamValue);
  mp_ValueEntry->set_tooltip_text(DescriptionTooltip);
  mp_ValueEntry->signal_changed().connect(
      sigc::mem_fun(*this, &FunctionParamRow::onValueChanged));
  m_RowWidgets.push_back(mp_ValueEntry);

  Gtk::Label* UnitLabel = Gtk::manage(new Gtk::Label(ParamUnit, 0, 0.5));
  m_RowWidgets.push_back(UnitLabel);

  mp_GlobalLabel = Gtk::manage(new Gtk::Label("", 0, 0.5));
  mp_GlobalLabel->set_sensitive(EngineHelper::isEmptyString(ParamValue));
  m_RowWidgets.push_back(mp_GlobalLabel);

  if (WithRemoveBt)
  {
    mp_RemoveButton = Gtk::manage(new Gtk::Button());
    mp_RemoveButton->set_image(
        *Gtk::manage(
            new Gtk::Image(Gtk::Stock::REMOVE, Gtk::ICON_SIZE_BUTTON)));
    mp_RemoveButton->set_relief(Gtk::RELIEF_NONE);
    mp_RemoveButton->set_tooltip_text(
        Glib::ustring::compose(_("Remove %1"), m_Name));
    mp_RemoveButton->signal_clicked().connect(
        sigc::mem_fun(*this, &FunctionParamRow::onRemoveButtonClicked));
    m_RowWidgets.push_back(mp_RemoveButton);
  }
  else
    m_RowWidgets.push_back(new Gtk::HBox());
}

// =====================================================================
// =====================================================================

void FunctionParamRow::setGlobalValue(std::string GlobalValue)
{
  if (GlobalValue.empty())
    mp_GlobalLabel->set_text("");
  else
    mp_GlobalLabel->set_text(
        Glib::ustring::compose("(%1 \"%2\")", _("global value:"), GlobalValue));
}

// =====================================================================
// =====================================================================

void FunctionParamRow::onValueChanged()
{
  std::string NewValue = mp_ValueEntry->get_text();

  m_FctDesc.setParameter(m_Name, NewValue);

  mp_GlobalLabel->set_sensitive(EngineHelper::isEmptyString(NewValue));

  m_signal_valueChangeOccured.emit();
}

// =====================================================================
// =====================================================================

void FunctionParamRow::onRemoveButtonClicked()
{
  m_FctDesc.eraseParameter(m_Name);
  m_signal_removeOccured.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamRow::signal_removeOccured()
{
  return m_signal_removeOccured;
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamRow::signal_valueChangeOccured()
{
  return m_signal_valueChangeOccured;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

FunctionParamFileRow::FunctionParamFileRow(std::string FileName,
                                           bool IsRequired) :
    m_FileName(FileName), m_IsRequired(IsRequired)
{
  m_File = Gio::File::create_for_path(
      Glib::ustring::compose(
          "%1/%2",
          openfluid::base::ProjectManager::getInstance()->getInputDir(),
          m_FileName));

  m_ColumnCount = 2;

  mp_FileButton = Gtk::manage(new Gtk::Button());
  mp_FileButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::OPEN, Gtk::ICON_SIZE_BUTTON)));
  mp_FileButton->set_visible(true);
  mp_FileButton->signal_clicked().connect(
      sigc::mem_fun(*this, &FunctionParamFileRow::onFileButtonClicked));
  m_RowWidgets.push_back(mp_FileButton);

  mp_FileNameLabel = Gtk::manage(
      new Gtk::Label("", Gtk::ALIGN_LEFT, Gtk::ALIGN_CENTER));
  mp_FileNameLabel->set_visible(true);
  mp_FileNameLabel->set_padding(10, 0);
  m_RowWidgets.push_back(mp_FileNameLabel);

  setFileFound();
}

// =====================================================================
// =====================================================================

void FunctionParamFileRow::onFileButtonClicked()
{
  Gtk::FileChooserDialog Dialog(
      _("Select a file to copy in the project directory"));

  Dialog.set_file(m_File);
  Dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  Dialog.add_button(Gtk::Stock::COPY, Gtk::RESPONSE_OK);

  if (Dialog.run() == Gtk::RESPONSE_OK)
  {
    Glib::RefPtr<Gio::File> ChoosedFile = Dialog.get_file();

    if (!ChoosedFile)
      return;

    if (m_File->get_path() == ChoosedFile->get_path())
      return;

    Gio::FileCopyFlags Flag = Gio::FILE_COPY_NONE;

    if (m_File->query_exists() && m_File->query_file_type()
        != Gio::FILE_TYPE_DIRECTORY)
    {
      if (!openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
          Glib::ustring::compose(
              _("File %1 already exists.\nDo you really want to overwrite it?"),
              m_FileName)))
        return;

      Flag = Gio::FILE_COPY_OVERWRITE;
    }

    try
    {
      Glib::RefPtr<Gio::File> Parent = m_File->get_parent();
      if (!Parent->query_exists())
        Parent->make_directory_with_parents();

      ChoosedFile->copy(m_File, Flag);
    }
    catch (Gio::Error& e)
    {
      openfluid::guicommon::DialogBoxFactory::showSimpleErrorMessage(
          _("Error while copying file"));
    }

    setFileFound();

    m_signal_FileChanged.emit();
  }
}

// =====================================================================
// =====================================================================

void FunctionParamFileRow::setFileFound()
{
  if (m_File->query_exists() && m_File->query_file_type()
      != Gio::FILE_TYPE_DIRECTORY)
  {
    mp_FileNameLabel->set_markup(
        Glib::ustring::compose("%1  <i>(%2)</i>", m_FileName, _("found")));
    mp_FileButton->set_tooltip_text(_("Change the file to use"));
  }
  else
  {
    mp_FileNameLabel->set_markup(
        Glib::ustring::compose("<span color='%3'>%1  <i>(%2)</i></span>",
                               m_FileName, _("not found"),
                               m_IsRequired ? "red" : "orange"));
    mp_FileButton->set_tooltip_text(_("Choose the file to use"));
  }
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamFileRow::signal_FileChanged()
{
  return m_signal_FileChanged;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================

FunctionParamWidget::FunctionParamWidget(
    openfluid::fluidx::ModelItemDescriptor& FctDesc,
    openfluid::machine::ModelItemSignatureInstance* Sign,
    FunctionAddParamDialog& AddParamDialog) :
    m_FctDesc(FctDesc), mp_Sign(Sign), m_AddParamDialog(AddParamDialog)
{
  Gtk::Button* AddButton = Gtk::manage(new Gtk::Button());
  AddButton->set_image(
      *Gtk::manage(new Gtk::Image(Gtk::Stock::ADD, Gtk::ICON_SIZE_BUTTON)));
  AddButton->set_tooltip_text(_("Add a parameter"));
  AddButton->signal_clicked().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onAddButtonClicked));

  Gtk::HBox* ButtonBox = Gtk::manage(new Gtk::HBox());
  ButtonBox->pack_start(*AddButton, Gtk::PACK_SHRINK, 0);

  mp_ParamsTable = Gtk::manage(new Gtk::Table());
  mp_ParamsTable->set_col_spacings(10);
  mp_ParamsTable->set_border_width(5);

  mp_RequiredFilesTable = Gtk::manage(new Gtk::Table());
  mp_RequiredFilesTable->set_col_spacings(10);
  mp_RequiredFilesTable->set_border_width(5);

  mp_UsedFilesTable = Gtk::manage(new Gtk::Table());
  mp_UsedFilesTable->set_col_spacings(10);
  mp_UsedFilesTable->set_border_width(5);

  pack_start(*ButtonBox, Gtk::PACK_SHRINK);
  pack_start(*mp_ParamsTable, Gtk::PACK_EXPAND_WIDGET);
  pack_start(*mp_RequiredFilesTable, Gtk::PACK_SHRINK);
  pack_start(*mp_UsedFilesTable, Gtk::PACK_SHRINK);

  updateRows();

  set_visible(true);
  show_all_children();
}

// =====================================================================
// =====================================================================

FunctionParamWidget::~FunctionParamWidget()
{

}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateRows()
{
  updateParamsRows();
  updateRequiredFilesRows();
  updateUsedFilesRows();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateParamsRows()
{
  int TableWidgetCount = mp_ParamsTable->children().size();
  for (int i = 0; i < TableWidgetCount; i++)
    mp_ParamsTable->remove(*mp_ParamsTable->children().begin()->get_widget());

  m_ParamsRows.clear();

  m_CurrentParamsTableBottom = 0;

  openfluid::ware::WareParams_t Params = m_FctDesc.getParameters();

  if (mp_Sign)
  {
    std::vector<openfluid::ware::SignatureHandledDataItem> Items =
        mp_Sign->Signature->HandledData.FunctionParams;
    for (std::vector<openfluid::ware::SignatureHandledDataItem>::iterator it =
        Items.begin(); it != Items.end(); ++it)
    {
      attachParamsRow(
          new FunctionParamRow(m_FctDesc, it->DataName, Params[it->DataName],
                               it->DataUnit, it->Description, false),
          it->DataName);

      Params.erase(it->DataName);
    }
  }

  // add any parameter that was set but not present in signature
  for (openfluid::ware::WareParams_t::iterator it = Params.begin();
      it != Params.end(); ++it)
  {
    attachParamsRow(
        new FunctionParamRow(m_FctDesc, it->first, it->second, "", "", true),
        it->first);
  }

  if (m_ParamsRows.empty())
  {
    Gtk::Label* EmptyLabel = Gtk::manage(new Gtk::Label("", 0.5, 0));
    EmptyLabel->set_markup(_("<i>No parameters</i>"));
    mp_ParamsTable->attach(*EmptyLabel, 0, 1, 0, 1);
  }

  mp_ParamsTable->show_all_children();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::attachParamsRow(FunctionParamRow* Row,
                                          std::string ParamName)
{
  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_ParamsTable->attach(
        *Row->getWidgets()[i], i, i + 1, m_CurrentParamsTableBottom,
        m_CurrentParamsTableBottom + 1,
        i == 1 ? Gtk::EXPAND | Gtk::FILL : Gtk::SHRINK | Gtk::FILL, Gtk::FILL,
        0, 0);
  }

  Row->signal_removeOccured().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onStructureChangeOccured));
  Row->signal_valueChangeOccured().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onValueChangeOccured));

  m_CurrentParamsTableBottom++;

  m_ParamsRows[ParamName] = Row;
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateRequiredFilesRows()
{
  int TableWidgetCount = mp_RequiredFilesTable->children().size();
  for (int i = 0; i < TableWidgetCount; i++)
    mp_RequiredFilesTable->remove(
        *mp_RequiredFilesTable->children().begin()->get_widget());

  if (mp_Sign)
  {
    std::vector<std::string> Items =
        mp_Sign->Signature->HandledData.RequiredExtraFiles;

    if (m_FctDesc.isType(openfluid::fluidx::WareDescriptor::Generator))
    {
      openfluid::fluidx::GeneratorDescriptor::GeneratorMethod Method =
          (static_cast<openfluid::fluidx::GeneratorDescriptor*>(&m_FctDesc))->getGeneratorMethod();

      if (Method == openfluid::fluidx::GeneratorDescriptor::Interp || Method
          == openfluid::fluidx::GeneratorDescriptor::Inject)
      {
        Items.clear();

        std::string FileNameFromParam = m_FctDesc.getParameters()["sources"];
        if (!FileNameFromParam.empty())
          Items.push_back(FileNameFromParam);

        FileNameFromParam = m_FctDesc.getParameters()["distribution"];
        if (!FileNameFromParam.empty())
          Items.push_back(FileNameFromParam);
      }
    }

    if (!Items.empty())
    {
      Gtk::HSeparator* HSep = Gtk::manage(new Gtk::HSeparator());
      Gtk::Label* ReqFilesLabel = Gtk::manage(
          new Gtk::Label(_("Required files"), 0, 0.5));
      mp_RequiredFilesTable->attach(*HSep, 0, 2, 0, 1, Gtk::SHRINK | Gtk::FILL,
                                    Gtk::FILL);
      mp_RequiredFilesTable->attach(*ReqFilesLabel, 0, 2, 1, 2,
                                    Gtk::SHRINK | Gtk::FILL, Gtk::FILL);
    }

    m_CurrentReqFilesTableBottom = 2;

    for (std::vector<std::string>::iterator it = Items.begin();
        it != Items.end(); ++it)
      attachRequiredFileRow(new FunctionParamFileRow(*it, true));
  }

  mp_RequiredFilesTable->show_all_children();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::attachRequiredFileRow(FunctionParamFileRow* Row)
{
  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_RequiredFilesTable->attach(
        *Row->getWidgets()[i], i, i + 1, m_CurrentReqFilesTableBottom,
        m_CurrentReqFilesTableBottom + 1,
        i == 1 ? Gtk::EXPAND | Gtk::FILL : Gtk::SHRINK | Gtk::FILL, Gtk::FILL,
        0, 0);
  }

  Row->signal_FileChanged().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onFileChangeOccured));

  m_CurrentReqFilesTableBottom++;
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateUsedFilesRows()
{
  int TableWidgetCount = mp_UsedFilesTable->children().size();
  for (int i = 0; i < TableWidgetCount; i++)
    mp_UsedFilesTable->remove(
        *mp_UsedFilesTable->children().begin()->get_widget());

  if (mp_Sign)
  {
    std::vector<std::string> Items =
        mp_Sign->Signature->HandledData.UsedExtraFiles;

    if (!Items.empty())
    {
      Gtk::HSeparator* HSep = Gtk::manage(new Gtk::HSeparator());
      Gtk::Label* ReqFilesLabel = Gtk::manage(
          new Gtk::Label(_("Used files"), 0, 0.5));
      mp_UsedFilesTable->attach(*HSep, 0, 2, 0, 1, Gtk::SHRINK | Gtk::FILL,
                                Gtk::FILL);
      mp_UsedFilesTable->attach(*ReqFilesLabel, 0, 2, 1, 2,
                                Gtk::SHRINK | Gtk::FILL, Gtk::FILL);
    }

    m_CurrentUsedFilesTableBottom = 2;

    for (std::vector<std::string>::iterator it = Items.begin();
        it != Items.end(); ++it)
      attachUsedFileRow(new FunctionParamFileRow(*it, false));
  }

  mp_UsedFilesTable->show_all_children();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::attachUsedFileRow(FunctionParamFileRow* Row)
{
  for (unsigned int i = 0; i < Row->getColumnCount(); i++)
  {
    mp_UsedFilesTable->attach(
        *Row->getWidgets()[i], i, i + 1, m_CurrentUsedFilesTableBottom,
        m_CurrentUsedFilesTableBottom + 1,
        i == 1 ? Gtk::EXPAND | Gtk::FILL : Gtk::SHRINK | Gtk::FILL, Gtk::FILL,
        0, 0);
  }

  Row->signal_FileChanged().connect(
      sigc::mem_fun(*this, &FunctionParamWidget::onFileChangeOccured));

  m_CurrentUsedFilesTableBottom++;
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onAddButtonClicked()
{
  if (m_AddParamDialog.show(&m_FctDesc, mp_Sign))
    onStructureChangeOccured();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateGlobals(
    const openfluid::ware::WareParams_t& GlobalParams)
{
  m_Globals = GlobalParams;

  updateGlobals();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::updateGlobals()
{
  for (std::map<std::string, FunctionParamRow*>::iterator it =
      m_ParamsRows.begin(); it != m_ParamsRows.end(); ++it)
  {
    openfluid::ware::WareParams_t::const_iterator Found = m_Globals.find(
        it->first);

    it->second->setGlobalValue(
        Found != m_Globals.end() ? Found->second.get() : "");
  }
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onStructureChangeOccured()
{
  updateRows();
  updateGlobals();
  m_signal_changeOccured.emit();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onValueChangeOccured()
{
  // for the required files of generators
  updateRequiredFilesRows();
  m_signal_changeOccured.emit();
}

// =====================================================================
// =====================================================================

void FunctionParamWidget::onFileChangeOccured()
{
  m_signal_fileChangeOccured.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamWidget::signal_changeOccured()
{
  return m_signal_changeOccured;
}

// =====================================================================
// =====================================================================

sigc::signal<void> FunctionParamWidget::signal_fileChangeOccured()
{
  return m_signal_fileChangeOccured;
}
// =====================================================================
// =====================================================================

