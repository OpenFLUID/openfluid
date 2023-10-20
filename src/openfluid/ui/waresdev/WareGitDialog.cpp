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
 @file WareGitDialog.cpp

 @author Aline LIBRES <aline.libres@gmail.com>
 @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#include <QTextDocument>
#if (QT_VERSION_MAJOR >= 6)
#include <QRegularExpression>
#endif

#include <openfluid/ui/waresdev/WareGitDialog.hpp>

#include "ui_WareGitDialog.h"


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareGitDialog::WareGitDialog(QWidget* Parent) :
    QDialog(Parent), ui(new Ui::WareGitDialog)
{
  ui->setupUi(this);

  ui->ContentTextEdit->setWordWrapMode(QTextOption::WordWrap);
}


// =====================================================================
// =====================================================================


void WareGitDialog::setContent(const QString& Content)
{
  QString Html = Qt::convertFromPlainText(Content, Qt::WhiteSpacePre);

  Html.replace("\x1b[30m", "<span style='color:black'>");
  Html.replace("\x1b[31m", "<span style='color:red'>");
  Html.replace("\x1b[32m", "<span style='color:green'>");
  Html.replace("\x1b[33m", "<span style='color:orange'>"); // yellow is too bright
  Html.replace("\x1b[34m", "<span style='color:blue'>");
  Html.replace("\x1b[m", "</span>");

  // in case there are other ANSI decorations
#if (QT_VERSION_MAJOR < 6)
  Html.replace(QRegExp("\x1b\[[0-9;]+m"), "<span>");
#else
  Html.replace(QRegularExpression("\x1b\[[0-9;]+m"), "<span>");
#endif

  ui->ContentTextEdit->setHtml(Html);
}


// =====================================================================
// =====================================================================


} } } // namespaces
