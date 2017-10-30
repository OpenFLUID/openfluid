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
  @file AboutDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_ABOUTDIALOG_HPP__
#define __OPENFLUID_UICOMMON_ABOUTDIALOG_HPP__


#include <QDialog>
#include <QAction>

#include <openfluid/dllexport.hpp>


namespace Ui
{
  class AboutDialog;
}


namespace openfluid { namespace ui { namespace common {



class OPENFLUID_API AboutDialog : public QDialog
{
  Q_OBJECT;

  private:

    Ui::AboutDialog* ui;

    const QAction* mp_WebAction;

    const QAction* mp_ContactAction;

    bool m_InfoIsCredits;

    QString resourceToString(const QString& ResName);

    QString generateBuildInfoText();

    static QString quickndirtyMardown2HTML(const QString& Content);


  public:

    AboutDialog(QWidget *Parent, const QAction* WebAction, const QAction* ContactAction);

    virtual ~AboutDialog();
};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_ABOUTDIALOG_HPP__ */
