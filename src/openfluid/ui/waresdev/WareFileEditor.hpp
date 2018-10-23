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
 @file WareFileEditor.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_WAREFILEEDITOR_HPP__
#define __OPENFLUID_UIWARESDEV_WAREFILEEDITOR_HPP__


#include <QString>

#include <openfluid/dllexport.hpp>
#include <openfluid/waresdev/WareSrcMsgParser.hpp>
#include <openfluid/ui/waresdev/FindReplaceDialog.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareFileEditor
{
  protected:

    QString m_FilePath;


  public:

    explicit WareFileEditor(const QString& FilePath);

    virtual ~WareFileEditor();

    QString getFilePath();

    virtual void saveContent() = 0;

    virtual void saveContentToPath(const QString& Path) = 0;

    virtual void updateContent() = 0;

    virtual bool findReplace(FindReplaceDialog::FindReplaceAction Action, const QString& StringToFind,
      const QString& StringForReplace, QTextDocument::FindFlags Options, QString& Message) = 0;

    virtual QString getSelectedText() = 0;

    virtual void clearLineMessages() = 0;

    virtual void addLineMessage(openfluid::waresdev::WareSrcMsgParser::WareSrcMsg Message) = 0;

    virtual void updateLineNumberArea() = 0;

    virtual bool isModified() = 0;

    virtual QWidget* getWidget() = 0;

    virtual void copy() = 0;

    virtual void cut() = 0;

    virtual void paste() = 0;

    virtual void selectLine(int LineNumber) = 0;

    virtual void goToLine() = 0;

    virtual void updateSettings() = 0;

    virtual void setFocus() = 0;

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WAREFILEEDITOR_HPP__ */
