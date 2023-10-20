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
 @file WareSrcFileEditor.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Armel THÖNI <armel.thoni@inrae.fr>
 */


// OpenFLUID:stylecheck:!brac-oneline


#include <QDir>
#include <QPainter>
#include <QTextStream>
#include <QMenu>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QToolTip>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/waresdev/WareSrcEnquirer.hpp>
#include <openfluid/ui/waresdev/WareSrcSyntaxHighlighter.hpp>
#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>
#include <openfluid/global.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcFileEditor::WareSrcFileEditor(const QString& FilePath, QWidget* Parent) :
    QPlainTextEdit(Parent), WareFileEditor(FilePath)
{
  m_SelectionTagsRegExp.setPattern("%%SEL_(START|END)%%");
  m_AllTagsRegExp.setPattern("%%(SEL_(START|END)|CURSOR|INDENT)%%");
  // TODO get "word part" characters from scanning xml completion strings ?
  m_WordPartRegExp.setPattern("\\w|:|#");

  // Line number management

  mp_LineNumberArea = new LineNumberArea(this);
  updateLineNumberAreaWidth();

  connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
  connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

  updateSettings();

  // Code insertion and code completion management

  QString ProgLang = openfluid::ui::waresdev::WareSrcFiletypeManager::instance()->getFileLanguage(FilePath);
  openfluid::ware::WareType WType =
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(FilePath.toStdString()).WareType;

  m_CompletionRules = openfluid::ui::waresdev::CompletionProvider::instance()->getRules(ProgLang,WType);

  mp_SignalMapper = new QSignalMapper(this);

  QStandardItemModel* CompletionModel = new QStandardItemModel();

  int CompRow = 0;
  for (auto& RuleItem : m_CompletionRules)
  {
    // initialize actions for code insertion

    QString IconPath;

    if (!RuleItem.Title.isEmpty())
    {
      QString MenuEntry = tr("Other");

      if (RuleItem.Orig == openfluid::ui::waresdev::CompletionProvider::Origin::OPENFLUID)
      {
        MenuEntry = "OpenFLUID";
        IconPath =  ":/ui/common/icons/OFcode.png";
      }
      else if (RuleItem.Orig == openfluid::ui::waresdev::CompletionProvider::Origin::CPP)
      {
        MenuEntry = "C++";
        IconPath =  ":/ui/common/icons/cppcode.png";
      }

      if (!m_InsertionMenus.contains(MenuEntry))
      {
        m_InsertionMenus[MenuEntry] = new QMenu(MenuEntry);
        m_InsertionMenus[MenuEntry]->setObjectName(MenuEntry);
      }

      QMenu* Menu = m_InsertionMenus[MenuEntry];

      for (int i = 0; i < RuleItem.MenuPath.size(); i++)
      {
        QMenu* SubMenu = Menu->findChild<QMenu*>(Menu->objectName()+"/"+RuleItem.MenuPath[i]);
        if (!SubMenu)
        {
          SubMenu = Menu->addMenu(RuleItem.MenuPath[i]);
          SubMenu->setObjectName(Menu->objectName()+"/"+RuleItem.MenuPath[i]);
        }
        Menu = SubMenu;
      }

      QAction* MenuAction = new openfluid::ui::common::DefaultAction(RuleItem.Title, this);
      mp_SignalMapper->setMapping(MenuAction, RuleItem.Content);
      connect(MenuAction, SIGNAL(triggered()), mp_SignalMapper, SLOT(map()));
      Menu->addAction(MenuAction);
    }

    // initialize model for code completion

    QString ContentWoTags = RuleItem.Content;
    ContentWoTags.remove(m_AllTagsRegExp);
    QStringList L = ContentWoTags.split("\n");
    QString ContentForList, ContentForDetail;

    if (L.size() > 1)
    {
      ContentForList = L[0].append("...");
      ContentForDetail = ContentWoTags;
    }
    else
    {
      ContentForList = ContentWoTags;
      ContentForDetail = "";
    }


    CompletionModel->setItem(CompRow, 0, new QStandardItem(ContentForList));
    // setItem(...QIcon()...) doesn't allow to resize icon
    if (!IconPath.isEmpty())
    {
      QPixmap PM = QPixmap::fromImage(QImage(IconPath));
      CompletionModel->setData(CompletionModel->index(CompRow, 0), PM.scaled(QSize(16, 16), Qt::KeepAspectRatio),
                               Qt::DecorationRole);
    }
    CompletionModel->setItem(CompRow, 1, new QStandardItem(ContentForDetail));
    CompletionModel->setItem(CompRow, 2, new QStandardItem(RuleItem.Content));

    CompRow++;
  }

  QSortFilterProxyModel* SortedCompletionModel = new QSortFilterProxyModel();
  SortedCompletionModel->setSourceModel(CompletionModel);
  SortedCompletionModel->setSortCaseSensitivity(Qt::CaseInsensitive);
  SortedCompletionModel->sort(0);

  mp_Completer = new QCompleter(this);
  mp_Completer->setModel(SortedCompletionModel);
  mp_Completer->setCaseSensitivity(Qt::CaseInsensitive);
  mp_Completer->setCompletionMode(QCompleter::PopupCompletion);
  mp_Completer->setCompletionColumn(0);
  mp_Completer->setWidget(this);


  connect(mp_Completer->popup()->selectionModel(), SIGNAL(currentRowChanged (const QModelIndex&, const QModelIndex&)),
          this, SLOT(onCompletionPopupCurrentRowChanged(const QModelIndex&, const QModelIndex&)));

  connect(mp_Completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion()));

  connect(mp_SignalMapper, SIGNAL(mapped(QString)), this, SLOT(onInsertRequested(QString)));


  updateContent();

  connect(document(), SIGNAL(modificationChanged(bool)), this, SLOT(onChanged(bool)));
}


// =====================================================================
// =====================================================================


WareSrcFileEditor::~WareSrcFileEditor()
{
  delete mp_SyntaxHighlighter;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateSettings()
{
  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();

  // Current line highlighting

  QString CurrentLineColor = QString::fromStdString(PrefMgr->getWaresdevCurrentlineColor());

  if (PrefMgr->isWaresdevCurrentlineHighlightingEnabled() && QColor::isValidColor(CurrentLineColor))
  {
    m_LineColor.setNamedColor(CurrentLineColor);
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    highlightCurrentLine();
  }
  else
  {
    disconnect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    setExtraSelections(QList<QTextEdit::ExtraSelection>());
  }

  // Syntax highlighting

  if (PrefMgr->isWaresdevSyntaxHighlightingEnabled())
  {
    delete mp_SyntaxHighlighter;
    mp_SyntaxHighlighter = new WareSrcSyntaxHighlighter(
        document(), WareSrcFiletypeManager::instance()->getHighlightingRules(m_FilePath));
  }
  else
  {
    delete mp_SyntaxHighlighter;
    mp_SyntaxHighlighter = nullptr;
  }

  // Text options

  QTextOption Option = document()->defaultTextOption();

  //   Line wrapping
  Option.setWrapMode(PrefMgr->isWaresdevLineWrappingEnabled() ? QTextOption::WordWrap : QTextOption::NoWrap);

  //   Invisible characters
  QFlags<QTextOption::Flag> CurrentTextFlags = Option.flags();
  CurrentTextFlags.setFlag(QTextOption::ShowTabsAndSpaces, PrefMgr->isWaresdevSpaceTabDisplayEnabled());
  CurrentTextFlags.setFlag(QTextOption::ShowLineAndParagraphSeparators, 
                           PrefMgr->isWaresdevCarriageReturnDisplayEnabled());
  Option.setFlags(CurrentTextFlags);

  document()->setDefaultTextOption(Option);

  // Fonts

  QFont Font;
  Font.setFamily(QString::fromStdString(PrefMgr->getWaresdevFontName()));
  Font.setFixedPitch(true);
  Font.setPointSize(10);
  setFont(Font);

  // Indent

#if (QT_VERSION_MAJOR < 6)
  m_SpaceCharWidth = fontMetrics().width(' ');
#else
  m_SpaceCharWidth = fontMetrics().horizontalAdvance(' ');
#endif

  m_IndentString = QString(" ").repeated(PrefMgr->getWaresdevIndentSpaceNb());
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::keyPressEvent(QKeyEvent* Event)
{
  int Key = Event->key();

  if (mp_Completer->popup()->isVisible())
  {
    switch (Key)
    {
      case Qt::Key_Enter:
      case Qt::Key_Return:
      case Qt::Key_Escape:
      case Qt::Key_Tab:
      case Qt::Key_Backtab:
        Event->ignore();
        return;
      default:
        break;
    }
  }


  // Adaptation of the completion shortcut according to the OS
#if defined(OPENFLUID_OS_MAC)
  // On mac OS Cmd+Space is usually dedicated to Spotlight, so we use Ctrl modifier (Meta in Qt) instead
  // ! 8239 == 0x202F == "narrow no-break space", may happen as a key with Ctrl modifier
  bool IsShortcut = ((Event->modifiers() & Qt::MetaModifier) && (Key == Qt::Key_Space || Key == 8239));
#else
  // ! 8239 == 0x202F == "narrow no-break space", may happen as a key with Ctrl modifier
  bool IsShortcut = ((Event->modifiers() & Qt::ControlModifier) && (Key == Qt::Key_Space || Key == 8239));
#endif

  if (!IsShortcut)
  {
    if (Key == Qt::Key_Return || Key == Qt::Key_Enter)
    {
      insertNewLine();
    }
    else if (Key == Qt::Key_Tab)
    {
      insertPlainText(m_IndentString);
    }
    else if ((Event->modifiers() & Qt::ControlModifier) && Key == Qt::Key_I)
    {
      return;
    }
    else
    {
      QPlainTextEdit::keyPressEvent(Event);
    }
  }

  // to skip if only Ctrl or Shift alone
  const bool CtrlOrShift = Event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
  if (CtrlOrShift && Event->text().isEmpty())
  {
    return;
  }

  QTextCursor TestCursor = textCursor();
  int TestPosition = TestCursor.position();
  QString TestSelection;
  bool RemoveFirstChar = true;

  do
  {
    TestPosition--;
    if (TestPosition < 0)
    {
      RemoveFirstChar = false;
      break;
    }

    TestCursor.setPosition(TestPosition, QTextCursor::KeepAnchor);
    TestSelection = TestCursor.selectedText();
  }
#if (QT_VERSION_MAJOR < 6)
  while (m_WordPartRegExp.exactMatch(TestSelection.left(1)));
#else
  while (m_WordPartRegExp.match(TestSelection.left(1)).hasMatch());
#endif  

  if (RemoveFirstChar)
  {
    TestSelection.remove(0, 1);
  }

  QString CompletionPrefix = TestSelection;

  if (IsShortcut || (mp_Completer->popup()->isVisible() && !CompletionPrefix.isEmpty()
                     && CompletionPrefix != mp_Completer->completionPrefix()))
  {
    mp_Completer->setCompletionPrefix(CompletionPrefix);

    QRect cr = cursorRect();
    cr.setWidth(
        mp_Completer->popup()->sizeHintForColumn(0) + mp_Completer->popup()->verticalScrollBar()->sizeHint().width());

    mp_Completer->complete(cr);

    // to do after the call of complete() for the signal "currentRowChanged" to be sent
    mp_Completer->popup()->setCurrentIndex(mp_Completer->completionModel()->index(0, 0));

    return;
  }

  mp_Completer->popup()->hide();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::insertCompletion()
{
  QTextCursor Cursor = textCursor();

  Cursor.setPosition(Cursor.position() - mp_Completer->completionPrefix().size(), QTextCursor::KeepAnchor);
  Cursor.removeSelectedText();

  onInsertRequested(
      mp_Completer->completionModel()->data(
          mp_Completer->completionModel()->index(mp_Completer->popup()->currentIndex().row(), 2)).toString());
}


// =====================================================================
// =====================================================================


int WareSrcFileEditor::lineNumberAreaWidth()
{
  int Digits = 1;
  int Max = qMax(1, blockCount());
  while (Max >= 10)
  {
    Max /= 10;
    ++Digits;
  }

#if (QT_VERSION_MAJOR < 6)
  int Space = 3 + fontMetrics().width(QLatin1Char('9')) * Digits;
#else
  int Space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * Digits;
#endif

  return Space;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberAreaWidth()
{
  setViewportMargins(lineNumberAreaWidth() + 3, 0, 0, 0);

  updateLineNumberArea();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberArea(const QRect& Rect, int Dy)
{
  if (Dy)
  {
    mp_LineNumberArea->scroll(0, Dy);
  }
  else
  {
    mp_LineNumberArea->update(0, Rect.y(), mp_LineNumberArea->width(), Rect.height());
  }

  if (Rect.contains(viewport()->rect()))
  {
    updateLineNumberAreaWidth();
  }
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::resizeEvent(QResizeEvent* Event)
{
  QPlainTextEdit::resizeEvent(Event);

  QRect CR = contentsRect();
  mp_LineNumberArea->setGeometry(QRect(CR.left(), CR.top(), lineNumberAreaWidth(), CR.height()));
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::contextMenuEvent(QContextMenuEvent* Event)
{
  QMenu* Menu = createStandardContextMenu();
  
  Menu->addSeparator();

  for (QMenu* SubMenu : m_InsertionMenus)
  {
    Menu->addMenu(SubMenu);
  }

  Menu->exec(Event->globalPos());

  delete Menu;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::onCompletionPopupCurrentRowChanged(const QModelIndex& Current, const QModelIndex& /*Previous*/)
{
  QString Str =
      mp_Completer->completionModel()->data(mp_Completer->completionModel()->index(Current.row(), 1)).toString();

  QToolTip::showText(mp_Completer->popup()->mapToGlobal(mp_Completer->popup()->rect().topRight()), Str);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::onInsertRequested(const QString& Str)
{
  int InitialIndentInSpaceNb = cursorRect().x() / m_SpaceCharWidth;

  QStringList L = Str.split("%%CURSOR%%");
  if (L.size() > 1)
  {
    writeString(L[0], InitialIndentInSpaceNb);
    int Pos = textCursor().position();
    writeString(L[1], InitialIndentInSpaceNb);

    QTextCursor Cursor = textCursor();
    Cursor.setPosition(Pos);
    setTextCursor(Cursor);
    return;
  }

  L = Str.split(m_SelectionTagsRegExp);
  if (L.size() > 2)
  {
    writeString(L[0], InitialIndentInSpaceNb);
    int PosStart = textCursor().position();
    writeString(L[1], InitialIndentInSpaceNb);
    int PosEnd = textCursor().position();
    writeString(L[2], InitialIndentInSpaceNb);

    QTextCursor Cursor = textCursor();
    Cursor.setPosition(PosStart);
    Cursor.setPosition(PosEnd, QTextCursor::KeepAnchor);
    setTextCursor(Cursor);
    return;
  }

  writeString(Str, InitialIndentInSpaceNb);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::writeString(const QString& Str, int InitialIndentInSpaceNb)
{
  QString ToInsert(Str);
  insertPlainText(ToInsert.replace("%%INDENT%%", QString(InitialIndentInSpaceNb, ' ')));
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> ExtraSelections;

  if (!isReadOnly())
  {
    QTextEdit::ExtraSelection Selection;

    Selection.format.setBackground(m_LineColor);
    Selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    Selection.cursor = textCursor();
    Selection.cursor.clearSelection();
    ExtraSelections.append(Selection);
  }

  setExtraSelections(ExtraSelections);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::lineNumberAreaPaintEvent(QPaintEvent* Event)
{
  QPainter Painter(mp_LineNumberArea);
  Painter.fillRect(Event->rect(), m_LineNbAreaColor);

  QTextBlock Block = firstVisibleBlock();
  int BlockNumber = Block.blockNumber();
  int Top = (int) blockBoundingGeometry(Block).translated(contentOffset()).top();
  int Bottom = Top + (int) blockBoundingRect(Block).height();

  while (Block.isValid() && Top <= Event->rect().bottom())
  {
    if (Block.isVisible() && Bottom >= Event->rect().top())
    {
      Painter.setPen(m_LineNbTextColor);

      QRect Rect(0, Top, mp_LineNumberArea->width(), fontMetrics().height());

      int Number = BlockNumber + 1;

      if (m_ShowLineMarkers)
      {
        QMap<QTextBlock, LineMarker>::iterator it = m_LineMarkersByBlock.find(Block);
        if (it != m_LineMarkersByBlock.end())
        {
          Painter.fillRect(Rect, it->getColor());
          it->m_Rect = Rect;
        }
      }

      Painter.drawText(Rect, Qt::AlignRight, QString::number(Number));
    }

    Block = Block.next();
    Top = Bottom;
    Bottom = Top + (int) blockBoundingRect(Block).height();
    ++BlockNumber;
  }
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::onChanged(bool Changed)
{
  emit editorTxtChanged(this, Changed);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::saveContent()
{
  saveContentToPath(m_FilePath);

  document()->setModified(false);

  emit editorSaved();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::saveContentToPath(const QString& Path)
{
  if (Path.isEmpty())
  {
    return;
  }

  if (!QDir().mkpath(QFileInfo(Path).absolutePath()))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Unable to create the path \"%1\"").arg(Path).toStdString());
  }

  QFile File(Path);

  if (!File.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    throw openfluid::base::FrameworkException(
        OPENFLUID_CODE_LOCATION, QString("Unable to open the file \"%1\" in write mode").arg(Path).toStdString());
  }

  QTextStream Str(&File);
  Str << toPlainText();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateContent()
{
  QFile File(m_FilePath);

  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              QString("Cannot open file %1").arg(m_FilePath).toStdString());
  }

  setPlainText(QTextStream(&File).readAll());

  document()->setModified(false);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::insertNewLine()
{
  QTextCursor Cursor = textCursor();
  bool AfterOpeningCurlyBrace = false;
  QString InitialIndent = "";
  QString StrToInsert = "\n";

  if (!Cursor.atBlockStart())
  {
    Cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
    AfterOpeningCurlyBrace = (Cursor.selectedText() == "{");

    Cursor.movePosition(QTextCursor::StartOfLine);

    do
    {
      Cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

      QString SelectedText = Cursor.selectedText();
      if (!SelectedText.endsWith(' ') && !SelectedText.endsWith('\t'))
      {
        Cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
        break;
      }
    }
    while (!Cursor.atBlockEnd());

    int InitialIndentInSpaceNb = Cursor.verticalMovementX() / m_SpaceCharWidth;
    InitialIndent = QString(InitialIndentInSpaceNb, ' ');
    StrToInsert.append(InitialIndent);

    if (AfterOpeningCurlyBrace)
    {
      StrToInsert.append(m_IndentString);
    }
  }

  insertPlainText(StrToInsert);

  int NewPos = textCursor().position();
  if (AfterOpeningCurlyBrace)
  {
    insertPlainText(QString("\n%1}").arg(InitialIndent));
    Cursor = textCursor();
    Cursor.setPosition(NewPos);
    setTextCursor(Cursor);
  }
}


// =====================================================================
// =====================================================================


bool WareSrcFileEditor::findReplace(FindReplaceDialog::FindReplaceAction Action, const QString& StringToFind,
  const QString& StringForReplace, QTextDocument::FindFlags Options, QString& Message)
{
  switch (Action)
  {
    case FindReplaceDialog::FindReplaceAction::FindOnly:
      if (findString(StringToFind, Options))
      {
        Message = "";
        return true;
      }
      break;
    case FindReplaceDialog::FindReplaceAction::ReplaceOnly:
      if (replaceString(StringToFind, StringForReplace,
                        Options & QTextDocument::FindCaseSensitively ? Qt::CaseSensitive : Qt::CaseInsensitive))
      {
        Message = "Replaced";
        return false;
      }
      break;
    case FindReplaceDialog::FindReplaceAction::ReplaceFind:
      if (replaceString(StringToFind, StringForReplace,
                        Options & QTextDocument::FindCaseSensitively ? Qt::CaseSensitive : Qt::CaseInsensitive))
      {
        Message = "Replaced";
        return findString(StringToFind, Options);
      }
      break;
    case FindReplaceDialog::FindReplaceAction::ReplaceAll:
      {
      QTextCursor Cursor = textCursor();
      Cursor.movePosition((Options & QTextDocument::FindBackward) ? QTextCursor::End : QTextCursor::Start);
      setTextCursor(Cursor);

      int ReplaceCount = 0;

      if (findString(StringToFind, Options))
      {
        while (findReplace(FindReplaceDialog::FindReplaceAction::ReplaceFind, StringToFind, StringForReplace, Options,
                           Message))
        {
          ReplaceCount++;
        }
      }

      if (ReplaceCount > 0)
      {
        Message = tr("%1 matches replaced").arg(ReplaceCount);
      }

      return true;
    }
      break;
    default:
      break;
  }

  Message = tr("String not found");
  return false;
}


// =====================================================================
// =====================================================================


bool WareSrcFileEditor::findString(const QString& StringToFind, QTextDocument::FindFlags Options)
{
  QTextCursor Cursor = document()->find(StringToFind, textCursor(), Options);

  if (!Cursor.isNull())
  {
    setTextCursor(Cursor);
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


bool WareSrcFileEditor::replaceString(const QString& StringToFind, const QString& StringForReplace,
  Qt::CaseSensitivity Cs)
{
  QTextCursor Cursor = textCursor();

  if (Cursor.selectedText().compare(StringToFind, Cs) == 0)
  {
    int From = Cursor.anchor();
    Cursor.insertText(StringForReplace);
    Cursor.setPosition(From, QTextCursor::KeepAnchor);
    setTextCursor(Cursor);

    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


QString WareSrcFileEditor::getSelectedText()
{
  return textCursor().selectedText();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::clearLineMessages()
{
  m_LineMarkersByBlock.clear();
  setShowLineMarkers(true);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::addLineMessage(WareSrcMsgParser::WareSrcMsg Message)
{
  QTextBlock Block = document()->findBlockByNumber(Message.m_LineNb - 1);

  if (Block.isValid())
  {
    QMap<QTextBlock, LineMarker>::iterator it = m_LineMarkersByBlock.find(Block);

    if (it != m_LineMarkersByBlock.end())
    {
      it->update(Message.m_Type, Message.m_Content);
    }
    else
    {
      m_LineMarkersByBlock.insert(Block, LineMarker(Message.m_Type, Message.m_Content));
    }
  }
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberArea()
{
  mp_LineNumberArea->update();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::tooltipEvent(const QPoint& Position)
{
  if (!m_ShowLineMarkers)
  {
    mp_LineNumberArea->setToolTip("");
    return;
  }

  for (LineMarker Marker : m_LineMarkersByBlock.values())
  {
    if (Marker.m_Rect.contains(Position))
    {
      QString MarkerContent = Marker.getContent();
      if (!MarkerContent.isEmpty())
      {
        mp_LineNumberArea->setToolTip(MarkerContent);
      }
      return;
    }
  }

  mp_LineNumberArea->setToolTip("");
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::selectLine(int LineNumber)
{
  QTextCursor Cursor = textCursor();
  Cursor.setPosition(document()->findBlockByNumber(LineNumber - 1).position());
  setTextCursor(Cursor);

  highlightCurrentLine();

  centerCursor();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::setShowLineMarkers(bool ShowMarkers)
{
  m_ShowLineMarkers = ShowMarkers;
  updateLineNumberArea();
}


// =====================================================================
// =====================================================================


bool WareSrcFileEditor::getShowLineMarkers()
{
  return m_ShowLineMarkers;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::goToLine()
{
  bool Ok;

  QInputDialog Dialog(this);
  int Nb = QInputDialog::getInt(this, tr("Go to line"), tr("Enter the line number:"),
                                        1,1,2147483647,1, &Ok);
  if (!Ok)
  {
    return;
  }

  if (Nb < document()->lineCount())
  {
    selectLine(Nb);
  }
}


// =====================================================================
// =====================================================================


bool WareSrcFileEditor::isModified()
{
  return document()->isModified();
}


// =====================================================================
// =====================================================================


QWidget* WareSrcFileEditor::getWidget()
{
  return this;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::copy()
{
  QPlainTextEdit::copy();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::cut()
{
  QPlainTextEdit::cut();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::paste()
{
  QPlainTextEdit::paste();
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::setFocus()
{
  QPlainTextEdit::setFocus();
}


} } } // namespaces
