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

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_UICOMMON_PREFERENCESDIALOG_HPP__
#define __OPENFLUID_UICOMMON_PREFERENCESDIALOG_HPP__

#include <openfluid/dllexport.hpp>
#include <openfluid/ui/common/OpenFLUIDDialog.hpp>

#include <QDateTime>
#include <QToolButton>


namespace Ui
{
  class PreferencesDialog;
}


class QTreeWidgetItem;
class WaresSearchPathsWidget;


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API PreferencesDialog : public OpenFLUIDDialog
{
  Q_OBJECT;

  public:

    enum DisplayMode { MODE_FULL,
                       MODE_BUILDER,
                       MODE_DEVSTUDIO
                      };


  private:

    enum PagesIndexes { ENVIRONMENT_PAGE = 0,
                        BUILDER_PAGE = 1,
                        SIMULATION_PAGE = 2,
                        DEVENV_PAGE = 3,
                        DEVEDITOR_PAGE = 4,
                        MARKET_PAGE = 5
                        };

  private slots:

    void changePage(QTreeWidgetItem* Current, QTreeWidgetItem* Previous);

    void updateLanguage(const QString& Lang);

    void clearRecentsList();

    void updateRecentsMax(int Val);

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

    void changeSyntaxElementColor(int ElementRow);

    void addMarketPlace();

    void editMarketPlace();

    void removeMarketPlace();

    void processSimUserPathsUpdate();

    void processObsUserPathsUpdate();

    void processBextUserPathsUpdate();

    void processWorkspacesPathsUpdate();

    void updateDevConfigPATH();

    void updateDevConfigGenerator();

    void updateDevConfigOptions();

    void updateDevBuildPATH();

    void updateDevShowPATH(bool Enabled);

    void detectQtDevToolsMinGW();

    void applyTextEditorSettings();

    void restoreDefaultsTextEditorSettings();

  private:

    Ui::PreferencesDialog *ui;

    bool m_RecentsChanged;

    bool m_SimPathsChanged;

    bool m_ObsPathsChanged;

    bool m_WaresWatchingChanged;

    bool m_TextEditorSettingsChanged;

    int m_OriginalLangIndex;

    DisplayMode m_Mode;

    QStringList m_Formats {"bold", "italic", "underline", "strike-through"};

    void initialize(const QStringList& ExtsPaths);

    void updateMarketplacesList();

    void intializeTextEditorSettings();

    void updateSyntaxElementLabel(QLabel* Label, const QStringList& Decorations, const QString& ColorName);

    void updateSyntaxElementColorButton(QToolButton* Button, const QString& ColorName);

  public:

    PreferencesDialog(QWidget *Parent, DisplayMode Mode = MODE_FULL, const QStringList& ExtsPaths = QStringList());

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

  signals:

    void applyTextEditorSettingsAsked();

};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_PREFERENCESDIALOG_HPP__ */
