/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file CompactMachineListener.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
  @author David CREVOISIER <david.crevoisier@inrae.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_COMPACTMACHINELISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_COMPACTMACHINELISTENER_HPP__


#include <iostream>
#include <iomanip>
#include <chrono>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/tools/Console.hpp>


void writeTime(std::ostream& Stream, int Seconds)
{
  auto TimeSec        = Seconds;
  auto TimeMin        = (long)(std::floor((double)(Seconds)/60.0));
  TimeSec             = TimeSec % 60;
  auto TimeHour   = (long)(std::floor((double)(TimeMin)/60.0));
  TimeMin         = TimeMin % 60;
  if (TimeHour != 0)
  {
    Stream << TimeHour << "h ";
  }
  if (TimeMin != 0)
  {
    Stream << TimeMin << "min ";
  }
  Stream << TimeSec << "s";
}


// =====================================================================
// =====================================================================


class ProgressBar
{
  public:
    unsigned int m_nBar = 20;
    char m_chBarDone = '#';
    char m_chBarTodo = '-';
    char m_chBarWarning = '!';
    char m_chBarError = 'x';
  
    std::chrono::time_point<std::chrono::system_clock> m_BeginTime;
    double m_refreshBarInfo;
    
    unsigned int m_CurrentPos = 0;
    std::string m_BarContent;
    std::map<int, openfluid::machine::MachineListener::Status> m_BarStatusLevels;

    void init()
    {
      m_BarContent = std::string(m_nBar, m_chBarTodo);
    }

    void writeColoredBarChar(std::ostream& Stream, const int Pos)
    {
      bool CustomColor = false;
      if (m_BarStatusLevels.find(Pos) != m_BarStatusLevels.end())
      { 
        switch (m_BarStatusLevels.at(Pos))
        {
          case openfluid::machine::MachineListener::Status::ERROR_STATUS :
            openfluid::tools::Console::setErrorColor();
            break;
          case openfluid::machine::MachineListener::Status::OK_STATUS :
            openfluid::tools::Console::setOKColor();
            break;
          case openfluid::machine::MachineListener::Status::WARNING_STATUS :
            openfluid::tools::Console::setWarningColor();
            break;
        }
        CustomColor = true;
      }
      Stream << m_BarContent[Pos];
      if (CustomColor)
      {
        openfluid::tools::Console::resetAttributes();
      }
    }

    void setCurrentPos(const unsigned int Pos)
    {
      if (Pos >= m_BarContent.size())
      {
        throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                "index overflow: "+std::to_string(Pos)+" over "+std::to_string(m_BarContent.size()));
      }
      // For intermediate prints:
      // if (m_CurrentPos != Pos && (m_CurrentPos*10/m_nBar != Pos*10/m_nBar))
      // {
      //   std::cout << std::endl;
      // }
      m_CurrentPos = Pos;
    }

    void writeBar(std::ostream& Stream, const double RatioDone, const bool IsEnd=false, 
                  const std::string& DateString="")
    {
      Stream << "\r  [";
      
      for (unsigned int k=0;k!=m_nBar;k++)
      {
        writeColoredBarChar(Stream, k);
      }          
      Stream << "] ";
      if (IsEnd)
      {
        Stream << "      Processed all dates      " << std::endl;  
      }
      else
      {
        Stream << "Processing " << DateString << " ";
        Stream << " [" << std::setw(3) << std::fixed << std::setprecision(0) << 100.0 * RatioDone << "\% in ";

        auto currentTime = std::chrono::system_clock::now();
        auto timeDoneSec    = std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_BeginTime).count();
        auto timeLeftSec    = (long)(std::floor((double)((1.0-RatioDone)/RatioDone * timeDoneSec)));
        if (RatioDone > m_refreshBarInfo)
        {
          writeTime(Stream, timeDoneSec);
          Stream << " < ";
          writeTime(Stream, timeLeftSec);
          Stream << "]                     ";
        }
        else
        {
          Stream << "not available yet";
        }
      }

      Stream << std::flush;
    }

    void applyStepStatus(const openfluid::base::Listener::Status& Status)
    {
      switch (Status)
      {
        case openfluid::machine::MachineListener::Status::ERROR_STATUS :
          m_BarContent[m_CurrentPos] = m_chBarError;

          m_BarStatusLevels[m_CurrentPos] = openfluid::machine::MachineListener::Status::ERROR_STATUS;
          break;
        case openfluid::machine::MachineListener::Status::OK_STATUS :
          if (m_BarContent[m_CurrentPos] == m_chBarTodo)
          {
            m_BarContent[m_CurrentPos] = m_chBarDone;
            m_BarStatusLevels[m_CurrentPos] = openfluid::machine::MachineListener::Status::OK_STATUS;
          }
          break;
        case openfluid::machine::MachineListener::Status::WARNING_STATUS :
          if (m_BarContent[m_CurrentPos] == m_chBarTodo || m_BarContent[m_CurrentPos] == m_chBarDone)
          {
            m_BarContent[m_CurrentPos] = m_chBarWarning;
            m_BarStatusLevels[m_CurrentPos] = openfluid::machine::MachineListener::Status::WARNING_STATUS;
          }
          break;
      }
    }
};


// =====================================================================
// =====================================================================


class CompactMachineListener : public openfluid::machine::MachineListener
{

  private:
  
    bool m_Initiated = false;

    ProgressBar m_ProgressBar;
    std::vector<openfluid::core::TimeIndex_t> m_TimeBar;

    std::chrono::time_point<std::chrono::system_clock> m_beginSimTime = std::chrono::system_clock::now();


  public:

    virtual void onRunStep(const openfluid::base::SimulationStatus* SimStatus)
    {
      if (!m_Initiated)  // hard to do in a previous step: sim duration is only available through SimulationStatus
      {
        m_ProgressBar.m_BeginTime = std::chrono::system_clock::now();
        openfluid::machine::MachineListener::onInitParams();
        openfluid::core::Duration_t OneTimeBar = std::floor( double(SimStatus->getSimulationDuration()) / 
                                                             double(m_ProgressBar.m_nBar) );
        for (unsigned int k=1;k!=m_ProgressBar.m_nBar+1;++k) //TODO check if works with Windows OS
        {
          m_TimeBar.push_back(k*OneTimeBar);
        }

        m_ProgressBar.m_refreshBarInfo = 0.01;
        m_ProgressBar.init();
        m_Initiated = true;
      }
      
      openfluid::core::TimeIndex_t CurrentIndex = SimStatus->getCurrentTimeIndex();
      for (unsigned int k=m_ProgressBar.m_nBar;k!=0;--k)
      {
        if (CurrentIndex >= m_TimeBar[k])
        {
          m_ProgressBar.setCurrentPos(k);
          break;          
        }
      }
      double RatioDone = double(CurrentIndex) / double(SimStatus->getSimulationDuration());
      bool IsPrint = false;
      if (CurrentIndex < SimStatus->getSimulationDuration())
      {
        if ( ((int)(floor(10.0 * RatioDone / m_ProgressBar.m_refreshBarInfo)) % 10 == 0) )
        {
          IsPrint = true;
        }
      }
      else
      {
        IsPrint = true;
      }
      if (IsPrint)
      {
        m_ProgressBar.writeBar(std::cout, RatioDone, false, 
                               SimStatus->getCurrentDate().getAsString("%Y-%m-%d %H:%M:%S"));
      }
    };

    
    virtual void onRunStepDone(const openfluid::base::Listener::Status& Status)
    {
      m_ProgressBar.applyStepStatus(Status);
    }


    virtual void onAfterRunSteps()
    {
      m_ProgressBar.writeBar(std::cout, 1, true);
    }
};


#endif /* __OPENFLUID_CMDLINEAPP_COMPACTMACHINELISTENER_HPP__ */
