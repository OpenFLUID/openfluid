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
 \file BuilderListToolBox.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "BuilderListToolBox.hpp"


void BuilderListToolBoxImpl::onAddButtonClicked()
{
  m_signal_AddCommandAsked.emit();
}
void BuilderListToolBoxImpl::onRemoveButtonClicked()
{
  m_signal_RemoveCommandAsked.emit();
}
void BuilderListToolBoxImpl::onUpButtonClicked()
{
  m_signal_UpCommandAsked.emit();
}
void BuilderListToolBoxImpl::onDownButtonClicked()
{
  m_signal_DownCommandAsked.emit();
}
void BuilderListToolBoxImpl::onEditButtonClicked()
{
  m_signal_EditCommandAsked.emit();
}
Gtk::Button* BuilderListToolBoxImpl::createImageStockButton(
    Gtk::StockID StockImage, Glib::ustring TooltipText)
{
  Gtk::Button* Button = Gtk::manage(new Gtk::Button());
  Button->set_image(*Gtk::manage(new Gtk::Image(StockImage,
      Gtk::ICON_SIZE_BUTTON)));
  if (TooltipText != "")
    Button->set_tooltip_text(TooltipText);
  return Button;
}

BuilderListToolBoxImpl::BuilderListToolBoxImpl()
{
  mp_AddButton = createImageStockButton(Gtk::Stock::ADD);
  mp_RemoveButton = createImageStockButton(Gtk::Stock::REMOVE);
  mp_UpButton = createImageStockButton(Gtk::Stock::GO_UP);
  mp_DownButton = createImageStockButton(Gtk::Stock::GO_DOWN);
  mp_EditButton = createImageStockButton(Gtk::Stock::EDIT);

  mp_AddButton->signal_clicked().connect(sigc::mem_fun(*this,
      &BuilderListToolBoxImpl::onAddButtonClicked));
  mp_RemoveButton->signal_clicked().connect(sigc::mem_fun(*this,
      &BuilderListToolBoxImpl::onRemoveButtonClicked));
  mp_UpButton->signal_clicked().connect(sigc::mem_fun(*this,
      &BuilderListToolBoxImpl::onUpButtonClicked));
  mp_DownButton->signal_clicked().connect(sigc::mem_fun(*this,
      &BuilderListToolBoxImpl::onDownButtonClicked));
  mp_EditButton->signal_clicked().connect(sigc::mem_fun(*this,
      &BuilderListToolBoxImpl::onEditButtonClicked));

  mp_ButtonBox = Gtk::manage(new Gtk::VButtonBox(Gtk::BUTTONBOX_CENTER, 10));
  mp_ButtonBox->set_border_width(10); // empty space around ButtonBox
  mp_ButtonBox->add(*mp_AddButton);
  mp_ButtonBox->add(*mp_RemoveButton);
  mp_ButtonBox->add(*mp_UpButton);
  mp_ButtonBox->add(*mp_DownButton);
  mp_ButtonBox->add(*mp_EditButton);
  mp_ButtonBox->set_visible(true);

  mp_Frame = Gtk::manage(new Gtk::Frame());
  mp_Frame->add(*mp_ButtonBox);
  mp_Frame->set_visible(true);

  mp_Alignment = Gtk::manage(new Gtk::Alignment(0.5, 0.5, 0, 0)); // nul scale to avoid empty space between Frame and ButtonBox
  mp_Alignment->add(*mp_Frame);
  mp_Alignment->set_visible(true);
}
sigc::signal<void> BuilderListToolBoxImpl::signal_AddCommandAsked()
{
  return m_signal_AddCommandAsked;
}
sigc::signal<void> BuilderListToolBoxImpl::signal_RemoveCommandAsked()
{
  return m_signal_RemoveCommandAsked;
}
sigc::signal<void> BuilderListToolBoxImpl::signal_UpCommandAsked()
{
  return m_signal_UpCommandAsked;
}
sigc::signal<void> BuilderListToolBoxImpl::signal_DownCommandAsked()
{
  return m_signal_DownCommandAsked;
}
sigc::signal<void> BuilderListToolBoxImpl::signal_EditCommandAsked()
{
  return m_signal_EditCommandAsked;
}
Gtk::Widget* BuilderListToolBoxImpl::asWidget()
{
  return mp_Alignment;
}
void BuilderListToolBoxImpl::setAllCommandVisible(bool Visible)
{
  setAddCommandVisible(Visible);
  setRemoveCommandVisible(Visible);
  setUpCommandVisible(Visible);
  setDownCommandVisible(Visible);
  setEditCommandVisible(Visible);
}
void BuilderListToolBoxImpl::setAddCommandVisible(bool Visible)
{
  mp_AddButton->set_visible(Visible);
}
void BuilderListToolBoxImpl::setRemoveCommandVisible(bool Visible)
{
  mp_RemoveButton->set_visible(Visible);
}
void BuilderListToolBoxImpl::setUpCommandVisible(bool Visible)
{
  mp_UpButton->set_visible(Visible);
}
void BuilderListToolBoxImpl::setDownCommandVisible(bool Visible)
{
  mp_DownButton->set_visible(Visible);
}
void BuilderListToolBoxImpl::setEditCommandVisible(bool Visible)
{
  mp_EditButton->set_visible(Visible);
}
void BuilderListToolBoxImpl::setAllCommandAvailable(bool Available)
{
  setAddCommandAvailable(Available);
  setRemoveCommandAvailable(Available);
  setUpCommandAvailable(Available);
  setDownCommandAvailable(Available);
  setEditCommandAvailable(Available);
}
void BuilderListToolBoxImpl::setAddCommandAvailable(bool Available)
{
  mp_AddButton->set_sensitive(Available);
}
void BuilderListToolBoxImpl::setRemoveCommandAvailable(bool Available)
{
  mp_RemoveButton->set_sensitive(Available);
}
void BuilderListToolBoxImpl::setUpCommandAvailable(bool Available)
{
  mp_UpButton->set_sensitive(Available);
}
void BuilderListToolBoxImpl::setDownCommandAvailable(bool Available)
{
  mp_DownButton->set_sensitive(Available);
}
void BuilderListToolBoxImpl::setEditCommandAvailable(bool Available)
{
  mp_EditButton->set_sensitive(Available);
}
void BuilderListToolBoxImpl::setAddCommandTooltipText(std::string TooltipText)
{
  mp_AddButton->set_tooltip_text(TooltipText);
}
void BuilderListToolBoxImpl::setRemoveCommandTooltipText(
    std::string TooltipText)
{
  mp_RemoveButton->set_tooltip_text(TooltipText);
}
void BuilderListToolBoxImpl::setUpCommandTooltipText(std::string TooltipText)
{
  mp_UpButton->set_tooltip_text(TooltipText);
}
void BuilderListToolBoxImpl::setDownCommandTooltipText(std::string TooltipText)
{
  mp_DownButton->set_tooltip_text(TooltipText);
}
void BuilderListToolBoxImpl::setEditCommandTooltipText(std::string TooltipText)
{
  mp_EditButton->set_tooltip_text(TooltipText);
}
bool BuilderListToolBoxImpl::isAddCommandAvailable()
{
  return mp_AddButton->is_sensitive();
}
bool BuilderListToolBoxImpl::isRemoveCommandAvailable()
{
  return mp_RemoveButton->is_sensitive();
}
bool BuilderListToolBoxImpl::isUpCommandAvailable()
{
  return mp_UpButton->is_sensitive();
}
bool BuilderListToolBoxImpl::isDownCommandAvailable()
{
  return mp_DownButton->is_sensitive();
}
bool BuilderListToolBoxImpl::isEditCommandAvailable()
{
  return mp_EditButton->is_sensitive();
}
