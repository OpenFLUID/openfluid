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
  @file ClickableLabel.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QMouseEvent>

#include <openfluid/ui/common/ClickableLabel.hpp>


namespace openfluid { namespace ui { namespace common {


ClickableLabel::ClickableLabel(QWidget* Parent) :
  QLabel(Parent)
{

}


// =====================================================================
// =====================================================================


ClickableLabel::ClickableLabel(const QString& Text, QWidget* Parent) :
  QLabel(Text,Parent)
{

}


// =====================================================================
// =====================================================================


void ClickableLabel::mouseReleaseEvent(QMouseEvent* /*Event*/)
{
  emit clicked();

  //QLabel::mouseReleaseEvent(Event);
}


// =====================================================================
// =====================================================================


void ClickableLabel::mouseDoubleClickEvent(QMouseEvent* /*Event*/)
{
  emit clicked();

  //QLabel::mouseDoubleClickEvent(Event);
}


// =====================================================================
// =====================================================================


void ClickableLabel::wheelEvent(QWheelEvent* Event)
{
  QPoint NumPixels = Event->pixelDelta();
  QPoint NumDegrees = Event->angleDelta() / 8;

  if (NumPixels.y() != 0)
  {
    emit scrolled(NumPixels.y() > 0);
  }
  else if (NumDegrees.y() !=0)
  {
    emit scrolled(NumDegrees.y() > 0);
  }
}


} } } // namespaces

