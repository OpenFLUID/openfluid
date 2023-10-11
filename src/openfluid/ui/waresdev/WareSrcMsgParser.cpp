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
  @file WareSrcMsgParser.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/ui/waresdev/WareSrcMsgParser.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcMsgParser::WareSrcMsg WareSrcMsgParserNone::parse(const QString& MessageLine,
                                                         WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType)
{
  WareSrcMsgParser::WareSrcMsg Msg(MessageLine);

  Msg.m_Type = DefaultMsgType;

  return Msg;
}


// =====================================================================
// =====================================================================


WareSrcMsgParser::WareSrcMsg WareSrcMsgParserGcc::parse(const QString& MessageLine,
                                                        WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType)
{
  WareSrcMsgParser::WareSrcMsg Msg(MessageLine);

  Msg.m_Type = DefaultMsgType;

#if (QT_VERSION_MAJOR < 6)
  if (m_GccMsgParseRx.indexIn(MessageLine) != -1)
  {
    Msg.m_Path = QDir::fromNativeSeparators(m_GccMsgParseRx.cap(1));
    Msg.m_LineNb = m_GccMsgParseRx.cap(2).toInt();
    Msg.m_ColNb = m_GccMsgParseRx.cap(3).toInt();
    Msg.m_Type = (m_GccMsgParseRx.cap(4) == "warning" || m_GccMsgParseRx.cap(4) == "note") ?
                 WareSrcMsg::MessageType::MSG_WARNING : WareSrcMsg::MessageType::MSG_ERROR;
    Msg.m_Content = m_GccMsgParseRx.cap(5);
  }
#else
  const auto& Match = m_GccMsgParseRx.match(MessageLine);
  if (Match.hasMatch())
  {
    Msg.m_Path = QDir::fromNativeSeparators(Match.captured(1));
    Msg.m_LineNb = Match.captured(2).toInt();
    Msg.m_ColNb = Match.captured(3).toInt();
    Msg.m_Type = (Match.captured(4) == "warning" || Match.captured(4) == "note") ?
                 WareSrcMsg::MessageType::MSG_WARNING : WareSrcMsg::MessageType::MSG_ERROR;
    Msg.m_Content = Match.captured(5);
  }
#endif

  return Msg;
}


// =====================================================================
// =====================================================================


WareSrcMsgParserCMake::WareSrcMsgParserCMake(const QString& AbsolutePath) :
    WareSrcMsgParser(), m_AbsoluteDir(QDir(AbsolutePath))
{

}


// =====================================================================
// =====================================================================


WareSrcMsgParser::WareSrcMsg WareSrcMsgParserCMake::parse(const QString& MessageLine,
  WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType)
{
  WareSrcMsgParser::WareSrcMsg Msg(MessageLine);

  Msg.m_Type = DefaultMsgType;

#if (QT_VERSION_MAJOR < 6)
  if (m_CMakeMsgParseRx.indexIn(MessageLine) != -1)
  {
    Msg.m_Path = QDir::fromNativeSeparators(m_AbsoluteDir.absoluteFilePath(m_CMakeMsgParseRx.cap(1)));
    Msg.m_LineNb = m_CMakeMsgParseRx.cap(2).toInt();
    Msg.m_Type = WareSrcMsg::MessageType::MSG_ERROR;
    Msg.m_Content = m_CMakeMsgParseRx.cap(4);
  }
#else
  const auto& Match = m_CMakeMsgParseRx.match(MessageLine);
  if (Match.hasMatch())
  {
    Msg.m_Path = QDir::fromNativeSeparators(m_AbsoluteDir.absoluteFilePath(Match.captured(1)));
    Msg.m_LineNb = Match.captured(2).toInt();
    Msg.m_Type = WareSrcMsg::MessageType::MSG_ERROR;
    Msg.m_Content = Match.captured(4);
  }
#endif

  return Msg;
}


} } } // namespaces
