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


#ifndef __OPENFLUID_UIWARESDEV_WARESHUBISSUEDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_WARESHUBISSUEDIALOG_HPP__


#include <QDate>

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>


namespace Ui {
class WareshubIssueDialog;
}


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API WareshubIssueDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT


  public:

    struct Issue
    {
        QString m_ID;
        QString m_Title;
        QString m_Creator;
        QDate m_Date;
        QString m_Type;
        QString m_State;
        QString m_Description;
        QString m_Urgency;

        Issue() :
            m_Date(QDate::currentDate())
        {
        }
    };


  private slots:

    void onChanged();


  private:

    Ui::WareshubIssueDialog* ui;

    QStringList m_IDs;

    QString m_DefaultMessage;

    QStringList m_Types = { "", "bug", "feature", "review" };

    QStringList m_Statuses = { "open", "closed" };

    QStringList m_Urgencies = { "low", "medium", "high" };


  public:

    WareshubIssueDialog(const QStringList& IDs, QWidget* Parent = nullptr, const Issue& I = Issue());

    ~WareshubIssueDialog();

    Issue getIssue();

};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_WARESHUBISSUEDIALOG_HPP__ */
