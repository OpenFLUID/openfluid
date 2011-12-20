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
 \file BuilderListToolBox.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __BUILDERLISTTOOLBOX_HPP__
#define __BUILDERLISTTOOLBOX_HPP__

#include <sigc++/sigc++.h>

#include <gtkmm/buttonbox.h>
#include <gtkmm/frame.h>
#include <gtkmm/alignment.h>
#include <gtkmm/button.h>

class BuilderListToolBox
{
  public:
    virtual Gtk::Widget* asWidget() = 0;
    virtual sigc::signal<void> signal_AddCommandAsked() = 0;
    virtual sigc::signal<void> signal_RemoveCommandAsked() = 0;
    virtual sigc::signal<void> signal_UpCommandAsked() = 0;
    virtual sigc::signal<void> signal_DownCommandAsked() = 0;
    virtual sigc::signal<void> signal_EditCommandAsked() = 0;
    virtual void setAllCommandVisible(bool Visible = true) = 0;
    virtual void setAddCommandVisible(bool Visible = true) = 0;
    virtual void setRemoveCommandVisible(bool Visible = true) = 0;
    virtual void setUpCommandVisible(bool Visible = true) = 0;
    virtual void setDownCommandVisible(bool Visible = true) = 0;
    virtual void setEditCommandVisible(bool Visible = true) = 0;
    virtual void setAllCommandAvailable(bool Available = true) = 0;
    virtual void setAddCommandAvailable(bool Available = true) = 0;
    virtual void setRemoveCommandAvailable(bool Available = true) = 0;
    virtual void setUpCommandAvailable(bool Available = true) = 0;
    virtual void setDownCommandAvailable(bool Available = true) = 0;
    virtual void setEditCommandAvailable(bool Available = true) = 0;
    virtual void setAddCommandTooltipText(std::string TooltipText) = 0;
    virtual void setRemoveCommandTooltipText(std::string TooltipText) = 0;
    virtual void setUpCommandTooltipText(std::string TooltipText) = 0;
    virtual void setDownCommandTooltipText(std::string TooltipText) = 0;
    virtual void setEditCommandTooltipText(std::string TooltipText) = 0;
    virtual bool isAddCommandAvailable() = 0;
    virtual bool isRemoveCommandAvailable() = 0;
    virtual bool isUpCommandAvailable() = 0;
    virtual bool isDownCommandAvailable() = 0;
    virtual bool isEditCommandAvailable() = 0;
};

class BuilderListToolBoxImpl: public BuilderListToolBox
{
  private:
    sigc::signal<void> m_signal_AddCommandAsked;
    sigc::signal<void> m_signal_RemoveCommandAsked;
    sigc::signal<void> m_signal_UpCommandAsked;
    sigc::signal<void> m_signal_DownCommandAsked;
    sigc::signal<void> m_signal_EditCommandAsked;
    Gtk::ButtonBox* mp_ButtonBox;
    Gtk::Frame* mp_Frame;
    Gtk::Alignment* mp_Alignment;
    Gtk::Button* mp_AddButton;
    Gtk::Button* mp_RemoveButton;
    Gtk::Button* mp_UpButton;
    Gtk::Button* mp_DownButton;
    Gtk::Button* mp_EditButton;
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onUpButtonClicked();
    void onDownButtonClicked();
    void onEditButtonClicked();
    Gtk::Button* createImageStockButton(Gtk::StockID StockImage,
        Glib::ustring TooltipText = "");
  public:
    BuilderListToolBoxImpl();
    sigc::signal<void> signal_AddCommandAsked();
    sigc::signal<void> signal_RemoveCommandAsked();
    sigc::signal<void> signal_UpCommandAsked();
    sigc::signal<void> signal_DownCommandAsked();
    sigc::signal<void> signal_EditCommandAsked();
    Gtk::Widget* asWidget();
    void setAllCommandVisible(bool Visible);
    void setAddCommandVisible(bool Visible);
    void setRemoveCommandVisible(bool Visible);
    void setUpCommandVisible(bool Visible);
    void setDownCommandVisible(bool Visible);
    void setEditCommandVisible(bool Visible);
    void setAllCommandAvailable(bool Available);
    void setAddCommandAvailable(bool Available);
    void setRemoveCommandAvailable(bool Available);
    void setUpCommandAvailable(bool Available);
    void setDownCommandAvailable(bool Available);
    void setEditCommandAvailable(bool Available);
    void setAddCommandTooltipText(std::string TooltipText);
    void setRemoveCommandTooltipText(std::string TooltipText);
    void setUpCommandTooltipText(std::string TooltipText);
    void setDownCommandTooltipText(std::string TooltipText);
    void setEditCommandTooltipText(std::string TooltipText);
    bool isAddCommandAvailable();
    bool isRemoveCommandAvailable();
    bool isUpCommandAvailable();
    bool isDownCommandAvailable();
    bool isEditCommandAvailable();
};

#endif /* __BUILDERLISTTOOLBOX_HPP__ */
