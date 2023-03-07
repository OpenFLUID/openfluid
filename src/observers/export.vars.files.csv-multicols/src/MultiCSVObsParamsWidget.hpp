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
  @file MultiCSVObsParamsWidget.hpp

  @author Armel THONI <armel.thoni@inra.fr>
*/


#ifndef __MULTICSVOBSPARAMSWIDGET_HPP__
#define __MULTICSVOBSPARAMSWIDGET_HPP__


#include <openfluid/ui/builderext/PluggableParameterizationExtension.hpp>

#include "MultiCSVObsTools.hpp"


namespace Ui
{
  class MultiCSVObsParamsWidget;
}


class MultiCSVObsParamsWidget: public openfluid::ui::builderext::PluggableParameterizationExtension
{
  Q_OBJECT;

  private:

    Ui::MultiCSVObsParamsWidget* ui;

    MultiFormatMap_t m_Formats;

    MultiSetMap_t m_Sets;

    void clearInternalDataAndWidgets();

    void removeParamsStartingWith(const QString& Str);

    QString getParamValue(const QString& Str, const QString& Default = "");

    QStringList getFormatNames();

  private slots:

    void addFormat();

    void editFormat();

    void removeFormat();

    void addSet();

    void editSet();

    void removeSet();

    void generateAutomaticFormatAndSets();


  public:

    MultiCSVObsParamsWidget();

    ~MultiCSVObsParamsWidget();

    void update();

};


#endif /* __MULTICSVOBSPARAMSWIDGET_HPP__ */
