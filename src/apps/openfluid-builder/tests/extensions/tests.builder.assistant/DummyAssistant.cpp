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

DECLARE_EXTENSION_HOOKS
;

DEFINE_EXTENSION_INFOS("tests.builder.assistant",
    "Dummy assistant",
    "Dummy assistant for tests",
    "This is an assistant for tests",
    "JC.Fabre;A.Libres",
    "fabrejc@supagro.inra.fr;libres@supagro.inra.fr")
;

// =====================================================================
// =====================================================================


class DummyAssistant: public openfluid::builderext::ModalWindow
{
  private:

    Gtk::Assistant* mp_Assistant;
    Gtk::Label* mp_Label2;

  public:

    DummyAssistant()
    {
      mp_Assistant = new Gtk::Assistant();

      mp_Assistant->set_title("Dummy assistant");
      mp_Assistant->set_default_size(400, 300);
      mp_Assistant->set_modal(true);

      Gtk::Label* Label1 = Gtk::manage(new Gtk::Label(
          "I'm a Dummy assistant for tests"));
      mp_Assistant->append_page(*Label1);
      mp_Assistant->set_page_title(*Label1, "Page 1/2");
      mp_Assistant->set_page_type(*Label1, Gtk::ASSISTANT_PAGE_INTRO);
      mp_Assistant->set_page_complete(*Label1, true);

      mp_Label2 = Gtk::manage(new Gtk::Label(
          "Nb of UnitClasses: no Core Repository available"));
      mp_Assistant->append_page(*mp_Label2);
      mp_Assistant->set_page_title(*mp_Label2, "Page 2/2");
      mp_Assistant->set_page_type(*mp_Label2, Gtk::ASSISTANT_PAGE_CONFIRM);
      mp_Assistant->set_page_complete(*mp_Label2, true);

      mp_Assistant->signal_apply().connect(sigc::mem_fun(*this,
          &DummyAssistant::hide));
      mp_Assistant->signal_cancel().connect(sigc::mem_fun(*this,
          &DummyAssistant::hide));
      mp_Assistant->signal_close().connect(sigc::mem_fun(*this,
          &DummyAssistant::hide));

      mp_Assistant->show_all_children();
    }
    ;

    // =====================================================================
    // =====================================================================


    ~DummyAssistant()
    {
      delete mp_Assistant;
    }


    // =====================================================================
    // =====================================================================


    Gtk::Widget* getExtensionAsWidget()
    {
      return mp_Assistant;
    }

    // =====================================================================
    // =====================================================================

    void show()
    {
      if (mp_SimulationBlob)
      {
        unsigned int Size =
            mp_SimulationBlob->getCoreRepository().getUnitsByClass()->size();
        mp_Label2 ->set_text(Glib::ustring::compose("Nb of UnitClasses: %1",
            Size));
      }

      Gtk::Main::run(*mp_Assistant);
    }

    // =====================================================================
    // =====================================================================

    void hide()
    {
      mp_Assistant->hide();
    }

};

// =====================================================================
// =====================================================================


DEFINE_EXTENSION_HOOKS(DummyAssistant)
;

