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
  @file RSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


//! [R_include]
#include <RInside.h>
//! [R_include]

#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("help.snippets.R")


END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
//! [R_decl]
class SnippetsSimulator : public openfluid::ware::PluggableSimulator
{

  private:
       
    RInside m_R;

  public:
    
    // rest of the simulator class
//! [R_decl]


    SnippetsSimulator(): PluggableSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    ~SnippetsSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {

    }


    // =====================================================================
    // =====================================================================


    void prepareData()
    {
//! [R_use]
m_R["varA"] = 1.2;
m_R["varB"] = 5.3;
m_R.parseEvalQ("varC = max(varA,varB)");
// value of the R varC variable can be accessed from C++ through the m_R["varC"] variable
//! [R_use]
    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {

    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest initializeRun()
    {

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    void finalizeRun()
    {

    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(SnippetsSimulator);

