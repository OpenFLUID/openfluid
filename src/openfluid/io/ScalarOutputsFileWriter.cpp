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
  @brief Implements ...

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#include <openfluid/io/ScalarOutputsFileWriter.hpp>
#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/config.hpp>
#include <openfluid/tools.hpp>
#include <boost/filesystem/path.hpp>

namespace openfluid { namespace io {


ScalarOutputsFileWriter::ScalarOutputsFileWriter(const std::string DirPath,
                                                 openfluid::core::CoreRepository& CoreRepos,
                                                 const openfluid::core::UnitClass_t UnitClass,
                                                 const openfluid::core::UnitID_t UnitID,
                                                 const std::string FileSuffix,
                                                 const std::string CommentChar,
                                                 const std::string DateFormat,
                                                 const std::string ColSeparator,
                                                 const openfluid::base::OutputFilesDescriptor::HeaderType Header,
                                                 const unsigned int Precision)
                         : OutputsFileWriter(DirPath,CoreRepos, UnitClass,UnitID,
                                             CommentChar, DateFormat, ColSeparator,
                                             Header, Precision)
{
  std::string UnitIDStr;
  openfluid::tools::ConvertValue(mp_Unit->getID(),&UnitIDStr);

  m_OutFilename = boost::filesystem::path(m_DirPath + "/" +
                                          mp_Unit->getClass() + UnitIDStr + "_" +
                                          FileSuffix + ".scalars." +
                                          openfluid::config::OUTFILES_EXT).string();

}


// =====================================================================
// =====================================================================


ScalarOutputsFileWriter::~ScalarOutputsFileWriter()
{

}


// =====================================================================
// =====================================================================


void ScalarOutputsFileWriter::initializeFile()
{
  std::string UnitIDStr;
  openfluid::tools::ConvertValue(mp_Unit->getID(),&UnitIDStr);

  m_OutFile.open(m_OutFilename.c_str(),std::ios::out);

  m_OutFile << std::fixed << std::setprecision(m_Precision);

  if(m_HeaderType == openfluid::base::OutputFilesDescriptor::Info
      || m_HeaderType == openfluid::base::OutputFilesDescriptor::Full)
  {
    m_OutFile << m_CommentChar << " simulation ID: " << openfluid::base::RuntimeEnvironment::getInstance()->getSimulationID() << "\n";
    m_OutFile << m_CommentChar << " file: " << boost::filesystem::path(m_OutFilename).leaf() << "\n";
    m_OutFile << m_CommentChar << " date: " << boost::posix_time::to_simple_string(openfluid::base::RuntimeEnvironment::getInstance()->getIgnitionDateTime()) << "\n";
    m_OutFile << m_CommentChar << " unit: " << mp_Unit->getClass() << " #" << UnitIDStr << "\n";
    m_OutFile << m_CommentChar << " scalar variables order (after date and time columns):";

  std::list<openfluid::core::VariableName_t>::iterator itNames;

  for (itNames = m_Variables.begin(); itNames != m_Variables.end() ; ++itNames)
   m_OutFile << " " << *itNames;

  m_OutFile << "\n" << "\n";
  }

  if(m_HeaderType == openfluid::base::OutputFilesDescriptor::ColnamesAsData
      || m_HeaderType == openfluid::base::OutputFilesDescriptor::Full)
  {
    m_OutFile << "DateTime";

    std::list<openfluid::core::VariableName_t>::iterator itNames;

    for (itNames = m_Variables.begin(); itNames != m_Variables.end() ; ++itNames)
      m_OutFile << m_ColSeparator << *itNames;

    m_OutFile << "\n";
  }

}


// =====================================================================
// =====================================================================


void ScalarOutputsFileWriter::saveCurrentDataToFile(const openfluid::core::DateTime& CurrentTime)
{
  openfluid::core::ScalarValue Value;

  m_OutFile << CurrentTime.getAsString(m_DateFormat);

  std::list<openfluid::core::VariableName_t>::iterator itNames;
  for (itNames = m_Variables.begin(); itNames != m_Variables.end() ; ++itNames)
  {
    if (!mp_Unit->getScalarVariables()->getCurrentValue(*itNames,&Value))
    {
      std::string UnitIDStr;
      openfluid::tools::ConvertValue(mp_Unit->getID(),&UnitIDStr);
      throw openfluid::base::OFException("OpenFLUID framework","ScalarOutputsFileWriter::saveCurrentDataToFile",
                                          "Value not found for variable " + *itNames +" on unit " + mp_Unit->getClass() + "#" + UnitIDStr);
    }
    m_OutFile << m_ColSeparator << Value;
  }
  m_OutFile << "\n";
}


} } //namespaces

