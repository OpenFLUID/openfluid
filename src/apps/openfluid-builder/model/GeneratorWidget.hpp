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
  @file GeneratorWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_GENERATORWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_GENERATORWIDGET_HPP__


#include <openfluid/fluidx/ModelItemDescriptor.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>

#include "ModelItemWidget.hpp"


class GeneratorWidget : public ModelItemWidget
{
  Q_OBJECT;


  private:

    const openfluid::machine::ModelItemSignatureInstance* mp_SignInstance;
    
    void updateParametersList();
    
    bool isClickable() {return false;}


  public slots:

    void refresh();


  public:

    GeneratorWidget(QWidget* Parent,
                    openfluid::fluidx::ModelItemDescriptor* Desc,
                    const openfluid::ware::WareID_t& ID,
                    int Index,
                    const openfluid::machine::ModelItemSignatureInstance* SignInstance);

    ~GeneratorWidget();

    openfluid::ware::WareType getType()
    { return openfluid::ware::WareType::GENERATOR; }

};


#endif /* __OPENFLUID_BUILDERAPP_GENERATORWIDGET_HPP__ */
