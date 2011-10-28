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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTSETDESCRIPTOR_HPP__
#define __OUTPUTSETDESCRIPTOR_HPP__


#include <vector>
#include <string>
#include <openfluid/dllexport.hpp>
#include <openfluid/core.hpp>

namespace openfluid { namespace base {


class DLLEXPORT OutputSetDescriptor
{
  private:

    std::string m_Name;
    std::string m_UnitsClass;
    bool m_AllUnits;
    std::vector<openfluid::core::UnitID_t> m_UnitsIDs;
    std::vector<openfluid::core::Unit*> m_UnitsPtr;
    bool m_AllVariables;
    std::vector<std::string> m_Variables;
    unsigned int m_Precision;

  public:

    OutputSetDescriptor();

    ~OutputSetDescriptor();

    inline void setName(const std::string Name) { m_Name = Name; };

    inline std::string getName() const { return m_Name; };

    inline void setUnitsClass(const std::string UnitsClass)  { m_UnitsClass = UnitsClass; };

    inline std::string getUnitsClass() const { return m_UnitsClass; };

    inline unsigned int getPrecision() const { return m_Precision; };

    inline void setPrecision(const unsigned int Precision) { m_Precision = Precision; };

    inline void setAllUnits(bool AllUnits) { m_AllUnits = AllUnits; };

    inline bool isAllUnits() const {return m_AllUnits; };

    inline std::vector<openfluid::core::UnitID_t>& getUnitsIDs() { return m_UnitsIDs; };

    inline const std::vector<openfluid::core::UnitID_t>& getUnitsIDs() const { return m_UnitsIDs; };

    inline std::vector<openfluid::core::Unit*>& getUnitsPtr() { return m_UnitsPtr; };


    void setAllVariables(bool AllVariables) { m_AllVariables = AllVariables; };

    bool isAllVariables() const {return m_AllVariables; };

    std::vector<std::string>& getVariables() { return m_Variables; };

    const std::vector<std::string>& getVariables() const { return m_Variables; };

    std::vector<std::string> getVariablesNameOnly() const;
};


} } // namespaces


#endif /* __OUTPUTSETDESCRIPTOR_HPP__ */
