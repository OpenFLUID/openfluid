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
 \file ModelAddFunctionDialog.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include "ModelAddFunctionDialog.hpp"

#include <gtkmm/paned.h>
#include <gtkmm/stock.h>
#include <openfluid/fluidx/AdvancedModelDescriptor.hpp>
#include "AvailFunctionsWidget.hpp"
#include "SignatureDetailWidget.hpp"

// =====================================================================
// =====================================================================

ModelAddFunctionDialog::ModelAddFunctionDialog(
    openfluid::fluidx::AdvancedModelDescriptor& Model) :
    m_Model(Model), mp_SelectedSignature(0)
{
  mp_AvailFct = Gtk::manage(new AvailFunctionsWidget(m_Model));
  mp_AvailFct->signal_SelectionChanged().connect(
      sigc::mem_fun(*this, &ModelAddFunctionDialog::whenSelectionChanged));
  mp_AvailFct->signal_Activated().connect(
      sigc::mem_fun(*this, &ModelAddFunctionDialog::whenActivated));

  mp_Detail = Gtk::manage(new SignatureDetailWidget());

  Gtk::HPaned* Paned = Gtk::manage(new Gtk::HPaned());

  Paned->set_border_width(5);
  Paned->pack1(*mp_AvailFct, true, true);
  Paned->pack2(*mp_Detail, true, false);
  Paned->set_visible(true);

  mp_Dialog = new Gtk::Dialog();
  mp_Dialog->set_modal(true);
  mp_Dialog->set_size_request(1000, 500);
  mp_Dialog->get_vbox()->add(*Paned);

  mp_Dialog->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  mp_Dialog->add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);
}

// =====================================================================
// =====================================================================

ModelAddFunctionDialog::~ModelAddFunctionDialog()
{
  delete mp_Dialog;
}

// =====================================================================
// =====================================================================

openfluid::machine::ModelItemSignatureInstance* ModelAddFunctionDialog::show()
{
  mp_SelectedSignature = 0;

  //selection of first item causes mp_SelectedSignature and mp_Detail to update
  mp_AvailFct->update();

  if (mp_Dialog->run() != Gtk::RESPONSE_OK)
    mp_SelectedSignature = 0;

  mp_Dialog->hide();

  return mp_SelectedSignature;
}

// =====================================================================
// =====================================================================

void ModelAddFunctionDialog::whenSelectionChanged(
    openfluid::machine::ModelItemSignatureInstance* Sign)
{
  mp_SelectedSignature = Sign;

  if (mp_SelectedSignature)
  {
    mp_Dialog->set_response_sensitive(
        Gtk::RESPONSE_OK,
        m_Model.getFirstItemIndex(mp_SelectedSignature->Signature->ID) == -1);
  }
  else
  {
    mp_Dialog->set_response_sensitive(Gtk::RESPONSE_OK, false);
  }

  mp_Detail->update(mp_SelectedSignature);
}

// =====================================================================
// =====================================================================

void ModelAddFunctionDialog::whenActivated(
    openfluid::machine::ModelItemSignatureInstance* Sign)
{
  mp_SelectedSignature = Sign;

  mp_Dialog->response(Gtk::RESPONSE_OK);
}

// =====================================================================
// =====================================================================

