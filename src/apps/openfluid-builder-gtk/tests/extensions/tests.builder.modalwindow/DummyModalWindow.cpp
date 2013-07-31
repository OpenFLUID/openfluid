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
 \file DummyModalWindow.cpp
 \brief Implements ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/builderext-gtk/ModalWindow.hpp>

#include <gtkmm/messagedialog.h>

DECLARE_EXTENSION_HOOKS

DEFINE_EXTENSION_INFOS("tests.builder.modalwindow",
    "Dummy modal window",
    "Dummy modal window for tests",
    "This is a modal window for tests",
    "JC.Fabre;A.Libres",
    "fabrejc@supagro.inra.fr;libres@supagro.inra.fr",
    openfluid::builderext::PluggableBuilderExtension::ModalWindow)

DEFINE_EXTENSION_DEFAULT_CONFIG()

// =====================================================================
// =====================================================================

class DummyModalWindow: public openfluid::builderext::ModalWindow
{
  private:

    Gtk::MessageDialog* mp_Dialog;

  public:

    DummyModalWindow()
    {
      mp_Dialog = new Gtk::MessageDialog("", false, Gtk::MESSAGE_QUESTION,
                                         Gtk::BUTTONS_OK_CANCEL);
    }

    // =====================================================================
    // =====================================================================

    ~DummyModalWindow()
    {
      delete mp_Dialog;
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
      std::set<int> m_IDs;

      if (!mp_AdvancedDesc)
      {
        mp_Dialog->set_message("I am DummyModalWindow\n"
                               "Nb of units in TestUnits class: no Domain\n"
                               "Nothing to do");
      }
      else
      {
        m_IDs = mp_AdvancedDesc->getDomain().getIDsOfClass("TestUnits");

        mp_Dialog->set_message(
            Glib::ustring::compose(
                "I am DummyModalWindow\n"
                "Nb of units in TestUnits class: %1\n"
                "Clicking ok will add a Unit of class \"TestUnits\"",
                m_IDs.size()));
      }

      if (mp_Dialog->run() == Gtk::RESPONSE_OK && mp_AdvancedDesc)
      {
        unsigned int NextId = 1;

        if (!m_IDs.empty())
          NextId = (*std::max_element(m_IDs.begin(), m_IDs.end())) + 1;

        openfluid::fluidx::UnitDescriptor* Unit =
            new openfluid::fluidx::UnitDescriptor;
        Unit->getUnitClass() = "TestUnits";
        Unit->getUnitID() = NextId;
        Unit->getProcessOrder() = 1;

        mp_AdvancedDesc->getDomain().addUnit(Unit);

        signal_ChangedOccurs().emit();
      }

      mp_Dialog->hide();
    }

    bool isReadyForShowtime() const
    {
      return (mp_AdvancedDesc != NULL);
    }

};

// =====================================================================
// =====================================================================

DEFINE_EXTENSION_HOOKS((DummyModalWindow))

