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
  @file WorkspaceWidget.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_WORKSPACEWIDGET_HPP__
#define __OPENFLUID_BUILDERAPP_WORKSPACEWIDGET_HPP__


#include <QWidget>

#include <openfluid/builderext/FluidXUpdateFlags.hpp>


namespace openfluid { namespace fluidx {
  class FluidXDescriptor;
} }


class WorkspaceWidget : public QWidget
{
  Q_OBJECT;

  protected:

    openfluid::fluidx::FluidXDescriptor& m_FluidxDesc;

  signals:

    void changed(openfluid::builderext::FluidXUpdateFlags::Flags UpdateFlags =
                 openfluid::builderext::FluidXUpdateFlags::Flag::FLUIDX_ALL);


  public slots:

    virtual void refresh()
    {

    };

  public:

    WorkspaceWidget(QWidget* Parent, openfluid::fluidx::FluidXDescriptor& Desc):
      QWidget(Parent), m_FluidxDesc(Desc)
    {

    };

};


#endif /* __OPENFLUID_BUILDERAPP_WORKSPACEWIDGET_HPP__ */
