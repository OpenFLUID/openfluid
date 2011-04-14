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
 \file ModelFctDetailInfoPage.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctDetailInfoPage.hpp"

// =====================================================================
// =====================================================================

void ModelFctDetailInfoPage::attachTableRow(
    ModelFctDetailInfoTableRow& TableRow)
{
  for (unsigned int i = 0; i < TableRow.getRowCount(); i++)
  {
    mp_InfoTable ->attach(*TableRow.getWidgetsOfRow(i)[0], 0, 1,
        m_CurrentTableBottom + i, m_CurrentTableBottom + i + 1, Gtk::FILL,
        Gtk::FILL, 0, 0);
    mp_InfoTable ->attach(*TableRow.getWidgetsOfRow(i)[1], 1, 2,
        m_CurrentTableBottom + i, m_CurrentTableBottom + i + 1, Gtk::FILL
            | Gtk::EXPAND, Gtk::FILL, 5, 0);
    m_CurrentTableBottom += 2;
  }
}

// =====================================================================
// =====================================================================


Gtk::HSeparator* ModelFctDetailInfoPage::attachTableSeparator()
{
  Gtk::HSeparator* p_Separator = Gtk::manage(new Gtk::HSeparator());
  p_Separator->set_visible(true);
  mp_InfoTable ->attach(*p_Separator, 0, 2, m_CurrentTableBottom,
      m_CurrentTableBottom + 1, Gtk::FILL | Gtk::EXPAND, Gtk::FILL, 0, 0);
  m_CurrentTableBottom++;
  return p_Separator;
}

// =====================================================================
// =====================================================================


ModelFctDetailInfoPage::ModelFctDetailInfoPage() :
  m_CurrentTableBottom(0)
{
  mp_InfoTable = Gtk::manage(new Gtk::Table());
  mp_InfoTable->set_border_width(3);

  mp_IdRow = new ModelFctDetailInfoTableRow(_("Id :"));
  mp_IdRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_IdRow);

  mp_NameRow = new ModelFctDetailInfoTableRow(_("Name :"));
  mp_NameRow->setVisible(true);

  mp_PathRow = new ModelFctDetailInfoTableRow(_("Path :"));
  mp_PathRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_PathRow);

  mp_DescriptionRow = new ModelFctDetailInfoTableRow(_("Description :"));
  mp_DescriptionRow->setVisible(true);

  mp_VersionRow = new ModelFctDetailInfoTableRow(_("Version :"));
  mp_VersionRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_VersionRow);

  mp_StatusRow = new ModelFctDetailInfoTableRow(_("Status :"));
  mp_StatusRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_StatusRow);

  mp_DomainRow = new ModelFctDetailInfoTableRow(_("Domain :"));
  mp_DomainRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_DomainRow);

  mp_ProcessRow = new ModelFctDetailInfoTableRow(_("Process :"));
  mp_ProcessRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_ProcessRow);

  mp_MethodRow = new ModelFctDetailInfoTableRow(_("Method :"));
  mp_MethodRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_MethodRow);

  mp_AuthorRow = new ModelFctDetailInfoTableRow(_("Author(s) :"));
  mp_AuthorRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_AuthorRow);

  mp_AuthorMailRow = new ModelFctDetailInfoTableRow(_("Author(s) email(s) :"));
  mp_AuthorMailRow->setVisible(true);
  m_RowsAboutPluggable.push_back(mp_AuthorMailRow);

  attachTableRow(*mp_IdRow);
  m_SeparatorsAboutPluggable.push_back(attachTableSeparator());
  attachTableRow(*mp_NameRow);
  attachTableSeparator();
  attachTableRow(*mp_PathRow);
  m_SeparatorsAboutPluggable.push_back(attachTableSeparator());
  attachTableRow(*mp_DescriptionRow);
  attachTableSeparator();
  attachTableRow(*mp_VersionRow);
  attachTableRow(*mp_StatusRow);
  m_SeparatorsAboutPluggable.push_back(attachTableSeparator());
  attachTableRow(*mp_DomainRow);
  attachTableRow(*mp_ProcessRow);
  attachTableRow(*mp_MethodRow);
  m_SeparatorsAboutPluggable.push_back(attachTableSeparator());
  attachTableRow(*mp_AuthorRow);
  attachTableRow(*mp_AuthorMailRow);
  m_SeparatorsAboutPluggable.push_back(attachTableSeparator());

  mp_InfoTable->set_visible(true);

  mp_MainWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_MainWin->set_visible(true);
  mp_MainWin->add(*mp_InfoTable);
}

// =====================================================================
// =====================================================================


void ModelFctDetailInfoPage::setInfos(std::map<std::string, std::string> Infos)
{
  mp_IdRow->setValueText(Infos["id"]);
  mp_NameRow->setValueText(Infos["name"]);
  mp_PathRow->setValueText(Infos["path"]);
  mp_DescriptionRow->setValueText(Infos["description"]);
  mp_VersionRow->setValueText(Infos["version"]);
  mp_StatusRow->setValueText(Infos["status"]);
  mp_DomainRow->setValueText(Infos["domain"]);
  mp_ProcessRow->setValueText(Infos["process"]);
  mp_MethodRow->setValueText(Infos["method"]);
  mp_AuthorRow->setValueText(Infos["author"]);
  mp_AuthorMailRow->setValueText(Infos["authormail"]);
}

// =====================================================================
// =====================================================================


Gtk::Widget* ModelFctDetailInfoPage::asWidget()
{
  return mp_MainWin;
}

// =====================================================================
// =====================================================================


void ModelFctDetailInfoPage::setPluggableElementsVisible(bool Visible)
{
  for (unsigned int i = 0; i < m_RowsAboutPluggable.size(); i++)
    m_RowsAboutPluggable[i]->setVisible(Visible);
  for (unsigned int i = 0; i < m_SeparatorsAboutPluggable.size(); i++)
    m_SeparatorsAboutPluggable[i]->set_visible(Visible);
}

// =====================================================================
// =====================================================================


std::string ModelFctDetailInfoPage::getIdValue()
{
  return mp_IdRow->getValueText();
}

// =====================================================================
// =====================================================================


bool ModelFctDetailInfoPage::isIdVisible()
{
  return mp_IdRow->isVisible();
}

