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
  @file NewslineWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/base/PreferencesManager.hpp>

#include "ui_NewslineWidget.h"
#include "NewslineWidget.hpp"
#include "NewslineDownloadWorker.hpp"
#include "builderconfig.hpp"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QThread>


NewslineWidget::NewslineWidget(QWidget* Parent):
  QWidget(Parent),ui(new Ui::NewslineWidget)
{
  ui->setupUi(this);

  ui->ContentsFrame->setStyleSheet(QString("QFrame#ContentsFrame { background-color : %1;}")
                                   .arg(BUILDER_NEWSLINE_BGCOLOR));

  ui->NewslineLabel->setText(QString("<b><big>%1</big></b>").arg(tr("Newsline")));
  ui->NewslineLabel->setStyleSheet(QString("color : %1;").arg(BUILDER_NEWSLINE_TEXTCOLOR));

  ui->NoNewsLabel->setStyleSheet(QString("color : %1;").arg(BUILDER_NEWSLINE_TEXTCOLOR));
  ui->NoNewsLabel->setVisible(false);

  ui->NewslineContents->setStyleSheet(QString("QWidget#NewslineContents { background-color : %1;}")
                                      .arg(BUILDER_NEWSLINE_BGCOLOR));

  if (openfluid::base::PreferencesManager::instance()->getLang() != "default")
    NewsItemWidget::setLocale(QLocale(openfluid::base::PreferencesManager::instance()->getLang()));
  else
    NewsItemWidget::setLocale(QLocale(QLocale::English));

  refreshFromCache();

  // check if update is needed
  if (NewslineDownloadWorker::isTimeForDownload(NewsItemWidget::getLocale().name().left(2)))
    launchDownload();

  connect(ui->UpdateButton,SIGNAL(clicked()),this,SLOT(launchDownload()));
}


// =====================================================================
// =====================================================================


NewslineWidget::~NewslineWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void NewslineWidget::setItems(const QList<NewsItemData>& Items)
{
  QBoxLayout* Layout = dynamic_cast<QBoxLayout*>(ui->NewslineContents->layout());
  QLayoutItem* LayoutItem;

  while ((LayoutItem = Layout->takeAt(0)) != NULL)
  {
    if (LayoutItem->widget() != NULL)
      LayoutItem->widget()->deleteLater();

    delete LayoutItem;
  }


  bool isFirst = true;

  foreach (NewsItemData IData, Items)
  {
    if (isFirst) isFirst = false;
    else
    {
      QFrame* SepFrame = new QFrame(this);
      SepFrame->setFrameShape(QFrame::HLine);
      SepFrame->setStyleSheet("color : grey;");
      Layout->addWidget(SepFrame);
    }
    Layout->addWidget(new NewsItemWidget(IData,this));
  }

  Layout->addStretch();
}


// =====================================================================
// =====================================================================


QList<NewsItemData> NewslineWidget::loadRSSFile(const QString& FilePath)
{
  QList<NewsItemData> News;

  QFile File(FilePath);

  if (File.open(QIODevice::ReadOnly))
  {
    QDomDocument Doc;
    QDomElement Root;

    bool Parsed = Doc.setContent(&File);
    File.close();

    if (Parsed)
    {
      Root = Doc.documentElement();

      if (!Root.isNull())
      {

        if (Root.tagName() == QString("rss"))
        {
          QDomElement ChannelNode = Root.firstChildElement("channel");

          if (!ChannelNode.isNull())
          {
            for(QDomElement CurrNode = ChannelNode.firstChildElement();
                !CurrNode.isNull();
                CurrNode = CurrNode.nextSiblingElement())
            {
              QDomElement CurrElement = CurrNode.toElement();
              if (!CurrElement.isNull() && CurrElement.tagName() == "item")
              {
                // TODO
                NewsItemData Item;
                Item.Title = CurrNode.firstChildElement("title").toElement().text();
                Item.Text = CurrNode.firstChildElement("description").toElement().text();
                Item.ISODate = CurrNode.firstChildElement("pubDate").toElement().text();

                for(QDomElement CurrTagNode = CurrNode.firstChildElement("category");
                    !CurrTagNode.isNull();
                    CurrTagNode = CurrTagNode.nextSiblingElement())
                {
                  Item.Tags.push_back(CurrTagNode.toElement().text());
                }

                if (Item.Tags.isEmpty()) Item.Tags.push_back(tr("misc"));

                News.push_back(Item);
              }
            }
          }
        }
      }
    }
  }
  return News;
}


// =====================================================================
// =====================================================================



void NewslineWidget::refreshFromCache()
{
  QString RSSDefaultFile =
      QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getUserDataPath("%1/en.rss"))
      .arg(BUILDER_NEWSLINE_CACHERELDIR);

  QString RSSFile =
      QString::fromStdString(openfluid::base::RuntimeEnvironment::instance()->getUserDataPath("%1/%2.rss"))
      .arg(BUILDER_NEWSLINE_CACHERELDIR,NewsItemWidget::getLocale().name().left(2));

  QList<NewsItemData> News = loadRSSFile(RSSFile);

  if (News.isEmpty())
    News = loadRSSFile(RSSDefaultFile);

  ui->NoNewsLabel->setVisible(News.isEmpty());

  setItems(News);
}


// =====================================================================
// =====================================================================


void NewslineWidget::launchDownload()
{
  ui->UpdateButton->setEnabled(false);

  QThread* WThread = new QThread;
  NewslineDownloadWorker* Worker = new NewslineDownloadWorker(NewsItemWidget::getLocale().name().left(2));
  Worker->moveToThread(WThread);

  connect(Worker, SIGNAL(finished()), this, SLOT(handleDowloadFinished()));
  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  WThread->start();
}


// =====================================================================
// =====================================================================


void NewslineWidget::handleDowloadFinished()
{
  ui->UpdateButton->setEnabled(true);

  refreshFromCache();
}


