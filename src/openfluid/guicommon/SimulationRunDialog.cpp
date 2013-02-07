/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file SimulationRunDialog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/guicommon/SimulationRunDialog.hpp>

#include <glibmm/main.h>
#include <glibmm/i18n.h>

#include <gtkmm/separator.h>

#include <openfluid/machine/Engine.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/guicommon/DialogBoxFactory.hpp>
#include <openfluid/guicommon/RunDialogMachineListener.hpp>

namespace openfluid { namespace guicommon {


// =====================================================================
// =====================================================================


SimulationRunDialog::SimulationRunDialog(openfluid::machine::Engine* Engine)
  : m_SimulationCompleted(false), mp_Engine(Engine), mp_SBlob(Engine->getSimulationBlob()),
    mp_Model(Engine->getModelInstance()), mp_MachineListen((RunDialogMachineListener*)(Engine->getMachineListener()))
{

  m_TotalIndex = mp_SBlob->getSimulationStatus().getSimulationDuration();

  openfluid::tools::ConvertValue((m_TotalIndex),&m_LastIndexStr);

  set_modal(true);
  set_title(_("Simulation"));
  set_border_width(10);
  set_default_size(600, -1);
  set_position(Gtk::WIN_POS_CENTER);
  set_resizable(false);


  Gtk::VBox* MainBox = get_vbox();

  m_TopLabel.set_text("");
  m_TopLabel.set_justify(Gtk::JUSTIFY_CENTER);
  m_TopLabel.set_size_request(580,-1);
  m_TopLabel.set_use_markup(true);

  m_DetailsExpander.set_label(_("Details"));
  m_DetailsSWindow.add(m_DetailsTextView);
  m_DetailsSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_DetailsSWindow.set_size_request(-1,200);
  m_DetailsExpander.add(m_DetailsSWindow);

  m_RefDetailsTextBuffer = Gtk::TextBuffer::create();
  m_DetailsTextView.set_buffer(m_RefDetailsTextBuffer);


  MainBox->pack_start(m_TopLabel,Gtk::PACK_SHRINK,8);
  MainBox->pack_start(*(Gtk::manage(new Gtk::HSeparator())),Gtk::PACK_SHRINK,8);
  MainBox->pack_start(m_RunStatusWidget,Gtk::PACK_SHRINK,16);
  MainBox->pack_start(m_DetailsExpander,Gtk::PACK_SHRINK,16);

  Gtk::ButtonBox* ButtonBox = get_action_area();

  m_ControlButton.set_label(_("Close"));
  m_ControlButton.set_sensitive(false);


  ButtonBox->pack_start(m_ControlButton,Gtk::PACK_SHRINK,8);


  m_ControlButton.signal_clicked().connect(
      sigc::mem_fun(*this, &SimulationRunDialog::onControlButtonClicked)
    );


  Glib::signal_timeout().connect_once(sigc::mem_fun(*this,
    &SimulationRunDialog::onIgnition), 250);

  resetWidgets();

  show_all_children();
}


// =====================================================================
// =====================================================================


SimulationRunDialog::~SimulationRunDialog()
{

}


// =====================================================================
// =====================================================================


void SimulationRunDialog::onIgnition()
{

  m_SimulationCompleted = false;

  /*
  Glib::Thread* RunThread = Glib::Thread::create(sigc::mem_fun(*this,
      &SimulationRunDialog::runSimulation),true);

  while(!m_SimulationCompleted)
  {
    while(Gtk::Main::events_pending()) Gtk::Main::iteration();
  }

  RunThread->join();
*/
  runSimulation();

  m_ControlButton.set_sensitive(true);

}


// =====================================================================
// =====================================================================


bool SimulationRunDialog::on_delete_event(GdkEventAny*)
{
  onControlButtonClicked();
  return true;
}


// =====================================================================
// =====================================================================


void SimulationRunDialog::onControlButtonClicked()
{
  if (m_SimulationCompleted) hide();
  else
  {

  }
}


// =====================================================================
// =====================================================================


void SimulationRunDialog::resetWidgets()
{

  m_TopLabel.set_markup(std::string("<b>")+_("Simulation from ") + mp_SBlob->getRunDescriptor().getBeginDate().getAsISOString() +
                                             _(" to ") +
                                             mp_SBlob->getRunDescriptor().getEndDate().getAsISOString() +
                        std::string("</b>"));

  m_RunStatusWidget.setProgressFraction(0.0);
  m_RunStatusWidget.setRunstepDefault();

  m_RefDetailsTextBuffer->set_text("");
}


// =====================================================================
// =====================================================================


void SimulationRunDialog::runSimulation()
{
  try
  {
    mp_MachineListen->setWidgets(&m_RunStatusWidget,
                                 &m_DetailsTextView);

    mp_MachineListen->setInfos(mp_Model->getItemsCount(),
                               m_TotalIndex);

    mp_Engine->initialize();
    mp_Engine->initParams();
    mp_Engine->prepareData();
    mp_Engine->checkConsistency();

    signal_SimulationStarted().emit();

    mp_Engine->run();

    signal_SimulationStopped().emit();

    m_SimulationCompleted = true;

  }
  catch (openfluid::base::OFException& E)
  {
    signal_SimulationStopped().emit();
    DialogBoxFactory::showSimpleErrorMessage(_("OpenFLUID error: ") + std::string(E.what()));
    m_SimulationCompleted = false;
    hide();
  }
  catch (std::bad_alloc& E)
  {
    signal_SimulationStopped().emit();
    DialogBoxFactory::showSimpleErrorMessage(_("Memory allocation error: ") + std::string(E.what()));
    m_SimulationCompleted = false;
    hide();
  }
  catch (std::exception& E)
  {
    signal_SimulationStopped().emit();
    DialogBoxFactory::showSimpleErrorMessage(_("System error: ") + std::string(E.what()));
    m_SimulationCompleted = false;
    hide();
  }
  catch (...)
  {
    signal_SimulationStopped().emit();
    DialogBoxFactory::showSimpleErrorMessage(_("Undetermined error."));
    m_SimulationCompleted = false;
    hide();
  }

  mp_Engine->finalize();

}


} } //namespaces


