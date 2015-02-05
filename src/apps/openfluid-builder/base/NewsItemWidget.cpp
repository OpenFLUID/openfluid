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
  @file NewsItemWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */



#include "ui_NewsItemWidget.h"
#include "NewsItemWidget.hpp"
#include "builderconfig.hpp"

#include <QLabel>
#include <QDateTime>


QLocale NewsItemWidget::m_Locale = QLocale(QLocale::English);


NewsItemWidget::NewsItemWidget(const NewsItemData& Data,QWidget *Parent) :
QWidget(Parent), ui(new Ui::NewsItemWidget)
{
  ui->setupUi(this);

  ui->TitleLabel->setStyleSheet(QString("color : %1;").arg(BUILDER_NEWSLINE_TEXTCOLOR));
  ui->DateLabel->setStyleSheet(QString("color : %1;").arg(BUILDER_NEWSLINE_TEXTCOLOR));
  ui->TextLabel->setStyleSheet(QString("color : %1;").arg(BUILDER_NEWSLINE_TEXTCOLOR));

  ui->TitleLabel->setText(QString("<b>%1</b>").arg(Data.Title));

  QDateTime DT = QDateTime::fromString(Data.ISODate,"yyyy-MM-dd'T'hh:mm:ss");
  ui->DateLabel->setText(m_Locale.toString(DT));

  QString HTMLText = Data.Text;
  HTMLText.replace("&gt;",">");
  HTMLText.replace("&lt;","<");


  ui->TextLabel->setText(HTMLText);

  // to change the color of links, uncomment and use this instead
  /*ui->TextLabel->setText(QString("<html><head><style type=text/css>"
                                 "a:link {color:%1;}"
                                 "</style></head><body>%2</body></html>").arg(BUILDER_NEWSLINE_LINKSCOLOR,HTMLText));*/

  foreach (QString Tag, Data.Tags)
  {
    QWidget* TagWidget = new QWidget(this);
    TagWidget->setMinimumWidth(75);
    QHBoxLayout* TagLayout = new QHBoxLayout();
    TagLayout->setSpacing(5);
    TagLayout->setContentsMargins(0,0,0,0);

    QString TagLabelText = Tag;
    TagLabelText.replace(" ","\n");

    QLabel* TagLabel = new QLabel(TagLabelText,TagWidget);
    TagLabel->setWordWrap(true);

    TagLabel->setStyleSheet(QString("qproperty-alignment: AlignCenter; color : %1; "
                                    "background-color : %2 ; border-radius: 4px; padding : 4px; font-size : 9pt;")
                            .arg("white",getCSSColorFromTag(Tag)));


    TagLayout->addStretch();
    TagLayout->addWidget(TagLabel);

    TagWidget->setLayout(TagLayout);
    TagWidget->setContentsMargins(0,0,0,0);

    ui->TagsLayout->addWidget(TagWidget);
  }
  ui->TagsLayout->addStretch();
}


// =====================================================================
// =====================================================================


NewsItemWidget::~NewsItemWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


QString NewsItemWidget::getCSSColorFromTag(const QString& Tag)
{
  QString TagColor = "#47617B"; // OpenFLUID grey-blue

  if (Tag == "software")
  {
    TagColor = "#4BA84F";  // green
  }
  else if (Tag == "doc" || Tag == "publication")
  {
    TagColor = "#4F75D1";  // blue
  }
  else if (Tag == "training" || Tag == "meeting")
  {
    TagColor = "#A04FD1";  // purple
  }
/*  else if (Tag == "meeting")
  {
    TagColor = "#FF78C7";  // pink
  }*/
  else if (Tag == "research project")
  {
    TagColor = "#979797";  // grey
  }
  else if (Tag == "important")
  {
    TagColor = "#FF7600";  // orange
  }

  return TagColor;
}


// =====================================================================
// =====================================================================


void NewsItemWidget::setLocale(QLocale Locale)
{
  m_Locale = Locale;
}


// =====================================================================
// =====================================================================


QLocale NewsItemWidget::getLocale()
{
  return m_Locale;
}
