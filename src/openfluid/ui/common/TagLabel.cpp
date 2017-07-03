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
  @file TagLabel.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


#include <openfluid/ui/common/TagLabel.hpp>


namespace openfluid { namespace ui { namespace common {


TagLabel::TagLabel(QWidget* Parent) :
  QLabel(Parent)
{
  applyStyle();
}


// =====================================================================
// =====================================================================


TagLabel::TagLabel(const QString& Text, QWidget* Parent) :
    QLabel(Text,Parent)
{
  applyStyle();
}


// =====================================================================
// =====================================================================


TagLabel::TagLabel(const QString& Text,
                   const QString& CSSTextColor, const QString& CSSBackgroundColor,
                   QWidget* Parent) :
    QLabel(Text,Parent), m_CSSTextColor(CSSTextColor), m_CSSBackgroundColor(CSSBackgroundColor)
{
  applyStyle();
}


// =====================================================================
// =====================================================================


void TagLabel::applyStyle()
{

  if (m_Outlined)
  {
    setStyleSheet(QString("qproperty-alignment: AlignCenter; color : %1; border : 1px solid %1; "
                          "background-color : none ; border-radius: 4px; padding : 4px; font-size : %2pt;")
                          .arg(m_CSSBackgroundColor).arg(m_TextSize));
  }
  else
  {
    setStyleSheet(QString("qproperty-alignment: AlignCenter; color : %1; "
                          "background-color : %2 ; border-radius: 4px; padding : 4px; font-size : %3pt;")
                          .arg(m_CSSTextColor).arg(m_CSSBackgroundColor).arg(m_TextSize));
  }
}


// =====================================================================
// =====================================================================


void TagLabel::setTextColor(const QString& CSSColor)
{
  m_CSSTextColor = CSSColor;

  applyStyle();
}


// =====================================================================
// =====================================================================


void TagLabel::setBackgroundColor(const QString& CSSColor)
{
  m_CSSBackgroundColor = CSSColor;

  applyStyle();
}


// =====================================================================
// =====================================================================


void TagLabel::setTextSize(int Pt)
{
  m_TextSize = Pt;

  applyStyle();
}


// =====================================================================
// =====================================================================


void TagLabel::setOutlined(bool Enabled)
{
  m_Outlined = Enabled;

  applyStyle();
}


} } } // namespaces
