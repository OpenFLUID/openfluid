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
  @file EditExternalToolDialog.hpp

  @author Armel THONI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_UICOMMON_EDITEXTERNALTOOLDIALOG_HPP__
#define __OPENFLUID_UICOMMON_EDITEXTERNALTOOLDIALOG_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/ui/common/FocusableLineEdit.hpp>


class QLineEdit;


namespace Ui
{
  class EditExternalToolDialog;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API EditExternalToolDialog : public MessageDialog
{
  Q_OBJECT;

  private slots:

    void checkGlobally();

    void addAppToWorkspaceLine();
    void addAppToWareLine();
    void addAppToFileLine();

    void addTargetToWorkspaceLine();
    void addTargetToWareLine();
    void addTargetToFileLine();


  private:

    Ui::EditExternalToolDialog* ui;

    bool m_IsEditMode;

    QString m_OriginalName;

    const openfluid::base::PreferencesManager::ExternalToolsCommands_t& m_AllCommands;

    void modifyCommandEditText(QLineEdit* LineEdit, QString NewText, std::pair<size_t, size_t> Selection);


  public:

    EditExternalToolDialog(QWidget* Parent, const QString& Name, const QStringList& ToolCommands, 
                           const openfluid::base::PreferencesManager::ExternalToolsCommands_t& AllCommands);

    virtual ~EditExternalToolDialog();

    QStringList getFullCommands() const;

    QString getName() const;

    QString getOriginalName() const 
    {
      return m_OriginalName; 
    }

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_EDITEXTERNALTOOLDIALOG_HPP__ */
