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
  @file WaresTranslationsRegistry.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_BUILDERAPP_WARESTRANSLATIONSREGISTRY_HPP__
#define __OPENFLUID_BUILDERAPP_WARESTRANSLATIONSREGISTRY_HPP__


#include <map>
#include <memory>

#include <QString>
#include <QTranslator>

#include <openfluid/utils/SingletonMacros.hpp>


class WaresTranslationsRegistry
{

  OPENFLUID_SINGLETON_DEFINITION(WaresTranslationsRegistry)

  private:

    typedef std::map<QString, std::unique_ptr<QTranslator>> TranslatorsByFile_t;

    TranslatorsByFile_t m_TranslatorsByFile;

    QString m_LangCode;

    WaresTranslationsRegistry();

    ~WaresTranslationsRegistry();


  public:

    QTranslator* tryLoadWareTranslation(const QString& WareFullPath);

    QString tryTranslate(const QString& WareFullPath, const QString& Context, const QString& Text);
};


#endif /* __OPENFLUID_BUILDERAPP_WARESTRANSLATIONSREGISTRY_HPP__ */
