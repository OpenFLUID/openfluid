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
 @file WareSrcFileEditor.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __OPENFLUID_UIWARESDEV_WARESRCFILEEDITOR_HPP__
#define __OPENFLUID_UIWARESDEV_WARESRCFILEEDITOR_HPP__

#include <openfluid/dllexport.hpp>

#include <QPlainTextEdit>
#include <QCompleter>
#include <QSignalMapper>
#include <QStandardItemModel>

#include <openfluid/ui/waresdev/WareSrcFiletypeManager.hpp>


namespace openfluid { namespace ui { namespace waresdev {

class WareSrcSyntaxHighlighter;

class OPENFLUID_API WareSrcFileEditor: public QPlainTextEdit
{
  Q_OBJECT

  private:

    QString m_FilePath;

    WareSrcSyntaxHighlighter* mp_SyntaxHighlighter;

    QRegExp m_SelectionTagsRegExp;
    QRegExp m_AllTagsRegExp;
    QRegExp m_WordPartRegExp;

    QWidget* mp_LineNumberArea;

    QColor m_LineColor;

    int m_SpaceCharWidth;

    WareSrcFiletypeManager::CompletionRules_t m_CompletionRules;

    QSignalMapper* mp_SignalMapper;

    QStandardItemModel* mp_CompletionModel;

    QMap<QString, QMenu*> m_InsertionMenus;

    QCompleter* mp_Completer;

    void writeString(const QString& Str, int InitialIndentInSpaceNb);

  private slots:

    void updateLineNumberAreaWidth(int NewBlockCount);

    void highlightCurrentLine();

    void updateLineNumberArea(const QRect& Rect, int);

    void onChanged(bool Changed);

    void onInsertRequested(const QString& Str);

    void insertCompletion();

    void onCompletionPopupCurrentRowChanged(const QModelIndex &Current, const QModelIndex& Previous);

  protected:

    void resizeEvent(QResizeEvent* Event);

    void contextMenuEvent(QContextMenuEvent* Event);

    void keyPressEvent(QKeyEvent* Event);

  public:

    WareSrcFileEditor(const QString& FilePath, QWidget* Parent = 0);

    ~WareSrcFileEditor();

    void lineNumberAreaPaintEvent(QPaintEvent* Event);

    int lineNumberAreaWidth();

    QString getFilePath();

    void saveContent();

    void saveContentToPath(const QString& Path);

    void updateContent();

  signals :

    void editorTxtChanged(WareSrcFileEditor* Editor, bool Changed);

    void editorSaved();

};


// =====================================================================
// =====================================================================


class LineNumberArea: public QWidget
{
  private:

    WareSrcFileEditor* mp_Editor;

  protected:

    void paintEvent(QPaintEvent* Event)
    {
      mp_Editor->lineNumberAreaPaintEvent(Event);
    }

  public:

    LineNumberArea(WareSrcFileEditor* Editor) :
        QWidget(Editor)
    {
      mp_Editor = Editor;
    }

    QSize sizeHint() const
    {
      return QSize(mp_Editor->lineNumberAreaWidth(), 0);
    }
};


} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_WARESRCFILEEDITOR_HPP__ */
