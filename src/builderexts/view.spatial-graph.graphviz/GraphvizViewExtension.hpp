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
  @file GraphvizViewExtension.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include <openfluid/builderext/PluggableWorkspaceExtension.hpp>

#include "GraphvizScene.hpp"


DECLARE_BUILDEREXT_PLUGIN


// =====================================================================
// =====================================================================

namespace Ui
{
  class GraphvizWidget;
}

class GraphvizViewExtension : public openfluid::builderext::PluggableWorkspaceExtension
{
  Q_OBJECT;

  private slots:

    void handleFileGenerationFinished();

    void handleSceneChanged();

    void launchSVGFileGeneration();


  private:

    Ui::GraphvizWidget* ui;

    QString m_GVFileName;

    QString m_SVGFileName;

    GraphvizScene* mp_GraphvizScene;

    static QString generateEdge(const QString& SrcClass, const QString& SrcID,
                                   const QString& DestClass, const QString& DestID,
                                   const QString& Options);

    static QString generateNode(const QString& UClass, const QString& UID,
                                   const QString& Options);


  public slots:

      void update(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags);


  public:

    GraphvizViewExtension();

    ~GraphvizViewExtension();

    bool initialize();

};


