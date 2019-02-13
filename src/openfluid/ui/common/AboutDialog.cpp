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
  @file AboutDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QFile>

#include <openfluid/config.hpp>
#include <openfluid/buildinfo.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/tools/QtHelpers.hpp>
#include <openfluid/ui/common/AboutDialog.hpp>

#include "ui_AboutDialog.h"


namespace openfluid { namespace ui { namespace common {


AboutDialog::AboutDialog(QWidget *Parent, const QAction* WebAction, const QAction* ContactAction):
  QDialog(Parent), ui(new Ui::AboutDialog),
  mp_WebAction(WebAction), mp_ContactAction(ContactAction),
  m_InfoIsCredits(false)
{
  ui->setupUi(this);

  setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

  ui->IconLabel->setPixmap(openfluid::ui::common::getImage("openfluid_icon_about","ui/common"));
  ui->TitleLabel->setPixmap(openfluid::ui::common::getImage("openfluid_title","ui/common"));

  ui->VersionLabel->setText(QString::fromStdString(openfluid::config::VERSION_FULL));

  ui->WebLabel->setText("<A HREF='"+QString::fromStdString(openfluid::config::WEBSITE_URL)+"'>"+
                        QString::fromStdString(openfluid::config::WEBSITE_DOMAIN)+"</A>");
  ui->WebLabel->setCursor(Qt::PointingHandCursor);

  ui->ContactLabel->setText("<A HREF='"+QString::fromStdString(openfluid::config::CONTACTEMAIL_URL)+"'>"+
                            QString::fromStdString(openfluid::config::CONTACTEMAIL_ADDRESS)+"</A>");
  ui->ContactLabel->setCursor(Qt::PointingHandCursor);

  ui->IconLabel->setFocus();

  ui->ContentTabWidget->setCurrentIndex(0);

  ui->CreditsEdit->setText(quickndirtyMardown2HTML(resourceToString(":/ui/common/texts/AUTHORS.md")));
  ui->ChangelogEdit->setText(quickndirtyMardown2HTML(resourceToString(":/ui/common/texts/CHANGELOG.md")));
  ui->LicenseEdit->setText(resourceToString(":/ui/common/texts/LICENSE"));
  ui->BuildInfoEdit->setText(generateBuildInfoText());


  ui->ButtonBox->button(QDialogButtonBox::Close)->setCursor(Qt::PointingHandCursor);

  ui->BottomFrame->setStyleSheet(QString("QFrame#BottomFrame {background-color: %1;}")
                                          .arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR));

  connect(ui->WebLabel,SIGNAL(clicked()),mp_WebAction,SLOT(trigger()));
  connect(ui->ContactLabel,SIGNAL(clicked()),mp_ContactAction,SLOT(trigger()));

  connect(ui->ButtonBox,SIGNAL(accepted()),this,SLOT(accept()));
  connect(ui->ButtonBox,SIGNAL(rejected()),this,SLOT(reject()));
}


// =====================================================================
// =====================================================================


AboutDialog::~AboutDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QString AboutDialog::resourceToString(const QString& ResName)
{
  QFile Res(ResName);

  if (!Res.open(QIODevice::ReadOnly | QIODevice::Text))
    return "";

  return QString::fromUtf8(Res.readAll());
}


// =====================================================================
// =====================================================================


QString AboutDialog::generateBuildInfoText()
{
  QString Text = tr("<b>Build environment</b><br/>"
                    "* Build type : %1<br/>"
                    "* CMake version : %2<br/>"
                    "* Compiler ID : %3<br/>"
                    "* Compiler version : %4<br/>"
                    "<br/>"
                    "<b>Dependencies</b><br/>"
                    "* Boost version : %5<br/>"
                    "* Qt version : %6<br/>"
                    "* RapidJSON version : %7<br/>"
                    "* GDAL version : %8<br/>"
                    "* GEOS version : %9<br/>")
                 .arg(QString::fromStdString(openfluid::config::BUILD_TYPE))
                 .arg(QString::fromStdString(openfluid::config::BUILD_CMAKE_VERSION))
                 .arg(QString::fromStdString(openfluid::config::BUILD_COMPILER_ID))
                 .arg(QString::fromStdString(openfluid::config::BUILD_COMPILER_VERSION))
                 .arg(QString::fromStdString(openfluid::config::BUILD_LIB_BOOST_VERSION))
                 .arg(QString::fromStdString(openfluid::config::BUILD_LIB_QT_VERSION))
                 .arg(QString::fromStdString(openfluid::config::BUILD_LIB_RAPIDJSON_VERSION))
                 .arg(QString::fromStdString(openfluid::config::BUILD_LIB_GDAL_VERSION))
                 .arg(QString::fromStdString(openfluid::config::BUILD_LIB_GEOS_VERSION));

  return Text;
}


// =====================================================================
// =====================================================================


QString AboutDialog::quickndirtyMardown2HTML(const QString& Content)
{
  QStringList Lines = Content.split(QRegExp("[\r\n]"));

  QStringList ProcessedLines;

  for (auto& Line : Lines)
  {
    Line = openfluid::tools::escapeXMLEntities(Line);
    if (Line.startsWith("### "))
    {
      Line.remove(0,4);
      ProcessedLines.append("<i>"+Line+"</i><br/>");
    }
    else if (Line.startsWith("## "))
    {
      Line.remove(0,3);
      ProcessedLines.append("<b>"+Line+"</b><br/>");
    }
    else if (Line.startsWith("# "))
    {
      Line.remove(0,2);
      ProcessedLines.append("<b><u>"+Line+"</u></b><br/>");
    }
    else if (Line.startsWith("* "))
    {
      Line.remove(0,2);
      ProcessedLines.append(Line+"<br/>");
    }
    else
      ProcessedLines.append(Line+"<br/>");
  }

  return ProcessedLines.join("");
}


} } } // namespaces

