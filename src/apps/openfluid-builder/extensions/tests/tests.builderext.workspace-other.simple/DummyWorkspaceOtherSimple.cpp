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
  \file DummyModalSpatialSimple.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "DummyWorkspaceOtherSimple.hpp"

#include <QPushButton>


// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("tests.builderext.workspace-other.simple", openfluid::builderext::TYPE_WORKSPACE)

  DECLARE_CATEGORY(openfluid::builderext::CAT_OTHER)
  DECLARE_MENUTEXT("Other simple (Workspace)")

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


DummyWorkspaceOtherSimple::DummyWorkspaceOtherSimple() :
  openfluid::builderext::PluggableWorkspaceExtension()
{
  QPushButton* Button = new QPushButton("emit signal",this);
  connect(Button,SIGNAL(clicked()),this,SLOT(change()));

}


// =====================================================================
// =====================================================================


void DummyWorkspaceOtherSimple::change()
{
  emit fluidxChanged();
}


// =====================================================================
// =====================================================================



DEFINE_BUILDEREXT_CLASS(DummyWorkspaceOtherSimple)
