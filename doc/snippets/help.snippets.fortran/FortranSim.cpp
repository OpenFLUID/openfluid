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
  @file FortranSim.cpp

  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


#include <openfluid/ware/PluggableSimulator.hpp>


//! [fortran_include]
#include <openfluid/tools/FortranCPP.hpp>
//! [fortran_include]


//! [fortran_decl]
BEGIN_EXTERN_FORTRAN
  EXTERN_FSUBROUTINE(displayvector)(FINT *Size, FREAL8 *Vect);
END_EXTERN_FORTRAN
//! [fortran_decl]


BEGIN_SIMULATOR_SIGNATURE("help.snippets.fortran")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


class SnippetsSimulator : public openfluid::ware::PluggableSimulator
{
  private:

  public:


    SnippetsSimulator() : PluggableSimulator()
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

    }


    // =====================================================================
    // =====================================================================


    void checkConsistency()
    {

    }


    // =====================================================================
    // =====================================================================


//! [fortran_use]
openfluid::base::SchedulingRequest initializeRun()
{
  openfluid::core::VectorValue MyVect;
  
  MyVect = openfluid::core::VectorValue(15,9);
  int Size = MyVect.getSize();

  CALL_FSUBROUTINE(displayvector)(&Size,(MyVect.data()));

  return DefaultDeltaT();
}
//! [fortran_use]


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


DEFINE_SIMULATOR_CLASS(SnippetsSimulator)

