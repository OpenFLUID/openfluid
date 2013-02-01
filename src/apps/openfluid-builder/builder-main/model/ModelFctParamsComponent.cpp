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
 \file ModelFctParamsComponent.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelFctParamsComponent.hpp"

#include "ModelFctParamsModel.hpp"
#include "ModelFctParamsView.hpp"
#include "ModelFctParamsPresenter.hpp"

// =====================================================================
// =====================================================================

ModelFctParamsComponent::ModelFctParamsComponent(
    openfluid::fluidx::ModelItemDescriptor* Item,
    openfluid::guicommon::BuilderModel& Model)
{
  mp_Model = new ModelFctParamsModelImpl(Model);
  mp_View = new ModelFctParamsViewImpl();
  mp_Presenter = new ModelFctParamsPresenter(*mp_Model, *mp_View);

  mp_Model->signal_ParamsChanged().connect(
      sigc::mem_fun(*this, &ModelFctParamsComponent::whenParamsChanged));

  mp_Model->signal_RequiredFileChanged().connect(
      sigc::mem_fun(*this, &ModelFctParamsComponent::whenRequiredFileChanged));

  mp_Model->setModelItemDescriptor(Item);
}

// =====================================================================
// =====================================================================

ModelFctParamsComponent::~ModelFctParamsComponent()
{
  delete mp_Presenter;
  delete mp_Model;
  delete mp_View;
}

// =====================================================================
// =====================================================================

Gtk::Widget* ModelFctParamsComponent::asWidget()
{
  return mp_View->asWidget();
}

// =====================================================================
// =====================================================================

ModelFctParamsModel* ModelFctParamsComponent::getModel()
{
  return mp_Model;
}

// =====================================================================
// =====================================================================

void ModelFctParamsComponent::updateGlobalValues()
{
  mp_Model->updateGlobalValues();
}

// =====================================================================
// =====================================================================

void ModelFctParamsComponent::whenParamsChanged()
{
  m_signal_ParamsChanged.emit();
}

// =====================================================================
// =====================================================================

void ModelFctParamsComponent::whenRequiredFileChanged()
{
  m_signal_RequiredFileChanged.emit();
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsComponent::signal_ParamsChanged()
{
  return m_signal_ParamsChanged;
}

// =====================================================================
// =====================================================================

sigc::signal<void> ModelFctParamsComponent::signal_RequiredFileChanged()
{
  return m_signal_RequiredFileChanged;
}
