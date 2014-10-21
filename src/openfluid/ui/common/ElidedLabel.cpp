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
  \file ElidedLabel.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include "ElidedLabel.hpp"

#include <QPainter>
#include <QResizeEvent>


namespace openfluid { namespace ui { namespace common {


ElidedLabel::ElidedLabel(QWidget* Parent, Qt::WindowFlags Flags):
  QLabel(Parent, Flags), m_ElideMode(Qt::ElideMiddle)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}


// =====================================================================
// =====================================================================


ElidedLabel::ElidedLabel(const QString& Text, QWidget* Parent, Qt::WindowFlags Flags):
  QLabel(Text, Parent, Flags), m_ElideMode(Qt::ElideMiddle)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}


// =====================================================================
// =====================================================================


ElidedLabel::ElidedLabel(const QString& Text, Qt::TextElideMode ElideMode,
                         QWidget* Parent, Qt::WindowFlags Flags):
  QLabel(Text, Parent, Flags), m_ElideMode(ElideMode)
{
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}


// =====================================================================
// =====================================================================


void ElidedLabel::resizeEvent(QResizeEvent* Ev)
{
  QLabel::resizeEvent(Ev);
  cacheElidedText(Ev->size().width());
}


// =====================================================================
// =====================================================================


void ElidedLabel::paintEvent(QPaintEvent* Ev)
{
  if(m_ElideMode == Qt::ElideNone)
  {
    QLabel::paintEvent(Ev);
  }
  else
  {
    QPainter p(this);
    p.drawText(0, 0, geometry().width(), geometry().height(),
               alignment(),m_CachedElidedText);
  }
}


// =====================================================================
// =====================================================================


void ElidedLabel::cacheElidedText(int Width)
{
  m_CachedElidedText = fontMetrics().elidedText(text(), m_ElideMode, Width, Qt::TextShowMnemonic);
}


// =====================================================================
// =====================================================================


void ElidedLabel::setElideMode(Qt::TextElideMode ElideMode)
{
  m_ElideMode = ElideMode;
  updateGeometry();
}


// =====================================================================
// =====================================================================


void ElidedLabel::setText(const QString& Text)
{
  QLabel::setText(Text);
  cacheElidedText(geometry().width());

  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
}


} } } // namespaces

