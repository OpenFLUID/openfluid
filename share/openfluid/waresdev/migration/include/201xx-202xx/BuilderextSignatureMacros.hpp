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
  @file BuilderextSignatureMacros.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_SHARE_MIGRATION_BUILDEREXTSIGNATUREMACROS_HPP__
#define __OPENFLUID_SHARE_MIGRATION_BUILDEREXTSIGNATUREMACROS_HPP__


#include <201xx-202xx/WareSignatureMacros.hpp>


/**
  Macro for the beginning of definition of signature for feature extension
*/
#ifndef BEGIN_BUILDEREXT_SIGNATURE
#define BEGIN_BUILDEREXT_SIGNATURE(id,extmode) \
  openfluid::builderext::BuilderExtensionSignature* generateSignature() \
  { \
    openfluid::builderext::BuilderExtensionSignature* Signature =\
      new openfluid::builderext::BuilderExtensionSignature(); \
    Signature->ID = (id); \
    Signature->Mode = (extmode);\
    Signature->Role = openfluid::builderext::ExtensionRole::FEATURE;
#endif

/**
  Macro for the end of definition of signature hook
*/
#ifndef END_BUILDEREXT_SIGNATURE
#define END_BUILDEREXT_SIGNATURE \
    return Signature; \
  }
#endif


// =====================================================================
// =====================================================================


/**
  Macro for declaration of a Builder extension configuration parameter
*/
#define DECLARE_CONFIGURATION_PARAMETER(name,description) \
  Signature->ConfigParameters[(name)] = (description);

/**
  @deprecated Since version 2.1.0. Use #DECLARE_CONFIGURATION_PARAMETER instead
*/
#define DECLARE_CONFIGURATION_PARAM(name,description) DECLARE_CONFIGURATION_PARAMETER(name,description)

/**
  Macro for declaration of the Builder extension category
*/
#define DECLARE_CATEGORY(category) \
  Signature->Category = (category);

/**
  Macro for declaration of the Builder extension menu text
*/
#define DECLARE_MENUTEXT(menutext) \
  Signature->MenuText = (menutext);


#endif /* __OPENFLUID_SHARE_MIGRATION_BUILDEREXTSIGNATUREMACROS_HPP__ */
