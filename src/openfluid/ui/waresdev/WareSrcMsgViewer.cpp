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
 @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QTextBlock>

#include <openfluid/ui/waresdev/WareSrcMsgViewer.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcMsgViewer::WareSrcMsgViewer(QWidget* Parent) :
    QPlainTextEdit(Parent)
{
  m_FormatByMsgType[WareSrcMsgParser::WareSrcMsg::MessageType::MSG_COMMAND].setForeground(QColor("blue"));
  m_FormatByMsgType[WareSrcMsgParser::WareSrcMsg::MessageType::MSG_STANDARD].setForeground(QColor("black"));
  m_FormatByMsgType[WareSrcMsgParser::WareSrcMsg::MessageType::MSG_WARNING].setForeground(QColor("orange"));
  m_FormatByMsgType[WareSrcMsgParser::WareSrcMsg::MessageType::MSG_ERROR].setForeground(QColor("red"));
  m_FormatByMsgType[WareSrcMsgParser::WareSrcMsg::MessageType::MSG_SUCCESS].setForeground(QColor("green"));
  m_FormatByMsgType[WareSrcMsgParser::WareSrcMsg::MessageType::MSG_SUCCESS].setFontWeight(QFont::Bold);
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


void WareSrcMsgViewer::writeMessage(const WareSrcMsgParser::WareSrcMsg& Msg)
{
  QTextCursor Cursor = textCursor();

  Cursor.setCharFormat(m_FormatByMsgType.value(Msg.m_Type, QTextCharFormat()));

  if (Msg.m_LineNb > 0)
  {
    m_MessagesByBlockNumber.insert(Cursor.blockNumber(), Msg);
  }

  const auto& MsgLine = QString::fromUtf8(Msg.m_OriginalMsgLine);

  if (!MsgLine.contains('\033'))
  {
    Cursor.insertText(MsgLine);
  }
  else
  {
    bool LookingCode = false;
    QString Code = "";

    bool Flush = false;
    QString CurrentChars = "";

    for (const auto& C : MsgLine)
    {
      if (C == '\033')
      {
        LookingCode = true;
        Flush = true;
      }
      else if (LookingCode)
      {
        if (C == 'm')
        {
          auto CharFormat = Cursor.charFormat();

          bool ColorFound = false;
          QColor ColorString;
          bool WeightFound = false;
          QFont::Weight Weight = QFont::Normal;
          if (Code.contains("31"))
          {
            ColorString = QColor("red");
            ColorFound = true;
          }
          else if (Code.contains("32"))
          {
            ColorString = QColor("green");
            ColorFound = true;
          }
          else if (Code.contains("33"))
          {
            ColorString = QColor("orange");
            ColorFound = true;
          }
          else if (Code.contains("39"))
          {
            ColorString = QColor("black");
            ColorFound = true;
          } 
          else if (Code.contains("1"))
          {
            Weight = QFont::Bold;
            WeightFound = true;
          } 
          else if(Code.isEmpty() || Code.contains("0"))
          {
            ColorString = QColor("black");
            WeightFound = true;
            ColorFound = true;
          }
          if (ColorFound)
          {
            CharFormat.setForeground(ColorString);
          }
          if (WeightFound)
          {
            CharFormat.setFontWeight(Weight);
          }
          Cursor.setCharFormat(CharFormat);
          LookingCode = false;
          Code = "";
        }
        else
        {
          Code += C;
        }
      }
      else
      {
        CurrentChars += C;
      }
      if (Flush)
      {
        Cursor.insertText(CurrentChars);
        Flush = false;
        CurrentChars = "";
      }
    }
    Cursor.insertText(CurrentChars);
  }

  ensureCursorVisible();
}


// =====================================================================
// =====================================================================


void WareSrcMsgViewer::mouseDoubleClickEvent(QMouseEvent* /*Event*/)
{
  QMap<int, WareSrcMsgParser::WareSrcMsg>::iterator it = m_MessagesByBlockNumber.find(
      textCursor().blockNumber());

  if (it != m_MessagesByBlockNumber.end())
  {
    emit messageClicked(it.value());
  }
}


} } } // namespaces
