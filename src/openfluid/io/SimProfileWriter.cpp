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
  \file SimProfileWriter.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/io/SimProfileWriter.hpp>
#include <openfluid/base/SimulationProfiler.hpp>
#include <openfluid/config.hpp>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>



namespace openfluid { namespace io {

// =====================================================================
// =====================================================================


double SimulationProfileWriter::getDurationInDecimalSeconds(const boost::posix_time::time_duration& Duration)
{
  return (static_cast<double>(Duration.total_seconds()) + (static_cast<double>(Duration.fractional_seconds()) / static_cast<double>(boost::posix_time::time_duration::ticks_per_second())));
}



// =====================================================================
// =====================================================================


void SimulationProfileWriter::saveToFiles(std::string Path)
{

  // TODO reactivate!
  return;

  if (!openfluid::base::SimulationProfiler::getInstance()->isEnabled()) return;

  std::ofstream ProfileFile;

  // profile by function
  ProfileFile.open(boost::filesystem::path(Path+"/"+openfluid::config::PROFILEFILE).string().c_str(),std::ios::out);
  ProfileFile << "#values are in seconds\n";
  ProfileFile << "#functionID;initParams();prepareData();checkConsistency();initializeRun();runStep()*;finalizeRun();TOTAL\n";

  ProfileFile << std::fixed << std::setprecision(9);

  openfluid::base::SimulationProfiler::ModelTimeProfile_t ModelProfile = openfluid::base::SimulationProfiler::getInstance()->getModelTimeProfile();

  BOOST_FOREACH(openfluid::base::SimulationProfiler::ModelTimeProfile_t::value_type &FuncVal, ModelProfile)
  {
    double Total = getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::INITPARAMS]) +
        getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::PREPAREDATA]) +
        getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::CHECKCONSISTENCY]) +
        getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::INITIALIZERUN]) +
        getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::RUNSTEP]) +
        getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::FINALIZERUN]);

    ProfileFile << FuncVal.first << ";";
    ProfileFile << getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::INITPARAMS]) << ";";
    ProfileFile << getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::PREPAREDATA]) << ";";
    ProfileFile << getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::CHECKCONSISTENCY]) << ";";
    ProfileFile << getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::INITIALIZERUN]) << ";";
    ProfileFile << getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::RUNSTEP]) << ";";
    ProfileFile << getDurationInDecimalSeconds(FuncVal.second[openfluid::base::SimulationProfiler::FINALIZERUN]) << ";";
    ProfileFile << Total << "\n";
  }

  ProfileFile.close();

  // profile by run step

  ProfileFile.open(boost::filesystem::path(Path+"/"+openfluid::config::PROFILESTEPSFILE).string().c_str(),std::ios::out);
  ProfileFile << "#values are in seconds\n";

  ProfileFile << std::fixed << std::setprecision(9);

  openfluid::base::SimulationProfiler::RunStepTimeProfile_t RunStepProfile = openfluid::base::SimulationProfiler::getInstance()->getRunStepTimeProfile();

  unsigned int CurrentStep = 0;

  ProfileFile << "#STEP";

  openfluid::base::SimulationProfiler::RunStepTimeProfile_t::iterator FuncIt;
  for (FuncIt = RunStepProfile.begin(); FuncIt != RunStepProfile.end(); ++FuncIt)
  {
    ProfileFile << ";";
    ProfileFile << (*FuncIt).first;
  }
  ProfileFile << "\n";

  while (!(*RunStepProfile.begin()).second.empty())
  {

    for (FuncIt = RunStepProfile.begin(); FuncIt != RunStepProfile.end(); ++FuncIt)
    {
      ProfileFile << CurrentStep << ";";
      ProfileFile << getDurationInDecimalSeconds((*FuncIt).second.front());
      (*FuncIt).second.pop_front();
    }
    ProfileFile << "\n";
    CurrentStep++;
  }
  ProfileFile << "\n";

  ProfileFile.close();

}

} } //namespaces


