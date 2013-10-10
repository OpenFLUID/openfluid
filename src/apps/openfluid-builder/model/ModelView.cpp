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
  \file ModelView.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/base/ProjectManager.hpp>

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
    SvgGen.setTitle(QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getName()));
    SvgGen.setDescription(QString::fromStdString(openfluid::base::ProjectManager::getInstance()->getDescription()));

    QPainter Painter(&SvgGen);
    scene()->render(&Painter);
  }
}

