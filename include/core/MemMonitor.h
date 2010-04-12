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


#ifndef __MEMMONITOR_H__
#define __MEMMONITOR_H__

namespace openfluid { namespace core {



class MemoryMonitor
{

  private:

    static MemoryMonitor* mp_Singleton;

    TimeStep_t m_LastMemRelease;

    unsigned int m_Packet;
    unsigned int m_Keep;


    MemoryMonitor() { m_LastMemRelease = -1; m_Packet = 0; m_Keep = 0;};


  public:

    static MemoryMonitor* getInstance()
    {
      if (mp_Singleton == NULL) mp_Singleton = new MemoryMonitor();
      return mp_Singleton;
    };


    void setPacketAndKeep(unsigned int Packet, unsigned int Keep) { m_Packet = Packet; m_Keep = Keep; StepsReservation::setReservation(Packet+Keep);};

    unsigned int getPacket() { return m_Packet; };

    unsigned int getKeep() { return m_Keep; };

    void setLastMemoryRelease(TimeStep_t CurrentStep) { m_LastMemRelease = CurrentStep-m_Keep;};

    TimeStep_t getLastMemoryRelease() { return m_LastMemRelease; };

    void getMemoryReleaseRange(TimeStep_t CurrentStep, bool WithoutKeep, TimeStep_t* BeginStep, TimeStep_t* EndStep)
    {
      *BeginStep = m_LastMemRelease+1;
      *EndStep = CurrentStep;

      if (!WithoutKeep) *EndStep = CurrentStep - m_Keep;
    };

    bool isMemReleaseStep(TimeStep_t Step)
    {
      return (Step-m_LastMemRelease == (m_Keep+m_Packet));
    };


};



} } // namespaces

#endif /* __MEMMONITOR_H__ */
