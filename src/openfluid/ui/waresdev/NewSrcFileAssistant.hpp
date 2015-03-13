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
 @file NewSrcFileAssistant.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_NEWSRCFILEASSISTANT_HPP__
#define __OPENFLUID_UIWARESDEV_NEWSRCFILEASSISTANT_HPP__

#include <openfluid/dllexport.hpp>

#include <QWizard>
#include <QWizardPage>
#include <QDir>

#include <QFrame>
#include <QLabel>

#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>

namespace Ui { class CppPage;
class CMakeConfigPage;
class NewSrcFileAssistant;
}

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


class OPENFLUID_API CppPage: public QWizardPage
{
  Q_OBJECT

  private:

    Ui::CppPage* ui;

    QDir m_WareDir;

    bool m_IsHpp;

    QString m_DefaultMsg;

  protected:

    bool isComplete() const;

  public:

    CppPage(const QDir& WareDir, const QStringList& BextType_Text, const QStringList& BextCategory_Text,
            QWidget* Parent = 0);

    ~CppPage();

    void initialize(bool IsHpp, bool IsUi, openfluid::waresdev::WareSrcManager::WareType Type);

  private slots:

    void onInformationChanged();
};


// =====================================================================
// =====================================================================


class OPENFLUID_API CMakeConfigPage: public QWizardPage
{
  Q_OBJECT

  private:

    Ui::CMakeConfigPage* ui;

    QString m_DefaultMsg;

  protected:

    bool isComplete() const;

  public:

    CMakeConfigPage(openfluid::waresdev::WareSrcManager::WareType Type, const QStringList& Sim2doc_Text,
                    QWidget* Parent = 0);

    ~CMakeConfigPage();

  private slots:

    void onRootfilenameTextChanged();
};


// =====================================================================
// =====================================================================


class OPENFLUID_API NewSrcFileAssistant: public QWizard
{
  Q_OBJECT

  private:

    enum
    {
      INTRO_PAGE = 0, EMPTY_PAGE = 1, CMAKECONFIG_PAGE = 2, CPP_PAGE = 3, HPP_PAGE = 4, CPP_UI_PAGE = 5, HPP_UI_PAGE = 6
    };

    Ui::NewSrcFileAssistant* ui;

    openfluid::waresdev::WareSrcFactory* mp_Factory;

    const openfluid::waresdev::WareSrcContainer& mref_Container;

    QString m_NewFilePath;

    QList<openfluid::builderext::ExtensionType> m_BextType_Data;
    QStringList m_BextCategory_Data;
    QStringList m_Sim2doc_Data;

  protected:

    void initializePage(int Id);

  public:

    NewSrcFileAssistant(const openfluid::waresdev::WareSrcContainer& Container, QWidget* Parent = 0);

    ~NewSrcFileAssistant();

    int nextId() const;

    void accept();

    QString getNewFilePath();

    static void setStatus(const QString DefaultMsg, const QString WarningMsg, QLabel* MsgLabel, QFrame* MsgFrame);

  private slots :

    void onFileTypeButtonClicked(int Id);

    void onEmptyBrowseToolButtonClicked();
};


} } }  // namespaces

#endif /* __OPENFLUID_UIWARESDEV_NEWSRCFILEASSISTANT_HPP__ */
