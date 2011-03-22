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
 \file DomainUnitEditionAdapter.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitEditionAdapter.hpp"

#include "DomainUnitEditionAdapterModel.hpp"
#include "DomainUnitEditionView.hpp"

void DomainUnitEditionAdapter::whenFromUserProcessOrderChanged()
{
  m_signal_ProcessOrderChanged.emit();
}
void DomainUnitEditionAdapter::whenFromUserRelationSelectionChanged()
{
  //update toolboxes visibility
  m_signal_RelationSelectionChanged.emit();
}
void DomainUnitEditionAdapter::whenFromUserRelationRemoveAsked()
{
  m_signal_SelectedRelationRemoveAsked.emit();
}
void DomainUnitEditionAdapter::whenFromUserRelationAddAsked()
{
  m_signal_RelationAddAsked.emit();
}

DomainUnitEditionAdapter::DomainUnitEditionAdapter(
    DomainUnitEditionAdapterModel& Model, DomainUnitEditionView& View) :
  m_Model(Model), m_View(View)
{
  m_View.signal_ProcessOrderChanged().connect(sigc::mem_fun(*this,
      &DomainUnitEditionAdapter::whenFromUserProcessOrderChanged));
  m_View.signal_RelationSelectionChanged().connect(sigc::mem_fun(*this,
      &DomainUnitEditionAdapter::whenFromUserRelationSelectionChanged));
  m_View.signal_RelationRemoveAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionAdapter::whenFromUserRelationRemoveAsked));
  m_View.signal_RelationAddAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionAdapter::whenFromUserRelationAddAsked));
}
sigc::signal<void> DomainUnitEditionAdapter::signal_ProcessOrderChanged()
{
  return m_signal_ProcessOrderChanged;
}
sigc::signal<void> DomainUnitEditionAdapter::signal_RelationSelectionChanged()
{
  return m_signal_RelationSelectionChanged;
}
sigc::signal<void> DomainUnitEditionAdapter::signal_SelectedRelationRemoveAsked()
{
  return m_signal_SelectedRelationRemoveAsked;
}
sigc::signal<void> DomainUnitEditionAdapter::signal_RelationAddAsked()
{
  return m_signal_RelationAddAsked;
}
void DomainUnitEditionAdapter::setClassName(std::string ClassName)
{
  m_View.setClass(ClassName);
}
void DomainUnitEditionAdapter::setId(int Id)
{
  m_View.setId(Id);
}
void DomainUnitEditionAdapter::setProcessOrder(int PcsOrder)
{
  m_View.setProcessOrder(PcsOrder);
}
void DomainUnitEditionAdapter::setRelations(std::map<std::string,
    openfluid::core::UnitsPtrList_t> Relations)
{
  m_Model.setRelations(Relations);

  m_View.setFromsModel(m_Model.getFromsModel());
  m_View.setTosModel(m_Model.getTosModel());
  m_View.setParentsModel(m_Model.getParentsModel());
  m_View.setChildrenModel(m_Model.getChildrenModel());
}
int DomainUnitEditionAdapter::getProcessOrder()
{
  return m_View.getProcessOrder();
}
std::string DomainUnitEditionAdapter::getAddClassName()
{
  return m_View.getAddClassName();
}
int DomainUnitEditionAdapter::getAddId()
{
  return m_View.getAddId();
}
void DomainUnitEditionAdapter::showDialog()
{
  m_View.showDialog();
}
std::pair<std::string, std::pair<std::string, int> > DomainUnitEditionAdapter::getSelectedRelation()
{
  return m_View.getSelectedRelation();
}
void DomainUnitEditionAdapter::removeSelectedRelation()
{
  m_Model.removeSelectedRelation(m_View.getSelectedRelationIter());
  //TODO: update ToolBoxes
}
void DomainUnitEditionAdapter::addRelation()
{
  m_Model.addRelation(getSelectedRelation().first, getAddClassName(),
      getAddId());
  m_View.closeAddDialog();
}
