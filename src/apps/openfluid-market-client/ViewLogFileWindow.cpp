/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file ViewLog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include "ViewLogFileWindow.hpp"
#include <fstream>

// =====================================================================
// =====================================================================


ViewLogFileWindow::ViewLogFileWindow(const std::string& PathToLogFile)
{
  set_default_size(500, 350);
  set_position(Gtk::WIN_POS_CENTER_ON_PARENT);

  set_title("Install log");

  m_CloseButton.set_label("Close");

  m_RefLogTextBuffer = Gtk::TextBuffer::create();
  m_RefLogTextBuffer->set_text("");

  m_LogTextView.set_editable(false);
  m_LogTextView.set_buffer(m_RefLogTextBuffer);
  m_LogTextView.set_wrap_mode(Gtk::WRAP_WORD);

  m_LogSWindow.add(m_LogTextView);
  m_LogSWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);


  m_VBox.set_border_width(12);
  m_VBox.pack_start(m_LogSWindow,Gtk::PACK_EXPAND_WIDGET,12);
  m_VBox.pack_start(m_CloseButton,Gtk::PACK_SHRINK,12);

  add(m_VBox);

  m_CloseButton.signal_clicked().connect(
      sigc::mem_fun(*this, &ViewLogFileWindow::onCloseClicked)
    );

  std::ifstream LogFile(PathToLogFile.c_str());
  std::string Line, FullContent;

  while(getline(LogFile,Line))
  {
    FullContent += Line + "\n";
  }
  LogFile.close();

  m_RefLogTextBuffer->set_text(FullContent);

  show_all_children();

  set_modal(true);
}


// =====================================================================
// =====================================================================


void ViewLogFileWindow::onCloseClicked()
{
  hide();
}
