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
  \file NewsItemWidget.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
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

    QLabel* TagLabel = new QLabel(Tag,TagWidget);
    TagLabel->setWordWrap(true);

    QString TagBGColor = "#47617B";

    if (Tag == "software")
    {
      TagBGColor = "#4BA84F";
    }
    else if (Tag == "doc" || Tag == "publication")
    {
      TagBGColor = "#4F75D1";
    }
    else if (Tag == "training")
    {
      TagBGColor = "#A04FD1";
    }

    TagLabel->setStyleSheet(QString("color : %1; background-color : %2 ; border-radius: 4px; padding : 3px; font-size : 8pt;")
                            .arg("white",TagBGColor));


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
