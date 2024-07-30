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
 @file BuilderextSetupWidget.hpp

 @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_UICOMMON_BUILDEREXTSETUPWIDGET_HPP__
#define __OPENFLUID_UICOMMON_BUILDEREXTSETUPWIDGET_HPP__


#include <QDir>

#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>


namespace Ui {
  class BuilderextSetupWidget;
}


namespace openfluid { namespace ui { namespace common {


class OPENFLUID_API BuilderextSetupWidget : public QWidget
{
  Q_OBJECT


  private:

    Ui::BuilderextSetupWidget* ui;

    static QStringList getBuilderExtTypeTexts();

    static QStringList getBuilderExtCategoryTexts();


  public:

    explicit BuilderextSetupWidget(QWidget* Parent = nullptr);

    virtual ~BuilderextSetupWidget();

    openfluid::builderext::ExtensionMode getBuilderextMode() const;

    openfluid::builderext::ExtensionCategory getBuilderextCategory() const;

    QString getBuilderextMenuText() const;

    void initializeFromSignature(const openfluid::builderext::BuilderExtensionSignature& Signature);
};


} } }  // namespaces


#endif /* __OPENFLUID_UICOMMON_BUILDEREXTSETUPWIDGET_HPP__ */
