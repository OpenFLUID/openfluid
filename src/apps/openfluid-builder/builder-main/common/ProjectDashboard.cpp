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
 \file ProjectDashboard.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ProjectDashboard.hpp"

#include "BuilderGraphicsHelper.hpp"
#include "ProjectChecker.hpp"

ProjectDashboard::ProjectDashboard()
{
  mref_TreeModel = Gtk::ListStore::create(m_Columns);

  mp_TreeView = Gtk::manage(new Gtk::TreeView(mref_TreeModel));

  Gtk::TreeView::Column* pColumn = Gtk::manage(
      new Gtk::TreeView::Column("Project Dashboard"));
  pColumn->pack_start(m_Columns.m_StateIcon, false);
  pColumn->pack_start(m_Columns.m_Title, false);
  mp_TreeView->append_column(*pColumn);

  mp_TreeView->append_column("", m_Columns.m_StateInfo);
  Gtk::CellRendererText* InfoCell =
      ((Gtk::CellRendererText*) mp_TreeView->get_column_cell_renderer(1));
  InfoCell->property_wrap_mode() = Pango::WRAP_WORD;
  InfoCell->property_wrap_width() = 200;

  mp_TreeView->set_visible(true);

  mp_MainWin = Gtk::manage(new Gtk::ScrolledWindow());
  mp_MainWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_MainWin->add(*mp_TreeView);
  mp_MainWin->set_visible(true);

  m_RedIcon = BuilderGraphicsHelper::createPixbufFromFile("red.png");
  m_OrangeIcon = BuilderGraphicsHelper::createPixbufFromFile("orange.png");
  m_GreenIcon = BuilderGraphicsHelper::createPixbufFromFile("green.png");

}

// =====================================================================
// =====================================================================

void ProjectDashboard::setCheckInfo(const ProjectChecker& Checker)
{
  mref_TreeModel->clear();

  Gtk::TreeRow Row;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Model definition");
  Row[m_Columns.m_StateIcon] = Checker.IsModelOk ? m_GreenIcon : m_RedIcon;
  Row[m_Columns.m_StateInfo] = Checker.IsModelOk ? "ok" : Checker.ModelMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Model parameters");
  if (Checker.IsParamsOk && Checker.IsGeneratorParamsOk)
  {
    Row[m_Columns.m_StateIcon] = m_GreenIcon;
    Row[m_Columns.m_StateInfo] = "ok";
  }
  else
  {
    if (!Checker.IsGeneratorParamsOk)
      Row[m_Columns.m_StateIcon] = m_RedIcon;
    else
      Row[m_Columns.m_StateIcon] = m_OrangeIcon;

    Row[m_Columns.m_StateInfo] = Checker.ParamsMsg;
  }

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Required files");
  Row[m_Columns.m_StateIcon] = Checker.IsExtraFilesOk ? m_GreenIcon : m_RedIcon;
  Row[m_Columns.m_StateInfo] =
      Checker.IsExtraFilesOk ? "ok" : Checker.ExtraFilesMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Spatial representation");
  Row[m_Columns.m_StateIcon] = Checker.IsDomainOk ? m_GreenIcon : m_OrangeIcon;
  Row[m_Columns.m_StateInfo] = Checker.IsDomainOk ? "ok" : Checker.DomainMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Inputdata");
  Row[m_Columns.m_StateIcon] = Checker.IsAttributeOk ? m_GreenIcon : m_RedIcon;
  Row[m_Columns.m_StateInfo] =
      Checker.IsAttributeOk ? "ok" : Checker.AttributeMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Project consistency");
  Row[m_Columns.m_StateIcon] = Checker.IsProjectOk ? m_GreenIcon : m_RedIcon;
  Row[m_Columns.m_StateInfo] = Checker.IsProjectOk ? "ok" : Checker.ProjectMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Datastore");
  Row[m_Columns.m_StateIcon] =
      Checker.IsDatastoreOk ? m_GreenIcon : m_OrangeIcon;
  Row[m_Columns.m_StateInfo] =
      Checker.IsDatastoreOk ? "ok" : Checker.DatastoreMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Monitoring");
  Row[m_Columns.m_StateIcon] =
      Checker.IsMonitoringOk ? m_GreenIcon : m_OrangeIcon;
  Row[m_Columns.m_StateInfo] =
      Checker.IsMonitoringOk ? "ok" : Checker.MonitoringMsg;

  Row = *mref_TreeModel->append();
  Row[m_Columns.m_Title] = _("Run configuration");
  Row[m_Columns.m_StateIcon] = Checker.IsRunConfigOk ? m_GreenIcon : m_RedIcon;
  Row[m_Columns.m_StateInfo] =
      Checker.IsRunConfigOk ? "ok" : Checker.RunConfigMsg;
}

// =====================================================================
// =====================================================================

Gtk::Widget* ProjectDashboard::asWidget()
{
  return mp_MainWin;
}
