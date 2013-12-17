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
  \file CMakeBExt.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ui_bext.h"
#include "CMakeBExt.hpp"

#include <QPushButton>


// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("tests.bext.cmake",openfluid::builderext::TYPE_MODAL)

  DECLARE_CATEGORY(openfluid::builderext::CAT_OTHER)
  DECLARE_MENUTEXT("tests.bext.cmake")

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


CMakeBuilderExtension::CMakeBuilderExtension() :
  openfluid::builderext::PluggableModalExtension(),
  ui(new Ui::BExtDialog)
{
  Q_INIT_RESOURCE(bext);

  ui->setupUi(this);

  ui->CloseButton->setIcon(QPixmap(":/file-close.png"));

  connect(ui->CloseButton,SIGNAL(clicked()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


CMakeBuilderExtension::~CMakeBuilderExtension()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void CMakeBuilderExtension::update()
{

}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(CMakeBuilderExtension)

