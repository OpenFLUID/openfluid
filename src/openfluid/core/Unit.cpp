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


#include <openfluid/core/Unit.hpp>

namespace openfluid { namespace core {


// =====================================================================
// =====================================================================


Unit::Unit(const UnitClass_t aClass, const UnitID_t anID,
           const PcsOrd_t aPcsOrder) :
  m_ID(anID), m_Class(aClass), m_PcsOrder(aPcsOrder)
{

}


// =====================================================================
// =====================================================================


Unit::~Unit()
{

}

// =====================================================================
// =====================================================================


bool Unit::addToUnit(Unit* aUnit)
{
  m_ToUnits[aUnit->getClass()].push_back(aUnit);
  return true;
}

// =====================================================================
// =====================================================================


bool Unit::addFromUnit(Unit* aUnit)
{
  m_FromUnits[aUnit->getClass()].push_back(aUnit);
  return true;

}

// =====================================================================
// =====================================================================


bool Unit::addParentUnit(Unit* aUnit)
{
  m_ParentUnits[aUnit->getClass()].push_back(aUnit);
  return true;
}

// =====================================================================
// =====================================================================


bool Unit::addChildUnit(Unit* aUnit)
{
  m_ChildrenUnits[aUnit->getClass()].push_back(aUnit);
  return true;

}

// =====================================================================
// =====================================================================


const UnitsPtrList_t* Unit::getToUnits(const UnitClass_t aClass) const
{
  return const_cast<UnitsPtrList_t*>(getToUnits(aClass));

}

// =====================================================================
// =====================================================================


const UnitsPtrList_t* Unit::getChildrenUnits(const UnitClass_t aClass) const
{
  return const_cast<UnitsPtrList_t*>(getChildrenUnits(aClass));
}


// =====================================================================
// =====================================================================


const UnitsPtrList_t* Unit::getParentUnits(const UnitClass_t aClass) const
{
  return const_cast<UnitsPtrList_t*>(getParentUnits(aClass));

}

// =====================================================================
// =====================================================================


const UnitsPtrList_t* Unit::getFromUnits(const UnitClass_t aClass) const
{
  return const_cast<UnitsPtrList_t*>(getFromUnits(aClass));
}


// =====================================================================
// =====================================================================


UnitsPtrList_t* Unit::getToUnits(const UnitClass_t aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ToUnits.find(aClass);

  if (it != m_ToUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================


UnitsPtrList_t* Unit::getFromUnits(const UnitClass_t aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_FromUnits.find(aClass);

  if (it != m_FromUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================


UnitsPtrList_t* Unit::getParentUnits(const UnitClass_t aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ParentUnits.find(aClass);

  if (it != m_ParentUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================


UnitsPtrList_t* Unit::getChildrenUnits(const UnitClass_t aClass)
{
  LinkedUnitsListByClassMap_t::iterator it = m_ChildrenUnits.find(aClass);

  if (it != m_ChildrenUnits.end())
    return &(it->second);
  else return NULL;
}

// =====================================================================
// =====================================================================



void Unit::streamContents(std::ostream& OStream)
{
  UnitsPtrList_t::iterator IDIt;
  LinkedUnitsListByClassMap_t::iterator ClassIt;
  UnitsPtrList_t UnitsList;

  OStream << m_Class << " #"<< m_ID << " (order: " << m_PcsOrder << ")";

  if (m_ToUnits.size() > 0 )
  {
    OStream << " To[";

    for (ClassIt=m_ToUnits.begin();ClassIt!=m_ToUnits.end();++ClassIt)
    {

      UnitsList = ClassIt->second;

     for (IDIt=UnitsList.begin();IDIt!=UnitsList.end();++IDIt)
      {
       OStream << "(" << (*IDIt)->getClass() << "," <<  (*IDIt)->getID() << ") ";
      }

    }
    OStream << "] ";
  }

  if (m_FromUnits.size() > 0 )
  {
    OStream << " From[";

    for (ClassIt=m_FromUnits.begin();ClassIt!=m_FromUnits.end();++ClassIt)
    {

      UnitsList = ClassIt->second;

     for (IDIt=UnitsList.begin();IDIt!=UnitsList.end();++IDIt)
      {
       OStream << "(" << (*IDIt)->getClass() << "," <<  (*IDIt)->getID() << ") ";
      }

    }
    OStream << "] ";
  }

  OStream << std::endl;
}



} } // namespaces
