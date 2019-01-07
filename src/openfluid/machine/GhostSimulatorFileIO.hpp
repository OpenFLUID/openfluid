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
  @file GhostSimulatorFileIO.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_MACHINE_GHOSTSIMULATORFILEIO_HPP__
#define __OPENFLUID_MACHINE_GHOSTSIMULATORFILEIO_HPP__


#include <openfluid/dllexport.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>


namespace openfluid { namespace machine {


/**
  Manages I/O of ghost simulators signatures
*/
class OPENFLUID_API GhostSimulatorFileIO
{
  public:

    GhostSimulatorFileIO() = delete;

    /**
      Saves given signature of a ghost simulator to file
      @param[in] Signature The signature to save to file
      @param[in] DirPath The directory path where the signature is saved as a file. 
                         The name of the created file is automatically created using the simulator ID.
      @return true if the signature was successfully saved, false otherwise
    */
    static bool saveToFile(const openfluid::ware::SimulatorSignature& Signature, const std::string& DirPath);

    /**
      Loads a signature of a ghost simulator from a file
      @param[in] FilePath The path to the file to load
      @param[out] Signature The signature loaded from file
      @return true if the signature was successfully loaded, false otherwise
    */
    static bool loadFromFile(const std::string& FilePath, openfluid::ware::SimulatorSignature& Signature);
};


} }  // namespaces


#endif /* __OPENFLUID_MACHINE_GHOSTSIMULATORFILEIO_HPP__ */
