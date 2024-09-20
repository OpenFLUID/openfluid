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
  @file WareWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_WAREWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_WAREWIDGET_HPP__


#include <QMouseEvent>
#include <QWidget>

#include <openfluid/ui/builderext/PluggableParameterizationExtension.hpp>
#include <openfluid/ui/common/ActionLabel.hpp>

#include "SignatureWidget.hpp"


namespace Ui {
  class WareWidget;
}


class WareWidget : public QWidget
{
  Q_OBJECT;

  private slots:

    void updateShowHideParams();

    void notifyUpClicked();

    void notifyDownClicked();

    void notifyRemoveClicked();


  protected slots:

    void setEnabledWare(bool Enabled);

    void updateParameterValue(const QString& Name, const QString& Value);

    void removeParameterFromList(const QString& Name);

    void openDocFile();

    void switchParameterizationMode();

    void notifyChangedFromParameterizationWidget();

    void addParameterToList();


  protected:

    Ui::WareWidget* ui;

    const openfluid::ware::WareID_t m_ID;

    const QString m_EnabledBGColor;

    bool m_Available;

    bool m_IsTranslated;

    bool m_Ghost;

    bool m_Enabled;

    std::string m_DocFilePath;

    int m_CurrentIndex;

    virtual openfluid::fluidx::WareDescriptor* getWareDescriptor() = 0;

    virtual void setAvailableWare(bool Available);

    void updateWidgetBackground();

    void updateBuildInfoIcons(bool Debug,bool Speed);

    void findDocFile(const std::string& WarePath, const openfluid::ware::WareID_t& WareID);

    virtual bool isClickable() = 0;
    
    virtual void mouseDoubleClickEvent(QMouseEvent* Event);
    
    // Params-related

    static std::string getParamValue(const std::string& ParamName, openfluid::ware::WareParams_t& DescParams);

    virtual void applyContainer()=0;

    void updateParametersList();

    bool addParameterWidget(const QString& Name, const QString& Value);

    bool removeParameterWidget(const QString& Name);

    void clearParameterWidgets();

    void updateParameterizationSwitch();

    bool m_ParamsExpanded;

    void setupParamExtension(bool IsParameterization, openfluid::machine::UUID_t LinkUID);

    openfluid::ui::builderext::PluggableParameterizationExtension* mp_ParamsWidget;
    
    QStringList createParamWidgetsFromSignature(const openfluid::ware::DataWareSignature* Signature);
        
    void addParam(const std::string& ParamName, const std::string& ParamValue, const std::string& ParamUnit, 
                  QStringList& ParamsInSign, const bool Required, const bool Removable);
    
    void updateParametersListWithSignature(const openfluid::ware::DataWareSignature* Signature);


  signals:

    void upClicked(const QString& ID, int Index);

    void downClicked(const QString& ID, int Index);

    void removeClicked(const QString& ID, int Index);

    void changed();
    
    void styleChanged();
    
    void srcEditAsked(const QString&, bool Ghost = false);


  public slots:

    virtual void refresh() = 0;


  public:

    WareWidget(QWidget* Parent,
               const openfluid::ware::WareID_t& ID,
               const QString& DisplayedText,
               bool Enabled, const QString& BGColor, int Index);

    virtual ~WareWidget();

    void updateWare();

    void prepareWareUpdate();

    void setExpanded(bool Expand);

    void setUpButtonEnabled(bool Enabled);

    void setDownButtonEnabled(bool Enabled);

    void setCurrentIndex(int Index);

    void displayParams();

    openfluid::ware::WareID_t getID() const
    { 
      return m_ID; 
    }

    virtual openfluid::ware::WareType getType() const = 0;

};

#endif /* __OPENFLUID_BUILDERAPP_WAREWIDGET_HPP__ */
