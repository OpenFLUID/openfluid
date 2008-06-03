/**
  \file DistributedEvent.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __DISTRIBUTEDEVENT_H__
#define __DISTRIBUTEDEVENT_H__

// TODO    zieuter pour les mois dt.Set(..., wxDateTime::Month(wxDateTime::Jan + month - 1), ...);


#include <wx/wx.h>
#include <wx/hashmap.h>
#include "TypeDefs.h"
#include <wx/datetime.h>

namespace mhydasdk { namespace core {

WX_DECLARE_STRING_HASH_MAP(wxString, EventInfosMap);



class DistributedEvent
{
	
  private:
	  wxDateTime m_Date;
    EventInfosMap m_Infos;
    	
  public:	
    DistributedEvent();
    
    DistributedEvent(wxDateTime Date);

    ~DistributedEvent();

    bool isInfoExists(wxString Key);

    bool isInfoEquals(wxString Key, wxString Value);    

    bool isInfoEquals(wxString Key, long Value);
    
    bool isInfoEquals(wxString Key, double Value);
    
    bool isInfoEquals(wxString Key, MHYDASScalarValue *Info);    
    
    int getInfosCount() { return m_Infos.size(); }
    
    EventInfosMap getInfos() { return m_Infos; }
    
    wxDateTime getDateTime() { return m_Date; }
    
    bool getInfoAsString(wxString Key, wxString *Info);

    bool getInfoAsLong(wxString Key, long *Info);

    bool getInfoAsDouble(wxString Key, double *Info);

    bool getInfoAsMHYDASScalarValue(wxString Key, MHYDASScalarValue *Info);	
    
    bool addInfo(wxString Key, wxString Info);
	
    void println();
    
};

} } // namespace mhydasdk::core

#endif /* __DISTRIBUTEDEVENT_H__ */
