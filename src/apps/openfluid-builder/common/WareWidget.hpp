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

#include <openfluid/builderext/PluggableParameterizationExtension.hpp>
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

    virtual void setEnabledWare(bool Enabled);

    virtual void updateParameterValue(const QString& Name, const QString& Value);

    virtual void removeParameterFromList(const QString& Name);

    void switchParameterizationMode();

    void notifyChangedFromParameterizationWidget();


  protected:

    Ui::WareWidget* ui;

    const openfluid::ware::WareID_t m_ID;

    const QString m_EnabledBGColor;

    bool m_Available;

    bool m_Ghost;

    bool m_Enabled;

    int m_CurrentIndex;

    bool m_ParamsExpanded;

    openfluid::builderext::PluggableParameterizationExtension* mp_ParamsWidget;


    virtual void setAvailableWare(bool Available);

    virtual void updateWidgetBackground();

    void updateBuildInfoIcons(bool Debug,bool Speed);

    bool addParameterWidget(const QString& Name, const QString& Value);

    bool removeParameterWidget(const QString& Name);

    void clearParameterWidgets();

    void updateParameterizationSwitch();

    virtual void updateParametersList();

    virtual bool isClickable() = 0;
    
    virtual void mouseDoubleClickEvent(QMouseEvent* Event);
    

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

    virtual void updateWare();

    virtual void prepareWareUpdate();

    void setExpanded(bool Expand);

    void setUpButtonEnabled(bool Enabled);

    void setDownButtonEnabled(bool Enabled);

    void setCurrentIndex(int Index);

    void displayParams();

    openfluid::ware::WareID_t getID() const
    { 
      return m_ID; 
    }

    virtual openfluid::ware::WareType getType() = 0;

};

#endif /* __OPENFLUID_BUILDERAPP_WAREWIDGET_HPP__ */
