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
  @file GroupStatusMessagesWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/ui/waresdev/GroupStatusMessagesWidget.hpp>
#include <openfluid/ui/waresdev/StatusButtonMessageWidget.hpp>

#include "ui_GroupStatusMessagesWidget.h"


namespace openfluid { namespace ui { namespace waresdev {


GroupStatusMessagesWidget::GroupStatusMessagesWidget(const QString& Category, QWidget* Parent):
  QWidget(Parent), ui(new Ui::GroupStatusMessagesWidget)
{
  ui->setupUi(this);

  setVisible(false);

}


// =====================================================================
// =====================================================================


GroupStatusMessagesWidget::~GroupStatusMessagesWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void GroupStatusMessagesWidget::clear()
{
  setVisible(false);
  while (QLayoutItem* item = layout()->takeAt(0))
  {
    if (item->widget())
    {
      delete item->widget();
    }
    delete item;
  }
}


// =====================================================================
// =====================================================================


void GroupStatusMessagesWidget::addMessage(openfluid::waresdev::WareSrcChecker::ReportingData::ReportingItem Item)
{
  setVisible(true);
  StatusButtonMessageWidget* MessageWidget = new StatusButtonMessageWidget(Item);
  layout()->addWidget(MessageWidget); 
  
  // TOIMPL store widget in list (or in dedicated widget so that clear operation is easier)
  // TOIMPL fix message (dyn version?)

  connect(MessageWidget, SIGNAL(actionTriggered(QString)), this, SIGNAL(actionTriggered(QString)));
  // TOIMPL disconnect at clear step (requires widget storage with info about signal)
}

} } } // namespaces
