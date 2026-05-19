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
  @author David Crevoisier <david.crevoisier@inrae.fr>
*/


#ifndef __OPENFLUID_CMDLINEAPP_COMPACTMACHINELISTENER_HPP__
#define __OPENFLUID_CMDLINEAPP_COMPACTMACHINELISTENER_HPP__


#include <iostream>
#include <iomanip>
#include <chrono>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/machine/MachineListener.hpp>
#include <openfluid/tools/Console.hpp>


void displayTime(int Seconds)
{
  auto timeSec        = Seconds;
  auto timeMin        = (long)(std::floor((double)(Seconds)/60.0));
  timeSec             = timeSec % 60;
  auto timeHour   = (long)(std::floor((double)(timeMin)/60.0));
  timeMin         = timeMin % 60;
  if (timeHour != 0)
  {
    std::cout << timeHour << "h ";
  }
  if (timeMin != 0)
  {
    std::cout << timeMin << "min ";
  }
  std::cout << timeSec << "s";
}


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
    
    int m_CurrentPos = 0;
    std::string m_BarContent;
    std::map<int, openfluid::machine::MachineListener::Status> m_BarStatusLevels;

    void init()
    {
      m_BarContent = std::string(m_nBar, m_chBarTodo);
    }

    void printColoredBarChar(const int Pos) const
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
      std::cout << m_BarContent[Pos];
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
      m_CurrentPos = Pos;
    }

    void printBar(const double RatioDone, const bool IsEnd=false, const std::string& DateString="") const
    {
      std::cout << "\r  [";
      
      for (unsigned int k=0;k!=m_nBar;k++)
      {
        printColoredBarChar(k);
      }          
      std::cout << "] ";
      if (IsEnd)
      {
        std::cout << "      Processed all dates      " << std::endl;  
      }
      else
      {
        std::cout << "Processing " << DateString << " ";
        std::cout << " [" << std::setw(3) << std::fixed << std::setprecision(0) << 100.0 * RatioDone << "\% in ";

        auto currentTime = std::chrono::system_clock::now();
        auto timeDoneSec    = std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_BeginTime).count();
        auto timeLeftSec    = (long)(std::floor((double)((1.0-RatioDone)/RatioDone * timeDoneSec)));
        if (RatioDone > m_refreshBarInfo)
        {
          displayTime(timeDoneSec);
          std::cout << " < ";
          displayTime(timeLeftSec);
          std::cout << "]                     ";
        }
        else
        {
          std::cout << "not available yet";
        }
      }

      std::cout << std::flush;
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
      if (!m_Initiated) // hard to do in a previous step: simulation duration information is only available through SimulationStatus
      {
        m_ProgressBar.m_BeginTime = std::chrono::system_clock::now();
        openfluid::machine::MachineListener::onInitParams();
        openfluid::core::Duration_t OneTimeBar = std::floor( double(SimStatus->getSimulationDuration()) / double(m_ProgressBar.m_nBar) );
        for (unsigned int k=1;k!=m_ProgressBar.m_nBar+1;++k) //TODO check if works with Windows OS
        {
          m_TimeBar.push_back(k*OneTimeBar);
        }

        m_ProgressBar.m_refreshBarInfo = 0.01;
        m_ProgressBar.init();
        m_Initiated = true;
      }
      
      openfluid::core::TimeIndex_t CurrentIndex = SimStatus->getCurrentTimeIndex();      
      for (unsigned int k=1;k!=m_ProgressBar.m_nBar+1;++k)
      {
        if (CurrentIndex >= m_TimeBar[k])
        {
          m_ProgressBar.setCurrentPos(k);          
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
        m_ProgressBar.printBar(RatioDone, false, SimStatus->getCurrentDate().getAsString("%Y-%m-%d %H:%M:%S"));
      }
    };

    
    virtual void onRunStepDone(const openfluid::base::Listener::Status& Status)
    {
      m_ProgressBar.applyStepStatus(Status);
    }

    virtual void onAfterRunSteps()
    {
      m_ProgressBar.printBar(1, true);
    }
};


#endif /* __OPENFLUID_CMDLINEAPP_COMPACTMACHINELISTENER_HPP__ */