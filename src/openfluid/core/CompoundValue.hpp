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
  \file CompoundValue.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __COMPOUNDVALUE_HPP___
#define __COMPOUNDVALUE_HPP___

#include <vector>

#include <openfluid/core/Value.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace core {

class DLLEXPORT CompoundValue : public Value
{
  public:

  typedef std::vector<std::string> SeparatorsByLevels_t;

  protected:

    static SeparatorsByLevels_t m_StreamSeparators;

  public:

    static std::string getStreamSeparator(const unsigned int& Level);

    static void setStreamSeparator(const unsigned int& Level, const std::string& Sep);

    inline bool isSimple() const { return false; };

    inline bool isCompound() const { return true; };
};


} }  // namespaces



#endif /* __COMPOUNDVALUE_HPP___ */
