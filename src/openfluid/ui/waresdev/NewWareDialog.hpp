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
 @file NewWareDialog.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_UIWARESDEV_NEWWAREDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_NEWWAREDIALOG_HPP__


#include <QDir>

#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/dllexport.hpp>


namespace Ui {
class NewWareDialog;
}


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API NewWareDialog : public openfluid::ui::common::MessageDialog
{
  Q_OBJECT


  private slots:

    void onInformationChanged();


  private:

    Ui::NewWareDialog* ui;

    QString m_DefaultMsg;

    openfluid::ware::WareType m_WareType;

    QDir m_WareTypeDir;

    QString m_NewWarePath;

    openfluid::ware::SimulatorSignature m_SimSignature;

    bool m_UseSimSignature;

    void setStatus(const QString WarningMsg);

    static QRegExp getClassnameRegExp(QString& Tooltip);

    static QRegExp getWareIdRegExp(QString& Tooltip);

    static QStringList getBuilderExtTypeTexts();

    static QStringList getBuilderExtCategoryTexts();


  public:

    explicit NewWareDialog(openfluid::ware::WareType Type, QWidget* Parent = nullptr);

    NewWareDialog(const openfluid::ware::SimulatorSignature& Signature, QWidget* Parent = nullptr);

    virtual ~NewWareDialog();

    openfluid::ware::WareID_t getID() const;

    std::string getClassName() const;

    bool isWareUI() const;

    openfluid::builderext::ExtensionMode getBuilderextMode() const;

    openfluid::builderext::ExtensionCategory getBuilderextCategory() const;

    QString getBuilderextMenuText() const;
};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARESDEV_NEWWAREDIALOG_HPP__ */
