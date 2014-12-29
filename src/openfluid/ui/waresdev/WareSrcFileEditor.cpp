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
 \file WareSrcFileEditor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>

#include <QDir>
#include <QPainter>
#include <QTextStream>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/waresdev/WareSrcSyntaxHighlighter.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcFileEditor::WareSrcFileEditor(const QString& FilePath, QWidget* Parent) :
    QPlainTextEdit(Parent)
{
  mp_lineNumberArea = new LineNumberArea(this);

  connect(this, SIGNAL(blockCountChanged(int)), this,
          SLOT(updateLineNumberAreaWidth(int)));
  connect(this, SIGNAL(updateRequest(QRect,int)), this,
          SLOT(updateLineNumberArea(QRect,int)));
  connect(this, SIGNAL(cursorPositionChanged()), this,
          SLOT(highlightCurrentLine()));

  updateLineNumberAreaWidth(0);
  highlightCurrentLine();

  QFile File(FilePath);
  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException(
        "WareSrcFileEditor constructor",
        QString("Cannot open file %1").arg(FilePath).toStdString());

  new WareSrcSyntaxHighlighter(
      document(),
      WareSrcFiletypeManager::getInstance()->getHighlightingRules(File));

  // TODO get defaults from conf file
  // setStyleSheet("fFont: 11pt \"Courier\";");
  QFont Font;
  Font.setFamily("Courier");
  Font.setFixedPitch(true);
  Font.setPointSize(11);
  setFont(Font);

  QTextStream In(&File);
  setPlainText(In.readAll());

  connect(document(), SIGNAL(modificationChanged ( bool )), this,
          SLOT(onChanged(bool)));

}


// =====================================================================
// =====================================================================


WareSrcFileEditor::~WareSrcFileEditor()
{

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

  int Space = 3 + fontMetrics().width(QLatin1Char('9')) * Digits;

  return Space;
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberAreaWidth(int /*NewBlockCount*/)
{
  setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::updateLineNumberArea(const QRect& Rect, int dy)
{
  if (dy)
    mp_lineNumberArea->scroll(0, dy);
  else
    mp_lineNumberArea->update(0, Rect.y(), mp_lineNumberArea->width(),
                              Rect.height());

  if (Rect.contains(viewport()->rect()))
    updateLineNumberAreaWidth(0);
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::resizeEvent(QResizeEvent* Event)
{
  QPlainTextEdit::resizeEvent(Event);

  QRect CR = contentsRect();
  mp_lineNumberArea->setGeometry(
      QRect(CR.left(), CR.top(), lineNumberAreaWidth(), CR.height()));
}


// =====================================================================
// =====================================================================


void WareSrcFileEditor::highlightCurrentLine()
{
  QList<QTextEdit::ExtraSelection> ExtraSelections;

  if (!isReadOnly())
  {
    QTextEdit::ExtraSelection Selection;

    QColor LineColor = QColor(Qt::yellow).lighter(160);

    Selection.format.setBackground(LineColor);
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
  QPainter painter(mp_lineNumberArea);
  painter.fillRect(Event->rect(), Qt::lightGray);


  QTextBlock Block = firstVisibleBlock();
  int BlockNumber = Block.blockNumber();
  int Top =
      (int) blockBoundingGeometry(Block).translated(contentOffset()).top();
  int Bottom = Top + (int) blockBoundingRect(Block).height();

  while (Block.isValid() && Top <= Event->rect().bottom())
  {
    if (Block.isVisible() && Bottom >= Event->rect().top())
    {
      QString Number = QString::number(BlockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(0, Top, mp_lineNumberArea->width(),
                       fontMetrics().height(), Qt::AlignRight, Number);
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


} } } // namespaces
