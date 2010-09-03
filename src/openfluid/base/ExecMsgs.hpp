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

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#ifndef __EXECMSGS_HPP__
#define __EXECMSGS_HPP__

#include <string>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/core/TypeDefs.hpp>
#include <openfluid/tools/SwissTools.hpp>
#include <openfluid/base/Message.hpp>


namespace openfluid { namespace base {

/**
  Class for managin messages during execution

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/
class DLLEXPORT ExecutionMessages
{

  private:

    bool m_WarningFlag;
    bool m_ErrorFlag;

    std::vector<Message> m_WarningMsgs;
    std::string m_ErrorMsg;

    unsigned int m_RealWarningsCount;

    void addWarning(Message Msg);


  public:

    /**
      Constructor
     */
    ExecutionMessages();

    /**
      Destructor
    */
    ~ExecutionMessages();

    void addWarning(std::string Sender, std::string Source, openfluid::core::TimeStep_t TimeStep, std::string WarningMsg) { addWarning(Message(Sender,Source,TimeStep,WarningMsg)); };

    void addWarning(std::string Sender, openfluid::core::TimeStep_t TimeStep, std::string WarningMsg) { addWarning(Message(Sender,"",TimeStep,WarningMsg)); };

    void addWarning(std::string Sender, std::string Source, std::string WarningMsg) { addWarning(Message(Sender,Source,WarningMsg)); };

    void addWarning(std::string Sender, std::string WarningMsg) { addWarning(Message(Sender,"",WarningMsg)); };

    inline void resetWarningFlag() { m_WarningFlag = false; };

    inline bool isWarningFlag() const { return m_WarningFlag; };

    inline std::vector<Message> getWarningMsgs() const { return m_WarningMsgs; };

    inline void doMemRelease() { m_WarningMsgs.clear(); };

    inline unsigned int getWarningsCount() const { return m_RealWarningsCount; };

};


} } // namespace openfluid::base


#endif /*__EXECMSGS_HPP__*/

