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
 \file SimulOutSetDescPresenter.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "SimulOutSetDescPresenter.hpp"

#include "SimulOutSetDescModel.hpp"
#include "SimulOutSetDescView.hpp"

void SimulOutSetDescPresenter::init()
{
  m_haveValuesToBeStored = false;

  m_View.setName(m_Model.getName());
  m_View.setPrecision(m_Model.getPrecision());

  m_View.setFormatNames(m_Model.getFormatNames());
  m_View.setFormatName(m_Model.getFormatName());

  m_View.setClasses(m_Model.getClasses());
  m_View.setByClassIDs(m_Model.getByClassIDs());
  m_View.setByClassVars(m_Model.getByClassVars());

  m_View.setClass(m_Model.getClass());
  m_View.setIDs(m_Model.getIDs());
  m_View.setVars(m_Model.getVars());

  m_haveValuesToBeStored = true;
}
void SimulOutSetDescPresenter::whenFromAppInitForCreationAsked()
{
  m_View.setCreationMode();
  init();
}
void SimulOutSetDescPresenter::whenFromAppInitForEditionAsked()
{
  m_View.setEditionMode();
  init();
}
void SimulOutSetDescPresenter::whenFromAppShowDialogAsked()
{
  m_View.showDialog();
}
void SimulOutSetDescPresenter::whenFromAppCloseDialogAsked()
{
  m_View.closeDialog();
}
void SimulOutSetDescPresenter::whenFromAppErrorDialogAsked(
    std::string ErrorMessage)
{
  m_View.showErrorMessageDialog(ErrorMessage);
}

void SimulOutSetDescPresenter::whenFromUserNameChanged()
{
  if (m_haveValuesToBeStored)
    m_Model.setName(m_View.getName());
}
void SimulOutSetDescPresenter::whenFromUserClassChanged()
{
  if (m_haveValuesToBeStored)
    m_Model.setClass(m_View.getClass());
}
void SimulOutSetDescPresenter::whenFromUserIDsChanged()
{
  if (m_haveValuesToBeStored)
    m_Model.setIDs(m_View.getIDs());
}
void SimulOutSetDescPresenter::whenFromUserVarsChanged()
{
  if (m_haveValuesToBeStored)
    m_Model.setVars(m_View.getVars());
}
void SimulOutSetDescPresenter::whenFromUserFormatNameChanged()
{
  if (m_haveValuesToBeStored)
    m_Model.setFormatName(m_View.getFormatName());
}
void SimulOutSetDescPresenter::whenFromUserPrecisionChanged()
{
  if (m_haveValuesToBeStored)
    m_Model.setPrecision(m_View.getPrecision());
}
void SimulOutSetDescPresenter::whenFromUserSaveAsked()
{
  m_Model.save();
}
void SimulOutSetDescPresenter::whenFromUserCancelAsked()
{
  m_Model.cancel();
}

SimulOutSetDescPresenter::SimulOutSetDescPresenter(SimulOutSetDescModel& Model,
    SimulOutSetDescView& View) :
  m_Model(Model), m_View(View), m_haveValuesToBeStored(false)
{
  m_Model.signal_InitForCreationAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromAppInitForCreationAsked));
  m_Model.signal_InitForEditionAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromAppInitForEditionAsked));

  m_Model.signal_ShowDialogAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromAppShowDialogAsked));
  m_Model.signal_CloseDialogAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromAppCloseDialogAsked));
  m_Model.signal_ErrorDialogAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromAppErrorDialogAsked));

  m_View.signal_NameChanged().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserNameChanged));
  m_View.signal_ClassChanged().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserClassChanged));
  m_View.signal_IDsChanged().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserIDsChanged));
  m_View.signal_VarsChanged().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserVarsChanged));
  m_View.signal_FormatNameChanged().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserFormatNameChanged));
  m_View.signal_PrecisionChanged().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserPrecisionChanged));
  m_View.signal_SaveAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserSaveAsked));
  m_View.signal_CancelAsked().connect(sigc::mem_fun(*this,
      &SimulOutSetDescPresenter::whenFromUserCancelAsked));
}
