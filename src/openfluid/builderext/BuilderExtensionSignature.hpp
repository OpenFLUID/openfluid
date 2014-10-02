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
  \file BuilderExtensionSignature.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __BUILDEREXTENSIONSIGNATURE_HPP__
#define __BUILDEREXTENSIONSIGNATURE_HPP__


#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>

#include <QString>

/**
  Macro for the beginning of definition of signature hook
*/
#define BEGIN_BUILDEREXT_SIGNATURE(id,exttype) \
  openfluid::builderext::BuilderExtensionSignature* WARESIGNATURE_PROC_DECL() \
  { \
    openfluid::builderext::BuilderExtensionSignature* Signature = new openfluid::builderext::BuilderExtensionSignature(); \
    Signature->setABIVersion(openfluid::config::FULL_VERSION); \
    Signature->ID = (id); \
    Signature->Type = (exttype);


/**
  Macro for the end of definition of signature hook
*/
#define END_BUILDEREXT_SIGNATURE \
    return Signature; \
  }


/**
  Macro for declaration of a Builder extension configuration parameter
  @param[in] name name of the parameter
  @param[in] description description of the parameter
*/
#define DECLARE_CONFIGURATION_PARAM(name,description) \
  Signature->ConfigParameters[(name)] = (description);


/**
  Macro for declaration of the Builder extension category
*/
#define DECLARE_CATEGORY(category) Signature->Category = (category);


/**
  Macro for declaration of the Builder extension menu text
*/
#define DECLARE_MENUTEXT(menutext) Signature->MenuText = (menutext);


// =====================================================================
// =====================================================================


#include <QString>


namespace openfluid { namespace builderext {


enum ExtensionCategory { CAT_SPATIAL, CAT_MODEL, CAT_RESULTS, CAT_OTHER };

enum ExtensionType { TYPE_UNKNOWN, TYPE_MODAL, TYPE_MODELESS, TYPE_WORKSPACE};


class DLLEXPORT BuilderExtensionSignature : public openfluid::ware::WareSignature
{
  public:

    ExtensionCategory Category;

    ExtensionType Type;

    QString MenuText;

    std::map<QString,QString> ConfigParameters;


    BuilderExtensionSignature():
      Category(CAT_OTHER), Type(TYPE_UNKNOWN)
    {  };

};

} } // namespaces

#endif /* __BUILDEREXTENSIONSIGNATURE_HPP__ */
