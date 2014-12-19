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
  \file ExtensionsRegistry.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OPENFLUID_BUILDERAPP_EXTENSIONSREGISTRY_HPP__
#define __OPENFLUID_BUILDERAPP_EXTENSIONSREGISTRY_HPP__

#include <QWidget>

#include "ExtensionPluginsManager.hpp"
#include "ExtensionContainer.hpp"


class ExtensionsRegistry
{
  public:

    typedef std::map<openfluid::ware::WareID_t, ExtensionContainer*> ExtensionsByName_t;

  private:

    static ExtensionsRegistry* mp_Instance;

    bool m_IsRegistered;

    ExtensionsByName_t m_Extensions;

    ExtensionsRegistry();

  public:

    static ExtensionsRegistry* getInstance();

    ~ExtensionsRegistry();


    void registerExtensions();

    ExtensionsByName_t* getRegisteredExtensions()
    { return &m_Extensions; };

    openfluid::builderext::PluggableBuilderExtension* instanciateExtension(const openfluid::ware::WareID_t& ID);

    void releaseExtension(const openfluid::ware::WareID_t& ID);

    void releaseExtension(openfluid::builderext::PluggableBuilderExtension* Ext);

    void releaseAllExtensions();

    bool isExtensionRegistered(const openfluid::ware::WareID_t& ID);

    bool isExtensionActive(const openfluid::ware::WareID_t& ID)
    { return (isExtensionRegistered(ID) && m_Extensions[ID]->Active); }

    openfluid::builderext::ExtensionType getExtensionType(const openfluid::ware::WareID_t& ID);
};



#endif /* __OPENFLUID_BUILDERAPP_EXTENSIONSREGISTRY_HPP__ */
