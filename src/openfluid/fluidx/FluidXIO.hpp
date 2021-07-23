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
  @file FluidXIO.hpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_FLUIDX_FLUIDXIO_HPP__
#define __OPENFLUID_FLUIDX_FLUIDXIO_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/fluidx/FluidXDescriptor.hpp>
#include <openfluid/base/IOListener.hpp>


namespace openfluid { namespace fluidx {


class OPENFLUID_API FluidXIO
{
  private:

    openfluid::base::IOListener* mp_Listener;


  public:

    FluidXIO(openfluid::base::IOListener* Listener);

    FluidXDescriptor loadFromDirectory(const std::string& DirPath) const;

    void writeToManyFiles(const FluidXDescriptor& Desc, const std::string& DirPath) const;

    void writeToSingleFile(const FluidXDescriptor& Desc, const std::string& FilePath) const;
};


} } // namespaces


#endif /* __OPENFLUID_FLUIDX_FLUIDXIO_HPP__ */
