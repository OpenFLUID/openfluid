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
  @file FocusableLineEdit.cpp

  @author Armel Thoni <armel.thoni@inra.fr>
*/


#include <openfluid/ui/common/FocusableLineEdit.hpp>


namespace openfluid { namespace ui { namespace common {


FocusableLineEdit::FocusableLineEdit(QWidget* Parent) :
  QLineEdit(Parent)
{
  connect(this, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(changeEditSelection()));
  connect(this, SIGNAL(selectionChanged()), this, SLOT(changeEditSelection()));
  connect(this, SIGNAL(focusChanged()), this, SLOT(changeEditSelection()));
}


// =====================================================================
// =====================================================================


void FocusableLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);
  emit(focusChanged());
}


// =====================================================================
// =====================================================================


void FocusableLineEdit::changeEditSelection()
{
  if (hasFocus())
  {
    // values of selection are updated only when widget has focus
    if (hasSelectedText())
    {
      m_TextSelection.first = selectionStart();
      m_TextSelection.second = selectedText().length();
    }
    else
    {
      m_TextSelection.first = cursorPosition();
      m_TextSelection.second = 0;
    }
  }
}


// =====================================================================
// =====================================================================


void FocusableLineEdit::modifyText(QString NewText)
{
 if (m_TextSelection.second > 0)
  {
    setSelection(m_TextSelection.first, m_TextSelection.second);
  }
  if (hasSelectedText())
  {
    backspace();
  }
  QString PreviousText = text();
  int CursorPosition = cursorPosition();
  setText(PreviousText.insert(CursorPosition, NewText));
}


} } } // namespaces
