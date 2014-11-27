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
 \file WareSrcWidget.hpp
 \brief Header of ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#ifndef __WARESRCWIDGET_HPP__
#define __WARESRCWIDGET_HPP__

#include <openfluid/dllexport.hpp>

#include <QWidget>

#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/waresdev/WareSrcContainer.hpp>

namespace Ui {
class WareSrcWidget;
}


namespace openfluid { namespace ui { namespace waresdev {

class WareSrcFileEditor;


class DLLEXPORT WareSrcWidget: public QWidget
{
  Q_OBJECT

  private:

    Ui::WareSrcWidget* ui;

    openfluid::waresdev::WareSrcContainer m_Container;


    /**
     * List of opened source file editors by file path relative to this ware directory
     */
    QMap<QString, WareSrcFileEditor*> m_WareSrcFilesByPath;

  public:

    WareSrcWidget(const openfluid::waresdev::WareSrcManager::PathInfo& Info,
                  bool IsStandalone, QWidget* Parent = 0);

    ~WareSrcWidget();

    void openFile(const openfluid::waresdev::WareSrcManager::PathInfo& Info);

    void openDefaultFiles();
};

} } }  // namespaces

#endif /* __WARESRCWIDGET_HPP__ */