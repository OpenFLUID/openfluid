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
 \file MonitoringModule.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "MonitoringModule.hpp"

#include <gtkmm/notebook.h>
#include <openfluid/fluidx/AdvancedFluidXDescriptor.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>
#include <openfluid/machine/ObserverInstance.hpp>
#include "MonitoringAddObserverDialog.hpp"
#include "WareSetWidget.hpp"
#include "ObserverParamWidget.hpp"
#include "ObserverAddParamDialog.hpp"
#include "WareItemInfoWidget.hpp"

// =====================================================================
// =====================================================================

MonitoringModule::MonitoringModule(
    openfluid::fluidx::AdvancedFluidXDescriptor& AdvancedDesc) :
    ProjectWorkspaceModule(AdvancedDesc), m_Monit(AdvancedDesc.getMonitoring())
{
  mp_MainPanel = 0;

  mp_MonitoringWidget = Gtk::manage(new WareSetWidget("Add observer"));

  mp_AddDialog = new MonitoringAddObserverDialog(m_Monit);

  mp_AddParamDialog = new ObserverAddParamDialog();

  mp_MonitoringWidget->signal_AddAsked().connect(
      sigc::mem_fun(*this, &MonitoringModule::whenAddObserverAsked));

  update();
}

// =====================================================================
// =====================================================================

MonitoringModule::~MonitoringModule()
{
  delete mp_AddDialog;
}

// =====================================================================
// =====================================================================

void MonitoringModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::VBox());

  mp_MainPanel->set_border_width(5);
  mp_MainPanel->pack_start(*mp_MonitoringWidget);

  mp_MainPanel->set_visible(true);
}

// =====================================================================
// =====================================================================

Gtk::Widget* MonitoringModule::asWidget()
{
  if (mp_MainPanel)
    return mp_MainPanel;
  throw std::logic_error(
      "MonitoringModule : you try to get a widget from a non yet composed module.");
}

// =====================================================================
// =====================================================================

void MonitoringModule::whenAddObserverAsked()
{
  if (mp_AddDialog->show())
  {
    update();
    onMonitoringChanged();
  }
}

// =====================================================================
// =====================================================================

void MonitoringModule::whenRemoveObserverAsked(std::string ID)
{
  m_Monit.removeFromObserverList(ID);
  update();
  onMonitoringChanged();
}

// =====================================================================
// =====================================================================

void MonitoringModule::whenUpAsked(std::string ID)
{
  m_Monit.moveItemTowardsTheBeginning(ID);
  update();
  onMonitoringChanged();
}

// =====================================================================
// =====================================================================

void MonitoringModule::whenDownAsked(std::string ID)
{
  m_Monit.moveItemTowardsTheEnd(ID);
  update();
  onMonitoringChanged();
}

// =====================================================================
// =====================================================================

sigc::signal<void> MonitoringModule::signal_ModuleChanged()
{
  return m_signal_MonitoringChanged;
}

// =====================================================================
// =====================================================================

void MonitoringModule::update()
{
  mp_MonitoringWidget->storeExpanderStates();

  mp_MonitoringWidget->clearItems();

  openfluid::machine::ObserverSignatureRegistry* Reg =
      openfluid::machine::ObserverSignatureRegistry::getInstance();

  const std::list<openfluid::fluidx::ObserverDescriptor*>& Items =
      m_Monit.getItems();

  std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator itLast =
      Items.end().operator --();

  for (std::list<openfluid::fluidx::ObserverDescriptor*>::const_iterator it =
      Items.begin(); it != Items.end(); ++it)
  {
    std::string ID = (*it)->getID();

    const openfluid::machine::ObserverSignatureInstance& Sign =
        Reg->getSignature(ID);

    std::string Name = Sign.Signature->Name;

    ObserverParamWidget* ItemParamWidget = Gtk::manage(
        new ObserverParamWidget(*(*it), *mp_AddParamDialog));
    ItemParamWidget->signal_changeOccured().connect(
        sigc::mem_fun(*this, &MonitoringModule::onMonitoringChanged));

    Gtk::Notebook* ItemInfo = Gtk::manage(new Gtk::Notebook());
    ItemInfo->append_page(
        *Gtk::manage(new WareItemInfoWidget(Sign, *Sign.Signature)),
        _("Information"));

    WareItemWidget* ItemWidget = Gtk::manage(
        new WareItemWidget(ID, *ItemParamWidget, *ItemInfo, Name,
                           (*it)->getType()));
    ItemWidget->signal_RemoveAsked().connect(
        sigc::mem_fun(*this, &MonitoringModule::whenRemoveObserverAsked));
    ItemWidget->signal_UpAsked().connect(
        sigc::mem_fun(*this, &MonitoringModule::whenUpAsked));
    ItemWidget->signal_DownAsked().connect(
        sigc::mem_fun(*this, &MonitoringModule::whenDownAsked));

    mp_MonitoringWidget->addItem(ItemWidget, ID);

    if (it == Items.begin())
      ItemWidget->setUpButtonSensitive(false);
    if (it == itLast)
      ItemWidget->setDownButtonSensitive(false);
  }

  mp_MonitoringWidget->applyExpanderStates();

  mp_MonitoringWidget->show_all_children();
}

// =====================================================================
// =====================================================================

void MonitoringModule::onMonitoringChanged()
{
  m_signal_MonitoringChanged.emit();
}

// =====================================================================
// =====================================================================

