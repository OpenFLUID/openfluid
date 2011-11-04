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
 \file EngineHelper.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "EngineHelper.hpp"

#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <glibmm/i18n.h>

#include <openfluid/core/DateTime.hpp>

// =====================================================================
// =====================================================================


std::set<std::string> EngineHelper::getClassNames(
    openfluid::core::CoreRepository* CoreRepos)
{
  std::set<std::string> ClassNames;

  for (openfluid::core::UnitsListByClassMap_t::const_iterator it =
      CoreRepos->getUnitsByClass()->begin(); it
      != CoreRepos->getUnitsByClass()->end(); ++it)
  {
    if (!it->second.getList()->empty())
      ClassNames.insert(it->first);
  }

  return ClassNames;
}

// =====================================================================
// =====================================================================


std::set<int> EngineHelper::getIDs(openfluid::core::CoreRepository* CoreRepos,
    std::string ClassName)
{
  std::set<int> IDs;

  if (CoreRepos->getUnits(ClassName) != NULL)
  {
    std::list<openfluid::core::Unit>* p_Units =
        CoreRepos->getUnits(ClassName)->getList();

    for (std::list<openfluid::core::Unit>::iterator it = p_Units->begin(); it
        != p_Units->end(); ++it)
      IDs.insert(static_cast<int> (it->getID()));
  }

  return IDs;
}

// =====================================================================
// =====================================================================


std::set<std::string> EngineHelper::getProducedVarNames(std::string ClassName,
    openfluid::machine::ModelInstance* ModelInstance)
{
  std::set<std::string> VarNames;

  std::string OnlyVarName;
  openfluid::core::Value::Type VarType;

  BOOST_FOREACH(openfluid::machine::ModelItemInstance* Item,ModelInstance->getItems())
{  BOOST_FOREACH(openfluid::base::SignatureHandledDataItem Var,Item->Signature->HandledData.ProducedVars)
  {
    if(Var.UnitClass == ClassName)
    {
      OnlyVarName = "";

      if(!openfluid::tools::GetVariableNameAndType(Var.DataName,OnlyVarName,VarType))
        throw openfluid::base::OFException("OpenFLUID builder","EngineHelper::getProducedVarNames","Variable " + Var.DataName + " for " + Item->Signature->ID + " is not well formated.");

      VarNames.insert(OnlyVarName);
    }
  }
}

return VarNames;
}


// =====================================================================
// =====================================================================


bool EngineHelper::hasAtLeastAProducedVariable(
    openfluid::machine::ModelInstance* ModelInstance,
    openfluid::core::CoreRepository* CoreRepos)
{
  BOOST_FOREACH(std::string ClassName, EngineHelper::getClassNames(CoreRepos))
{  if(!EngineHelper::getProducedVarNames(ClassName,ModelInstance).empty())
  return true;
}

return false;
}

// =====================================================================
// =====================================================================


std::string EngineHelper::minimiseInfoString(std::string InfoString)
{
  std::size_t i = InfoString.find("(sent by");
  if (i != std::string::npos)
  {
    Glib::ustring MinimizedStr = InfoString.erase(i, InfoString.length() - i);
    return MinimizedStr;
  }

  return InfoString;
}

// =====================================================================
// =====================================================================

struct SortById
{
    bool operator ()(openfluid::core::Unit& U1, openfluid::core::Unit& U2) const
    {
      return (U1.getID() <= U2.getID());
    }
};

void EngineHelper::sortUnitsCollectionById(
    openfluid::core::UnitsCollection& Coll)
{
  Coll.getList()->sort(SortById());
}

// =====================================================================
// =====================================================================

struct SortByDateTime
{
    bool operator ()(openfluid::core::Event& E1, openfluid::core::Event& E2) const
    {
      return (E1.getDateTime() <= E2.getDateTime());
    }
};

void EngineHelper::sortEventsListByDateTime(
    openfluid::core::EventsList_t& Events)
{
  Events.sort(SortByDateTime());
}

// =====================================================================
// =====================================================================


bool EngineHelper::isEmptyString(std::string Str)
{
  for (unsigned int i = 0; i < Str.size(); i++)
  {
    if (!std::isspace(Str[i]))
      return false;
  }

  return true;
}

// =====================================================================
// =====================================================================

/* this way because static variables aren't translated */
std::string EngineHelper::getBlankSubstitute()
{
  return _("[blank]");
}

// =====================================================================
// =====================================================================


std::string EngineHelper::getTabSubstitute()
{
  return _("[tab]");
}

// =====================================================================
// =====================================================================


std::string EngineHelper::fromRealCharToSubstitute(std::string RealChar)
{
  boost::algorithm::replace_all(RealChar, " ",
      EngineHelper::getBlankSubstitute());
  boost::algorithm::replace_all(RealChar, "\t",
      EngineHelper::getTabSubstitute());

  return RealChar;
}

// =====================================================================
// =====================================================================


std::string EngineHelper::fromSubstituteToRealChar(std::string Substitute)
{
  boost::algorithm::replace_all(Substitute, EngineHelper::getBlankSubstitute(),
      " ");
  boost::algorithm::replace_all(Substitute, EngineHelper::getTabSubstitute(),
      "\t");

  return Substitute;
}

// =====================================================================
// =====================================================================


Glib::ustring EngineHelper::fromHeaderTypeToHeaderString(
    openfluid::base::OutputFilesDescriptor::HeaderType Header)
{
  if (Header == openfluid::base::OutputFilesDescriptor::None)
    return _("None");
  else if (Header == openfluid::base::OutputFilesDescriptor::Full)
    return _("Full");
  else if (Header == openfluid::base::OutputFilesDescriptor::ColnamesAsData)
    return _("Column names as data");
  else
    return _("Info");
}

// =====================================================================
// =====================================================================


openfluid::base::OutputFilesDescriptor::HeaderType EngineHelper::fromHeaderStringToHeaderType(
    Glib::ustring Header)
{
  if (Header == _("None"))
    return openfluid::base::OutputFilesDescriptor::None;
  else if (Header == _("Full"))
    return openfluid::base::OutputFilesDescriptor::Full;
  else if (Header == _("Column names as data"))
    return openfluid::base::OutputFilesDescriptor::ColnamesAsData;
  else
    return openfluid::base::OutputFilesDescriptor::Info;
}
