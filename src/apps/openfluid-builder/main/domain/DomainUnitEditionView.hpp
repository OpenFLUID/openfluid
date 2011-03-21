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
 \file DomainUnitEditionView.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __DOMAINUNITEDITIONVIEW_HPP__
#define __DOMAINUNITEDITIONVIEW_HPP__

#include <sigc++/sigc++.h>

#include <gtkmm.h>
#include <glibmm/i18n.h>

class BuilderListToolBox;
class DomainUnitRelationTreeView;

class DomainUnitEditionView
{
  public:
    virtual sigc::signal<void> signal_ProcessOrderChanged() = 0;
    virtual sigc::signal<void> signal_RelationSelectionChanged() = 0;
    virtual sigc::signal<void>
    signal_RelationRemoveAsked() = 0;
    virtual sigc::signal<void>
    signal_RelationAddAsked() = 0;
    virtual Gtk::Widget* asWidget() = 0;
    virtual void showDialog() = 0;
    virtual void setClass(std::string ClassName) = 0;
    virtual void setId(int Id) = 0;
    virtual void setProcessOrder(int PcsOrder) = 0;
    virtual void setFromsModel(Glib::RefPtr<Gtk::TreeModel> FromsModel) = 0;
    virtual void setTosModel(Glib::RefPtr<Gtk::TreeModel> TosModel) = 0;
    virtual void setParentsModel(Glib::RefPtr<Gtk::TreeModel> ParentsModel) = 0;
    virtual void
    setChildrenModel(Glib::RefPtr<Gtk::TreeModel> ChildrenModel) = 0;
    virtual int getProcessOrder() = 0;
    virtual std::pair<std::string, std::pair<std::string, int> >
    getSelectedRelation() = 0;
    virtual std::pair<std::string, Gtk::TreeIter> getSelectedRelationIter() = 0;
    virtual void closeAddDialog() = 0;
    virtual std::string getAddClassName() = 0;
    virtual int getAddId() = 0;
};

class DomainUnitEditionViewImpl: public DomainUnitEditionView
{
  private:
    sigc::signal<void> m_signal_ProcessOrderChanged;
    sigc::signal<void> m_signal_RelationSelectionChanged;
    sigc::signal<void> m_signal_RelationRemoveAsked;
    sigc::signal<void> m_signal_RelationAddAsked;
    Gtk::Dialog* mp_Dialog;
    Gtk::Box* mp_MainBox;
    Gtk::Table* mp_InfoTable;
    Gtk::Label* mp_ClassValueLabel;
    Gtk::Label* mp_IdValueLabel;
    Gtk::SpinButton* mp_PcsOrderSpin;
    BuilderListToolBox* mp_FromsToolBox;
    BuilderListToolBox* mp_TosToolBox;
    BuilderListToolBox* mp_ParentsToolBox;
    BuilderListToolBox* mp_ChildrenToolBox;
    Gtk::Button* mp_CloseEditButton;
    Gtk::Dialog* mp_AddDialog;
    void initGlobalVisibility();
    void createAddDialog();
    void onProcessOrderChanged();
    void onFromSelectionChanged();
    void onToSelectionChanged();
    void onParentSelectionChanged();
    void onChildSelectionChanged();
    void onRelationRemoveAsked();
    void showAddDialog();
    void onFromRelationAddButtonClicked();
    void onToRelationAddButtonClicked();
    void onParentRelationAddButtonClicked();
    void onChildRelationAddButtonClicked();
    void onRelationAddAsked();
  protected:
    DomainUnitRelationTreeView* mp_FromsView;
    DomainUnitRelationTreeView* mp_TosView;
    DomainUnitRelationTreeView* mp_ParentsView;
    DomainUnitRelationTreeView* mp_ChildrenView;
    DomainUnitRelationTreeView* mp_SelectedView;
    Gtk::Entry* mp_ClassAddEntry;
    Gtk::Entry* mp_IdAddEntry;
  public:
    DomainUnitEditionViewImpl();
    ~DomainUnitEditionViewImpl();
    sigc::signal<void> signal_ProcessOrderChanged();
    sigc::signal<void> signal_RelationSelectionChanged();
    sigc::signal<void> signal_RelationRemoveAsked();
    sigc::signal<void> signal_RelationAddAsked();
    void setClass(std::string ClassName);
    void setId(int Id);
    void setProcessOrder(int PcsOrder);
    void setFromsModel(Glib::RefPtr<Gtk::TreeModel> FromsModel);
    void setTosModel(Glib::RefPtr<Gtk::TreeModel> TosModel);
    void setParentsModel(Glib::RefPtr<Gtk::TreeModel> ParentsModel);
    void setChildrenModel(Glib::RefPtr<Gtk::TreeModel> ChildrenModel);
    void showDialog();
    Gtk::Widget* asWidget();
    std::string getClassName();
    int getId();
    int getProcessOrder();
    std::pair<std::string, std::pair<std::string, int> > getSelectedRelation();
    std::pair<std::string, Gtk::TreeIter> getSelectedRelationIter();
    void closeAddDialog();
    std::string getAddClassName();
    int getAddId();
};

class DomainUnitEditionViewSub: public DomainUnitEditionViewImpl
{
  public:
    Gtk::TreeView* getParentsView();
    void selectParentsView();
    void setAddInfo(std::string AddClassName, std::string AddId);
};

#endif /* __DOMAINUNITEDITIONVIEW_HPP__ */
