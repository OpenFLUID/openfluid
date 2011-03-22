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
 \file DomainUnitEditionView.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitEditionView.hpp"

#include <glibmm/i18n.h>

#include "BuilderListToolBoxFactory.hpp"
#include "BuilderListToolBox.hpp"
#include "DomainUnitRelationTreeView.hpp"

void DomainUnitEditionViewImpl::initGlobalVisibility()
{
  mp_Dialog->show_all_children();

  mp_FromsToolBox->setDownCommandVisible(false);
  mp_FromsToolBox->setUpCommandVisible(false);
  mp_FromsToolBox->setEditCommandVisible(false);

  mp_TosToolBox->setDownCommandVisible(false);
  mp_TosToolBox->setUpCommandVisible(false);
  mp_TosToolBox->setEditCommandVisible(false);

  mp_ParentsToolBox->setDownCommandVisible(false);
  mp_ParentsToolBox->setUpCommandVisible(false);
  mp_ParentsToolBox->setEditCommandVisible(false);

  mp_ChildrenToolBox->setDownCommandVisible(false);
  mp_ChildrenToolBox->setUpCommandVisible(false);
  mp_ChildrenToolBox->setEditCommandVisible(false);
}
void DomainUnitEditionViewImpl::createAddDialog()
{
  Gtk::Label* ClassLabel = Gtk::manage(new Gtk::Label(_("Class Unit")));
  mp_ClassAddEntry = Gtk::manage(new Gtk::Entry());
  Gtk::Label* IdLabel = Gtk::manage(new Gtk::Label(_("Id")));
  mp_IdAddEntry = Gtk::manage(new Gtk::Entry());
  Gtk::Table* AddTable = Gtk::manage(new Gtk::Table());

  AddTable->attach(*ClassLabel, 0, 1, 0, 1);
  AddTable->attach(*IdLabel, 0, 1, 1, 2);
  AddTable->attach(*mp_ClassAddEntry, 1, 2, 0, 1);
  AddTable->attach(*mp_IdAddEntry, 1, 2, 1, 2);

  mp_AddDialog = new Gtk::Dialog("Unit to link to");

  mp_AddDialog->get_vbox()->pack_start(*AddTable);

  mp_AddDialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);

  Gtk::Button* OkAddButton = Gtk::manage(new Gtk::Button(Gtk::Stock::OK));
  OkAddButton->signal_clicked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onRelationAddAsked));
  mp_AddDialog->get_action_area()->pack_start(*OkAddButton);

  mp_AddDialog->show_all_children();
}
void DomainUnitEditionViewImpl::onProcessOrderChanged()
{
  m_signal_ProcessOrderChanged.emit();
}
void DomainUnitEditionViewImpl::onFromSelectionChanged()
{
  mp_SelectedView = mp_FromsView;
  m_signal_RelationSelectionChanged.emit();
}
void DomainUnitEditionViewImpl::onToSelectionChanged()
{
  mp_SelectedView = mp_TosView;
  m_signal_RelationSelectionChanged.emit();
}
void DomainUnitEditionViewImpl::onParentSelectionChanged()
{
  mp_SelectedView = mp_ParentsView;
  m_signal_RelationSelectionChanged.emit();
}
void DomainUnitEditionViewImpl::onChildSelectionChanged()
{
  mp_SelectedView = mp_ChildrenView;
  m_signal_RelationSelectionChanged.emit();
}
void DomainUnitEditionViewImpl::onRelationRemoveAsked()
{
  m_signal_RelationRemoveAsked .emit();
}
void DomainUnitEditionViewImpl::showAddDialog()
{
  if (mp_AddDialog->run() == Gtk::RESPONSE_CANCEL)
    closeAddDialog();
}
void DomainUnitEditionViewImpl::onFromRelationAddButtonClicked()
{
  mp_SelectedView = mp_FromsView;
  showAddDialog();
}
void DomainUnitEditionViewImpl::onToRelationAddButtonClicked()
{
  mp_SelectedView = mp_TosView;
  showAddDialog();
}
void DomainUnitEditionViewImpl::onParentRelationAddButtonClicked()
{
  mp_SelectedView = mp_ParentsView;
  showAddDialog();
}
void DomainUnitEditionViewImpl::onChildRelationAddButtonClicked()
{
  mp_SelectedView = mp_ChildrenView;
  showAddDialog();
}
void DomainUnitEditionViewImpl::onRelationAddAsked()
{
  m_signal_RelationAddAsked.emit();
}

DomainUnitEditionViewImpl::DomainUnitEditionViewImpl()
{
  Gtk::Label* ClassLabel = Gtk::manage(new Gtk::Label(_("Class Unit")));
  mp_ClassValueLabel = Gtk::manage(new Gtk::Label());

  Gtk::Label* IdLabel = Gtk::manage(new Gtk::Label(_("Id")));
  mp_IdValueLabel = Gtk::manage(new Gtk::Label());

  Gtk::Label* PcsOrderLabel = Gtk::manage(new Gtk::Label(_("Process Order")));
  mp_PcsOrderSpin = Gtk::manage(new Gtk::SpinButton());
  mp_PcsOrderSpin->set_range(1.0, 9999.0);
  mp_PcsOrderSpin->set_increments(1, 10);
  mp_PcsOrderSpin->set_numeric(true);
  mp_PcsOrderSpin->signal_value_changed().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onProcessOrderChanged));

  mp_InfoTable = Gtk::manage(new Gtk::Table());
  mp_InfoTable->attach(*ClassLabel, 0, 1, 0, 1);
  mp_InfoTable->attach(*IdLabel, 0, 1, 1, 2);
  mp_InfoTable->attach(*PcsOrderLabel, 0, 1, 2, 3);
  mp_InfoTable->attach(*mp_ClassValueLabel, 1, 2, 0, 1);
  mp_InfoTable->attach(*mp_IdValueLabel, 1, 2, 1, 2);
  mp_InfoTable->attach(*mp_PcsOrderSpin, 1, 2, 2, 3);

  BuilderListToolBoxFactory ToolBoxFactory;

  mp_FromsView = new DomainUnitRelationTreeView("Froms");
  mp_FromsView->get_selection()->signal_changed().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onFromSelectionChanged));
  mp_FromsToolBox = ToolBoxFactory.createDomainUnitRelationToolBox("From");
  mp_FromsToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onRelationRemoveAsked));
  mp_FromsToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onFromRelationAddButtonClicked));
  Gtk::HBox* FromsBox = Gtk::manage(new Gtk::HBox());
  FromsBox->pack_start(*mp_FromsView);
  FromsBox->pack_start(*mp_FromsToolBox->asWidget());
  Gtk::Expander* FromsExpander =
      Gtk::manage(new Gtk::Expander(_("From Units")));
  FromsExpander->add(*FromsBox);

  mp_TosView = new DomainUnitRelationTreeView("Tos");
  mp_TosView->get_selection()->signal_changed().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onToSelectionChanged));
  mp_TosToolBox = ToolBoxFactory.createDomainUnitRelationToolBox("To");
  mp_TosToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onRelationRemoveAsked));
  mp_TosToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onToRelationAddButtonClicked));
  Gtk::HBox* TosBox = Gtk::manage(new Gtk::HBox());
  TosBox->pack_start(*mp_TosView);
  TosBox->pack_start(*mp_TosToolBox->asWidget());
  Gtk::Expander* TosExpander = Gtk::manage(new Gtk::Expander(_("To Units")));
  TosExpander->add(*TosBox);

  mp_ParentsView = new DomainUnitRelationTreeView("Parents");
  mp_ParentsView->get_selection()->signal_changed().connect(sigc::mem_fun(
      *this, &DomainUnitEditionViewImpl::onParentSelectionChanged));
  mp_ParentsToolBox = ToolBoxFactory.createDomainUnitRelationToolBox("Parent");
  mp_ParentsToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onRelationRemoveAsked));
  mp_ParentsToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onParentRelationAddButtonClicked));
  Gtk::HBox* ParentsBox = Gtk::manage(new Gtk::HBox());
  ParentsBox->pack_start(*mp_ParentsView);
  ParentsBox->pack_start(*mp_ParentsToolBox->asWidget());
  Gtk::Expander* ParentsExpander = Gtk::manage(new Gtk::Expander(_(
      "Parents Units")));
  ParentsExpander->add(*ParentsBox);

  mp_ChildrenView = new DomainUnitRelationTreeView("Children");
  mp_ChildrenView->get_selection()->signal_changed().connect(sigc::mem_fun(
      *this, &DomainUnitEditionViewImpl::onChildSelectionChanged));
  mp_ChildrenToolBox = ToolBoxFactory.createDomainUnitRelationToolBox("Child");
  mp_ChildrenToolBox->signal_RemoveCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onRelationRemoveAsked));
  mp_ChildrenToolBox->signal_AddCommandAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionViewImpl::onChildRelationAddButtonClicked));
  Gtk::HBox* ChildrenBox = Gtk::manage(new Gtk::HBox());
  ChildrenBox->pack_start(*mp_ChildrenView);
  ChildrenBox->pack_start(*mp_ChildrenToolBox->asWidget());
  Gtk::Expander* ChildrenExpander = Gtk::manage(new Gtk::Expander(_(
      "Children Units")));
  ChildrenExpander->add(*ChildrenBox);

  mp_MainBox = Gtk::manage(new Gtk::VBox());
  mp_MainBox->pack_start(*mp_InfoTable);
  mp_MainBox->pack_start(*FromsExpander);
  mp_MainBox->pack_start(*TosExpander);
  mp_MainBox->pack_start(*ParentsExpander);
  mp_MainBox->pack_start(*ChildrenExpander);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->get_vbox()->pack_start(*mp_MainBox);

  mp_CloseEditButton = mp_Dialog->add_button(Gtk::Stock::CLOSE,
      Gtk::RESPONSE_CLOSE);

  mp_Dialog->set_title(_("Unit Edition"));

  initGlobalVisibility();

  createAddDialog();
}
DomainUnitEditionViewImpl::~DomainUnitEditionViewImpl()
{
  delete mp_Dialog;
}
sigc::signal<void> DomainUnitEditionViewImpl::signal_ProcessOrderChanged()
{
  return m_signal_ProcessOrderChanged;
}
sigc::signal<void> DomainUnitEditionViewImpl::signal_RelationSelectionChanged()
{
  return m_signal_RelationSelectionChanged;
}
sigc::signal<void> DomainUnitEditionViewImpl::signal_RelationRemoveAsked()
{
  return m_signal_RelationRemoveAsked;
}
sigc::signal<void> DomainUnitEditionViewImpl::signal_RelationAddAsked()
{
  return m_signal_RelationAddAsked;
}
void DomainUnitEditionViewImpl::setClass(std::string ClassName)
{
  mp_ClassValueLabel->set_text(ClassName);
}
void DomainUnitEditionViewImpl::setId(int Id)
{
  std::ostringstream ss;
  ss << Id;
  mp_IdValueLabel->set_text(ss.str());
}
void DomainUnitEditionViewImpl::setProcessOrder(int PcsOrder)
{
  mp_PcsOrderSpin->set_value(PcsOrder);
}
void DomainUnitEditionViewImpl::setFromsModel(
    Glib::RefPtr<Gtk::TreeModel> FromsModel)
{
  mp_FromsView-> set_model(FromsModel);
}
void DomainUnitEditionViewImpl::setTosModel(
    Glib::RefPtr<Gtk::TreeModel> TosModel)
{
  mp_TosView->set_model(TosModel);
}
void DomainUnitEditionViewImpl::setParentsModel(
    Glib::RefPtr<Gtk::TreeModel> ParentsModel)
{
  mp_ParentsView->set_model(ParentsModel);
}
void DomainUnitEditionViewImpl::setChildrenModel(
    Glib::RefPtr<Gtk::TreeModel> ChildrenModel)
{
  mp_ChildrenView->set_model(ChildrenModel);
}
void DomainUnitEditionViewImpl::showDialog()
{
  if (mp_Dialog->run() == Gtk::RESPONSE_CLOSE)
    mp_Dialog->hide();
}
Gtk::Widget* DomainUnitEditionViewImpl::asWidget()
{
  return mp_MainBox;
}
std::string DomainUnitEditionViewImpl::getClassName()
{
  return mp_ClassValueLabel->get_text();
}
int DomainUnitEditionViewImpl::getId()
{
  std::istringstream ss(mp_IdValueLabel->get_text());
  int val;
  ss >> val;
  return val;
}
int DomainUnitEditionViewImpl::getProcessOrder()
{
  return mp_PcsOrderSpin->get_value_as_int();
}
std::pair<std::string, std::pair<std::string, int> > DomainUnitEditionViewImpl::getSelectedRelation()
{
  if (mp_SelectedView)
    return mp_SelectedView->getSelectedRelation();
  else
    return std::make_pair("", std::make_pair("", 0));
}
std::pair<std::string, Gtk::TreeIter> DomainUnitEditionViewImpl::getSelectedRelationIter()
{
  if (mp_SelectedView)
    return mp_SelectedView->getSelectedRelationIter();
  else
    return std::make_pair("", (Gtk::TreeIter) 0);
}
void DomainUnitEditionViewImpl::closeAddDialog()
{
  mp_AddDialog->hide();
}
std::string DomainUnitEditionViewImpl::getAddClassName()
{
  return mp_ClassAddEntry->get_text();
}
int DomainUnitEditionViewImpl::getAddId()
{
  std::istringstream ss(mp_IdAddEntry->get_text());
  int val;
  ss >> val;
  return val;
}

Gtk::TreeView* DomainUnitEditionViewSub::getParentsView()
{
  return mp_ParentsView;
}
void DomainUnitEditionViewSub::selectParentsView()
{
  mp_SelectedView = mp_ParentsView;
}
void DomainUnitEditionViewSub::setAddInfo(std::string AddClassName,
    std::string AddId)
{
  mp_ClassAddEntry->set_text(AddClassName);
  mp_IdAddEntry->set_text(AddId);
}
