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
 \file PreferencesDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "PreferencesDialog.hpp"

#include <glibmm/i18n.h>
#include <gtkmm/treeselection.h>

#include <openfluid/guicommon/PreferencesManager.hpp>
#include <openfluid/guicommon/PreferencesPanel.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>

#include "PreferencesPanelImpl.hpp"
#include "BuilderExtensionsManager.hpp"
#include <openfluid/machine/SimulatorSignatureRegistry.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include "EngineProject.hpp"
#include "WaresHelper.hpp"

// =====================================================================
// =====================================================================

PreferencesDialog::PreferencesDialog() :
    mp_CurrentPanel(0), mp_Project(0)
{
  mref_GroupsTreeModel = Gtk::TreeStore::create(m_GroupsColumns);
  Gtk::TreeRow Row;

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "interface";
  Row[m_GroupsColumns.m_Name] = _("Interface");

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "paths";
  Row[m_GroupsColumns.m_Name] = _("Paths");

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "simulation";
  Row[m_GroupsColumns.m_Name] = _("Simulations");

  Row = *mref_GroupsTreeModel->append();
  Row[m_GroupsColumns.m_Id] = "market";
  Row[m_GroupsColumns.m_Name] = _("Market");

  mp_GroupsTreeView = Gtk::manage(new Gtk::TreeView(mref_GroupsTreeModel));
  mp_GroupsTreeView->append_column("", m_GroupsColumns.m_Name);
  mp_GroupsTreeView->set_headers_visible(false);
  mp_GroupsTreeView->set_border_width(5);
  mp_GroupsTreeView->get_selection()->signal_changed().connect(
      sigc::mem_fun(*this, &PreferencesDialog::onGroupSelectionChanged));

  mp_GroupsSWindow = Gtk::manage(new Gtk::ScrolledWindow());
  mp_GroupsSWindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  mp_GroupsSWindow->set_visible(true);
  mp_GroupsSWindow->set_size_request(170, -1);
  mp_GroupsSWindow->add(*mp_GroupsTreeView);
  mp_GroupsSWindow->set_shadow_type(Gtk::SHADOW_ETCHED_IN);

  m_GroupPanels["interface"] = new PreferencesInterfacePanel();
  m_GroupPanels["paths"] = new PreferencesPathsPanel();
  m_GroupPanels["simulation"] = new PreferencesSimPanel();
  m_GroupPanels["market"] = new PreferencesMarketPanel();

  mp_MainBox = Gtk::manage(new Gtk::HBox());
  mp_MainBox->pack_start(*mp_GroupsSWindow, Gtk::PACK_SHRINK, 6);

  Gtk::Button* CloseButton = Gtk::manage(new Gtk::Button(Gtk::Stock::CLOSE));
  CloseButton->signal_clicked().connect(
      sigc::mem_fun(*this, &PreferencesDialog::onCloseClicked));

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_MainBox, Gtk::PACK_EXPAND_WIDGET, 6);
  mp_Dialog->get_action_area()->add(*CloseButton);
  mp_Dialog->set_default_size(950, 500);
  mp_Dialog->set_border_width(6);
  mp_Dialog->set_title(_("Preferences"));
  mp_Dialog->show_all_children();

  //select first group
  mp_GroupsTreeView->get_selection()->select(
      mref_GroupsTreeModel->children().begin());
}

// =====================================================================
// =====================================================================

PreferencesDialog::~PreferencesDialog()
{
  BuilderExtensionsManager::getInstance()->deleteRegisteredExtensionPreferences();
}

// =====================================================================
// =====================================================================

void PreferencesDialog::show(EngineProject* Project)
{
  mp_Project = Project;

  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  m_PlugPathsHaveChanged = false;
  m_RecentsHaveChanged = false;
  m_ObsPathsHaveChanged = false;

  BuilderExtensionsManager* BEM = BuilderExtensionsManager::getInstance();

  if (!BEM->isPreferencesInstantiationDone())
  {
    Gtk::TreeRow Row = *mref_GroupsTreeModel->append();
    Row[m_GroupsColumns.m_Id] = "extensions";
    Row[m_GroupsColumns.m_Name] = _("Extensions");

    BEM->instantiateRegisteredExtensionPreferences();

    ExtensionContainerMap_t* Exts = BEM->getRegisteredExtensionPreferences();

    for (ExtensionContainerMap_t::iterator it = Exts->begin();
        it != Exts->end(); ++it)
    {
      Gtk::TreeRow ExtRow = *mref_GroupsTreeModel->append(Row.children());
      ExtRow[m_GroupsColumns.m_Id] = it->second.Infos.ID;
      ExtRow[m_GroupsColumns.m_Name] = it->second.Infos.ShortName;
      m_GroupPanels[it->second.Infos.ID] = it->second.Preferences;
    }

  }

  for (std::map<Glib::ustring, openfluid::guicommon::PreferencesPanel*>::iterator it =
      m_GroupPanels.begin(); it != m_GroupPanels.end(); ++it)
    it->second->init();

  //select first group
  mp_GroupsTreeView->get_selection()->select(
      mref_GroupsTreeModel->children().begin());

  if (mp_Dialog->run())
  {
    PrefMgr->save();
    mp_Dialog->hide();
  }
}

// =====================================================================
// =====================================================================

void PreferencesDialog::onGroupSelectionChanged()
{
  Gtk::TreeIter SelectedIter =
      mp_GroupsTreeView->get_selection()->get_selected();

  if (!SelectedIter)
    return;

  std::string SelectedGroup = SelectedIter->get_value(m_GroupsColumns.m_Id);

  if (m_GroupPanels.find(SelectedGroup) == m_GroupPanels.end())
    return;

  if (mp_CurrentPanel)
    mp_MainBox->remove(*mp_CurrentPanel);

  mp_CurrentPanel = m_GroupPanels[SelectedGroup]->asWidget();

  mp_MainBox->pack_start(*mp_CurrentPanel, Gtk::PACK_EXPAND_WIDGET, 6);

}

// =====================================================================
// =====================================================================

void PreferencesDialog::onCloseClicked()
{
  if (checkSimulatorsPaths() && checkObserversPaths())
    mp_Dialog->response(Gtk::RESPONSE_CLOSE);
}

// =====================================================================
// =====================================================================

bool PreferencesDialog::checkObserversPaths()
{
  openfluid::base::RuntimeEnvironment* RunEnv =
      openfluid::base::RuntimeEnvironment::getInstance();

  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  std::vector<std::string> ExistingObsPaths =
      RunEnv->getExtraObserversPluginsPaths();
  std::vector<std::string> PrefObsPaths = PrefMgr->getExtraObserversPaths();

  if (ExistingObsPaths.size() == PrefObsPaths.size() && std::equal(
      ExistingObsPaths.begin(), ExistingObsPaths.end(), PrefObsPaths.begin()))
  {
    return true;
  }

  // RunEnv <- Pref
  RunEnv->resetExtraObserversPluginsPaths();
  for (int i = PrefObsPaths.size() - 1; i >= 0; i--)
    RunEnv->addExtraObserversPluginsPaths(PrefObsPaths[i]);

  if (!mp_Project)
  {
    m_ObsPathsHaveChanged = true;
    return true;
  }

  openfluid::fluidx::AdvancedMonitoringDescriptor& Monit =
      mp_Project->getAdvancedDesc().getMonitoring();

  std::list<std::string> MissingObservers;

  std::list<openfluid::fluidx::ObserverDescriptor*> ModifiedObservers =
      WaresHelper::checkAndGetModifiedMonitoring(Monit, MissingObservers);

  if (MissingObservers.empty())
  {
    m_ObsPathsHaveChanged = true;
    return true;
  }

  std::string MissingObserversStr = "";
  for (std::list<std::string>::iterator it = MissingObservers.begin();
      it != MissingObservers.end(); ++it)
    MissingObserversStr += "- " + *it + "\n";

  Glib::ustring Msg = Glib::ustring::compose(
      _("These plugin file(s) are no more available:\n%1\n\n"
          "Corresponding observers will be removed from the monitoring.\n"
          "Do you want to continue?"),
      MissingObserversStr);

  if (openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
      Msg))
  {
    Monit.setItems(ModifiedObservers);

    m_ObsPathsHaveChanged = true;
    return true;
  }

  // reset RunEnv
  RunEnv->resetExtraObserversPluginsPaths();
  for (int i = ExistingObsPaths.size() - 1; i >= 0; i--)
    RunEnv->addExtraObserversPluginsPaths(ExistingObsPaths[i]);

  // reset Monitoring
  openfluid::machine::ObserverSignatureRegistry::getInstance()->update();

  return false;
}

// =====================================================================
// =====================================================================

bool PreferencesDialog::checkSimulatorsPaths()
{
  openfluid::base::RuntimeEnvironment* RunEnv =
      openfluid::base::RuntimeEnvironment::getInstance();

  openfluid::guicommon::PreferencesManager* PrefMgr =
      openfluid::guicommon::PreferencesManager::getInstance();

  std::vector<std::string> ExistingSimPaths =
      RunEnv->getExtraSimulatorsPluginsPaths();
  std::vector<std::string> PrefSimPaths = PrefMgr->getExtraPlugPaths();

  if (ExistingSimPaths.size() == PrefSimPaths.size() && std::equal(
      ExistingSimPaths.begin(), ExistingSimPaths.end(), PrefSimPaths.begin()))
  {
    return true;
  }

  // RunEnv <- Pref
  RunEnv->resetExtraSimulatorsPluginsPaths();
  for (int i = PrefSimPaths.size() - 1; i >= 0; i--)
    RunEnv->addExtraSimulatorsPluginsPaths(PrefSimPaths[i]);

  if (!mp_Project)
  {
    m_PlugPathsHaveChanged = true;
    return true;
  }

  openfluid::fluidx::AdvancedModelDescriptor& Model =
      mp_Project->getAdvancedDesc().getModel();

  std::list<std::string> MissingSimulators;

  std::list<openfluid::fluidx::ModelItemDescriptor*> ModifiedSimulators =
      WaresHelper::checkAndGetModifiedModel(Model, MissingSimulators);

  if (MissingSimulators.empty())
  {
    m_PlugPathsHaveChanged = true;
    return true;
  }

  std::string MissingSimulatorsStr = "";
  for (std::list<std::string>::iterator it = MissingSimulators.begin();
      it != MissingSimulators.end(); ++it)
    MissingSimulatorsStr += "- " + *it + "\n";

  Glib::ustring Msg = Glib::ustring::compose(
      _("These plugin file(s) are no more available:\n%1\n\n"
          "Corresponding simulator(s) will be removed from the model.\n"
          "Do you want to continue?"),
      MissingSimulatorsStr);

  if (openfluid::guicommon::DialogBoxFactory::showSimpleOkCancelQuestionDialog(
      Msg))
  {
    Model.setItems(ModifiedSimulators);

    m_PlugPathsHaveChanged = true;
    return true;
  }

  // reset RunEnv
  RunEnv->resetExtraSimulatorsPluginsPaths();
  for (int i = ExistingSimPaths.size() - 1; i >= 0; i--)
    RunEnv->addExtraSimulatorsPluginsPaths(ExistingSimPaths[i]);

  // reset Model
  openfluid::machine::SimulatorSignatureRegistry::getInstance()->updatePluggableSignatures();

  return false;
}

// =====================================================================
// =====================================================================
