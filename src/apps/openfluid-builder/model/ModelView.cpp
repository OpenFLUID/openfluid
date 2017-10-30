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
  @file ModelView.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/base/RunContextManager.hpp>
#include "ModelView.hpp"

#include <QWheelEvent>
#include <QSvgGenerator>
#include <QFileDialog>


ModelView::ModelView(QWidget* Parent):
  QGraphicsView(Parent)
{

}


// =====================================================================
// =====================================================================


void ModelView::wheelEvent(QWheelEvent* Event)
{
  if (Event->modifiers().testFlag(Qt::ControlModifier)) // zoom only when Ctrl key is pressed
  {
    if (Event->delta() < 0) scale(0.9,0.9);
    else scale(1.1,1.1);
  }
//  Event->ignore();
  QGraphicsView::wheelEvent(Event);
}


// =====================================================================
// =====================================================================


void ModelView::fitViewToItems()
{
  fitInView(scene()->itemsBoundingRect(),Qt::KeepAspectRatio);
}


// =====================================================================
// =====================================================================


void ModelView::resetView()
{
  resetTransform();
}


// =====================================================================
// =====================================================================


void ModelView::exportSceneAsPNG()
{
  QString FileName = QFileDialog::getSaveFileName(this,
                                                  tr("Export as PNG image file"),
                                                  "export_modelview.png",
                                                  tr("PNG Files (*.png)"));

  if (!FileName.isEmpty())
  {
    QImage Image((int)scene()->sceneRect().width(),
                 (int)scene()->sceneRect().height(),
                 QImage::Format_ARGB32_Premultiplied);
    QPainter Painter(&Image);
    Painter.setRenderHint(QPainter::Antialiasing);
    scene()->render(&Painter);
    Image.save(FileName);
  }
}


// =====================================================================
// =====================================================================


void ModelView::exportSceneAsSVG()
{
  QString FileName = QFileDialog::getSaveFileName(this,
                                                  tr("Export as SVG vector file"),
                                                  "export_modelview.svg",
                                                  tr("SVG Files (*.svg)"));

  if (!FileName.isEmpty())
  {
    QSvgGenerator SvgGen;

    QRectF ExportRect = scene()->itemsBoundingRect();

    SvgGen.setFileName(FileName);
    SvgGen.setSize(QSize((int)ExportRect.width(),
                         (int)ExportRect.height()));
    SvgGen.setViewBox(QRectF(0,0,ExportRect.width(),ExportRect.height()));
    SvgGen.setTitle(QString::fromStdString(openfluid::base::RunContextManager::instance()->getProjectName()));
    SvgGen.setDescription(QString::fromStdString(openfluid::base::RunContextManager::instance()
                            ->getProjectDescription()));

    QPainter Painter(&SvgGen);
    scene()->render(&Painter);
  }
}

