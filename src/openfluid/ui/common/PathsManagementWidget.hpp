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
  @file PathsManagementWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_PATHSMANAGEMENTWIDGET_HPP__
#define __OPENFLUID_UICOMMON_PATHSMANAGEMENTWIDGET_HPP__


#include <QWidget>

#include <openfluid/dllexport.hpp>


namespace Ui
{
  class PathsManagementWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API PathsManagementWidget : public QWidget
{
  Q_OBJECT

  private slots:

    void addPath();

    void removePath();

    void moveupPath();

    void movedownPath();


  private:

    Ui::PathsManagementWidget* ui;

    bool m_AllowEmpty;


  signals:

    void pathsUpdated();


  public:

    explicit PathsManagementWidget(QWidget* Parent = nullptr);

    virtual ~PathsManagementWidget();


    /**
      Returns the ordered list of paths
      @return The ordered lists of paths
    */
    QStringList getPathsList() const;


    /**
      Sets the ordered list of paths.
      Existing paths in the list are removed before setting the paths list
      @param[in] Paths The ordered lists of paths
    */
    void setPathsList(const QStringList& Paths);

    /**
      Sets if empty paths list is allowed.
      If set to false, at least one path must remains in the list and cannot be removed.
      @param[in] Empty The flag to allow an empty paths list
    */
    void setAllowEmpty(bool Empty)
    { m_AllowEmpty = Empty; };

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_PATHSMANAGEMENTWIDGET_HPP__ */
