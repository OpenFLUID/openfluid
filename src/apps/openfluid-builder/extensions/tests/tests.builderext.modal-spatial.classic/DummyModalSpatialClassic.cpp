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
  \file DummyModalSpatialClassic.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */



#include "ui_DummyModalSpatialClassic.h"
#include "DummyModalSpatialClassic.hpp"

#include <QInputDialog>



// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("tests.builderext.modal-spatial.classic", openfluid::builderext::TYPE_MODAL)

  DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)
  DECLARE_MENUTEXT("Spatial classic")

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


DummyModalSpatialClassic::DummyModalSpatialClassic() :
  openfluid::builderext::PluggableModalExtension(),ui(new Ui::DummyModalSpatialClassic)
{
  Q_INIT_RESOURCE(spatialclassic);

  ui->setupUi(this);

  ui->ImageLabel->setText("");
  ui->ImageLabel->setPixmap(QPixmap(":/logo_title.png"));


  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addUnitClass()));
  connect(ui->ClearButton,SIGNAL(clicked()),this,SLOT(clearSpatialDomain()));
  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));

}


// =====================================================================
// =====================================================================


DummyModalSpatialClassic::~DummyModalSpatialClassic()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void DummyModalSpatialClassic::addUnitClass()
{
  bool OK;
  QString UnitClass = QInputDialog::getText(this, tr("Add a unit Class"),
                                            tr("Unit class name:"),
                                            QLineEdit::Normal,
                                            "",
                                            &OK);

  if (OK && !UnitClass.isEmpty())
  {
    openfluid::fluidx::UnitDescriptor* UDesc = new openfluid::fluidx::UnitDescriptor();
    UDesc->getUnitClass() = UnitClass.toStdString();
    UDesc->getUnitID() = 1;
    UDesc->getProcessOrder() = 1;
    mp_AdvancedDesc->getDomain().addUnit(UDesc);

    emit fluidxChanged(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT);
  }
}


// =====================================================================
// =====================================================================


void DummyModalSpatialClassic::clearSpatialDomain()
{
  mp_AdvancedDesc->getDomain().clearDomain();

  emit fluidxChanged(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT |
                     openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS);

}


// =====================================================================
// =====================================================================


void DummyModalSpatialClassic::update(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags)
{
  if (UpdateFlags.testFlag(openfluid::builderext::FluidXUpdateFlags::FLUIDX_ALL) ||
      UpdateFlags.testFlag(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALATTRS) ||
      UpdateFlags.testFlag(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT))
  ui->CountLabel->setText(QString("The spatial domain is made of %1 units class(es)").arg(mp_AdvancedDesc->getDomain().getClassNames().size()));
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(DummyModalSpatialClassic)

