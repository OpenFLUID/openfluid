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
 \file ModelFctDetailView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctDetailView.hpp"

#include <glibmm/i18n.h>

#include "ModelFctDetailInfoPage.hpp"

// =====================================================================
// =====================================================================


ModelFctDetailViewImpl::ModelFctDetailViewImpl(ModelFctDetailColumns& Columns) :
  m_Columns(Columns)
{
  mp_Notebook = Gtk::manage(new Gtk::Notebook());

  mp_InfoPage = new ModelFctDetailInfoPage();

  mp_ParamsTreeView = Gtk::manage(new Gtk::TreeView());
  mp_ParamsTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_ParamsTreeView->append_column(_("Unit"), m_Columns.m_Unit);
  mp_ParamsTreeView->append_column(_("Description"), m_Columns.m_Description);
  mp_ParamsTreeView->set_visible(true);

  mp_VarsTreeView = Gtk::manage(new Gtk::TreeView());
  mp_VarsTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_VarsTreeView->append_column(_("Unit"), m_Columns.m_Unit);
  mp_VarsTreeView->append_column(_("Unit class"), m_Columns.m_Class);
  mp_VarsTreeView->append_column(_("Description"), m_Columns.m_Description);

  mp_IDataTreeView = Gtk::manage(new Gtk::TreeView());
  mp_IDataTreeView->append_column(_("Name"), m_Columns.m_Name);
  mp_IDataTreeView->append_column(_("Unit class"), m_Columns.m_Class);

  mp_EventsTreeView = Gtk::manage(new Gtk::TreeView());
  mp_EventsTreeView->append_column(_("Unit class"), m_Columns.m_Class);

  mp_ExtraFilesTreeView = Gtk::manage(new Gtk::TreeView());
  mp_ExtraFilesTreeView->append_column(_("Name"), m_Columns.m_Name);

  Gtk::ScrolledWindow* ParamsWin = Gtk::manage(new Gtk::ScrolledWindow());
  ParamsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ParamsWin->set_visible(true);
  ParamsWin->add(*mp_ParamsTreeView);

  Gtk::ScrolledWindow* VarsWin = Gtk::manage(new Gtk::ScrolledWindow());
  VarsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  VarsWin->set_visible(true);
  VarsWin->add(*mp_VarsTreeView);

  Gtk::ScrolledWindow* IDataWin = Gtk::manage(new Gtk::ScrolledWindow());
  IDataWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  IDataWin->set_visible(true);
  IDataWin->add(*mp_IDataTreeView);

  Gtk::ScrolledWindow* EventsWin = Gtk::manage(new Gtk::ScrolledWindow());
  EventsWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  EventsWin->set_visible(true);
  EventsWin->add(*mp_EventsTreeView);

  Gtk::ScrolledWindow* ExtraFilesWin = Gtk::manage(new Gtk::ScrolledWindow());
  ExtraFilesWin->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  ExtraFilesWin->set_visible(true);
  ExtraFilesWin->add(*mp_ExtraFilesTreeView);

  mp_Notebook->append_page(*mp_InfoPage->asWidget(), _("Information"));
  mp_Notebook->append_page(*ParamsWin, _("Parameters"));
  mp_Notebook->append_page(*VarsWin, _("Variables"));
  mp_Notebook->append_page(*IDataWin, _("Inputdata"));
  mp_Notebook->append_page(*EventsWin, _("Events"));
  mp_Notebook->append_page(*ExtraFilesWin, _("Extra Files"));
  mp_Notebook->set_visible(true);
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setPluggableElementsVisible(bool Visible)
{
  mp_InfoPage->setPluggableElementsVisible(Visible);
  mp_VarsTreeView->set_visible(Visible);
  mp_IDataTreeView->set_visible(Visible);
  mp_EventsTreeView->set_visible(Visible);
  mp_ExtraFilesTreeView->set_visible(Visible);
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setInfos(std::map<std::string, std::string> Infos)
{
  mp_InfoPage->setInfos(Infos);
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setParamsModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_ParamsTreeView->set_model(Model);
  mp_ParamsTreeView->get_column(0)->set_sort_column(m_Columns.m_Name);
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setVarsModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_VarsTreeView->set_model(Model);
  mp_VarsTreeView->get_column(0)->set_sort_column(m_Columns.m_Name);
  mp_VarsTreeView->expand_all();
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setIDataModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_IDataTreeView->set_model(Model);
  mp_IDataTreeView->get_column(0)->set_sort_column(m_Columns.m_Name);
  mp_IDataTreeView->expand_all();
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setEventsModel(Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_EventsTreeView->set_model(Model);
  mp_EventsTreeView->get_column(0)->set_sort_column(m_Columns.m_Class);
}

// =====================================================================
// =====================================================================


void ModelFctDetailViewImpl::setExtraFilesModel(
    Glib::RefPtr<Gtk::TreeModel> Model)
{
  mp_ExtraFilesTreeView->set_model(Model);
  mp_ExtraFilesTreeView->get_column(0)->set_sort_column(m_Columns.m_Name);
  mp_ExtraFilesTreeView->expand_all();
}

// =====================================================================
// =====================================================================


Gtk::Widget* ModelFctDetailViewImpl::asWidget()
{
  return mp_Notebook;
}

// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


ModelFctDetailViewSub::ModelFctDetailViewSub(ModelFctDetailColumns& Columns) :
  ModelFctDetailViewImpl(Columns)
{
}

// =====================================================================
// =====================================================================

int ModelFctDetailViewSub::getNotebookVisiblePageNb()
{
  int NbVisibles = 0;
  for (int i = 0; i < mp_Notebook->get_n_pages(); i++)
  {
    if (mp_Notebook->get_nth_page(i)->is_visible())
      NbVisibles++;
  }
  return NbVisibles;
}

// =====================================================================
// =====================================================================


std::string ModelFctDetailViewSub::getIdValue()
{
  return mp_InfoPage->getIdValue();
}

// =====================================================================
// =====================================================================


bool ModelFctDetailViewSub::isIdVisible()
{
  return mp_InfoPage->isIdVisible();
}

// =====================================================================
// =====================================================================


int ModelFctDetailViewSub::getParamsRowNb()
{
  return mp_ParamsTreeView->get_model()->children().size();
}
