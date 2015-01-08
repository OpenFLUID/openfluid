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
  @file WaresSearchPathsWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_WARESSEARCHPATHSWIDGET_HPP__
#define __OPENFLUID_UICOMMON_WARESSEARCHPATHSWIDGET_HPP__


#include <openfluid/dllexport.hpp>
#include <QWidget>
#include <QDir>


namespace Ui
{
  class WaresSearchPathsWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API WaresSearchPathsWidget : public QWidget
{
  Q_OBJECT;

  private:

    Ui::WaresSearchPathsWidget *ui;


  private slots:

    void processUserPathsUpdate();


  signals:

    void userPathsUpdated();


  public:

    WaresSearchPathsWidget(QWidget* Parent = 0);

    virtual ~WaresSearchPathsWidget();

    /**
      Initializes the ordered list of user and predefined paths.
      Existing paths in the list are removed before setting the paths list
      @param[in] UserPaths The ordered lists of user paths
      @param[in] PredefPaths The ordered lists of static predefined paths
    */
    void initialize(const QStringList& UserPaths, const QStringList& PredefPaths);

    /**
      Returns the ordered list of user paths
      @return The ordered lists of user paths
    */
    QStringList getUserPaths() const;

};


} } } // namespaces

#endif /* __OPENFLUID_UICOMMON_WARESSEARCHPATHSWIDGET_HPP__ */
