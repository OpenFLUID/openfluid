/*
  This file is part of OpenFLUID-engine software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro

 == GNU General Public License Usage ==

  OpenFLUID-engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID-engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID-engine.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID-engine with code not covered 
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID-engine dynamically through the ofelib library
  interface, and only for building OpenFLUID-engine plugins. The files of
  Non-GPL Code may be link to the ofelib library without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID-engine code and other code used in conjunction with
  OpenFLUID-engine except the Non-GPL Code covered by this exception. If
  you modify this OpenFLUID-engine, you may extend this exception to your
  version of the file, but you are not obligated to do so. If you do not
  wish to provide this exception without modification, you must delete this
  exception statement from your version and license this OpenFLUID-engine
  solely under the GPL without exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID-Engine that is inconsistent with
  the GPL license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID-engine may use this file in
  accordance with the terms contained in the written agreement between
  You and INRA.
*/


/**
  \file OutputSetDescriptor.h
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __OUTPUTSETDESCRIPTOR_HPP__
#define __OUTPUTSETDESCRIPTOR_HPP__


#include <vector>
#include <string>
#include <openfluid/core.hpp>

namespace openfluid { namespace base {


class OutputSetDescriptor
{
  private:

    std::string m_Name;
    std::string m_UnitsClass;
    bool m_AllUnits;
    std::vector<openfluid::core::UnitID_t> m_UnitsIDs;
    std::vector<openfluid::core::Unit*> m_UnitsPtr;
    bool m_AllScalars;
    std::vector<std::string> m_ScalarVariables;
    bool m_AllVectors;
    std::vector<std::string> m_VectorVariables;
    unsigned int m_Precision;

  public:

    OutputSetDescriptor();

    ~OutputSetDescriptor();

    void setName(const std::string Name) { m_Name = Name; };

    std::string getName() const { return m_Name; };

    void setUnitsClass(const std::string UnitsClass)  { m_UnitsClass = UnitsClass; };

    std::string getUnitsClass() const { return m_UnitsClass; };

    unsigned int getPrecision() const { return m_Precision; };

    void setPrecision(const unsigned int Precision) { m_Precision = Precision; };

    void setAllUnits(bool AllUnits) { m_AllUnits = AllUnits; };

    bool isAllUnits() const {return m_AllUnits; };

    std::vector<openfluid::core::UnitID_t>& getUnitsIDs() { return m_UnitsIDs; };

    std::vector<openfluid::core::Unit*>& getUnitsPtr() { return m_UnitsPtr; };


    void setAllScalars(bool AllScalars) { m_AllScalars = AllScalars; };

    bool isAllScalars() const {return m_AllScalars; };

    std::vector<std::string>& getScalars() { return m_ScalarVariables; };

    void setAllVectors(bool AllVectors) { m_AllVectors = AllVectors; };

    bool isAllVectors() const {return m_AllVectors; };

    std::vector<std::string>& getVectors() { return m_VectorVariables; };

    void fillUnitsPtr();
};


} } // namespaces


#endif /* __OUTPUTSETDESCRIPTOR_HPP__ */
