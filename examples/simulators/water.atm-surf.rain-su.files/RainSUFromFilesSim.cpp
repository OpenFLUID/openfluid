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
  @file RainSUFromFilesSim.cpp
  
  @author Armel THONI <armel.thoni@inra.fr>
*/


/*
<sim2doc>

</sim2doc>
*/


#include <openfluid/ware/PluggableSimulator.hpp>
#include <openfluid/tools/DistributionBindings.hpp>
#include <openfluid/tools/ChronFileLinearInterpolator.hpp>
#include <openfluid/tools/Filesystem.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("water.atm-surf.rain-su.files")

  DECLARE_NAME("Distribution of rainfall on SU");
  DECLARE_DESCRIPTION("Linear interpolation of rainfall from rain gauge data source files and spatial distribution "
                      "on SU.");
  DECLARE_DOMAIN("meteo");
  DECLARE_VERSION("13.05");

  DECLARE_STATUS(openfluid::ware::STABLE);

  DECLARE_AUTHOR("Jean-Christophe Fabre","fabrejc@supagro.inra.fr");
  DECLARE_AUTHOR("Simon Lalauze","lalauze@supagro.inra.fr");
  DECLARE_AUTHOR("Xavier Louchart","louchart@supagro.inra.fr")

  // Produced variables
  DECLARE_PRODUCED_VARIABLE("water.atm-surf.H.rain","SU","rainfall height on SU","m");
  
  // Function parameters
  DECLARE_USED_PARAMETER("threshold","rainfall height lower than or equal to this value are replaced by zero","m");
  
  // Required extra files
  DECLARE_REQUIRED_EXTRAFILE("SUraindistri.dat");
  DECLARE_REQUIRED_EXTRAFILE("rainsources.xml");

// Scheduling
  DECLARE_SCHEDULING_DEFAULT;

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class RainSUFromFilesSimulator : public openfluid::ware::PluggableSimulator
{
  private:

    openfluid::tools::DistributionBindings* m_DistriBindings;

    double m_Threshold;

    openfluid::core::IDDoubleValueMap m_PreviousValue;

    std::string m_TempDir;


  public:

  
    RainSUFromFilesSimulator(): PluggableSimulator(),
      m_DistriBindings(0), m_Threshold(0.00001), m_TempDir("")
    {

    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~RainSUFromFilesSimulator()
    {
      if( m_DistriBindings)
      {
        delete m_DistriBindings;
      }

      if (!m_TempDir.empty())
      {
        openfluid::tools::Filesystem::removeDirectory(m_TempDir);
      }

    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void initParams(const openfluid::ware::WareParams_t& Params)
    {
      OPENFLUID_GetSimulatorParameter(Params,"threshold",m_Threshold);
      if (m_Threshold <= 0)
      {
        OPENFLUID_RaiseError("The rainfall threshold should be higher or equal to 0.");
      }
    }


    // =====================================================================
    // =====================================================================
  
  
    void prepareData()
    {
      openfluid::tools::DistributionTables DistriTables;
      std::string InputDir, BaseTmpDir, TmpDir;

      OPENFLUID_GetRunEnvironment("dir.input",InputDir);
      OPENFLUID_GetRunEnvironment("dir.temp",BaseTmpDir);

      // Create temporary directory for intermediate data
      unsigned int DirNameIncr = 0;
      do
      {
        std::ostringstream TmpSubDir;
        TmpSubDir << "water.atm-surf.rain-su.files-" << DirNameIncr;
        TmpDir = BaseTmpDir+"/"+TmpSubDir.str();
        DirNameIncr++;
      }
      while (openfluid::tools::Filesystem::isDirectory(TmpDir));

      openfluid::tools::Filesystem::makeDirectory(TmpDir);
      m_TempDir = TmpDir;

      // Build distribution tables
      DistriTables.build(InputDir,"rainsources.xml","SUraindistri.dat");

      openfluid::tools::DistributionTables::SourceIDFile_t::iterator itb = DistriTables.SourcesTable.begin();
      openfluid::tools::DistributionTables::SourceIDFile_t::iterator ite = DistriTables.SourcesTable.end();

      // Interpolate data (time)
      for (openfluid::tools::DistributionTables::SourceIDFile_t::iterator it = itb; it != ite; ++it)
      {

        std::string InFileName = openfluid::tools::Filesystem::filename((*it).second);
        std::string OutFilePath = TmpDir+"/interp_"+InFileName;

        // Prepare and run interpolator with cumulate preprocessing
        openfluid::tools::ChronFileLinearInterpolator CFLI((*it).second,
                                                           OutFilePath,
                                                           OPENFLUID_GetBeginDate(),OPENFLUID_GetEndDate(),
                                                           OPENFLUID_GetDefaultDeltaT(),
                                                           openfluid::tools::ChronFileInterpolator::PREPROCESS_CUMULATE
                                                           );

        CFLI.runInterpolation();

        (*it).second = OutFilePath;
      }

      // Instantiate bindings between spatial units and data sources, using distribution tables
      m_DistriBindings = new openfluid::tools::DistributionBindings(DistriTables);
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void checkConsistency()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest initializeRun()
    {  
      openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

      // Advance bindings time cursor to current datetime
      m_DistriBindings->advanceToTime(CurrentDT);

      openfluid::core::SpatialUnit* SU;
      openfluid::core::DoubleValue Value(0.0);

      // Initialize all rain variables on SU units class
      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        if (m_DistriBindings->getValue(SU->getID(),CurrentDT,Value))
        {
          m_PreviousValue[SU->getID()] = Value;

          OPENFLUID_InitializeVariable(SU,"water.atm-surf.H.rain",0.0);
        }
      }


      openfluid::core::DateTime NextDT;

      // Determine the duration to the next execution
      if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
      {
        return Duration(NextDT.diffInSeconds(CurrentDT));
      }
      else
      {
        return DefaultDeltaT();
      }
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {

      openfluid::core::DateTime CurrentDT(OPENFLUID_GetCurrentDate());

      m_DistriBindings->advanceToTime(CurrentDT);

      openfluid::core::SpatialUnit* SU;
      openfluid::core::DoubleValue Value(0.0);
      openfluid::core::DoubleValue MSValue(0.0);


      OPENFLUID_UNITS_ORDERED_LOOP("SU",SU)
      {
        if (m_DistriBindings->getValue(SU->getID(),CurrentDT,Value))
        {
          // Compute and convert value from cumulated millimeters to meters
          MSValue = (Value-m_PreviousValue[SU->getID()])/1000;

          m_PreviousValue[SU->getID()] = Value;

          if (std::isnan(MSValue.get()) || MSValue < m_Threshold)
          {
            MSValue = 0;
          }

          OPENFLUID_AppendVariable(SU,"water.atm-surf.H.rain",MSValue);
        }
      }


      openfluid::core::DateTime NextDT;

      // Determine the duration to the next execution
      if (m_DistriBindings->advanceToNextTimeAfter(CurrentDT,NextDT))
      {
        return Duration(NextDT.diffInSeconds(CurrentDT));
      }
      else
      {
        return DefaultDeltaT();
      }
    }


    // =====================================================================
    // =====================================================================
  
  
    void finalizeRun()
    {
  
  
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(RainSUFromFilesSimulator);

