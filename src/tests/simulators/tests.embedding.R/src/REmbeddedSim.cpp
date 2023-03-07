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
  @file REmbeddedSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <iomanip>

#include <RInside.h>

#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/Filesystem.hpp>


class REmbeddedSimulator : public openfluid::ware::PluggableSimulator
{
  private:
    
    /*
      R environment.
      As the RInside handle cannot be threaded outside the main thread, 
      it cannot be run in a threaded execution such as in OpenFLUID-Builder 
      and must be run using the `openfluid`command line
    */
    RInside m_R;

  public:


    REmbeddedSimulator(): PluggableSimulator()
    {

    }


    // =====================================================================
    // =====================================================================


    ~REmbeddedSimulator()
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
      m_R.parseEvalQ("verstr = R.version.string");
      std::string RVer = m_R["verstr"];

      std::cout << "\n" << RVer << std::endl;


      std::string INPath;
      OPENFLUID_GetRunEnvironment("dir.input",INPath);
      std::string ScriptPath = openfluid::tools::Filesystem::joinPath({INPath,"script.R"});

      m_R["scriptpath"] = ScriptPath;
      m_R.parseEvalQ("source(scriptpath)");
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
      openfluid::core::SpatialUnit* U;


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",U)
      {
        OPENFLUID_InitializeVariable(U,"varA",0.0);
        OPENFLUID_InitializeVariable(U,"varB",0.0);
        OPENFLUID_InitializeVariable(U,"varC",openfluid::core::VectorValue());
      }

      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================


    openfluid::base::SchedulingRequest runStep()
    {
      openfluid::core::SpatialUnit* U;
      double ValA;
      double ResA;
      std::vector<double> ValB;
      double ResB;
      double ValC;
      std::vector<double> ResC;


      OPENFLUID_UNITS_ORDERED_LOOP("TestUnits",U)
      {
        std::cout << "\n==== TestUnits#" << U->getID() << std::endl;
        OPENFLUID_GetAttribute(U,"indataA",ValA);
        ValB = {ValA,ValA,ValA/double(U->getID())};
        ValC = U->getID();

        // --------------------------------------

        m_R["valA"] = ValA;
        m_R.parseEvalQ("resA = computeA(valA)");
        m_R.parseEvalQ("cat('  R:',valA,'->',resA,'\n')");
        ResA = m_R["resA"];
        std::cout << std::setprecision(5) << "C++: " << ValA << " -> " << ResA << std::endl;
        OPENFLUID_AppendVariable(U,"varA",ResA);

        // --------------------------------------

        m_R["valB"] = ValB;
        m_R.parseEvalQ("resB = computeB(valB)");
        m_R.parseEvalQ("cat('  R:',valB,'->',resB,'\n')");
        ResB = m_R["resB"];
        std::cout << std::setprecision(5)
                  << "C++: " << ValB[0] << " " << ValB[1] << " " << ValB[2] << " -> " << ResB << std::endl;
        OPENFLUID_AppendVariable(U,"varB",ResB);

        // --------------------------------------

        m_R["valC"] = ValC;
        m_R.parseEvalQ("resC = computeC(valC)");
        m_R.parseEvalQ("cat('  R:',resC,'\n')");
        ResC = Rcpp::as<std::vector<double>>(m_R["resC"]);

        if (ResC.size() != 3)
        {
          OPENFLUID_RaiseError("Size of ResC vector is wrong");
        }

        std::cout << std::setprecision(5)
                  << "C++: " << ValC << " -> " << ResC[0] << "," << ResC[1] << "," << ResC[2] << std::endl;
        OPENFLUID_AppendVariable(U,"varC",openfluid::core::VectorValue(ResC.data(),ResC.size()));
      }

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


DEFINE_SIMULATOR_CLASS(REmbeddedSimulator);

