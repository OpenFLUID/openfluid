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
  @file WareSignatureMacros.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_SHARE_MIGRATION_WARESIGNATUREMACROS_HPP__
#define __OPENFLUID_SHARE_MIGRATION_WARESIGNATUREMACROS_HPP__


/**
  Macro for declaration of the ware name
*/
#define DECLARE_NAME(name) \
  Signature->Name = (name);

/**
  Macro for declaration of the ware description
*/
#define DECLARE_DESCRIPTION(desc) \
  Signature->Description = (desc);

/**
  Macro for declaration of the ware author name and email
*/
#define DECLARE_AUTHOR(name,email) \
  Signature->Authors.push_back(std::make_pair((name),(email)));

/**
  Macro for declaration of the ware version
*/
#define DECLARE_VERSION(version) \
  Signature->Version = (version);

/**
  Macro for declaration of the ware status
*/
#define DECLARE_STATUS(status) \
  Signature->Status = (status);

/**
  Macro for declaration of ABI version used to build the ware
*/
#define DECLARE_SDKVERSION \
  Signature->BuildInfo.SDKVersion = openfluid::config::VERSION_FULL;


#endif /* __OPENFLUID_SHARE_MIGRATION_WARESIGNATUREMACROS_HPP__ */
