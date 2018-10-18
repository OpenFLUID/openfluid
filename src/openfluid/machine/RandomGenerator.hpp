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
  @file RandomGenerator.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_MACHINE_RANDOMGENERATOR_HPP__
#define __OPENFLUID_MACHINE_RANDOMGENERATOR_HPP__


#include <random>

#include <openfluid/dllexport.hpp>
#include <openfluid/machine/Generator.hpp>


namespace openfluid { namespace machine {


class OPENFLUID_API RandomGenerator : public Generator
{
  private:

    openfluid::core::DoubleValue m_Min;

    openfluid::core::DoubleValue m_Max;

    openfluid::core::Duration_t m_DeltaT;

    std::mt19937 m_RandomEngine;


  public:

    RandomGenerator();

    ~RandomGenerator();

    void initParams(const openfluid::ware::WareParams_t& Params);

    void checkConsistency();

    openfluid::base::SchedulingRequest initializeRun();

    openfluid::base::SchedulingRequest runStep();

    void finalizeRun()
    { }

};


} }  //namespaces


#endif /* __OPENFLUID_MACHINE_RANDOMGENERATOR_HPP__ */
