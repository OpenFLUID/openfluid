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
  \file BuilderExtensionSignature.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __BUILDEREXTENSIONSIGNATURE_HPP__
#define __BUILDEREXTENSIONSIGNATURE_HPP__


#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/config.hpp>

#include <QString>

/**
  Macro for the beginning of definition of signature hook
*/
#define BEGIN_BUILDEREXT_SIGNATURE(id,exttype) \
  openfluid::builderext::BuilderExtensionSignature* GetWareSignature() \
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


class BuilderExtensionSignature : public openfluid::ware::WareSignature
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
