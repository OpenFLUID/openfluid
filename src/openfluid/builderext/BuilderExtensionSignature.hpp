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
  @file BuilderExtensionSignature.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__
#define __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__


#include <QString>

#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace builderext {


enum class ExtensionCategory { SPATIAL, MODEL, RESULTS, OTHER };

enum class ExtensionRole { UNKNOWN, FEATURE, PARAMETERIZATION};

enum class ExtensionMode { UNKNOWN, MODAL, MODELESS, WORKSPACE};


class OPENFLUID_API BuilderExtensionSignature : public openfluid::ware::WareSignature
{
  public:

    ExtensionCategory Category;

    ExtensionMode Mode;

    ExtensionRole Role;

    QString MenuText;

    std::map<QString,QString> ConfigParameters;


    BuilderExtensionSignature():
      Category(ExtensionCategory::OTHER), Mode(ExtensionMode::UNKNOWN), Role(ExtensionRole::UNKNOWN)
    {  }

    virtual ~BuilderExtensionSignature() = default;

    openfluid::ware::WareType getType() const
    {
      return openfluid::ware::WareType::BUILDEREXT;
    }

};

} } // namespaces

#endif /* __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__ */
