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


#include <openfluid/machine/InjectGenerator.hpp>

#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/tools/DataSrcFile.hpp>
#include <openfluid/tools/ColTextParser.hpp>


namespace openfluid { namespace machine {


InjectGenerator::InjectGenerator() : Generator(),
  m_IsMin(false), m_IsMax(false), m_Min(0.0), m_Max(0.0),
  m_SourcesFile(""),m_DistriFile("")
{

}


// =====================================================================
// =====================================================================


InjectGenerator::~InjectGenerator()
{


}

// =====================================================================
// =====================================================================


void InjectGenerator::initParams(const openfluid::ware::WareParams_t& Params)
{
  if (!OPENFLUID_GetSimulatorParameter(Params,"sources",m_SourcesFile))
    throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::initParams","missing sources value for generator");

  if (!OPENFLUID_GetSimulatorParameter(Params,"distribution",m_DistriFile))
    throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::initParams","missing distribution value for generator");


  if (OPENFLUID_GetSimulatorParameter(Params,"thresholdmin",m_Min)) m_IsMin = true;

  if (OPENFLUID_GetSimulatorParameter(Params,"thresholdmax",m_Max)) m_IsMax = true;

};


// =====================================================================
// =====================================================================


void InjectGenerator::checkConsistency()
{
  if (m_IsMin && m_IsMax && m_Min > m_Max)
    throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::checkConsistency","threshold max value must be greater or equal to threshold min value for generator");
}


// =====================================================================
// =====================================================================


void InjectGenerator::LoadDistribution(const std::string& FilePath)
{
  long UnitID;
  long DataSrcID;

  int i;


  openfluid::tools::ColumnTextParser DistriFileParser("%");


  if (boost::filesystem::exists(boost::filesystem::path(FilePath)))
  {

    if ((DistriFileParser.loadFromFile(FilePath)) && (DistriFileParser.getColsCount() == 2) && (DistriFileParser.getLinesCount() >0))
    {

      for (i=0;i<DistriFileParser.getLinesCount();i++)
      {

        if (DistriFileParser.getLongValue(i,0,&UnitID) && DistriFileParser.getLongValue(i,1,&DataSrcID))
        {
          m_SerieIDByUnit[UnitID] = DataSrcID;
        }
        else
        {
          throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::LoadDistribution","Error in distribution file " + FilePath + ", format error");
        }
      }
    }
    else
    {
      throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::LoadDistribution","Error in distribution file " + FilePath + ", file not found or format error");
    }
  }
  else
  {
    throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::LoadDistribution","Distribution file " + FilePath + " not found");
  }
}


// =====================================================================
// =====================================================================


void InjectGenerator::LoadDataAsSerie(const std::string& FilePath, const int& ID)
{
  openfluid::tools::ColumnTextParser FileParser("%");

  long int Year, Month, Day, Hour, Min, Sec;
  double Value;
  openfluid::core::DateTime ZeDT;



  if (FileParser.loadFromFile(FilePath) && (FileParser.getLinesCount() > 0) && (FileParser.getColsCount() == 7))
  {
    int i = 0;

    while (i<FileParser.getLinesCount())
    {
      if (FileParser.getLongValue(i,0,&Year) && FileParser.getLongValue(i,1,&Month) &&
          FileParser.getLongValue(i,2,&Day) && FileParser.getLongValue(i,3,&Hour) &&
          FileParser.getLongValue(i,4,&Min) && FileParser.getLongValue(i,5,&Sec) &&
          FileParser.getDoubleValue(i,6,&Value))
      {

        if (!boost::math::isnan(Value) && !boost::math::isinf(Value))
        {

          ZeDT = openfluid::core::DateTime(Year,Month,Day,Hour,Min,Sec);

          m_Series[ID].push(std::make_pair<openfluid::core::DateTime,double>(ZeDT,Value));
        }
      }
      else
      {
        throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::LoadDataAsSerie","Error loading file " + FilePath +", line format error");
      }

      i++;
    }
  }
  else
  {
    throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::LoadDataAsSerie","Error loading file " + FilePath +", file format error");
  }

}


// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InjectGenerator::initializeRun()
{
  openfluid::tools::DataSourcesFile DSFile;
  std::string InputDir;

  OPENFLUID_GetRunEnvironment("dir.input",InputDir);

  boost::filesystem::path DSFilePath(InputDir + "/" + m_SourcesFile);

  if (!DSFile.load(DSFilePath.string()) && DSFile.getIDs().size() <= 1)
  {
    throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::initializeRun","Error loading file " + m_SourcesFile);
  }
  else
  {
    std::vector<int> IDs = DSFile.getIDs();

    boost::filesystem::path SourcefilePath;

    for (unsigned int i=0;i<IDs.size();i++)
    {
      SourcefilePath = boost::filesystem::path(InputDir + "/" + DSFile.getSource(IDs[i]));
      if (!boost::filesystem::exists(SourcefilePath))
      {
        throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::initializeRun","Error loading "+ DSFile.getSource(IDs[i]) + " file as data source");
      }
      else
      {
        LoadDataAsSerie(SourcefilePath.string(),IDs[i]);
      }
    }

    boost::filesystem::path DistriFilePath(InputDir + "/" + m_DistriFile);

    LoadDistribution(DistriFilePath.string());
  }

  // ============


  openfluid::core::Unit* LU;
  openfluid::core::DoubleValue CurrentValue;

  for (std::map<int,DatedValueSerie_t>::iterator it=m_Series.begin(); it!=m_Series.end();++it)
  {

    // accessing the next correct value
    while (!((*it).second.empty()) && !((*it).second.front().first == OPENFLUID_GetCurrentDate()))
      (*it).second.pop();

    // exit if the next correct value doesn't exist
    if ((*it).second.empty())
    {
      std::string StrID;
      openfluid::tools::ConvertValue((*it).first,&StrID);
      throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::runStep","value to inject for variable " + m_VarName + " cannot be found for source ID " + StrID);
    }
  }


  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
  {
    CurrentValue = m_Series[m_SerieIDByUnit[LU->getID()]].front().second;

    if (m_IsMax && CurrentValue > m_Max) CurrentValue = m_Max;
    if (m_IsMin && CurrentValue < m_Min) CurrentValue = m_Min;

    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,CurrentValue);
      OPENFLUID_InitializeVariable(LU,m_VarName,VV);
    }
    else
      OPENFLUID_InitializeVariable(LU,m_VarName,CurrentValue);
  }


  return DefaultDeltaT();
}

// =====================================================================
// =====================================================================


openfluid::base::SchedulingRequest InjectGenerator::runStep()
{

  openfluid::core::Unit* LU;
  openfluid::core::DoubleValue CurrentValue;

  for (std::map<int,DatedValueSerie_t>::iterator it=m_Series.begin(); it!=m_Series.end();++it)
  {

    // accessing the next correct value
    while (!((*it).second.empty()) && !((*it).second.front().first == OPENFLUID_GetCurrentDate()))
      (*it).second.pop();

    // exit if the next correct value doesn't exist
    if ((*it).second.empty())
    {
      std::string StrID;
      openfluid::tools::ConvertValue((*it).first,&StrID);
      throw openfluid::base::OFException("OpenFLUID framework","InjectGenerator::runStep","value to inject for variable " + m_VarName + " cannot be found for source ID " + StrID);
    }
  }


  OPENFLUID_UNITS_ORDERED_LOOP(m_UnitClass,LU)
  {
    CurrentValue = m_Series[m_SerieIDByUnit[LU->getID()]].front().second;

    if (m_IsMax && CurrentValue > m_Max) CurrentValue = m_Max;
    if (m_IsMin && CurrentValue < m_Min) CurrentValue = m_Min;

    if (isVectorVariable())
    {
      openfluid::core::VectorValue VV(m_VarSize,CurrentValue);
      OPENFLUID_AppendVariable(LU,m_VarName,VV);
    }
    else
      OPENFLUID_AppendVariable(LU,m_VarName,CurrentValue);
  }

  return DefaultDeltaT();
}


} } //namespaces


