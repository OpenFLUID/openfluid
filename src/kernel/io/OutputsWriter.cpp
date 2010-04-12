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
  \file OutputsWriter.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */

#include "OutputsWriter.h"
#include "openfluid-base.h"
#include <fstream>
#include "config.h"
#include "RuntimeEnv.h"

// =====================================================================
// =====================================================================


OutputsWriter::OutputsWriter(const std::string DirPath, const OutputDescriptor& OutDesc)
{
  m_DirPath = DirPath;
  m_OutDesc = OutDesc;

  m_OutDesc.fillUnitsPtr();
}


// =====================================================================
// =====================================================================


OutputsWriter::~OutputsWriter()
{

}

// =====================================================================
// =====================================================================


std::string OutputsWriter::generateOuputFilename(const std::string UnitClass, const openfluid::core::UnitID_t UnitID,
                                             const std::string Suffix, const std::string VectorName)
{
  std::ostringstream GeneratedName;

  GeneratedName << UnitClass << UnitID << "_" << Suffix;

  if (VectorName == "")
  {
    GeneratedName << ".scalars." << CONFIG_OUTFILES_EXT;
  }
  else
  {
    GeneratedName << ".vector." << VectorName<< "." << CONFIG_OUTFILES_EXT;
  }

  return GeneratedName.str();
}


// =====================================================================
// =====================================================================


std::string OutputsWriter::generateOutputScalarsFileHeader(const openfluid::core::UnitClass_t UnitClass,
    const openfluid::core::UnitID_t UnitID,
    const std::string Filename, const std::vector<std::string> ScalarsNames,
    const std::string CommentChar)
{
  std::ostringstream GeneratedHeader;

  GeneratedHeader << CommentChar << " simulation ID: " << RuntimeEnvironment::getInstance()->getSimulationID() << "\n"
  << CommentChar << " file: " << Filename << "\n"
  << CommentChar << " date: " << boost::posix_time::to_simple_string(RuntimeEnvironment::getInstance()->getIgnitionDateTime()) << "\n"
  << CommentChar << " unit: " << UnitClass << " #" << UnitID << "\n"
  << CommentChar << " scalar variables order (after date and time columns):";


  for (unsigned int i = 0 ; i < ScalarsNames.size(); i++ ) GeneratedHeader << " " << ScalarsNames[i];


  GeneratedHeader << "\n" << "\n";

  return GeneratedHeader.str();

}


// =====================================================================
// =====================================================================


std::string OutputsWriter::generateOutputVectorFileHeader(openfluid::core::UnitClass_t UnitClass,
    openfluid::core::UnitID_t UnitID,
    std::string Filename, std::string VectorName,
    std::string CommentChar)
{
  std::ostringstream GeneratedHeader;

  GeneratedHeader << CommentChar << " simulation ID: " << RuntimeEnvironment::getInstance()->getSimulationID() << "\n"
  << CommentChar << " file: " << Filename << "\n"
  << CommentChar << " date: " << boost::posix_time::to_simple_string(RuntimeEnvironment::getInstance()->getIgnitionDateTime()) << "\n"
  << CommentChar << " unit: " << UnitClass << " #" << UnitID << "\n"
  << CommentChar << " vector variable: " << VectorName << "\n"
  << CommentChar << " the vector values are ordered left to right, from index 0 to size-1 (after date and time columns)" << "\n" << "\n";


  return GeneratedHeader.str();
}


// =====================================================================
// =====================================================================


std::string OutputsWriter::generateOutputScalarsFileContent(const openfluid::core::Unit* aUnit, const std::vector<std::string> ScalarsNames,
    openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
    openfluid::base::SimulationInfo *SimInfo,
    std::string DateFormat,
    std::string ColSeparator,
    unsigned int Precision)
{
  std::ostringstream GeneratedContent;
  openfluid::core::ScalarValue Value = 0;

  GeneratedContent << std::fixed << std::setprecision(Precision);

  for (unsigned int iStep = BeginStep; iStep <= EndStep; iStep++)
  {
    GeneratedContent << GenerateDateTimeFromStep(SimInfo->getStartTime(),SimInfo->getTimeStep(),iStep).getAsString(DateFormat);

    for (unsigned int iVar = 0 ; iVar < ScalarsNames.size(); iVar++ )
    {

      if (!aUnit->getScalarVariables()->getValue(ScalarsNames[iVar],iStep,&Value))
        throw openfluid::base::OFException("kernel","IOManager::generateOutputScalarsFileContent",iStep,"value not found for variable " + ScalarsNames[iVar]);
      GeneratedContent << ColSeparator << Value;
    }

    GeneratedContent << "\n";

  }

  return GeneratedContent.str();
}


// =====================================================================
// =====================================================================


std::string OutputsWriter::generateOutputVectorFileContent(const openfluid::core::Unit* aUnit, const std::string VectorName,
    openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
    openfluid::base::SimulationInfo *SimInfo,
    std::string DateFormat,
    std::string ColSeparator,
    unsigned int Precision)
{
  std::ostringstream GeneratedContent;
  openfluid::core::VectorValue Value;

  GeneratedContent << std::fixed << std::setprecision(Precision);

  for (unsigned int iStep = BeginStep; iStep <= EndStep; iStep++)
  {
    GeneratedContent << GenerateDateTimeFromStep(SimInfo->getStartTime(),SimInfo->getTimeStep(),iStep).getAsString(DateFormat);


    if (!aUnit->getVectorVariables()->getValue(VectorName,iStep,&Value))
      throw openfluid::base::OFException("kernel","IOManager::generateOutputVectorFileContent",iStep,"value not found for vector variable " + VectorName);

    for (unsigned long iVal = 0 ; iVal < Value.getSize(); iVal++ )
    {
      GeneratedContent << ColSeparator << Value.getElement(iVal);
    }

    GeneratedContent << "\n";

  }

  return GeneratedContent.str();
}


// =====================================================================
// =====================================================================


void OutputsWriter::saveUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex,
                                       openfluid::core::TimeStep_t& BeginStep, openfluid::core::TimeStep_t& EndStep,
                                       openfluid::base::SimulationInfo *SimInfo)
{

  std::ofstream OutFile;
  boost::filesystem::path OutFilePath;

  std::string ScalarsFilename;
  std::string VectorFilename;
  std::string FileContent;
  std::vector<openfluid::core::VariableName_t> VarNames;
  std::string ColSep = m_OutDesc.getFileSets()[FileOutputIndex].getColSeparator();

  std::string NameSuffix = m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getName();

  // processing scalars outputs
  if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllScalars() ||
      m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getScalars().size() > 0)
  {

    ScalarsFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix);

    if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllScalars())
      FileContent = generateOutputScalarsFileContent(aUnit,
          aUnit->getScalarVariables()->getVariablesNames(),
          BeginStep,EndStep,
          SimInfo,
          m_OutDesc.getFileSets()[FileOutputIndex].getDateFormat(),
          ColSep,
          m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getPrecision());
    else
      FileContent = generateOutputScalarsFileContent(aUnit,
          m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getScalars(),
          BeginStep,EndStep,
          SimInfo,
          m_OutDesc.getFileSets()[FileOutputIndex].getDateFormat(),
          ColSep,
          m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getPrecision());

    OutFilePath = boost::filesystem::path(m_DirPath+"/"+ScalarsFilename);
    OutFile.open(OutFilePath.string().c_str(),std::ios::app);
    OutFile << FileContent;
    OutFile.close();
  }



  // processing vectors outputs
  if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllVectors() ||
      m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getVectors().size() > 0)
  {

    if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllVectors())
      VarNames = aUnit->getVectorVariables()->getVariablesNames();
    else
      VarNames = m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getVectors();

    for (unsigned int j =  0; j < VarNames.size(); j++)
    {

      VectorFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix,VarNames[j]);

      FileContent = generateOutputVectorFileContent(aUnit,VarNames[j],
          BeginStep,EndStep,
          SimInfo,
          m_OutDesc.getFileSets()[FileOutputIndex].getDateFormat(),
          ColSep,
          m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getPrecision());

      OutFilePath = boost::filesystem::path(m_DirPath+"/"+VectorFilename);
      OutFile.open(OutFilePath.string().c_str(),std::ios::app);
      OutFile << FileContent;
      OutFile.close();

    }

  }

}


// =====================================================================
// =====================================================================


void OutputsWriter::prepareUnitFileOutput(openfluid::core::Unit* aUnit, int FileOutputIndex, int OutputSetIndex)
{

  std::ofstream OutFile;
  boost::filesystem::path OutFilePath;

  std::string ScalarsFilename;
  std::string VectorFilename;
  std::string FileContent;
  std::vector<openfluid::core::VariableName_t> VarNames;
  std::string CommentChar = m_OutDesc.getFileSets()[FileOutputIndex].getCommentChar();

  std::string NameSuffix = m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getName();

  // processing scalars outputs
  if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllScalars() ||
      m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getScalars().size() > 0)
  {

    ScalarsFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix);

    if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllScalars())
      FileContent = generateOutputScalarsFileHeader(aUnit->getClass(),aUnit->getID(),ScalarsFilename,
          aUnit->getScalarVariables()->getVariablesNames(),CommentChar);
    else
      FileContent = generateOutputScalarsFileHeader(aUnit->getClass(), aUnit->getID(),ScalarsFilename,
          m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getScalars(),CommentChar);

    OutFilePath = boost::filesystem::path(m_DirPath+"/"+ScalarsFilename);
    OutFile.open(OutFilePath.string().c_str(),std::ios::out);
    OutFile << FileContent;
    OutFile.close();
  }



  // processing vectors outputs
  if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllVectors() ||
      m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getVectors().size() > 0)
  {

    if (m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].isAllVectors())
      VarNames = aUnit->getVectorVariables()->getVariablesNames();
    else
      VarNames = m_OutDesc.getFileSets()[FileOutputIndex].getSets()[OutputSetIndex].getVectors();

    for (unsigned int j =  0; j < VarNames.size(); j++)
    {

      VectorFilename = generateOuputFilename(aUnit->getClass(),aUnit->getID(),NameSuffix,VarNames[j]);

      FileContent = generateOutputVectorFileHeader(aUnit->getClass(), aUnit->getID(),VectorFilename,
          VarNames[j],CommentChar);

      OutFilePath = boost::filesystem::path(m_DirPath+"/"+VectorFilename);
      OutFile.open(OutFilePath.string().c_str(),std::ios::out);
      OutFile << FileContent;
      OutFile.close();

    }

  }


}


// =====================================================================
// =====================================================================


void OutputsWriter::prepareDirectory()
{

  openfluid::core::UnitsList_t* Units;
  openfluid::core::UnitsList_t::iterator UnitIt;


  for (unsigned int i=0; i< m_OutDesc.getFileSets().size();i++)
  {

    for (unsigned int j=0; j< m_OutDesc.getFileSets()[i].getSets().size();j++)
    {

      if (m_OutDesc.getFileSets()[i].getSets()[j].isAllUnits())
      {
        Units = openfluid::core::CoreRepository::getInstance()->getUnits(m_OutDesc.getFileSets()[i].getSets()[j].getUnitsClass())->getList();

        for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
        {
          prepareUnitFileOutput(&(*UnitIt),i,j);
        }

      }
      else
      {
        for (unsigned int iUnits = 0; iUnits < m_OutDesc.getFileSets()[i].getSets()[j].getUnitsPtr().size();iUnits++)
        {
          prepareUnitFileOutput(m_OutDesc.getFileSets()[i].getSets()[j].getUnitsPtr()[iUnits],i,j);
        }
      }
    }


  }
}


// =====================================================================
// =====================================================================


void OutputsWriter::saveToDirectory(openfluid::core::TimeStep_t CurrentStep, openfluid::base::SimulationInfo *SimInfo, bool WithoutKeep)
{
  openfluid::core::UnitsList_t* Units;
  openfluid::core::UnitsList_t::iterator UnitIt;


  openfluid::core::TimeStep_t BeginStep, EndStep;
  openfluid::core::MemoryMonitor::getInstance()->getMemoryReleaseRange(CurrentStep,WithoutKeep,&BeginStep,&EndStep);


  for (unsigned int i=0; i< m_OutDesc.getFileSets().size();i++)
  {


    for (unsigned int j=0; j< m_OutDesc.getFileSets()[i].getSets().size();j++)
    {

      if (m_OutDesc.getFileSets()[i].getSets()[j].isAllUnits())
      {


        Units = openfluid::core::CoreRepository::getInstance()->getUnits(m_OutDesc.getFileSets()[i].getSets()[j].getUnitsClass())->getList();

        for (UnitIt = Units->begin();UnitIt != Units->end();++UnitIt)
        {
          saveUnitFileOutput(&(*UnitIt),i,j,BeginStep,EndStep,SimInfo);
        }

      }
      else
      {
        for (unsigned int iUnits = 0; iUnits < m_OutDesc.getFileSets()[i].getSets()[j].getUnitsPtr().size();iUnits++)
        {
          saveUnitFileOutput(m_OutDesc.getFileSets()[i].getSets()[j].getUnitsPtr()[iUnits],i,j,BeginStep,EndStep,SimInfo);
        }
      }
    }


  }
}

