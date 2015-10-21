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
  @file DistributionBindings.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@supagro.inra.fr>
 */


#ifndef __OPENFLUID_TOOLS_DISTRIBUTIONBINDINGS_HPP__
#define __OPENFLUID_TOOLS_DISTRIBUTIONBINDINGS_HPP__

#include <openfluid/tools/DistributionTables.hpp>
#include <openfluid/tools/ProgressiveChronFileReader.hpp>
#include <openfluid/dllexport.hpp>


namespace openfluid { namespace tools {


class ReaderNextValue
{
  public:

    ProgressiveChronFileReader* Reader;

    ChronItem_t NextValue;

    bool isAvailable;

    ReaderNextValue(): Reader(nullptr), isAvailable(false)
    { }
};


// =====================================================================
// =====================================================================


class OPENFLUID_API DistributionBindings
{
  public:

    typedef std::map<openfluid::core::UnitID_t,ReaderNextValue*> UnitIDReader_t;

    typedef std::list<ReaderNextValue> ReadersNextValues_t;


  private:

    UnitIDReader_t m_UnitIDReaders;

    ReadersNextValues_t m_ReadersNextValues;

  public:

    DistributionBindings(const DistributionTables& DistriTables);

    ~DistributionBindings();

    void advanceToTime(const openfluid::core::DateTime& DT);

    bool advanceToNextTimeAfter(const openfluid::core::DateTime& DT, openfluid::core::DateTime& NextDT);

    bool getValue(const openfluid::core::UnitID_t& UnitID,
                  const openfluid::core::DateTime& DT,
                  openfluid::core::DoubleValue& Value);

    void displayBindings();

};


} }



#endif /* __OPENFLUID_TOOLS_DISTRIBUTIONBINDINGS_HPP__ */
