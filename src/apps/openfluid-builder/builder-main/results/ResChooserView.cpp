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
 \file ResChooserView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResChooserView.hpp"

#include <glibmm/i18n.h>


bool ResChooserViewImpl::IDsListStoreExistsForCurrentSet()
{
  return m_BySetNameIDsListStores.find(getSetName())
      != m_BySetNameIDsListStores.end();
}
bool ResChooserViewImpl::ClassNameExistsForCurrentSet()
{
  return m_BySetNameClassNames.find(getSetName())
      != m_BySetNameClassNames.end();
}
void ResChooserViewImpl::onSetChanged()
{
  if (IDsListStoreExistsForCurrentSet() && ClassNameExistsForCurrentSet())
  {
    std::string SetName = getSetName();
    mp_ClassNameValue->set_text(m_BySetNameClassNames[SetName]);
    mp_IdCombo->set_model(m_BySetNameIDsListStores[SetName]);
    mp_IdCombo->set_active(mp_IdCombo->get_model()->children().begin());
  }
}
void ResChooserViewImpl::onIdChanged()
{
  m_signal_SelectionChanged.emit();
}
void ResChooserViewImpl::onOkButtonClicked()
{
  m_signal_SelectionValidated.emit();
}
ResChooserViewImpl::ResChooserViewImpl()
{
  Gtk::Label* SetsLabel = Gtk::manage(new Gtk::Label(_("Set Name")));
  Gtk::Label* ClassNameLabel = Gtk::manage(new Gtk::Label(_("Unit Class : ")));
  Gtk::Label* IDsLabel = Gtk::manage(new Gtk::Label(_("Unit ID")));

  mp_MessageLabel = Gtk::manage(new Gtk::Label(""));
  mp_MessageLabel->set_visible(true);
  mp_ClassNameValue = Gtk::manage(new Gtk::Label());

  mp_SetCombo = Gtk::manage(new Gtk::ComboBoxText());
  mp_SetCombo->signal_changed().connect(sigc::mem_fun(*this,
      &ResChooserViewImpl::onSetChanged));

  mp_IdCombo = Gtk::manage(new Gtk::ComboBox());
  mp_IdCombo->pack_start(m_Columns.m_ID);
  mp_IdCombo->signal_changed().connect(sigc::mem_fun(*this,
      &ResChooserViewImpl::onIdChanged));

  mp_OkButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
  mp_OkButton->signal_clicked().connect(sigc::mem_fun(*this,
      &ResChooserViewImpl::onOkButtonClicked));

  Gtk::Box* BottomBox = Gtk::manage(new Gtk::HBox());
  BottomBox->pack_start(*SetsLabel, Gtk::SHRINK, 0);
  BottomBox->pack_start(*mp_SetCombo, Gtk::SHRINK, 10);
  BottomBox->pack_start(*ClassNameLabel, Gtk::SHRINK, 0);
  BottomBox->pack_start(*mp_ClassNameValue, Gtk::SHRINK, 10);
  BottomBox->pack_start(*IDsLabel, Gtk::SHRINK, 0);
  BottomBox->pack_start(*mp_IdCombo, Gtk::SHRINK, 10);
  BottomBox->pack_start(*mp_OkButton);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*mp_MessageLabel, Gtk::PACK_SHRINK);
  mp_MainBox->pack_start(*BottomBox, Gtk::PACK_SHRINK);

  mp_MainBox->show_all_children();
}
sigc::signal<void> ResChooserViewImpl::signal_SelectionChanged()
{
  return m_signal_SelectionChanged;
}
sigc::signal<void> ResChooserViewImpl::signal_SelectionValidated()
{
  return m_signal_SelectionValidated;
}
void ResChooserViewImpl::setBySetNameClassNames(std::map<std::string,
    std::string> ClassNames)
{
  m_BySetNameClassNames = ClassNames;
  m_BySetNameIDsListStores.clear();

  mp_SetCombo->clear_items();

  for (std::map<std::string, std::string>::iterator it =
      m_BySetNameClassNames.begin(); it != m_BySetNameClassNames.end(); ++it)
    mp_SetCombo->append_text(it->first);
}
void ResChooserViewImpl::setBySetNameIDs(std::map<std::string, std::vector<
    unsigned int> > IDsBySet)
{
  m_BySetNameIDsListStores.clear();

  for (std::map<std::string, std::vector<unsigned int> >::iterator it =
      IDsBySet.begin(); it != IDsBySet.end(); ++it)
  {
    Glib::RefPtr<Gtk::ListStore> List = Gtk::ListStore::create(m_Columns);
    for (unsigned int i = 0; i < it->second.size(); i++)
    {
      Gtk::TreeRow Row = *(List->append());
      Row[m_Columns.m_ID] = it->second[i];
    }
    m_BySetNameIDsListStores[it->first] = List;
  }
}
void ResChooserViewImpl::selectFirstItem()
{
  mp_SetCombo->set_active(0);
}
std::string ResChooserViewImpl::getSetName()
{
  return mp_SetCombo->get_active_text();
}
std::string ResChooserViewImpl::getClassName()
{
  return mp_ClassNameValue->get_text();
}
int ResChooserViewImpl::getID()
{
  if (Gtk::TreeIter it = mp_IdCombo->get_active())
    return it->get_value(m_Columns.m_ID);
  return -1;
}
Gtk::Widget* ResChooserViewImpl::asWidget()
{
  return mp_MainBox;
}

std::string ResChooserViewSub::getClassName()
{
  return mp_ClassNameValue->get_text();
}
void ResChooserViewSub::selectSetName(std::string Value)
{
  mp_SetCombo->set_active_text(Value);
}
void ResChooserViewSub::selectId(int Index)
{
  mp_IdCombo->set_active(mp_IdCombo->get_model()->children().begin() + Index);
}
std::map<std::string, std::string> ResChooserViewSub::getBySetNameClassNames()
{
  return m_BySetNameClassNames;
}
std::map<std::string, Glib::RefPtr<Gtk::ListStore> > ResChooserViewSub::getBySetNameIDsListStores()
{
  return m_BySetNameIDsListStores;
}
