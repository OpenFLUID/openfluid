/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file MessageDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QVBoxLayout>

#include <openfluid/ui/common/MessageDialog.hpp>


namespace openfluid { namespace ui { namespace common {


MessageDialog::MessageDialog(QWidget* Parent) :
  OpenFLUIDDialog(Parent), mp_MsgFrame(nullptr), mp_ButtonBox(nullptr)
{

}


// =====================================================================
// =====================================================================


MessageDialog::~MessageDialog()
{
  if (mp_MsgFrame)
  {
    mp_MsgFrame->deleteLater();
  }

  if (mp_ButtonBox)
  {
    mp_ButtonBox->deleteLater();
  }
}


// =====================================================================
// =====================================================================


void MessageDialog::setupMessageUi(const QString& DefaultMsg,
                                   QDialogButtonBox::StandardButtons Buttons)
{
  if (QVBoxLayout* Layout = dynamic_cast<QVBoxLayout*>(layout()))
  {
    mp_MsgFrame = new MessageFrame(this,DefaultMsg);

    Layout->insertWidget(0,mp_MsgFrame);

    if (!Buttons.testFlag(QDialogButtonBox::NoButton))
    {
      mp_ButtonBox = new QDialogButtonBox(Buttons,this);
      mp_ButtonBox->setContentsMargins(12,9,12,0);
      Layout->addWidget(mp_ButtonBox);

      connect(mp_ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
      connect(mp_ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
    }

    Layout->setContentsMargins(0,0,0,9);
    setContentsMargins(0,0,0,0);
  }
}


// =====================================================================
// =====================================================================


void MessageDialog::updateDefaultMessage(const QString& Msg)
{
  if (mp_MsgFrame)
  {
    mp_MsgFrame->updateDefaultMessage(Msg);
  }
}


// =====================================================================
// =====================================================================


void MessageDialog::setMessage(const QString& Msg)
{
  if (mp_MsgFrame)
  {
    mp_MsgFrame->setMessage(Msg);
  }

  if (mp_ButtonBox)
  {
    if (Msg.isEmpty())
    {
      mp_ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
      mp_ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
  }
}


// =====================================================================
// =====================================================================


QPushButton* MessageDialog::buttonOfButtonBox(QDialogButtonBox::StandardButton Which)
{
  if (mp_ButtonBox)
  {
    return mp_ButtonBox->button(Which);
  }

  return nullptr;
}

} } } // namespaces

