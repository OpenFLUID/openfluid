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
 \file ModelFctParamsView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctParamsView.hpp"

#include <glibmm/i18n.h>

#include "ModelFctParamRow.hpp"
#include "ModelFctFileRow.hpp"

// =====================================================================
// =====================================================================


ModelFctParamsViewImpl::ModelFctParamsViewImpl() :
  m_ParamsTableBottom(0), m_FilesTableBottom(0)
{
  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->set_border_width(5);

  mp_ParamsTable = Gtk::manage(new Gtk::Table());
  mp_ParamsTable->set_spacings(5);
  mp_ParamsTable->set_visible(true);

  Gtk::HSeparator* HSep = Gtk::manage(new Gtk::HSeparator());
  HSep->set_visible(true);

  mp_FilesTable = Gtk::manage(new Gtk::Table());
  mp_FilesTable->set_spacings(5);
  mp_FilesTable->set_visible(true);

  mp_MainBox->pack_start(*mp_ParamsTable, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*HSep, Gtk::PACK_SHRINK, 15);
  mp_MainBox->pack_start(*mp_FilesTable, Gtk::PACK_SHRINK);
  mp_MainBox->set_visible(true);

  mp_MainWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_MainWin->set_visible(true);
  mp_MainWin->add(*mp_MainBox);

}

// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::setParams(
    std::map<std::string, std::string> ParamsMap)
{
  Gtk::Label* ParamsLabel = Gtk::manage(new Gtk::Label(_("Parameters :"),
      Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP));
  ParamsLabel->set_visible(true);

  mp_ParamsTable->attach(*ParamsLabel, 0, 5, 0, 1, Gtk::FILL, Gtk::EXPAND, 0, 0);
  m_ParamsTableBottom++;

  for (std::map<std::string, std::string>::iterator it = ParamsMap.begin(); it
      != ParamsMap.end(); ++it)
  {
    ModelFctParamRow* ParamRow = new ModelFctParamRow(it->first, it->second);
    ParamRow->signal_ValueChanged().connect(sigc::bind<ModelFctParamRow*>(
        sigc::mem_fun(*this, &ModelFctParamsViewImpl::onParamValueChanged),
        ParamRow));
    attachTableRow(*ParamRow, mp_ParamsTable, m_ParamsTableBottom);
    m_ByParamNameParamRow[it->first] = ParamRow;
  }
}

// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::attachTableRow(BuilderTableRowWidget& TableRow,
    Gtk::Table* Table, unsigned int& TableBottom)
{
  for (unsigned int i = 0; i < TableRow.getColumnCount(); i++)
  {
    Table->attach(*TableRow.getWidgets()[i], i, i + 1, TableBottom, TableBottom
        + 1, Gtk::FILL, Gtk::FILL, 0, 0);
  }

  TableBottom++;
}

// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::onParamValueChanged(ModelFctParamRow* FctParamRow)
{
  m_signal_ParamValueChanged.emit(FctParamRow->getParamName(),
      FctParamRow->getValue());
}

// =====================================================================
// =====================================================================


sigc::signal<void, std::string, std::string> ModelFctParamsViewImpl::signal_ParamValueChanged()
{
  return m_signal_ParamValueChanged;
}

// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::setParamValues(
    std::map<std::string, std::string> ParamValuesMap)
{
  for (std::map<std::string, std::string>::iterator it = ParamValuesMap.begin(); it
      != ParamValuesMap.end(); ++it)
  {
    if (m_ByParamNameParamRow.find(it->first) != m_ByParamNameParamRow.end())
      m_ByParamNameParamRow[it->first]->setValue(it->second);
  }
}

// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::setRequiredFiles(std::vector<std::string> Files)
{
  Gtk::Label* RequiredFilesLabel = Gtk::manage(new Gtk::Label(
      _("Required files :"), Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP));
  RequiredFilesLabel->set_visible(true);

  mp_FilesTable->attach(*RequiredFilesLabel, 0, 3, 0, 1, Gtk::FILL,
      Gtk::EXPAND, 0, 0);
  m_FilesTableBottom++;

  for (unsigned int i=0 ; i< Files.size() ; i++)
  {
    ModelFctFileRow* FileRow = new ModelFctFileRow(Files[i]);
    attachTableRow(*FileRow, mp_FilesTable, m_FilesTableBottom);
    m_ByFileNameFileRow[Files[i]] = FileRow;
  }

}

// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::setUsedFiles(std::vector<std::string> Files)
{
  Gtk::Label* UsedFilesLabel = Gtk::manage(new Gtk::Label(_("Used files :"),
      Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP));
  UsedFilesLabel->set_visible(true);

  mp_FilesTable->attach(*UsedFilesLabel, 0, 3, m_FilesTableBottom,
      m_FilesTableBottom + 1, Gtk::FILL, Gtk::EXPAND, 0, 0);
  m_FilesTableBottom++;

  for (unsigned int i=0 ; i< Files.size() ; i++)
  {
    ModelFctFileRow* FileRow = new ModelFctFileRow(Files[i]);
    attachTableRow(*FileRow, mp_FilesTable, m_FilesTableBottom);
    m_ByFileNameFileRow[Files[i]] = FileRow;
  }

}


// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::setGlobalValue(std::string ParamName,std::string GlobalValue)
{
  if (m_ByParamNameParamRow.find(ParamName) != m_ByParamNameParamRow.end())
        m_ByParamNameParamRow[ParamName]->setGlobalValue(GlobalValue);
}


// =====================================================================
// =====================================================================


void ModelFctParamsViewImpl::unsetGlobalValue(std::string ParamName)
{
  if (m_ByParamNameParamRow.find(ParamName) != m_ByParamNameParamRow.end())
          m_ByParamNameParamRow[ParamName]->unsetGlobalValue();
}


// =====================================================================
// =====================================================================


Gtk::Widget* ModelFctParamsViewImpl::asWidget()
{
  return mp_MainWin;
}
