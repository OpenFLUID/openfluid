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
  @file PreferencesDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_PREFERENCESDIALOG_HPP__
#define __OPENFLUID_UICOMMON_PREFERENCESDIALOG_HPP__


#include <QDateTime>
#include <QToolButton>
#include <QLabel>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui
{
  class PreferencesDialog;
}


class QTreeWidgetItem;
class WaresSearchPathsWidget;


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API PreferencesDialog : public MessageDialog
{
  Q_OBJECT;

  public:

    enum class DisplayMode { FULL,
                       BUILDER,
                       DEVSTUDIO
                      };


  private:

    enum class PagesIndexes { 
      GENERAL_PAGE = 0,
      WORKSPACES_PAGE = 1,
      BUILDER_PAGE = 2,
      SIMULATION_PAGE = 3,
      DEVENV_PAGE = 4,
      DEVEDITOR_PAGE = 5
    };

  private slots:

    void changePage(QTreeWidgetItem* Current, QTreeWidgetItem* Previous);

    void updateLanguage(int Index);

    void clearRecentsList();

    void confirmItemRemoval(bool Confirm);

    void confirmParamRemoval(bool Confirm);

    void confirmUnitsRemoval(bool Confirm);

    void confirmConnectionsRemoval(bool Confirm);

    void confirmAttributesRemoval(bool Confirm);

    void enableWatchers(bool Active);

    void enableAutoSaveBeforeRun(bool AutoSave);

    void updateDeltaT(int Val);

    void updatePeriodBegin(const QDateTime& DT);

    void updatePeriodEnd(const QDateTime& DT);

    void enableSyntaxHighlighting(bool Enable);

    void changeSyntaxElementDecoration(int ElementRow);

    void enableCurrentLineHighlighting(bool Enable);

    void changeCurrentLineColor();

    void changeCurrentFont(const QFont& Font);

    void enableLineWrapping(bool Enable);

    void changeIndentSpaceNumber(int SpaceNb);

    void enableInvisibleCharsDisplay(bool Enable);

    void enableCarriageReturnDisplay(bool Enable);

    void changeSyntaxElementColor(int ElementRow);

    void processSimUserPathsUpdate();

    void processObsUserPathsUpdate();

    void processBextUserPathsUpdate();

    void processWorkspacesPathsUpdate();
    
    void processExtToolsUpdate();
    
    void enableAutosaveBeforeBuild(bool Enabled);

    void updateDevConfigPATH();

    void updateDevConfigGenerator();

    void updateDevConfigOptions();

    void updateDevBuildPATH();

    void updateDevShowPATH(bool Enabled);

    void updateDevSslNoVerify(bool NoVerify);

    void detectQtDevToolsMinGW();

    void applyTextEditorSettings();

    void restoreDefaultsTextEditorSettings();


  private:

    Ui::PreferencesDialog* ui;

    openfluid::base::PreferencesManager* mp_PrefsMan;
    openfluid::base::WorkspaceManager* mp_WorksMan;

    bool m_RecentsChanged;

    bool m_SimPathsChanged;

    bool m_ObsPathsChanged;

    bool m_WaresWatchingChanged;

    bool m_TextEditorSettingsChanged;

    bool m_RestartRequired;

    int m_OriginalLangIndex;

    QString m_OriginalActiveWorkspace;

    DisplayMode m_Mode;

    QStringList m_Formats {"bold", "italic", "underline", "strike-through"};

    void initialize();

    void intializeTextEditorSettings();

    void updateSyntaxElementLabel(QLabel* Label, const QStringList& Decorations, const QString& ColorName);

    void updateSyntaxElementColorButton(QToolButton* Button, const QString& ColorName);

    void updateRestartStatus();

    static QString getLanguageAsPrettyString(const QString& LangCode);


  signals:

    void applyTextEditorSettingsAsked();


  public:

    PreferencesDialog(QWidget *Parent, DisplayMode Mode = DisplayMode::FULL);

    virtual ~PreferencesDialog();

    bool isRecentsChanged() const
    { return m_RecentsChanged; }

    bool isSimPathsChanged() const
    { return m_SimPathsChanged; }

    bool isObsPathsChanged() const
    { return m_ObsPathsChanged; }

    bool isWaresWatchingChanged() const
    { return m_WaresWatchingChanged; }

    bool isTextEditorSettingsChanged() const
    { return m_TextEditorSettingsChanged; }

    bool isRestartRequired()
    { return m_RestartRequired; }

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_PREFERENCESDIALOG_HPP__ */
