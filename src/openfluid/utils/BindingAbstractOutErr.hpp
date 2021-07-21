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
  @file BindingAbstractOutErr.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#ifndef __OPENFLUID_UTILS_BINDINGABSTRACTOUTERR_HPP__
#define __OPENFLUID_UTILS_BINDINGABSTRACTOUTERR_HPP__


#include <cstdarg>


namespace openfluid { namespace utils {


class BindingAbstractOutErr
{
  public:

    /**
      Prints to the standard output stream. This method must be redefined by a derived class
      @param[in] fmt The format string for the arguments to print
      @param[in] ... The variadic arguments to print
    */
    virtual void printfOut(const char* fmt, ... ) const = 0;

    /**
      Prints to the standard error stream. This method must be redefined by a derived class
      @param[in] fmt The format string for the arguments to print
      @param[in] ... The variadic arguments to print

    */
    virtual void printfErr(const char* fmt, ... ) const = 0;
};


} } // namespaces


#endif /* __OPENFLUID_UTILS_BINDINGABSTRACTOUTERR_HPP__ */
