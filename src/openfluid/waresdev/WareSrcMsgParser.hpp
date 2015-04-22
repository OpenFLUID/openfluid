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
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCMSGPARSER_HPP__
#define __OPENFLUID_WARESDEV_WARESRCMSGPARSER_HPP__

#include <openfluid/dllexport.hpp>

#include <QRegExp>
#include <QDir>


namespace openfluid { namespace waresdev {


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcMsgParser
{
  public:

    class WareSrcMsg
    {
      public:

        enum MessageType
        {
          MSG_COMMAND, MSG_STANDARD, MSG_WARNING, MSG_ERROR,
        };

        QByteArray m_OriginalMsgLine;
        MessageType m_Type;
        QString m_Path;
        int m_LineNb = -1;
        int m_ColNb = -1;
        QString m_Content;

        WareSrcMsg(const QString& MessageLine, MessageType MsgType = MSG_STANDARD) :
            m_Type(MsgType)
        {
          m_OriginalMsgLine = MessageLine.toUtf8();
        }
    };

    virtual ~WareSrcMsgParser()
    {
    }
    virtual WareSrcMsg parse(const QString& MessageLine) = 0;
};


// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcMsgParserGcc: public WareSrcMsgParser
{
  private:

    QRegExp m_GccMsgParseRx = QRegExp("^(.+):(\\d+):(\\d+): *(warning|error): *(.+)");

  public:

    WareSrcMsgParser::WareSrcMsg parse(const QString& MessageLine);
};



// =====================================================================
// =====================================================================


class OPENFLUID_API WareSrcMsgParserCMake: public WareSrcMsgParser
{
  private:

    QRegExp m_CMakeMsgParseRx = QRegExp("^CMake Error at (.+):(\\d+) *(.+)");

    QDir m_AbsoluteDir;

  public:

    WareSrcMsgParserCMake(const QString& AbsolutePath);

    WareSrcMsgParser::WareSrcMsg parse(const QString& MessageLine);
};


// =====================================================================
// =====================================================================


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_WARESRCMSGPARSER_HPP__ */
