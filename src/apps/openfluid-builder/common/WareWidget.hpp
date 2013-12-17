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
  \file WareWidget.hpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __WAREWIDGET_HPP__
#define __WAREWIDGET_HPP__


#include <QWidget>

#include <openfluid/ware/WareSignature.hpp>

#include "ActionLabel.hpp"
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

    virtual void updateParamValue(const QString& Name, const QString& Value);

    virtual void removeParam(const QString& Name);


  protected:

    Ui::WareWidget* ui;

    const openfluid::ware::WareID_t m_ID;

    const QString m_EnabledBGColor;

    bool m_Available;

    bool m_Enabled;

    bool m_ParamsExpanded;

    ActionLabel* mp_ShowHideParamsLabel;

    ClickableLabel* mp_EnableLabel;

    SignatureWidget* mp_SignatureWidget;

    virtual void setAvailableWare(bool Available);

    virtual void updateWidgetBackground();

    bool addParameterWidget(const QString& Name, const QString& Value);

    bool removeParameterWidget(const QString& Name, bool WithFinalStretch = true);

    void clearParameterWidgets();


  signals:

    void upClicked(const QString&);

    void downClicked(const QString&);

    void removeClicked(const QString&);

    void changed();


  public slots:

    virtual void refresh() = 0;


  public:

    WareWidget(QWidget* Parent, const openfluid::ware::WareID_t& ID, bool Enabled, const QString& BGColor);

    virtual ~WareWidget();

    virtual void updateWare();

    void setExpanded(bool Expand);

    void setUpButtonEnabled(bool Enabled);

    void setDownButtonEnabled(bool Enabled);

    void displayParams();

    openfluid::ware::WareID_t getID() const
    { return m_ID; };

    virtual openfluid::fluidx::WareDescriptor::WareType getType() = 0;


};

#endif /* __WAREWIDGET_HPP__ */
