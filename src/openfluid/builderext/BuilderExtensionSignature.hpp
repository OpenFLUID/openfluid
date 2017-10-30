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


#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>

#include <QString>


namespace openfluid { namespace builderext {


enum ExtensionCategory { CAT_SPATIAL, CAT_MODEL, CAT_RESULTS, CAT_OTHER };

enum ExtensionType { TYPE_UNKNOWN, TYPE_FEATURE, TYPE_PARAMETERIZATION};

enum ExtensionMode { MODE_UNKNOWN, MODE_MODAL, MODE_MODELESS, MODE_WORKSPACE};


class OPENFLUID_API BuilderExtensionSignature : public openfluid::ware::WareSignature
{
  public:

    ExtensionCategory Category;

    ExtensionMode Mode;

    ExtensionType Type;

    QString MenuText;

    std::map<QString,QString> ConfigParameters;


    BuilderExtensionSignature():
      Category(CAT_OTHER), Mode(MODE_UNKNOWN), Type(TYPE_UNKNOWN)
    {  };

};

} } // namespaces

#endif /* __OPENFLUID_BUILDEREXT_BUILDEREXTENSIONSIGNATURE_HPP__ */
