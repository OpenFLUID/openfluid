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
  @file ExecutionStatusLabel.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/common/ExecutionStatusLabel.hpp>


namespace openfluid { namespace ui { namespace common {


ExecutionStatusLabel::ExecutionStatusLabel(QWidget* Parent):
  QLabel(Parent)
{
 
}


// =====================================================================
// =====================================================================


ExecutionStatusLabel::ExecutionStatusLabel(const QString& Text, QWidget* Parent):
  QLabel(Text,Parent)
{
  
}


// =====================================================================
// =====================================================================


void ExecutionStatusLabel::setStatus(Status S)
{
  setStatusTextOnly(S);
  setStatusStyleOnly(S);
}


// =====================================================================
// =====================================================================


void ExecutionStatusLabel::setStatusTextOnly(Status S)
{
  if (S == Status::NOTSTARTED)
  {
    setText(tr("not started"));
  }
  else if (S == Status::RUNNING)
  {
    setText(tr("running"));
  }
  else if (S == Status::REQUESTING_PAUSE)
  {
    setText(tr("pause requested"));
  }
  else if (S == Status::PAUSED)
  {
    setText(tr("paused"));
  }
  else if (S == Status::REQUESTING_ABORT)
  {
    setText(tr("abort requested"));
  }
  else if (S == Status::ABORTED)
  {
    setText(tr("aborted by user"));
  }
  else if (S == Status::FAILED)
  {
    setText(tr("failed"));
  }
  else if (S == Status::SUCCEEDED)
  {
    setText(tr("succeeded"));
  }
  else if (S == Status::USERKILLED)
  {
    setText(tr("aborted by user (KILL signal)"));
  }
  else
  {
    setText("");
  }
}


// =====================================================================
// =====================================================================


void ExecutionStatusLabel::setStatusStyleOnly(Status S)
{
  if (S == Status::RUNNING)
  {
    setStyleSheet("QLabel {font-weight: bold;}");
  }
  else if (S == Status::REQUESTING_PAUSE)
  {
    setStyleSheet("QLabel {color: #336DA5;}");
  }
  else if (S == Status::PAUSED)
  {
    setStyleSheet("QLabel {color: #336DA5; font-weight: bold;}");
  }
  else if (S == Status::REQUESTING_ABORT)
  {
    setStyleSheet("QLabel {color: #D11919;}");
  }
  else if (S == Status::ABORTED)
  {
    setStyleSheet("QLabel {color: #D11919; font-weight: bold;}");
  }
  else if (S == Status::FAILED)
  {
    setStyleSheet("QLabel {color: #D11919; font-weight: bold;} "
                  "QToolTip {color: #D11919; background-color: #F7F7F7;} ");
  }
  else if (S == Status::SUCCEEDED)
  {
    setStyleSheet("QLabel {color: #4E983E; font-weight: bold;}");
  }
  else if (S == Status::USERKILLED)
  {
    setStyleSheet("QLabel {color: #D11919; font-weight: bold;}");
  }
  else
  {
    setStyleSheet("");
  }
}


} } } // namespaces

