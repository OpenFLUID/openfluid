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
 \file SimulRunModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __SIMULRUNMODEL_HPP__
#define __SIMULRUNMODEL_HPP__

#include <sigc++/sigc++.h>
#include <string>

namespace openfluid {
namespace base {
class RunDescriptor;
}
}

class SimulRunModel
{
  public:

    virtual sigc::signal<void> signal_FromAppDescriptorChanged() = 0;

    virtual sigc::signal<void> signal_FromAppValidityChanged() = 0;

    virtual sigc::signal<void> signal_SimulRunChanged() = 0;

    virtual void
    setEngineRequirements(openfluid::base::RunDescriptor& RunDesc) = 0;

    virtual int getDelta() = 0;

    virtual std::string getBeginColor() = 0;

    virtual std::string getEndColor() = 0;

    virtual bool isDateTimeCoherent() = 0;

    virtual bool isDateTimeValid() = 0;

    virtual std::string getBegin() = 0;

    virtual std::string getEnd() = 0;

    virtual bool isValuesBuffSet() = 0;

    virtual int getValuesBuff() = 0;

    virtual int getFilesBuff() = 0;

    virtual void setDelta(int Value) = 0;

    virtual void setBegin(std::string Begin) = 0;

    virtual void setEnd(std::string End) = 0;

    virtual void setValuesBuffIsSet(bool IsSet) = 0;

    virtual void setValuesBuff(int Value) = 0;

    virtual void setFilesBuff(int Value) = 0;
};

class SimulRunModelImpl: public SimulRunModel
{
  private:

    sigc::signal<void> m_signal_FromAppDescriptorChanged;

    sigc::signal<void> m_signal_FromAppValidityChanged;

    sigc::signal<void> m_signal_SimulRunChanged;

    openfluid::base::RunDescriptor* mp_RunDesc;

    bool m_isCurrentBeginValid;
    bool m_isCurrentEndValid;

    bool m_isDateTimeCoherent;

    bool m_isDateTimeValid;

    std::string m_BeginColorString;
    std::string m_EndColorString;

    void checkDateTimeCoherence();

  public:

    SimulRunModelImpl();

    sigc::signal<void> signal_FromAppDescriptorChanged();

    sigc::signal<void> signal_FromAppValidityChanged();

    sigc::signal<void> signal_SimulRunChanged();

    void setEngineRequirements(openfluid::base::RunDescriptor& RunDesc);

    int getDelta();

    std::string getBeginColor();
    std::string getEndColor();

    bool isDateTimeValid();

    bool isDateTimeCoherent();

    std::string getBegin();
    std::string getEnd();

    int getValuesBuff();
    bool isValuesBuffSet();

    int getFilesBuff();

    void setDelta(int Value);

    void setBegin(std::string Begin);
    void setEnd(std::string End);

    void setValuesBuffIsSet(bool IsSet);
    void setValuesBuff(int Value);

    void setFilesBuff(int Value);
};

class SimulRunModelSub: public SimulRunModelImpl
{
  public:
};

#endif /* __SIMULRUNMODEL_HPP__ */
