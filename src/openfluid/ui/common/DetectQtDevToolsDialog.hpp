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
  @file DetectQtDevToolsDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_DETECTQTDEVTOOLSDIALOG_HPP__
#define __OPENFLUID_UICOMMON_DETECTQTDEVTOOLSDIALOG_HPP__


#include <QAbstractButton>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/OpenFLUIDDialog.hpp>


namespace Ui
{
  class DetectQtDevToolsDialog;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API DetectQtDevToolsDialog : public OpenFLUIDDialog
{
  Q_OBJECT;

  private slots:

    void handleButtonBoxClick(QAbstractButton* Button);

    void browseQtRootPath();

    void runDetection();


  private:

    Ui::DetectQtDevToolsDialog* ui;

    QString m_FrameworkPath;

    QString m_DevToolsPath;

    void setWidgetsEnabled(bool Enabled);


  public:

    DetectQtDevToolsDialog(QWidget* Parent);

    virtual ~DetectQtDevToolsDialog();

    bool isDetectionSuccessful()
    { return (!m_FrameworkPath.isEmpty() && !m_DevToolsPath.isEmpty()); }

    QString getFrameworkPath() const
    { return m_FrameworkPath; }

    QString getDevToolsPath() const
    { return m_DevToolsPath; }

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_DETECTQTDEVTOOLSDIALOG_HPP__ */
