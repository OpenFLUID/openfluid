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
  @file GNUplotObsParamsWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/



#ifndef __GNUPLOTOBSPARAMSWIDGET_HPP__
#define __GNUPLOTOBSPARAMSWIDGET_HPP__


#include <openfluid/builderext/PluggableParameterizationExtension.hpp>

#include "GNUplotObsTools.hpp"


class QListWidget;
class QListWidgetItem;


namespace Ui
{
  class GNUplotObsParamsWidget;
}


class GNUplotObsParamsWidget: public openfluid::builderext::PluggableParameterizationExtension
{
  Q_OBJECT;


  private:

    Ui::GNUplotObsParamsWidget* ui;

    GNUplotInfo m_Plot;

    QString m_CurrentSerieName;
    QString m_CurrentGraphName;

    void removeParamsStartingWith(const QString& Str);

    void renameParamsStartingWith(const QString& Before, const QString& After);

    QString generateListItemName(const QListWidget* ListWidget,const QString& NameRoot);

    int findItemRowInList(const QListWidget* ListWidget,const QString& ItemText);


  private slots:

    void setTerminal(const QString& Terminal);

    void setOutputFileName();

    void setGNUplotOption(bool Checked);

    void setPersistantOption(bool Checked);

    void addSerie();

    void removeSerie();

    void renameSerie(QListWidgetItem* Item);

    void editSerieName();

    void setCurrentSerie(int CurrentRow);

    void changeSerieType();

    void setVariableName();

    void setUnitsClass(const QString& UnitsClass);

    void updateClassIDs(const QString& UnitsClass);

    void setUnitID(const QString& UnitID);

    void setSourceFileName();

    void setSerieLabel();

    void setSerieStyle(const QString& Style);

    void addGraph();

    void removeGraph();

    void renameGraph(QListWidgetItem* Item);

    void editGraphName();

    void setCurrentGraph(int CurrentRow);

    void setGraphTitle();

    void setGraphYLabel();

    void setGraphLegend(const QString& Key);

    void setGraphSeries();


  public:

    GNUplotObsParamsWidget();

    ~GNUplotObsParamsWidget();

    void update();

};


#endif /* __GNUPLOTOBSPARAMSWIDGET_HPP__ */
