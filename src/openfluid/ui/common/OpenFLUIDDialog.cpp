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
  @file OpenFLUIDDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QApplication>
#include <QMessageBox>

#include <openfluid/ui/common/OpenFLUIDDialog.hpp>
#include <openfluid/ui/config.hpp>


namespace openfluid { namespace ui { namespace common {


OpenFLUIDDialog::OpenFLUIDDialog(QWidget* Parent, const QString& Title) :
  QDialog(Parent)
{
  setWindowTitle(Title);
}


// =====================================================================
// =====================================================================


QString OpenFLUIDDialog::getPlaceholderRequired() const
{
  return QApplication::translate("openfluid::ui::config",openfluid::ui::config::PLACEHOLDER_REQUIRED);
}


// =====================================================================
// =====================================================================


bool OpenFLUIDDialog::confirmRestartAfterPreferences(QWidget* Parent)
{
  return (QMessageBox::question(Parent,
                                tr("Restart"),
                                tr("Modified preferences require the restart of the application to take effect.")+"\n"+
                                tr("Would you like to restart now?")+"\n"+
                                tr("Any unsaved data will be lost."),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}


} } } // namespaces


