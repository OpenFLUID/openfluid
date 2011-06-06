/*
 This file is part of OpenFLUID software
 Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
 along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

 In addition, as a special exception, INRA gives You the additional right
 to dynamically link the code of OpenFLUID with code not covered
 under the GNU General Public License ("Non-GPL Code") and to distribute
 linked combinations including the two, subject to the limitations in this
 paragraph. Non-GPL Code permitted under this exception must only link to
 the code of OpenFLUID dynamically through the OpenFLUID libraries
 interfaces, and only for building OpenFLUID plugins. The files of
 Non-GPL Code may be link to the OpenFLUID libraries without causing the
 resulting work to be covered by the GNU General Public License. You must
 obey the GNU General Public License in all respects for all of the
 OpenFLUID code and other code used in conjunction with OpenFLUID
 except the Non-GPL Code covered by this exception. If you modify
 this OpenFLUID, you may extend this exception to your version of the file,
 but you are not obligated to do so. If you do not wish to provide this
 exception without modification, you must delete this exception statement
 from your version and license this OpenFLUID solely under the GPL without
 exception.


 == Other Usage ==

 Other Usage means a use of OpenFLUID that is inconsistent with the GPL
 license, and requires a written agreement between You and INRA.
 Licensees for Other Usage of OpenFLUID may use this file in accordance
 with the terms contained in the written agreement between You and INRA.
 */

/**
 \file i18nManager.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "i18nManager.hpp"

#include <boost/filesystem.hpp>
#include <openfluid/config.hpp>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>

#include <iostream>

// =====================================================================
// =====================================================================


i18nManager* i18nManager::mp_Instance = 0;

// =====================================================================
// =====================================================================


i18nManager::i18nManager() :
  m_DefaultLanguage("en_US.UTF-8")
{
  m_CurrentLanguage = "";

  m_AvailableLanguages[m_DefaultLanguage] = _("English (US)");
  m_AvailableLanguages["fr_FR.UTF-8"] = _("French (France)");
  m_AvailableLanguages["es_ES.UTF-8"] = _("Spanish (Spain)");
  m_AvailableLanguages["it_IT.UTF-8"] = _("Italian (Italy)");

  tryToSetCurrentLanguage(Glib::getenv("LANG"));
}

// =====================================================================
// =====================================================================


i18nManager* i18nManager::getInstance()
{
  if (!mp_Instance)
    mp_Instance = new i18nManager();

  return mp_Instance;
}

// =====================================================================
// =====================================================================


std::map<Glib::ustring, Glib::ustring> i18nManager::getAvailableLanguages()
{
  return m_AvailableLanguages;
}

// =====================================================================
// =====================================================================


Glib::ustring i18nManager::getCurrentLanguage()
{
  return m_CurrentLanguage;
}

// =====================================================================
// =====================================================================


void i18nManager::tryToSetCurrentLanguage(Glib::ustring Language)
{
  if (m_AvailableLanguages.find(Language) != m_AvailableLanguages.end())
  {
    try
    {
      /* throws runtime_error if locale is not available on the system
       * else call also setlocale(LC_ALL,)
       */
      std::locale::global(std::locale(Language.c_str()));

      m_CurrentLanguage = Language;

      Glib::setenv("LANGUAGE", m_CurrentLanguage);
      Glib::setenv("LANG", m_CurrentLanguage);

      std::cout << "Language set to " << m_CurrentLanguage << std::endl;

    } catch (std::runtime_error const& e)
    {
      std::cout << Language << " is not available on your system. ";

      if (m_CurrentLanguage.empty())
        std::cout << setToDefaultLanguage();
      else
        std::cout << "Language not changed.";

      std::cout << std::endl;
    }
  } else
  {
    std::cout << Language << " is not (yet) available for OpenFLUID Builder. ";

    if (m_CurrentLanguage.empty())
      std::cout << setToDefaultLanguage();
    else
      std::cout << "Language not changed.";

    std::cout << std::endl;
  }

  bindtextdomain(OPENFLUID_NLS_PACKAGE, OPENFLUID_NLS_LOCALEDIR);
  bind_textdomain_codeset(OPENFLUID_NLS_PACKAGE, "UTF-8");
  textdomain(OPENFLUID_NLS_PACKAGE);

}

// =====================================================================
// =====================================================================


Glib::ustring i18nManager::setToDefaultLanguage()
{
  std::locale::global(std::locale::classic());

  m_CurrentLanguage = m_DefaultLanguage;

  Glib::setenv("LANGUAGE", m_CurrentLanguage);
  Glib::setenv("LANG", m_CurrentLanguage);

  return Glib::ustring::compose("Set to 'C' locale and default language: %1",
      m_DefaultLanguage);
}

// =====================================================================
// =====================================================================


void i18nManager::setNLSEnable(bool Enable)
{
  m_isNLSEnable = Enable;
}

// =====================================================================
// =====================================================================


bool i18nManager::isNLSenable()
{
  return m_isNLSEnable;
}
