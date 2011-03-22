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
 \file DomainUnitEditionPresenter.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "DomainUnitEditionPresenter.hpp"

#include "DomainUnitEditionModel.hpp"
#include "DomainUnitEditionAdapter.hpp"

void DomainUnitEditionPresenter::whenFromAppInitAsked()
{
  m_Adapter.setClassName(m_Model.getClassName());
  m_Adapter.setId(m_Model.getId());
  m_Adapter.setProcessOrder(m_Model.getProcessOrder());
  m_Adapter.setRelations(m_Model.getRelationsByName());
}
void DomainUnitEditionPresenter::whenFromAppShowDialogAsked()
{
  m_Adapter.showDialog();
}
void DomainUnitEditionPresenter::whenFromAppSelectedRelationRemoved()
{
  m_Adapter.removeSelectedRelation();
}
void DomainUnitEditionPresenter::whenFromAppRelationAdded()
{
  m_Adapter.addRelation();
}
void DomainUnitEditionPresenter::whenFromUserProcessOrderChanged()
{
  m_Model.setProcessOrder(m_Adapter.getProcessOrder());
}
void DomainUnitEditionPresenter::whenFromUserRelationSelectionChanged()
{
  m_Model.setSelectedRelation(m_Adapter.getSelectedRelation());
}
void DomainUnitEditionPresenter::whenFromUserRelationRemoveAsked()
{
  m_Model.removeSelectedRelation();
}
void DomainUnitEditionPresenter::whenFromUserRelationAddAsked()
{
  m_Model.addRelationAsked(m_Adapter.getSelectedRelation().first,
      m_Adapter.getAddClassName(), m_Adapter.getAddId());
}

DomainUnitEditionPresenter::DomainUnitEditionPresenter(
    DomainUnitEditionModel& Model, DomainUnitEditionAdapter& Adapter) :
  m_Model(Model), m_Adapter(Adapter)
{
  m_Model.signal_InitAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromAppInitAsked));
  m_Model.signal_ShowDialogAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromAppShowDialogAsked));
  m_Model.signal_SelectedRelationRemoved().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromAppSelectedRelationRemoved));
  m_Model.signal_RelationAdded().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromAppRelationAdded));

  m_Adapter.signal_ProcessOrderChanged().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromUserProcessOrderChanged));
  m_Adapter.signal_RelationSelectionChanged().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromUserRelationSelectionChanged));
  m_Adapter.signal_SelectedRelationRemoveAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromUserRelationRemoveAsked));
  m_Adapter.signal_RelationAddAsked().connect(sigc::mem_fun(*this,
      &DomainUnitEditionPresenter::whenFromUserRelationAddAsked));
}
