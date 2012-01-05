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
 \file DummyAssistant.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include <openfluid/builderext/ModalWindow.hpp>

#include <gtkmm/assistant.h>
#include <gtkmm/main.h>
#include <gtkmm/label.h>

// =====================================================================
// =====================================================================

class TheDummyAssistant: public Gtk::Assistant
{
  private:

    openfluid::machine::SimulationBlob* mp_SimBlob;
    openfluid::core::UnitsCollection* mp_TestUnitsColl;
    bool m_Applied;

  public:

    TheDummyAssistant(openfluid::machine::SimulationBlob* SimBlob) :
      Gtk::Assistant(), mp_SimBlob(SimBlob), mp_TestUnitsColl(0), m_Applied(false)
    {
      set_title("Dummy assistant");
      set_default_size(400, 300);
      set_modal(true);

      Gtk::Label* Label1 = Gtk::manage(new Gtk::Label(
          "I'm a Dummy assistant for tests"));
      append_page(*Label1);
      set_page_title(*Label1, "Page 1/2");
      set_page_type(*Label1, Gtk::ASSISTANT_PAGE_INTRO);
      set_page_complete(*Label1, true);

      Gtk::Label* Label2 = Gtk::manage(new Gtk::Label());

      if (!mp_SimBlob)
      {
        Label2->set_text("Nb of units in TestUnits class: no CoreRepository\n"
          "Nothing to do");

        signal_apply().connect(sigc::mem_fun(*this, &Gtk::Assistant::hide));
      }
      else
      {
        unsigned int Size = 0;

        mp_TestUnitsColl = mp_SimBlob->getCoreRepository().getUnits("TestUnits");

        if (mp_TestUnitsColl)
          Size = mp_TestUnitsColl->getList()->size();

        Label2->set_text(Glib::ustring::compose(
            "Nb of units in TestUnits class: %1\n"
              "Clicking ok will add a Unit of class \"TestUnits\"", Size));

        signal_apply().connect(
            sigc::mem_fun(*this, &TheDummyAssistant::m_apply));
      }

      append_page(*Label2);
      set_page_title(*Label2, "Page 2/2");
      set_page_type(*Label2, Gtk::ASSISTANT_PAGE_CONFIRM);
      set_page_complete(*Label2, true);

      signal_cancel().connect(sigc::mem_fun(*this, &Gtk::Assistant::hide));
      signal_close().connect(sigc::mem_fun(*this, &Gtk::Assistant::hide));

      show_all_children();
    }

    void m_apply()
    {
      unsigned int NextId = 1;

      if (mp_TestUnitsColl)
      {
        openfluid::core::UnitsList_t* TestUnits = mp_TestUnitsColl->getList();
        if (!TestUnits->empty())
        {
          NextId = TestUnits->end().operator --()->getID() + 1;

          while (mp_TestUnitsColl->getUnit(NextId))
            NextId++;
        }
      }

      openfluid::core::Unit U("TestUnits", NextId, 1,
          openfluid::core::InstantiationInfo::DESCRIPTOR);

      mp_SimBlob->getCoreRepository().addUnit(U);

      m_Applied = true;

      hide();
    }

    bool getApplied()
    {
      return m_Applied;
    }

};

// =====================================================================
// =====================================================================


DECLARE_EXTENSION_HOOKS

DEFINE_EXTENSION_INFOS("tests.builder.assistant",
    "Dummy assistant",
    "Dummy assistant for tests",
    "This is an assistant for tests",
    "JC.Fabre;A.Libres",
    "fabrejc@supagro.inra.fr;libres@supagro.inra.fr",
    openfluid::builderext::PluggableBuilderExtension::ModalWindow)

DEFINE_EXTENSION_DEFAULT_CONFIG()

// =====================================================================
// =====================================================================


class DummyAssistant: public openfluid::builderext::ModalWindow
{
  public:

    DummyAssistant()
    {
    }

    // =====================================================================
    // =====================================================================


    ~DummyAssistant()
    {
    }

    // =====================================================================
    // =====================================================================


    Gtk::Widget* getExtensionAsWidget()
    {
      return (Gtk::Widget*) 0;
    }

    // =====================================================================
    // =====================================================================

    void show()
    {
      TheDummyAssistant Assist(mp_SimulationBlob);

      Gtk::Main::run(Assist);

      if (Assist.getApplied())
        signal_ChangedOccurs().emit();
    }

};


// =====================================================================
// =====================================================================

DEFINE_EXTENSION_HOOKS((DummyAssistant))

