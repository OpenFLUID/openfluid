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
  \file DotViewExtension.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/tools/ExternalProgram.hpp>

#include "ui_GraphvizWidget.h"
#include "GraphvizViewExtension.hpp"
#include "SVGFileGeneratorWorker.hpp"

#include <QDir>
#include <QThread>
#include <QMessageBox>


// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("view.spatial-graph.graphviz", openfluid::builderext::TYPE_WORKSPACE)

  DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)
  DECLARE_MENUTEXT("Spatial graph viewer (GraphViz)")

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


GraphvizViewExtension::GraphvizViewExtension() :
  openfluid::builderext::PluggableWorkspaceExtension(), ui(new Ui::GraphvizWidget),
  mp_GraphvizScene(NULL)
{
  ui->setupUi(this);
}


// =====================================================================
// =====================================================================


GraphvizViewExtension::~GraphvizViewExtension()
{
  delete ui;
}


// =====================================================================
// =====================================================================


bool GraphvizViewExtension::initialize()
{
  openfluid::tools::ExternalProgram DotProgram("dot");

  if (!DotProgram.isFound())
  {
    QMessageBox::critical(this,QString("Error"),tr("GraphViz software (dot) cannot be found!\n"
                                                   "Please make sure the dot program is accessible in the PATH\n"
                                                   "and try launching this extension again."),
                                                QMessageBox::Close);

    return false;
  }

  std::string TmpDir;
  OPENFLUID_GetRunEnvironment("dir.temp",TmpDir);

  QDir().mkpath(QString::fromStdString(TmpDir));

  m_GVFileName = QString("%1/graphviz-extension.gv").arg(QString::fromStdString(TmpDir));
  m_SVGFileName = QString("%1/graphviz-extension.svg").arg(QString::fromStdString(TmpDir));

  mp_GraphvizScene = new GraphvizScene(m_SVGFileName,this);
  ui->GraphView->setScene(mp_GraphvizScene);

  connect(mp_GraphvizScene,SIGNAL(changed(const QList<QRectF>)),this,SLOT(handleSceneChanged()));

  connect(ui->RankDirComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(launchSVGFileGeneration()));
  connect(ui->FitViewButton,SIGNAL(clicked()),ui->GraphView,SLOT(fitViewToItems()));
  connect(ui->PNGExportButton,SIGNAL(clicked()),ui->GraphView,SLOT(exportSceneAsPNG()));
  connect(ui->SVGExportButton,SIGNAL(clicked()),ui->GraphView,SLOT(exportSceneAsSVG()));

  return true;
}


// =====================================================================
// =====================================================================


void GraphvizViewExtension::launchSVGFileGeneration()
{
  ui->GraphView->setEnabled(false);
  ui->ToolsWidget->setEnabled(false);

  QThread* WThread = new QThread;
  SVGFileGeneratorWorker* Worker = new SVGFileGeneratorWorker(m_GVFileName,m_SVGFileName,
                                                            &(mp_AdvancedDesc->getDomain()));
  Worker->moveToThread(WThread);

  if (ui->RankDirComboBox->currentIndex() == 0)
    Worker->setOption("rankdir","TB");
  else if (ui->RankDirComboBox->currentIndex() == 1)
    Worker->setOption("rankdir","BT");
  else if (ui->RankDirComboBox->currentIndex() == 2)
    Worker->setOption("rankdir","LR");
  else if (ui->RankDirComboBox->currentIndex() == 3)
    Worker->setOption("rankdir","RL");

  connect(Worker, SIGNAL(finished()), this, SLOT(handleFileGenerationFinished()));
  connect(WThread, SIGNAL(started()), Worker, SLOT(run()));
  connect(Worker, SIGNAL(finished()), WThread, SLOT(quit()));
  connect(Worker, SIGNAL(finished()), Worker, SLOT(deleteLater()));
  connect(WThread, SIGNAL(finished()), WThread, SLOT(deleteLater()));

  WThread->start();

}


// =====================================================================
// =====================================================================


void GraphvizViewExtension::update(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags)
{
  if (UpdateFlags.testFlag(openfluid::builderext::FluidXUpdateFlags::FLUIDX_ALL) ||
      UpdateFlags.testFlag(openfluid::builderext::FluidXUpdateFlags::FLUIDX_SPATIALSTRUCT))
  {

    if (ui->ToolsWidget->isEnabled())  // check if no other worker is running
      launchSVGFileGeneration();
  }
}


// =====================================================================
// =====================================================================


void GraphvizViewExtension::handleFileGenerationFinished()
{
  if (QFile::exists(m_SVGFileName))
  {
    mp_GraphvizScene->clear();
    mp_GraphvizScene->reloadFromFile();
  }
  ui->GraphView->setEnabled(true);
  ui->ToolsWidget->setEnabled(true);
}


// =====================================================================
// =====================================================================


void GraphvizViewExtension::handleSceneChanged()
{
  ui->GraphView->fitViewToItems();
}


// =====================================================================
// =====================================================================


DEFINE_BUILDEREXT_CLASS(GraphvizViewExtension)
