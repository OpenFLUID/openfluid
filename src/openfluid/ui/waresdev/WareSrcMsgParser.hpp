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
  @file WareSrcMsgParser.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UIWARESDEV_WARESRCMSGPARSER_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCMSGPARSER_HPP__


#ifndef QT_VERSION_MAJOR
#pragma message "Qt version not found in source"
#else
#pragma message "Qt version found in source"
#endif

#if (QT_VERSION_MAJOR < 6)
#  pragma message("QT_VERSION_MAJOR < 6")


#  include <QRegExp>
#else
#  pragma message("QT_VERSION_MAJOR >= 6?")


#  include <QRegularExpression>  
#endif
#include <QDir>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace ui { namespace waresdev {

const int a = 0;
class OPENFLUID_API WareSrcMsgParser
{
  public:

    class WareSrcMsg
    {
      public:

        enum class MessageType
        {
          MSG_COMMAND, MSG_STANDARD, MSG_WARNING, MSG_ERROR,
        };

        QByteArray m_OriginalMsgLine;
        MessageType m_Type;
        QString m_Path;
        int m_LineNb = -1;
        int m_ColNb = -1;
        QString m_Content;

        WareSrcMsg(const QString& MessageLine, MessageType MsgType = MessageType::MSG_STANDARD) :
            m_Type(MsgType)
        {
          m_OriginalMsgLine = MessageLine.toUtf8();
        }
    };

    virtual ~WareSrcMsgParser()
    { }

    virtual WareSrcMsg parse(const QString& MessageLine,
                             WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType) = 0;
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcMsgParserNone: public WareSrcMsgParser
{

  public:

    WareSrcMsgParser::WareSrcMsg parse(const QString& MessageLine,
                                       WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType);
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcMsgParserGcc: public WareSrcMsgParser
{
  private:

#if (QT_VERSION_MAJOR < 6)
    QRegExp m_GccMsgParseRx = QRegExp("^(.+):(\\d+):(\\d+): *(warning|error|note): *(.+)");
#else
    QRegularExpression m_GccMsgParseRx = QRegularExpression("^(.+):(\\d+):(\\d+): *(warning|error|note): *(.+)");
#endif

  public:

    WareSrcMsgParser::WareSrcMsg parse(const QString& MessageLine,
                                       WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType);
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcMsgParserCMake: public WareSrcMsgParser
{
  private:

#if (QT_VERSION_MAJOR < 6)
    QRegExp m_CMakeMsgParseRx = QRegExp("^CMake Error at (.+):(\\d+) *(.+)");
#else
    QRegularExpression m_CMakeMsgParseRx = QRegularExpression("^CMake Error at (.+):(\\d+) *(.+)");
#endif

    QDir m_AbsoluteDir;

  public:

    WareSrcMsgParserCMake(const QString& AbsolutePath);

    WareSrcMsgParser::WareSrcMsg parse(const QString& MessageLine,
                                       WareSrcMsgParser::WareSrcMsg::MessageType DefaultMsgType);
};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESRCMSGPARSER_HPP__ */
