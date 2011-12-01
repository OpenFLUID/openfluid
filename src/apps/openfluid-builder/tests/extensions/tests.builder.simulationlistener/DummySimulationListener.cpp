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
 \file DummySimulationListener.cpp
 \brief Implements ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/builderext/SimulationListener.hpp>

#include <gtkmm/dialog.h>
#include <gtkmm/stock.h>
#include <gtkmm/textview.h>

DECLARE_EXTENSION_HOOKS

DEFINE_EXTENSION_INFOS("tests.builder.simulationlistener",
    "Dummy simulation listener",
    "Dummy simulation listener for tests",
    "This is a dummy simulation listener for tests",
    "JC.Fabre;A.Libres",
    "fabrejc@supagro.inra.fr;libres@supagro.inra.fr",
    openfluid::builderext::PluggableBuilderExtension::SimulationListener)

DEFINE_EXTENSION_DEFAULT_CONFIG(("unit_class=TestUnits")("unit_id=5")("variable=tests.scalar"))

// =====================================================================
// =====================================================================


class DummySimulationListener: public openfluid::builderext::SimulationListener
{
  private:

    Gtk::Dialog* mp_Dialog;

    Glib::RefPtr<Gtk::TextBuffer> mref_TextBuffer;

    openfluid::core::Unit* mp_U;

    std::string m_VarName;

    sigc::connection m_connect;

    // =====================================================================
    // =====================================================================

    bool writeValue()
    {
      if (mp_U && !m_VarName.empty() && mp_U->getVariables()->getCurrentValue(m_VarName))
      {
        mref_TextBuffer->insert_at_cursor("\n");
        mref_TextBuffer->insert_at_cursor(
            mp_U->getVariables()->getCurrentValue(m_VarName)->toString());
      }

      return true;
    }

    // =====================================================================
    // =====================================================================

    void response(int Response)
    {
      if (Response == Gtk::RESPONSE_APPLY)
        mref_TextBuffer->set_text("");
      else
      {
        mp_Dialog->hide();
        signal_Hidden().emit();
      }
    }


  public:

    DummySimulationListener() :
      mp_U(0),m_VarName("")
    {
      mp_Dialog = new Gtk::Dialog(
          "I am DummySimulationListener and I'm modeless", false, true);

      mp_Dialog->add_button(Gtk::Stock::CLOSE, Gtk::RESPONSE_CLOSE);
      mp_Dialog->add_button(Gtk::Stock::CLEAR, Gtk::RESPONSE_APPLY);

      mp_Dialog->signal_response().connect(sigc::mem_fun(*this,
          &DummySimulationListener::response));

      mref_TextBuffer = Gtk::TextBuffer::create();

      mp_Dialog->get_vbox()->pack_start(*Gtk::manage(new Gtk::TextView(
          mref_TextBuffer)));

      mp_Dialog->set_default_size(400, 300);

      mp_Dialog->show_all_children();

    }

    // =====================================================================
    // =====================================================================


    ~DummySimulationListener()
    {
      delete mp_Dialog;
    }

    // =====================================================================
    // =====================================================================


    void onRefresh()
    {
    }

    // =====================================================================
    // =====================================================================


    Gtk::Widget* getExtensionAsWidget()
    {
      return mp_Dialog;
    }

    // =====================================================================
    // =====================================================================

    void show()
    {
      mref_TextBuffer->set_text("First run a simulation");

      mp_Dialog->show();
    }

    // =====================================================================
    // =====================================================================

    void onRunStarted()
    {
      mp_U = 0;
      m_VarName = "";

      mref_TextBuffer->insert_at_cursor(
          "\n------------------\nSimulation starts");

      if (!(m_Config.count("unit_class") && m_Config.count("unit_id")))
      {
        mref_TextBuffer->insert_at_cursor("\nUnit to display is not defined");
        return;
      }

      std::string UnitClass = m_Config["unit_class"];
      std::string UnitIdStr = m_Config["unit_id"];
      int UnitId;

      if (!openfluid::tools::ConvertString(UnitIdStr, &UnitId))
      {
        mref_TextBuffer->insert_at_cursor(
            "\nUnit to display is not well formatted");
        return;
      }

      mp_U = mp_SimulationBlob->getCoreRepository().getUnit(UnitClass, UnitId);

      if (!mp_U)
      {
        mref_TextBuffer->insert_at_cursor(Glib::ustring::compose(
            "\nUnit %1 %2 doesn't exist", UnitClass, UnitIdStr));
        return;
      }

      if (!m_Config.count("variable"))
      {
        mref_TextBuffer->insert_at_cursor(
            "\nVariable to display is not defined");
        return;
      }

      m_VarName = m_Config["variable"];

      if (!mp_U->getVariables()->isVariableExist(m_VarName))
      {
        mref_TextBuffer->insert_at_cursor(Glib::ustring::compose(
            "\nVariable %1 doesn't exist for Unit %2 %3", m_VarName, UnitClass,
            UnitIdStr));
        return;
      }

      mref_TextBuffer->insert_at_cursor(Glib::ustring::compose(
          "\nValues of %1 on %2 %3:", m_VarName, UnitClass, UnitIdStr));

      m_connect = Glib::signal_timeout().connect(sigc::mem_fun(*this,
          &DummySimulationListener::writeValue), 250);

    }

    // =====================================================================
    // =====================================================================

    void onRunStopped()
    {
      if (m_connect.connected())
        m_connect.disconnect();

      mref_TextBuffer->insert_at_cursor("\nSimulation stops");
    }

    // =====================================================================
    // =====================================================================

    void onProjectClosed()
    {
      mp_Dialog->hide();
      signal_Hidden().emit();
    }

};

// =====================================================================
// =====================================================================

class DummySimulationListenerPrefs: public openfluid::builderext::BuilderExtensionPrefs
{
  private:

  public:

    DummySimulationListenerPrefs();

};

// =====================================================================
// =====================================================================


DEFINE_EXTENSION_HOOKS(DummySimulationListener, DummySimulationListenerPrefs)
;

