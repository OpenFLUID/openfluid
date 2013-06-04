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
  \file OFEFunc2DocSim.cpp
  \brief Implements ...
*/



/*
<sim2doc>

\begin{center}
\includegraphics[scale=0.4]{doc/images/openfluid_official.png}
\end{center}


This is the \LaTeX -style documentation of the \funcID\ function (\funcNAME, version \funcVERSION).\\

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
  DECLARE_DESCRIPTION("This function is a test for the ofefunc2doc doc extraction tool. Do not use it in a model.");

  DECLARE_VERSION("1.0");
  DECLARE_SDKVERSION;
  DECLARE_STATUS(openfluid::ware::BETA);

  DECLARE_DOMAIN("Test");
  DECLARE_PROCESS("No process");
  DECLARE_METHOD("No method");
  DECLARE_AUTHOR("JC Fabre","fabrejc@supagro.inra.fr");

  DECLARE_SIMULATOR_PARAM("testparam","a param just for test","kg/m3")
  DECLARE_SIMULATOR_PARAM("dummy_param","a dummy param just for test","snail/m2")

  DECLARE_PRODUCED_VAR("tests.prodvector[]","TestUnits","produced vector on TestUnits","");
  DECLARE_PRODUCED_VAR("tests.prodscalar","TestUnits","produced scalar on TestUnits","joker/batman");

  DECLARE_UPDATED_VAR("tests.updscalar","TestUnits","updated scalar on TestUnits","");

  DECLARE_REQUIRED_VAR("tests.reqvector[]","TestUnits","required vector on TestUnits","m2");
  DECLARE_REQUIRED_VAR("tests.reqscalar","TestUnits","required scalar on TestUnits","");

  DECLARE_USED_VAR("tests.usedscalar","TestUnits","used scalar on TestUnits","");

  DECLARE_REQUIRED_INPUTDATA("indataA","TestUnits","required input data A on TestUnits","m")
  DECLARE_USED_INPUTDATA("indataB","TestUnits","used input data B on TestUnits","mm/h")
  DECLARE_USED_INPUTDATA("indata_CC","TestUnits","used input data CC on TestUnits","")

  DECLARE_USED_EVENTS("TestUnits");
  DECLARE_USED_EVENTS("FakeTestUnits");

  DECLARE_USED_EXTRAFILE("test01.txt");
  DECLARE_USED_EXTRAFILE("test02.txt");
  DECLARE_REQUIRED_EXTRAFILE("test03req.txt");

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

