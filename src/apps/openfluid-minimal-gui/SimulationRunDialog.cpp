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


#include <glibmm.h>
#include <glibmm/i18n.h>

#include "SimulationRunDialog.hpp"

// =====================================================================
// =====================================================================


SimulationRunDialog::SimulationRunDialog(openfluid::machine::SimulationBlob* SBlob,
                                         openfluid::machine::ModelInstance* Model,
                                         RunDialogMachineListener* MachineListen,
                                         openfluid::io::IOListener* IOListen)
  : m_SimulationCompleted(false), mp_SBlob(SBlob), mp_Model(Model), mp_MachineListen(MachineListen), mp_IOListen(IOListen)
{
  set_border_width(10);
  set_default_size(600, -1);
  set_position(Gtk::WIN_POS_CENTER);
  set_resizable(false);


  Gtk::VBox* MainBox = get_vbox();

  m_TopLabel.set_text("Simulation from date to date\nTime step : delta second(s)");
  m_TopLabel.set_justify(Gtk::JUSTIFY_CENTER);
  m_TopLabel.set_size_request(580,-1);
  m_TopLabel.set_use_markup(true);

  m_PreSimLabel.set_text(_("Pre-simulation"));
  m_PreSimLabel.set_alignment(1,0.5);
  m_PreSimProgressBar.set_size_request(-1,20);
  m_InitLabel.set_text(_("Initialization"));
  m_InitLabel.set_alignment(1,0.5);
  m_InitProgressBar.set_size_request(-1,20);
  m_RunLabel.set_text(_("Run"));
  m_RunLabel.set_alignment(1,0.5);
  m_RunProgressBar.set_size_request(-1,40);
  m_FinalLabel.set_text(_("Finalization"));
  m_FinalLabel.set_alignment(1,0.5);
  m_FinalProgressBar.set_size_request(-1,20);


  m_RunTable.attach(m_PreSimLabel,0,1,0,1,Gtk::FILL,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_PreSimProgressBar,1,2,0,1,Gtk::FILL|Gtk::EXPAND,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_InitLabel,0,1,1,2,Gtk::FILL,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_InitProgressBar,1,2,1,2,Gtk::FILL|Gtk::EXPAND,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_RunLabel,0,1,2,3,Gtk::FILL,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_RunProgressBar,1,2,2,3,Gtk::FILL|Gtk::EXPAND,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_FinalLabel,0,1,3,4,Gtk::FILL,Gtk::FILL|Gtk::EXPAND,4,4);
  m_RunTable.attach(m_FinalProgressBar,1,2,3,4,Gtk::FILL|Gtk::EXPAND,Gtk::FILL|Gtk::EXPAND,4,4);


  m_DetailsExpander.set_label(_("Details"));
  m_DetailsSWindow.add(m_DetailsTextView);
  m_DetailsSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
  m_DetailsSWindow.set_size_request(-1,200);
  m_DetailsExpander.add(m_DetailsSWindow);

  m_RefDetailsTextBuffer = Gtk::TextBuffer::create();
  m_DetailsTextView.set_buffer(m_RefDetailsTextBuffer);


  MainBox->pack_start(m_TopLabel,Gtk::PACK_SHRINK,8);
  MainBox->pack_start(*(Gtk::manage(new Gtk::HSeparator())),Gtk::PACK_SHRINK,8);
  MainBox->pack_start(m_RunTable,Gtk::PACK_SHRINK,8);
  MainBox->pack_start(m_DetailsExpander,Gtk::PACK_SHRINK,8);


  Gtk::ButtonBox* ButtonBox = get_action_area();

  m_ControlButton.set_label(_("Abort simulation"));
//  m_ControlButton.set_image(*(Gtk::manage(new Gtk::Image(Gtk::Stock::MEDIA_STOP,Gtk::IconSize(Gtk::ICON_SIZE_BUTTON)))));


  ButtonBox->pack_start(m_ControlButton,Gtk::PACK_SHRINK,8);


  m_ControlButton.signal_clicked().connect(
      sigc::mem_fun(*this, &SimulationRunDialog::onControlButtonClicked)
    );


  Glib::signal_timeout().connect_once(sigc::mem_fun(*this,
    &SimulationRunDialog::onIgnition), 250);

  resetWidgets();

  mp_MachineListen->setWidgets(&m_PreSimProgressBar,&m_InitProgressBar,&m_RunProgressBar,&m_FinalProgressBar,
                               &m_DetailsTextView);

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
  Glib::Thread* RunThread = Glib::Thread::create(sigc::mem_fun(*this,
      &SimulationRunDialog::runSimulation),true);

  m_SimulationCompleted = false;

  while(!m_SimulationCompleted || Gtk::Main::events_pending()) Gtk::Main::iteration();

  RunThread->join();
  m_ControlButton.set_label(_("Close"));

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
  std::string TStepStr = "";

  openfluid::tools::ConvertValue(mp_SBlob->getRunDescriptor().getDeltaT(),&TStepStr);

  m_TopLabel.set_text(_("Simulation from ") + mp_SBlob->getRunDescriptor().getBeginDate().getAsISOString() +
                                             _(" to ") +
                                             mp_SBlob->getRunDescriptor().getEndDate().getAsISOString() +
                                             "\n" + _("Time step: ") + TStepStr + _(" second(s)"));

  m_PreSimProgressBar.set_fraction(0.0);
  m_InitProgressBar.set_fraction(0.0);
  m_RunProgressBar.set_fraction(0.0);
  m_FinalProgressBar.set_fraction(0.0);

  m_RefDetailsTextBuffer->set_text("");
}


// =====================================================================
// =====================================================================


void SimulationRunDialog::runSimulation()
{
  try
  {
    mp_MachineListen->setFunctionsCount(mp_Model->getItemsCount());

    mp_Engine = new openfluid::machine::Engine(*mp_SBlob, *mp_Model, mp_MachineListen, mp_IOListen);

    mp_Engine->initParams();
    mp_Engine->prepareData();
    mp_Engine->checkConsistency();

    mp_MachineListen->setTotalStepsCount(mp_Engine->getSimulationInfo()->getStepsCount());
    mp_Engine->run();

    mp_Engine->saveReports();

    delete mp_Engine;
  }
  catch (openfluid::base::OFException& E)
  {
    std::cerr << "ERROR: " + std::string(E.what()) << std::endl;
    delete mp_Engine;
  }
  catch (std::bad_alloc& E)
  {
    std::cerr << "MEMORY ALLOCATION ERROR: " + std::string(E.what()) + ". Possibly not enough memory available" << std::endl;
    delete mp_Engine;
  }
  catch (std::exception& E)
  {
    std::cerr << "SYSTEM ERROR: " + std::string(E.what()) << std::endl;
    delete mp_Engine;
  }
  catch (...)
  {
    std::cerr << "UNKNOWN ERROR" << std::endl;
    delete mp_Engine;
  }

  m_SimulationCompleted = true;

}
