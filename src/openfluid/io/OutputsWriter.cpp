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

#include <openfluid/io/OutputsWriter.hpp>
#include <openfluid/base.hpp>
#include <fstream>
#include <openfluid/config.hpp>

// =====================================================================
// =====================================================================


namespace openfluid { namespace io {


OutputsWriter::OutputsWriter(const std::string DirPath,
                             const openfluid::base::OutputDescriptor& OutDesc,
                             openfluid::core::CoreRepository& CoreRepos)
             : m_CoreRepos(CoreRepos)
{
  m_DirPath = DirPath;
  m_OutDesc = OutDesc;


  buildFilesList(DirPath,OutDesc);
}


// =====================================================================
// =====================================================================


OutputsWriter::~OutputsWriter()
{
  std::list<OutputsFileWriter*>::iterator  itFiles;

  for (itFiles = m_Files.begin(); itFiles != m_Files.end(); ++itFiles)
    delete (*itFiles);

}

// =====================================================================
// =====================================================================


void OutputsWriter::buildFilesList(const std::string& DirPath, const openfluid::base::OutputDescriptor& OutDesc)
{
  openfluid::io::OutputsFileWriter* Writer;
  std::vector<std::string> VariablesNames;
  std::vector<openfluid::core::UnitClassID_t> UnitsClassIDs;

  for (unsigned int i=0; i< OutDesc.getFileSets().size();i++)
  {

    for (unsigned int j=0; j< OutDesc.getFileSets()[i].getSets().size();j++)
    {

      UnitsClassIDs.clear();

      if (OutDesc.getFileSets()[i].getSets()[j].isAllUnits())
      {
        // ------- All units
        openfluid::core::UnitsList_t* Units;
        openfluid::core::UnitsList_t::iterator UnitIt;

        Units = m_CoreRepos.getUnits(OutDesc.getFileSets()[i].getSets()[j].getUnitsClass())->getList();


        if (Units != NULL)
        {
          for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
          {
            UnitsClassIDs.push_back(make_pair(UnitIt->getClass(),UnitIt->getID()));
          }
        }
      }
      else
      {
        for (unsigned int iUnits = 0; iUnits < OutDesc.getFileSets()[i].getSets()[j].getUnitsIDs().size();iUnits++)
        {
          openfluid::core::UnitClassID_t ClassID = make_pair(OutDesc.getFileSets()[i].getSets()[j].getUnitsClass(),
                                                             OutDesc.getFileSets()[i].getSets()[j].getUnitsIDs()[iUnits]);
          if (m_CoreRepos.getUnit(ClassID.first,ClassID.second) != NULL)
          {
              UnitsClassIDs.push_back(ClassID);
          }
        }
      }


      // ------- Variables

      for (unsigned int jUnits = 0; jUnits < UnitsClassIDs.size(); jUnits++)
      {
        VariablesNames.clear();

        if (OutDesc.getFileSets()[i].getSets()[j].isAllVariables())
          VariablesNames = m_CoreRepos.getUnit(UnitsClassIDs[jUnits].first,UnitsClassIDs[jUnits].second)->getVariables()->getVariablesNames();
        else
          VariablesNames = OutDesc.getFileSets()[i].getSets()[j].getVariables();

        if (!VariablesNames.empty())
        {
          Writer = new openfluid::io::OutputsFileWriter(DirPath, m_CoreRepos,
              UnitsClassIDs[jUnits].first,UnitsClassIDs[jUnits].second,
              OutDesc.getFileSets()[i].getSets()[j].getName(),
              OutDesc.getFileSets()[i].getCommentChar(),
              OutDesc.getFileSets()[i].getDateFormat(),
              OutDesc.getFileSets()[i].getColSeparator(),
              OutDesc.getFileSets()[i].getSets()[j].getPrecision());

          for (unsigned int n = 0; n < VariablesNames.size(); n++)
            Writer->addVariable(VariablesNames[n]);

          m_Files.push_back(Writer);
        }
      }
    }
  }
}


// =====================================================================
// =====================================================================


void OutputsWriter::prepareDirectory()
{

  std::list<OutputsFileWriter*>::iterator  itFiles;

  for (itFiles = m_Files.begin(); itFiles != m_Files.end(); ++itFiles)
    (*itFiles)->initializeFile();

}


// =====================================================================
// =====================================================================


void OutputsWriter::saveToDirectory(const openfluid::core::DateTime& CurrentDT)
{
  std::list<OutputsFileWriter*>::iterator  itFiles;

  for (itFiles = m_Files.begin(); itFiles != m_Files.end(); ++itFiles)
    (*itFiles)->saveCurrentDataToFile(CurrentDT);
}


// =====================================================================
// =====================================================================


void OutputsWriter::closeFiles()
{
  std::list<OutputsFileWriter*>::iterator  itFiles;

  for (itFiles = m_Files.begin(); itFiles != m_Files.end(); ++itFiles)
    (*itFiles)->closeFile();

}

} } //namespaces

