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
  @file FluidXUpdateFlags.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUILDEREXT_FLUIDXUPDATEFLAGS_HPP__
#define __OPENFLUID_BUILDEREXT_FLUIDXUPDATEFLAGS_HPP__


#include <QFlags>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace builderext {


class OPENFLUID_API FluidXUpdateFlags
{
  public:

    enum class Flag
    {
      FLUIDX_MODELDEF = 1 << 0,
      FLUIDX_MODELPARAMS = 1 << 2,
      FLUIDX_SPATIALSTRUCT = 1 << 3,
      FLUIDX_SPATIALATTRS = 1 << 4,
      FLUIDX_SPATIALEVENTS = 1 << 5,
      FLUIDX_DATASTORE = 1 << 6,
      FLUIDX_MONITORING = 1 << 7,
      FLUIDX_RUNCONFIG = 1 << 8,
      FLUIDX_ALL = 1 << 9
    };

    Q_DECLARE_FLAGS(Flags, Flag)

};

Q_DECLARE_OPERATORS_FOR_FLAGS(FluidXUpdateFlags::Flags)

} }  // namespaces


#endif /* __OPENFLUID_BUILDEREXT_FLUIDXUPDATEFLAGS_HPP__ */
