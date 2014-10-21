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
  \file ViewLog.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/

#include <openfluid/ui/common/ViewLogFileWindow.hpp>
#include <openfluid/base/FrameworkException.hpp>
#include <QFile>
#include <QTextStream>

namespace openfluid { namespace ui { namespace common {


// =====================================================================
// =====================================================================


ViewLogFileWindow::ViewLogFileWindow(const QString& PathToLogFile) : QDialog()
{
  resize(500, 350);
  setWindowTitle(tr("Install log"));

  m_CloseButton.setText(tr("Close"));

  mp_LogTextView = new QTextEdit();
  mp_LogTextView->setReadOnly(true);
  mp_LogTextView->setText("");
  mp_LogTextView->setWordWrapMode(QTextOption::WordWrap);

  m_LogSWindow.setWidget(mp_LogTextView);
  m_LogSWindow.setWidgetResizable(true);


  m_VBox.setSpacing(20);
  m_VBox.addWidget(&m_LogSWindow);
  m_VBox.addWidget(&m_CloseButton);

  setLayout(&m_VBox);

  connect(&m_CloseButton, SIGNAL(clicked()), this, SLOT(onCloseClicked()));


  QFile File(PathToLogFile);
  QString FullContent;
  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    throw openfluid::base::FrameworkException("ViewLogFileWindow::ViewLogFileWindow",
       "error opening " + PathToLogFile.toStdString());
  }

  QTextStream In(&File);
  while (!In.atEnd())
  {
    QString Line = In.readLine() + "\n";
    FullContent += Line;
  }
  File.close();

  mp_LogTextView->setText(FullContent);

  setModal(true);
}


// =====================================================================
// =====================================================================


void ViewLogFileWindow::onCloseClicked()
{
  hide();
}


} } } //namespaces
