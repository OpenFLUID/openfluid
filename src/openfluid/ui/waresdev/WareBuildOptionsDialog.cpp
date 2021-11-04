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
  @file WareBuildOptionsDialog.cpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <openfluid/ui/waresdev/WareBuildOptionsDialog.hpp>
#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_WareBuildOptionsDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


WareBuildOptionsDialog::WareBuildOptionsDialog(const openfluid::waresdev::WareBuildOptions& BuildOptions, 
                                               QWidget* Parent):
  openfluid::ui::common::MessageDialog(Parent), ui(new Ui::WareBuildOptionsDialog)
{
  ui->setupUi(this);

  setupMessageUi(tr("Configure build options"));
  setMessage();

  // copy to handle save or cancel at last moment
  openfluid::waresdev::WareBuildOptions TmpBuildOptions = BuildOptions; 
  ui->BuildOptionsWidget->setBuildOptions(TmpBuildOptions);
}


// =====================================================================
// =====================================================================


WareBuildOptionsDialog::~WareBuildOptionsDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


openfluid::waresdev::WareBuildOptions WareBuildOptionsDialog::getOptions()
{
  return ui->BuildOptionsWidget->getOptions();
}


} } } // namespaces
