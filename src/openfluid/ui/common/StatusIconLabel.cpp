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
  @file StatusIconLabel.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/common/StatusIconLabel.hpp>


namespace openfluid { namespace ui { namespace common {


StatusIconLabel::StatusIconLabel(QWidget* Parent):
  QLabel(Parent)
{

}


// =====================================================================
// =====================================================================


void StatusIconLabel::StatusIconLabel::setOKStatus()
{
  setPixmap(openfluid::ui::common::getImage("status-ok","ui/common"));
}


// =====================================================================
// =====================================================================


void StatusIconLabel::StatusIconLabel::setWarningStatus()
{
  setPixmap(openfluid::ui::common::getImage("status-warning","ui/common"));
}


// =====================================================================
// =====================================================================


void StatusIconLabel::StatusIconLabel::setErrorStatus()
{
  setPixmap(openfluid::ui::common::getImage("status-error","ui/common"));
}


// =====================================================================
// =====================================================================


void StatusIconLabel::setDisabledStatus()
{
  setPixmap(openfluid::ui::common::getImage("status-disabled","ui/common"));
}

} } } // namespaces

