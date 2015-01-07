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
  @file ExtensionsRegistry.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#include "ExtensionsRegistry.hpp"
#include <openfluid/builderext/PluggableModelessExtension.hpp>


ExtensionsRegistry* ExtensionsRegistry::mp_Instance = NULL;


ExtensionsRegistry::ExtensionsRegistry():
  m_IsRegistered(false)
{

}


// =====================================================================
// =====================================================================


ExtensionsRegistry* ExtensionsRegistry::instance()
{
  if (!mp_Instance)
    mp_Instance = new ExtensionsRegistry();

  return mp_Instance;
}


// =====================================================================
// =====================================================================


ExtensionsRegistry::~ExtensionsRegistry()
{

}


// =====================================================================
// =====================================================================


void ExtensionsRegistry::registerExtensions()
{
  if (m_IsRegistered) return;

  std::vector<ExtensionContainer*> ExtVector = ExtensionPluginsManager::instance()->getAvailableWaresSignatures();

  for (unsigned int i=0; i<ExtVector.size(); i++)
    m_Extensions[ExtVector[i]->Signature->ID] = ExtVector[i];

  m_IsRegistered = true;
}


// =====================================================================
// =====================================================================


openfluid::builderext::PluggableBuilderExtension* ExtensionsRegistry::instanciateExtension(const openfluid::ware::WareID_t& ID)
{
  if (isExtensionRegistered(ID) && !m_Extensions[ID]->Active)
  {
    ExtensionPluginsManager::instance()->completeSignatureWithWareBody(m_Extensions[ID]);
    m_Extensions[ID]->Body->linkToRunEnvironment(openfluid::base::RuntimeEnvironment::instance()->wareEnvironment());
    m_Extensions[ID]->Body->initializeWare(ID);
    m_Extensions[ID]->Active = true;
    return m_Extensions[ID]->Body;
  }

  return NULL;
}


// =====================================================================
// =====================================================================


void ExtensionsRegistry::releaseExtension(const openfluid::ware::WareID_t& ID)
{
  if (isExtensionRegistered(ID))
  {
    m_Extensions[ID]->Active = false;
  }
}


// =====================================================================
// =====================================================================


void ExtensionsRegistry::releaseExtension(openfluid::builderext::PluggableBuilderExtension* Ext)
{
  releaseExtension(Ext->getID());
}


// =====================================================================
// =====================================================================


void ExtensionsRegistry::releaseAllExtensions()
{
  ExtensionsByName_t::iterator it;
  ExtensionsByName_t::iterator itb = m_Extensions.begin();
  ExtensionsByName_t::iterator ite = m_Extensions.end();

  for (it=itb;it!=ite;++it)
  {
    if ((*it).second->Active)
      (*it).second->Active = false;

    if ((*it).second->Body != NULL && (*it).second->Signature != NULL &&
        (*it).second->Signature->Type == openfluid::builderext::TYPE_MODELESS)
    {
      dynamic_cast<openfluid::builderext::PluggableModelessExtension*>((*it).second->Body)->close();
      dynamic_cast<openfluid::builderext::PluggableModelessExtension*>((*it).second->Body)->deleteLater();
    }


  }

}


// =====================================================================
// =====================================================================


bool ExtensionsRegistry::isExtensionRegistered(const openfluid::ware::WareID_t& ID)
{
  return (m_Extensions.find(ID) != m_Extensions.end());
}


// =====================================================================
// =====================================================================


openfluid::builderext::ExtensionType ExtensionsRegistry::getExtensionType(const openfluid::ware::WareID_t& ID)
{
  if (isExtensionRegistered(ID))
    return m_Extensions[ID]->Signature->Type;

  return openfluid::builderext::TYPE_UNKNOWN;
}

