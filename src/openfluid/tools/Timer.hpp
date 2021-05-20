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
  @file Timer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
*/


#ifndef __OPENFLUID_TOOLS_TIMER_HPP__
#define __OPENFLUID_TOOLS_TIMER_HPP__


#include <chrono>
#include <string>
#include <iomanip>

#include <openfluid/tools/DataHelpers.hpp>


namespace openfluid { namespace tools {

/**
  Class to instanciate timers and measure durations between their starts and stops.
  @snippet misc/timer.cpp usage
*/
class Timer
{
  private:

    typedef std::chrono::system_clock TimerClock;

    std::chrono::time_point<TimerClock> m_StartTime;
    std::chrono::time_point<TimerClock> m_EndTime;

    bool m_Running;


  public:

    /**
      Constructs a timer (not started by default)
      @param[in] AutoStart If true, the timer is automatically started when it is instanciated (false by default)
    */
    Timer(bool AutoStart = false):
      m_Running(false)
    {
      if (AutoStart)
      {
        start();
      }
    }

    /**
      Starts the timer (if not currently running)
    */
    void start()
    {
      if (!m_Running)
      {
        m_Running = true;
        m_StartTime = TimerClock::now();
      }
    }


    /**
      Stops the timer (if currently running)
    */
    void stop()
    {
      if (m_Running)
      {
        m_EndTime = TimerClock::now();
        m_Running = false;
      }
    }


    /**
      Restarts the timer (even if currently running)
    */
    void restart()
    {
      m_Running = true;
      m_EndTime = std::chrono::time_point<TimerClock>();
      m_StartTime = TimerClock::now();
    }


    /**
      Stops and resets the timer. If the timer is currently running, the elapsed time is lost
    */
    void reset()
    {
      m_StartTime = std::chrono::time_point<TimerClock>();
      m_EndTime = std::chrono::time_point<TimerClock>();
      m_Running = false;
    }


    /**
      Returns true if the timer is currently running
      @return True if timer is running
    */
    bool isRunning()
    {
      return m_Running;
    }


    /**
      Returns the duration of the timer in milliseconds. 
      If the timer is stopped, it returns the duration between start and stop.
      If the timer is running, it returns the duration between start and the call of the elapsed() method.
      @return True if timer is running
    */
    long int elapsed()
    {
      std::chrono::time_point<TimerClock> EndTime;
      
      if(m_Running)
      {
          EndTime = TimerClock::now();
      }
      else
      {
          EndTime = m_EndTime;
      }
      
      return std::chrono::duration_cast<std::chrono::milliseconds>(EndTime - m_StartTime).count();
    }


    /**
      Returns the elapsed time as a pretty string representing days, hours, minutes and decimal seconds.
      @return the converted duration as a string
    */
    std::string elapsedAsPrettyString()
    {
      return getDurationAsPrettyString(elapsed());
    }


    /**
      Splits the given duration in milliseconds into days, hours, minutes, seconds and milliseconds.
      @param[in] MSecsDuration the duration in milliseconds
      @param[out] Days the number of days
      @param[out] Hours the number of hours
      @param[out] Minutes the number of minutes
      @param[out] Seconds the number of seconds
      @param[out] MSecs the number of milliseconds
    */
    static void splitDuration(long int MSecsDuration, int& Days, int& Hours, int& Minutes, int& Seconds, int& MSecs)
    {
      MSecs = (int) (MSecsDuration % 1000);
      Seconds = (int) (MSecsDuration / 1000) % 60;
      Minutes = (int) ((MSecsDuration / (1000*60)) % 60);
      Hours   = (int) ((MSecsDuration / (1000*60*60)) % 24);
      Days   = (int) (MSecsDuration / (1000*60*60*24));
    }


    /**
      Returns the given duration as a pretty string representing days, hours, minutes and decimal seconds.
      @snippet misc/timer.cpp durationstr
      @param[in] MSecsDuration the duration in milliseconds
      @return the converted duration as a string
    */
    static std::string getDurationAsPrettyString(long int MSecsDuration)
    {
      int MSecs, Seconds, Minutes, Hours, Days  = 0;

      splitDuration(MSecsDuration,Days,Hours,Minutes,Seconds,MSecs);

      std::stringstream MSecsSS;
      MSecsSS << std::setw(3) << std::setfill('0') << MSecs;

      std::string TmpStr = MSecsSS.str()+"s";

      if (!Days)
      {
        if (!Hours)
        {
          if (!Minutes)
          {
            if (!Seconds)
            {
              TmpStr = "0."+
                      TmpStr;
            }
            else
            {
              TmpStr = openfluid::tools::convertValue(Seconds)+"."+
                      TmpStr;
            }
          }
          else
          {
            TmpStr = openfluid::tools::convertValue(Minutes)+"m "+
                    openfluid::tools::convertValue(Seconds)+"."+
                    TmpStr;
          }
        }
        else
        {
          TmpStr = openfluid::tools::convertValue(Hours)+"h "+
                  openfluid::tools::convertValue(Minutes)+"m "+
                  openfluid::tools::convertValue(Seconds)+"."+
                  TmpStr;
        }
      }
      else
      {
        TmpStr = openfluid::tools::convertValue(Days)+"d "+
                openfluid::tools::convertValue(Hours)+"h "+
                openfluid::tools::convertValue(Minutes)+"m "+
                openfluid::tools::convertValue(Seconds)+"."+
                TmpStr;
      }

      return TmpStr;
    }
};


} }  // namespaces


#endif /* __OPENFLUID_TOOLS_TIMER_HPP__ */
