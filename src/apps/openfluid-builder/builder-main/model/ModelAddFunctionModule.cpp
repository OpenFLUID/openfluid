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
 \file ModelAddFunctionModule.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ModelAddFunctionModule.hpp"

#include <gtkmm/stock.h>

#include <openfluid/machine/ModelInstance.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ware/FunctionSignature.hpp>

#include "ModelAvailFctComponent.hpp"
#include "ModelFctDetailComponent.hpp"

#include "ModelAddFunctionCoordinator.hpp"


// =====================================================================
// =====================================================================


ModelAddFunctionModule::ModelAddFunctionModule()
{
  mp_ModelAvailFctMVP = new ModelAvailFctComponent();
  mp_ModelFctDetailMVP = new ModelFctDetailComponent();

  mp_Coordinator = new ModelAddFunctionCoordinator(
      *mp_ModelAvailFctMVP->getModel(), *mp_ModelFctDetailMVP->getModel());

  mp_Coordinator->signal_AvailFctSelectionChanged().connect(sigc::mem_fun(
      *this, &ModelAddFunctionModule::whenAvailFctSelectionChanged));

  compose();

}

// =====================================================================
// =====================================================================


ModelAddFunctionModule::~ModelAddFunctionModule()
{
  delete mp_Coordinator;
  delete mp_ModelAvailFctMVP;
  delete mp_ModelFctDetailMVP;
}

// =====================================================================
// =====================================================================

void ModelAddFunctionModule::setEngineRequirements(
    openfluid::machine::ModelInstance& ModelInstance)
{
  mp_ModelInstance = &ModelInstance;
}

// =====================================================================
// =====================================================================


void ModelAddFunctionModule::compose()
{
  mp_MainPanel = Gtk::manage(new Gtk::HPaned());

  mp_MainPanel->set_border_width(5);
  mp_MainPanel->pack1(*mp_ModelAvailFctMVP->asWidget(), Gtk::EXPAND);
  mp_MainPanel->pack2(*mp_ModelFctDetailMVP->asWidget(), false, false);
  mp_MainPanel->set_visible(true);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->set_modal(true);
  mp_Dialog->set_size_request(800, 500);
  mp_Dialog->get_vbox()->add(*mp_MainPanel);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

}

// =====================================================================
// =====================================================================


sigc::signal<void> ModelAddFunctionModule::signal_ModelFunctionAdded()
{
  return m_signal_ModelFunctionAdded;
}

// =====================================================================
// =====================================================================


openfluid::machine::SignatureItemInstance* ModelAddFunctionModule::showDialog()
{
  openfluid::machine::SignatureItemInstance* SelectedSignature = 0;

  whenAvailFctSelectionChanged();

  if (mp_Dialog->run() == Gtk::RESPONSE_OK)
    SelectedSignature = mp_Coordinator->getSelectedSignature();

  mp_Dialog->hide();

  return SelectedSignature;
}

// =====================================================================
// =====================================================================


void ModelAddFunctionModule::whenAvailFctSelectionChanged()
{
  bool SelectedFctAlreadyInModel = false;

  if (mp_Coordinator->getSelectedSignature())
  {
    std::string SelectedFctId =
        mp_Coordinator->getSelectedSignature()->Signature->ID;

    std::list<openfluid::machine::ModelItemInstance*>::const_iterator it;
    for (it = mp_ModelInstance->getItems().begin(); it
        != mp_ModelInstance->getItems().end(); ++it)
    {
      if ((*it)->Signature->ID == SelectedFctId)
      {
        SelectedFctAlreadyInModel = true;
        break;
      }
    }
  }

  mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK,
      !SelectedFctAlreadyInModel);
}

// =====================================================================
// =====================================================================


void ModelAddFunctionModule::setSignatures(
    FunctionSignatureRegistry& Signatures)
{
  mp_Coordinator->setSignatures(Signatures);
}

// =====================================================================
// =====================================================================
