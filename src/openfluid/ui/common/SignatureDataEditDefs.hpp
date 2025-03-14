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
  @file SignatureDataEditDefs.hpp
 
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_UICOMMON_SIGNATUREDATAEDITDEFS_HPP__
#define __OPENFLUID_UICOMMON_SIGNATUREDATAEDITDEFS_HPP__


namespace openfluid { namespace ui { namespace common {


enum class DataTableType { ATTRIBUTES, VARIABLES };


struct DataTablesHeader
{
  static constexpr const char* NAME         = QT_TRANSLATE_NOOP("openfluid::ui::common", "Name");
  static constexpr const char* UNITS_CLASS  = QT_TRANSLATE_NOOP("openfluid::ui::common", "Units class");
  static constexpr const char* IO_CONDITION = QT_TRANSLATE_NOOP("openfluid::ui::common", "I/O condition");
  static constexpr const char* TYPE         = QT_TRANSLATE_NOOP("openfluid::ui::common", "Type");
  static constexpr const char* DESCRIPTION  = QT_TRANSLATE_NOOP("openfluid::ui::common", "Description");
  static constexpr const char* SIUNIT       = QT_TRANSLATE_NOOP("openfluid::ui::common", "SI unit");
};


} } } // namespaces


#endif /* __OPENFLUID_UICOMMON_SIGNATUREDATAEDITDEFS_HPP__ */
