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
 @file WareSrcMsgViewer.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcMsgViewer.hpp>

#include <QTextBlock>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcMsgViewer::WareSrcMsgViewer(QWidget* Parent) :
    QPlainTextEdit(Parent)
{
  m_FormatByMsgType[openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND].setForeground(
      QColor("blue"));
  m_FormatByMsgType[openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_STANDARD].setForeground(
      QColor("black"));
  m_FormatByMsgType[openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_WARNING].setForeground(
      QColor("orange"));
  m_FormatByMsgType[openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType::MSG_ERROR].setForeground(
      QColor("red"));
}


// =====================================================================
// =====================================================================


WareSrcMsgViewer::~WareSrcMsgViewer()
{

}


// =====================================================================
// =====================================================================


void WareSrcMsgViewer::clearMessages()
{
  clear();

  ensureCursorVisible();

  m_MessagesByBlockNumber.clear();
}


// =====================================================================
// =====================================================================


void WareSrcMsgViewer::writeMessage(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg& Msg)
{
  QTextCursor Cursor = textCursor();

  Cursor.setCharFormat(m_FormatByMsgType.value(Msg.m_Type, QTextCharFormat()));

  if (Msg.m_LineNb > 0)
    m_MessagesByBlockNumber.insert(Cursor.blockNumber(), Msg);

  Cursor.insertText(QString::fromUtf8(Msg.m_OriginalMsgLine));

  ensureCursorVisible();
}


// =====================================================================
// =====================================================================


void WareSrcMsgViewer::mouseDoubleClickEvent(QMouseEvent* /*Event*/)
{
  QMap<int, openfluid::waresdev::WareSrcMsgParser::WareSrcMsg>::iterator it = m_MessagesByBlockNumber.find(
      textCursor().blockNumber());

  if (it != m_MessagesByBlockNumber.end())
    emit messageClicked(it.value());
}


// =====================================================================
// =====================================================================


} } } // namespaces
