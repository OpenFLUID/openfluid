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
  @file MultiInjectGenerator.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
*/


#ifndef __OPENFLUID_MACHINE_MULTIINJECTGENERATOR_HPP__
#define __OPENFLUID_MACHINE_MULTIINJECTGENERATOR_HPP__


#include <queue>
#include <map>

#include <openfluid/dllexport.hpp>
#include <openfluid/machine/Generator.hpp>
#include <openfluid/tools/DistributionBindings.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API MultiInjectGenerator : public Generator
{
  private:

    std::string m_DataFile;

    std::string m_Selection;

    std::map<std::string, std::set<std::string>> m_VarsByUnitsClass;

    openfluid::tools::ReaderNextValue<std::vector<std::string>> m_ReaderNextValue;
    
    openfluid::tools::MulticolDistributionBindings* m_DistriBindings;


  public:

    MultiInjectGenerator(const std::string& m_Selection="");

    ~MultiInjectGenerator();

    void initParams(const openfluid::ware::WareParams_t& Params);

    void prepareData();

    void checkConsistency();

    openfluid::base::SchedulingRequest initializeRun();

    openfluid::base::SchedulingRequest runStep();

    void finalizeRun()
    { }

};


} } //namespaces


#endif /* __OPENFLUID_MACHINE_MULTIINJECTGENERATOR_HPP__ */
