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
#include <QDir>

#include <QFrame>
#include <QLabel>

#include <openfluid/waresdev/WareSrcContainer.hpp>
#include <openfluid/waresdev/WareSrcFactory.hpp>

namespace Ui {
class EmptyPage;
class CppPage;
class CMakeConfigPage;
class NewSrcFileAssistant;
}

namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


class OPENFLUID_API EmptyPage : public QWizardPage
{
  Q_OBJECT

  private:

    Ui::EmptyPage* ui;

    QString m_WarePath;

    QString m_DefaultMsg = tr("Empty file");

    QString m_FilePathPlaceholder = "<i>"+tr("choose a file to create")+"</i>";

  protected:

    /**
     * Reimplemented function
     */
    bool isComplete() const;

  public:

    EmptyPage(const QString& WarePath, QWidget* Parent = nullptr);

    ~EmptyPage();

    void initialize();

  private slots:

    void onEmptyBrowseToolButtonClicked();
};


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

    /**
     * Reimplemented function
     */
    bool isComplete() const;

  public:

    CppPage(const QDir& WareDir, QWidget* Parent = nullptr);

    ~CppPage();

    void initialize(bool IsHpp, bool IsUi, openfluid::ware::WareType Type);

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

    /**
     * Reimplemented function
     */
    bool isComplete() const;

  public:

    CMakeConfigPage(openfluid::ware::WareType Type, QWidget* Parent = nullptr);

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

    enum class PageType
    {
      INTRO_PAGE = 0,
      EMPTY_PAGE = 1,
      CMAKECONFIG_PAGE = 2,
      CPP_PAGE = 3,
      HPP_PAGE = 4,
      CPP_UI_PAGE = 5,
      HPP_UI_PAGE = 6
    };

    Ui::NewSrcFileAssistant* ui;

    openfluid::waresdev::WareSrcFactory* mp_Factory;

    const openfluid::waresdev::WareSrcContainer& mref_Container;

    QString m_NewFilePath;

  protected:

    /**
     * Reimplemented function
     */
    void initializePage(int ID);


  public:

    NewSrcFileAssistant(const openfluid::waresdev::WareSrcContainer& Container, QWidget* Parent = nullptr);

    ~NewSrcFileAssistant();

    /**
     * Reimplemented function
     */
    int nextId() const;

    /**
     * Reimplemented function
     */
    void accept();

    QString getNewFilePath();

    static void setStatus(const QString DefaultMsg, const QString WarningMsg, QLabel* MsgLabel, QFrame* MsgFrame);


  private slots :

    void onFileTypeButtonClicked(int ID);
};


} } } // namespaces

#endif /* __OPENFLUID_UIWARESDEV_NEWSRCFILEASSISTANT_HPP__ */
