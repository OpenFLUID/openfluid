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
  @file singleton.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


// OpenFLUID:stylecheck:!incs


//! [singletonmacros_def]
// .hpp file

#include <openfluid/utils/SingletonMacros.hpp>

class ClassicExample
{
  OPENFLUID_SINGLETON_DEFINITION(ClassicExample)

  private:

    ClassicExample();

    ~ClassicExample();


  public:

    // some public members
};
//! [singletonmacros_def]

//! [singletonmacros_init]
// .cpp file

OPENFLUID_SINGLETON_INITIALIZATION(ClassicExample)
//! [singletonmacros_init]


//! [killablesingleton]
#include <openfluid/utils/KillableSingleton.hpp>

class ExampleKillable : public openfluid::utils::KillableSingleton<ExampleKillable>
{
  friend class openfluid::utils::KillableSingleton<ExampleKillable>;

  private:

    ExampleKillable();

    ~ExampleKillable();


  public:

    // some public members
};
//! [killablesingleton]


//! [meyersingleton]
#include <openfluid/utils/MeyerSingleton.hpp>

class MeyerExample : public openfluid::utils::MeyerSingleton<MeyerExample>
{
  friend class openfluid::utils::MeyerSingleton<MeyerExample>;


  private:

    MeyerExample();

    ~MeyerExample();


  public:

    // some public members
};
//! [meyersingleton]