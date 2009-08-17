
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
