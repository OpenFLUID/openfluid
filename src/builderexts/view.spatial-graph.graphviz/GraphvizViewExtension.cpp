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
  @file GraphvizViewExtension.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QDir>
#include <QThread>
#include <QMessageBox>

#include <openfluid/utils/ExternalProgram.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>

#include "ui_GraphvizWidget.h"
#include "GraphvizViewExtension.hpp"
#include "SVGFileGeneratorWorker.hpp"


// =====================================================================
// =====================================================================


BEGIN_BUILDEREXT_SIGNATURE("view.spatial-graph.graphviz", openfluid::builderext::MODE_WORKSPACE)

  DECLARE_CATEGORY(openfluid::builderext::CAT_SPATIAL)
  DECLARE_MENUTEXT(QT_TRANSLATE_NOOP("signature","Spatial graph viewer (GraphViz)"))

END_BUILDEREXT_SIGNATURE


// =====================================================================
// =====================================================================


GraphvizViewExtension::GraphvizViewExtension() :
  openfluid::builderext::PluggableWorkspaceExtension(), ui(new Ui::GraphvizWidget),
  mp_GraphvizScene(nullptr)
{
  ui->setupUi(this);

  ui->PNGExportButton->setIcon(openfluid::ui::common::getIcon("export","/ui/common"));
  ui->PNGExportButton->setIconSize(QSize(20,20));

  ui->SVGExportButton->setIcon(openfluid::ui::common::getIcon("export","/ui/common"));
  ui->SVGExportButton->setIconSize(QSize(20,20));
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
#if defined(OPENFLUID_OS_WINDOWS)
  openfluid::utils::ExternalProgram DotProgram("dot.exe");
#else
  openfluid::utils::ExternalProgram DotProgram("dot");
#endif

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
                                                            &(mp_AdvancedDesc->spatialDomain()));
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
