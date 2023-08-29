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
  @file WareshubIssueDialog.hpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UICOMMON_WARESHUBISSUEDIALOG_HPP__
#define __OPENFLUID_UICOMMON_WARESHUBISSUEDIALOG_HPP__


#include <QDate>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/WareIssues.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>


namespace Ui {
class WareshubIssueDialog;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API WareshubIssueDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT


  public:

    struct Issue
    {
        unsigned int m_ID = 0;
        QString m_Title;
        QString m_Creator;
        QDateTime m_DateCreation;
        QString m_State;
        QString m_Description;
        QString m_Tags;

        Issue() :
            m_DateCreation(QDateTime::currentDateTime())
        {
        }

        Issue(const openfluid::ware::WareIssue& Issue)
        {
          m_ID = Issue.ID;
          m_Title = QString::fromStdString(Issue.Title);
          m_Description = QString::fromStdString(Issue.Description);
          m_Creator = QString::fromStdString(Issue.Creator);
          m_DateCreation = QDateTime::fromString(QString::fromStdString(Issue.CreatedAt.getAsISOString()), 
                                                 "yyyy-MM-dd HH:mm:ss");

          m_Tags = QString::fromStdString(openfluid::tools::join(Issue.Tags, ";"));
          m_State = Issue.IsOpen ? "open" : "closed"; 
        }


        Issue(const openfluid::thirdparty::json& IssueJson) : Issue(openfluid::ware::WareIssue::fromJSON(IssueJson))
        {
        }


        openfluid::ware::WareIssue toWareIssue() const
        {
          openfluid::ware::WareIssue Issue;
          Issue.ID = m_ID;
          Issue.Title = m_Title.toStdString();
          Issue.Description = m_Description.toStdString();
          Issue.Creator = m_Creator.toStdString();
          Issue.CreatedAt = openfluid::core::DateTime::fromISOString(m_DateCreation.toString(
            "yyyy-MM-dd HH:mm:ss").toStdString());
          Issue.IsOpen = m_State.toStdString() != "closed";

          for (const auto& T : openfluid::tools::split(m_Tags.toStdString(), ';'))
          {
            Issue.Tags.push_back(T);
          }
          return Issue;
        }


    };


  private slots:

    void onChanged();


  private:

    Ui::WareshubIssueDialog* ui;

    QList<unsigned int> m_IDs;

    QString m_DefaultMessage;

    //QStringList m_Types = { "", "bug", "feature", "review" }; TODO find new way of describing it, as tag
    //TOIMPL Add tag list

    QStringList m_Statuses = { "open", "closed" };


  public:

    WareshubIssueDialog(const QList<unsigned int>& IDs, QWidget* Parent = nullptr, const Issue& I = Issue());

    virtual ~WareshubIssueDialog();

    Issue getIssue();

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_WARESHUBISSUEDIALOG_HPP__ */
