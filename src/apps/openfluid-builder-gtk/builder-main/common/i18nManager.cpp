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
#include <openfluid/base/RuntimeEnv.hpp>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>

#ifdef WIN32
#include <windows.h>
#else
#include <langinfo.h>
#endif

// =====================================================================
// =====================================================================


i18nManager* i18nManager::mp_Instance = 0;

// =====================================================================
// =====================================================================


i18nManager::i18nManager() :
  m_DefaultLanguage("en_US")
{
  m_CurrentLanguage = "";

  m_AvailableLanguages[m_DefaultLanguage] = "English (US)";
  m_AvailableLanguages["fr_FR"] = "Français (France)";

  // Languages for further works on translations
//  m_AvailableLanguages["es_ES"] = _("Spanish (Spain)");
//  m_AvailableLanguages["it_IT"] = _("Italian (Italy)");


  Glib::ustring EnvLang;

#ifdef WIN32

  EnvLang = g_win32_getlocale();

#else

  EnvLang = Glib::getenv("LANG");

#endif

  tryToSetCurrentLanguage(EnvLang);

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
  Language = Language.substr(0, 5);

  if (m_AvailableLanguages.find(Language) != m_AvailableLanguages.end())
  {

#ifdef WIN32

    m_CurrentLanguage = Language;

    Glib::setenv("LANGUAGE", m_CurrentLanguage);
    Glib::setenv("LANG", m_CurrentLanguage);

#else

    try
    {
      Language.append(".");

      /*
       * Returns well uppercased or lowercased "utf-8/UTF-8"
       */
      Language.append(nl_langinfo(CODESET));

      /*
       * Throws runtime_error if locale is not available on the system
       */
      std::locale(Language.c_str());

      setlocale(LC_MESSAGES, Language.c_str());

      m_CurrentLanguage = Language;

      Glib::setenv("LANGUAGE", m_CurrentLanguage);
      Glib::setenv("LANG", m_CurrentLanguage);
    }
    catch (std::runtime_error const& e)
    {
      /*
       * Remains unchanged if language already set, otherwise set to default one
       */
      if (m_CurrentLanguage.empty())
        setToDefaultLanguage();
    }
#endif
  }
  else
  {
    /*
     * Remains unchanged if language already set, otherwise set to default one
     */
    if (m_CurrentLanguage.empty())
      setToDefaultLanguage();
  }

  bindtextdomain(
      openfluid::config::NLS_PACKAGE.c_str(),
      openfluid::base::RuntimeEnvironment::getInstance()->getLocaleDir().c_str());
  bind_textdomain_codeset(openfluid::config::NLS_PACKAGE.c_str(), "UTF-8");
  textdomain(openfluid::config::NLS_PACKAGE.c_str());
}

// =====================================================================
// =====================================================================


void i18nManager::setToDefaultLanguage()
{
  std::locale::global(std::locale::classic());

  m_CurrentLanguage = m_DefaultLanguage;

  Glib::setenv("LANGUAGE", m_CurrentLanguage);
  Glib::setenv("LANG", m_CurrentLanguage);

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
