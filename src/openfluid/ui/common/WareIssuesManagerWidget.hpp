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
  @file WareIssuesManagerWidget.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_UICOMMON_WAREISSUESMANAGERWIDGET_HPP__
#define __OPENFLUID_UICOMMON_WAREISSUESMANAGERWIDGET_HPP__


#include <QLineEdit>
#include <QComboBox>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/WareshubIssueDialog.hpp>
#include <openfluid/thirdparty/JSON.hpp>
#include <openfluid/ware/WareSignature.hpp>


namespace Ui {
  class WareIssuesManagerWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API WareIssuesManagerWidget: public QWidget
{
  Q_OBJECT

  private:

    Ui::WareIssuesManagerWidget* ui;

    openfluid::thirdparty::json m_Doc;

    QMap<unsigned int, WareshubIssueDialog::Issue> m_IssuesByID;

    void jsonIssuesToIssuesMap();

    void issuesMapToJsonIssues();

    void updateIssuesTable();


  private slots:

    void onRemoveIssueClicked();

    void onEditIssueClicked();

    void onAddIssueClicked();


  public:

    WareIssuesManagerWidget(QWidget* Parent = nullptr);

    virtual ~WareIssuesManagerWidget();

    void loadContent(const openfluid::ware::WareIssues& WareIssues);
    
    openfluid::ware::WareIssues getIssues() const;

    void updateContent();

    QString getSelectedText();

    bool isModified();

    QWidget* getWidget();


};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_WAREISSUESMANAGERWIDGET_HPP__ */
