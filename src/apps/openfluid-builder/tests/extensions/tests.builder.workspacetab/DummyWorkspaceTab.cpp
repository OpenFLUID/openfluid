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
 \file DummyWorkspaceTab.cpp
 \brief Implements ...

 \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include <openfluid/builderext/WorkspaceTab.hpp>

#include <gtkmm/label.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>

DECLARE_EXTENSION_HOOKS

DEFINE_EXTENSION_INFOS("tests.builder.workspacetab",
    "Dummy workspace tab",
    "Dummy workspace tab for tests",
    "This is a dummy workspace tab for tests",
    "JC.Fabre;A.Libres",
    "fabrejc@supagro.inra.fr;libres@supagro.inra.fr",
    openfluid::builderext::PluggableBuilderExtension::WorkspaceTab)

DEFINE_EXTENSION_DEFAULT_CONFIG()

// =====================================================================
// =====================================================================

class DummyWorkspaceTab: public openfluid::builderext::WorkspaceTab
{
  private:

    Gtk::VBox* mp_MainBox;
    Gtk::Label* mp_LabelTestRefresh;

  public:

    //TODO change workspaceTab extensions constructors
    DummyWorkspaceTab() :
        WorkspaceTab(*(openfluid::fluidx::AdvancedFluidXDescriptor*) 0)
    {
      Gtk::Label* Label = Gtk::manage(new Gtk::Label("I am DummyWorkspaceTab"));

      mp_LabelTestRefresh = Gtk::manage(new Gtk::Label());

      Gtk::HBox* Box = Gtk::manage(new Gtk::HBox());
      Gtk::Label* AddLabel = Gtk::manage(
          new Gtk::Label(
              "Clicking the button will add a Unit of class \"TestUnits\""));
      Gtk::Button* AddButton = Gtk::manage(new Gtk::Button("Add"));
      AddButton->signal_clicked().connect(
          sigc::mem_fun(*this, &DummyWorkspaceTab::whenAddButtonClicked));

      Box->pack_start(*AddLabel);
      Box->pack_start(*AddButton, Gtk::PACK_SHRINK, 20);

      mp_MainBox = Gtk::manage(new Gtk::VBox());
      mp_MainBox->pack_start(*Label);
      mp_MainBox->pack_start(*Box, Gtk::PACK_SHRINK);
      mp_MainBox->pack_start(*mp_LabelTestRefresh);

      mp_MainBox->show_all_children();
      mp_MainBox->set_visible(true);
    }

    // =====================================================================
    // =====================================================================

    ~DummyWorkspaceTab()
    {

    }

    // =====================================================================
    // =====================================================================

    void update()
    {
      unsigned int Size = 0;

      if (mp_AdvancedDesc)
      {
        Size = mp_AdvancedDesc->getDomain().getIDsOfClass("TestUnits").size();

        mp_LabelTestRefresh->set_text(
            Glib::ustring::compose("Nb of units in TestUnits class: %1", Size));
      }
      else
        mp_LabelTestRefresh->set_text(
            "Nb of units in TestUnits class: no Domain available");
    }

    // =====================================================================
    // =====================================================================

    Gtk::Widget* getExtensionAsWidget()
    {
      return mp_MainBox;
    }

    // =====================================================================
    // =====================================================================

    void whenAddButtonClicked()
    {
      unsigned int NextId = 1;

      std::set<int> m_IDs = mp_AdvancedDesc->getDomain().getIDsOfClass(
          "TestUnits");

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

    bool isReadyForShowtime() const
    {
      return (mp_AdvancedDesc != NULL);
    }

};

// =====================================================================
// =====================================================================

class DummyWorkspaceTabPrefs: public openfluid::builderext::BuilderExtensionPrefs
{
  private:

  public:

    DummyWorkspaceTabPrefs() :
        BuilderExtensionPrefs("Dummy Workspace Tab Preferences")
    {
      Gtk::Box* TheBox = Gtk::manage(new Gtk::HBox());

      Gtk::Label* Lab = Gtk::manage(
          new Gtk::Label("I'm the DummyWorkspaceTab Preferences"));

      TheBox->pack_start(*Lab);

      mp_ContentWindow->add(*TheBox);

      mp_ContentWindow->show_all_children();
    }

    void init()
    {
    }
    ;

};

// =====================================================================
// =====================================================================

DEFINE_EXTENSION_HOOKS((DummyWorkspaceTab) (DummyWorkspaceTabPrefs))

