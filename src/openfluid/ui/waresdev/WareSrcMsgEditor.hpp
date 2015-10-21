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
 @file WareSrcMsgEditor.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WARESRCMSGEDITOR_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCMSGEDITOR_HPP__

#include <openfluid/dllexport.hpp>

#include <QPlainTextEdit>

#include <openfluid/waresdev/WareSrcMsgParser.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareSrcMsgEditor: public QPlainTextEdit
{
  Q_OBJECT

  private:

    QMap<openfluid::waresdev::WareSrcMsgParser::WareSrcMsg::MessageType, QTextCharFormat> m_FormatByMsgType;

    QMap<int, openfluid::waresdev::WareSrcMsgParser::WareSrcMsg> m_MessagesByBlockNumber;

  public:

    WareSrcMsgEditor(QWidget* Parent = nullptr);

    ~WareSrcMsgEditor();

    void clearMessages();

    void writeMessage(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg& Msg);

  protected:

    void mouseDoubleClickEvent(QMouseEvent* Event);

  signals:

    void messageClicked(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg& Msg);
};


} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESRCMSGEDITOR_HPP__ */
