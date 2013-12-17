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
  \file ObserverWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OBSERVERWIDGET_HPP__
#define __OBSERVERWIDGET_HPP__


#include <openfluid/fluidx/ObserverDescriptor.hpp>
#include "WareWidget.hpp"


class ObserverWidget : public WareWidget
{
  Q_OBJECT;

  private:

    openfluid::fluidx::ObserverDescriptor* mp_Desc;

    void updateParams();


  private slots:

    void setEnabledWare(bool Enabled);

    void addParam();

    void updateParamValue(const QString& Name, const QString& Value);

    void removeParam(const QString& Name);


  public slots:

    void refresh();


  public:

    ObserverWidget(QWidget* Parent,
                   openfluid::fluidx::ObserverDescriptor* Desc,
                   const openfluid::ware::WareID_t& ID);

    ~ObserverWidget();

    void updateWare();

    openfluid::fluidx::WareDescriptor::WareType getType()
    { return openfluid::fluidx::WareDescriptor::PluggedObserver; }

};



#endif /* __OBSERVERWIDGET_HPP__ */
