/**
  \file ExecMsgs.h
  \brief Header of class managing execution messages

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __EXECMSGS_H__
#define __EXECMSGS_H__

#include <string>
#include <vector>

#include "TypeDefs.h"
#include "SwissTools.h"

namespace openfluid { namespace base {

/**
  Class ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class ExecutionMessages
{

  private:
    bool m_WarningFlag;
    bool m_ErrorFlag;

    std::vector<std::string> m_WarningMsgs;
    std::string m_ErrorMsg;


  public:

    /**
      Constructor
    */
    ExecutionMessages();

    /**
      Destructor
    */
    ~ExecutionMessages();

    void addWarning(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string WarningMsg);

    void addWarning(std::string Sender, std::string WarningMsg) { addWarning(Sender,-1,WarningMsg); };

    void resetWarningFlag() { m_WarningFlag = false; };

    bool isWarningFlag() const { return m_WarningFlag; };

    std::vector<std::string> getWarningMsgs() const { return m_WarningMsgs; };

};

} } // namespace openfluid::base


#endif /*__EXECMSGS_H__*/

