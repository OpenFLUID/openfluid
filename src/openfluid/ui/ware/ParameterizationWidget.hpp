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
  @file ParameterizationWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/



#ifndef __OPENFLUID_UIWARE_PARAMETERIZATIONWIDGET_HPP__
#define __OPENFLUID_UIWARE_PARAMETERIZATIONWIDGET_HPP__


#include <QWidget>

#include <openfluid/ware/PluggableWare.hpp>
#include <openfluid/dllexport.hpp>

/**
  Macro for definition of parameters widget hook
  @param[in] widgetclassname The name of the class to instantiate
*/
#define DEFINE_PARAMETERIZATIONWIDGET_CLASS(widgetclassname) \
  extern "C" \
  { \
    OPENFLUID_PLUGIN openfluid::ui::ware::ParameterizationWidget* WAREPARAMSWIDGET_PROC_DECL() \
    { \
      return new widgetclassname(); \
    } \
  }


// =====================================================================
// =====================================================================


namespace openfluid { namespace ui { namespace ware {


class OPENFLUID_API ParameterizationWidget : public QWidget
{
  Q_OBJECT;

  protected:

    openfluid::ware::WareParams_t* mp_Params;

  signals:

    void changed();


  public:

    ParameterizationWidget() : QWidget()
    {

    }


    void linkParams(openfluid::ware::WareParams_t* Params)
    {
      mp_Params = Params;
    }


    virtual ~ParameterizationWidget()
    {

    }


    virtual void update() = 0;

};


} } }  // namespaces


#endif /* __OPENFLUID_UIWARE_PARAMETERIZATIONWIDGET_HPP__ */
