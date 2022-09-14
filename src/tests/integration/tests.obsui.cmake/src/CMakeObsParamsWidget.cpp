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
  @file CMakeObsParamsWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QMessageBox>


#include "CMakeObsParamsWidget.hpp"
#include "ui_CMakeObsParamsWidget.h"


CMakeObsParamsWidget::CMakeObsParamsWidget(): openfluid::ui::builderext::PluggableParameterizationExtension(),
  ui(new Ui::CMakeObsParamsWidget)
{
  ui->setupUi(this);

  connect(ui->ClickButton,SIGNAL(clicked()),this,SLOT(showMessage()));

}


// =====================================================================
// =====================================================================


CMakeObsParamsWidget::~CMakeObsParamsWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void CMakeObsParamsWidget::update()
{

}


// =====================================================================
// =====================================================================


void CMakeObsParamsWidget::showMessage()
{
  QMessageBox::information(nullptr,tr("Clicked!"),"You clicked on me!");
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(CMakeObsParamsWidget)

DEFINE_WARE_LINKUID(WARE_LINKUID)
