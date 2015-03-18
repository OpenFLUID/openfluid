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
  @file Sim2DocSim.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
*/


/*
<sim2doc>

\begin{center}
\includegraphics[scale=0.4]{doc/images/openfluid_official.png}
\end{center}


This is the \LaTeX -style documentation of the \simID\ function (\simNAME, version \simVERSION).\\

\input{doc/part1}
\input{doc/part2}

\cite{Moussa2002b}

\bibliographystyle{plain}
\bibliography{doc/bibliography/MHYDAS_OpenFLUID}

</sim2doc>

*/


#include "Sim2DocSim.h"


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(Sim2DocSimulator)


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("tests.ofefunc2doc")

  DECLARE_NAME("test simulator for ofefunc2doc");
  DECLARE_DESCRIPTION("This function is a test for the ofefunc2doc doc extraction tool. "
      "Do not use it in a model.");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::BETA);

  DECLARE_DOMAIN("Test");
  DECLARE_PROCESS("No process");
  DECLARE_METHOD("No method");
  DECLARE_AUTHOR("JC Fabre","jean-christophe.fabre@supagro.inra.fr");
  DECLARE_AUTHOR("Manuel Chataigner","manuel.chataigner@supagro.inra.fr");

  DECLARE_USED_PARAMETER("testparam","a param just for test","kg/m3")
  DECLARE_USED_PARAMETER("dummy_param","a dummy param just for test","snail/m2")


  /**
   *  Multiple lines of comments
   *  to test the ignoring of linemarkers
   *  inserted by the preprocessor to mean
   *  the original position of lines in the source file.
  */


  DECLARE_PRODUCED_VARIABLE("tests.prodvector[]","TestUnits","produced vector on TestUnits","");
  DECLARE_PRODUCED_VARIABLE("tests.prodscalar","TestUnits","produced scalar on TestUnits","joker/batman");

  DECLARE_UPDATED_VARIABLE("tests.updscalar","TestUnits","updated scalar on TestUnits","");

  DECLARE_REQUIRED_VARIABLE("tests.reqvector[]","TestUnits","required vector on TestUnits","m2");
  DECLARE_REQUIRED_VARIABLE("tests.reqscalar","TestUnits","required scalar on TestUnits","");

  DECLARE_USED_VARIABLE("tests.usedscalar","TestUnits","used scalar on TestUnits","");

  DECLARE_PRODUCED_ATTRIBUTE("attr1","TestUnits","a description","")
  DECLARE_REQUIRED_ATTRIBUTE("indataA","TestUnits","required attribute A on TestUnits","m")
  DECLARE_USED_ATTRIBUTE("indataB","TestUnits","used attribute B on TestUnits","mm/h")
  DECLARE_USED_ATTRIBUTE("indata_CC","TestUnits","used attribute CC on TestUnits","")

  DECLARE_USED_EVENTS("TestUnits");
  DECLARE_USED_EVENTS("FakeTestUnits");

  DECLARE_USED_EXTRAFILE("test01.txt");
  DECLARE_USED_EXTRAFILE("test02.txt");
  DECLARE_REQUIRED_EXTRAFILE("test03req.txt");

  DECLARE_UPDATED_UNITSGRAPH("a unitsgraph description")
  DECLARE_UPDATED_UNITSCLASS("TestUnits","a unitsclass description")

  DECLARE_SCHEDULING_UNDEFINED
  DECLARE_SCHEDULING_DEFAULT
  DECLARE_SCHEDULING_FIXED(52)
  DECLARE_SCHEDULING_RANGE(2,3)


END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


Sim2DocSimulator::Sim2DocSimulator()
                : PluggableSimulator()
{


}


// =====================================================================
// =====================================================================


Sim2DocSimulator::~Sim2DocSimulator()
{


}


// =====================================================================
// =====================================================================


void Sim2DocSimulator::initParams(const openfluid::ware::WareParams_t& /*Params*/)
{

}


// =====================================================================
// =====================================================================


void Sim2DocSimulator::prepareData()
{

}


// =====================================================================
// =====================================================================


void Sim2DocSimulator::checkConsistency()
{

}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest Sim2DocSimulator::initializeRun()
{
  return DefaultDeltaT();
}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest Sim2DocSimulator::runStep()
{

  return Never();
}


// =====================================================================
// =====================================================================


void Sim2DocSimulator::finalizeRun()
{

}

