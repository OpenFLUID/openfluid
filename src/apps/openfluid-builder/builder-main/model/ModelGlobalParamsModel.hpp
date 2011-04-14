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
 \file ModelGlobalParamsModel.h
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __MODELGLOBALPARAMSMODEL_H__
#define __MODELGLOBALPARAMSMODEL_H__

#include <sigc++/sigc++.h>
#include <set>

#include <openfluid/machine.hpp>

class ModelGlobalParamsModel
{
  public:

    virtual sigc::signal<void> signal_FromAppModelChanged() = 0;

    virtual sigc::signal<void, std::string> signal_GlobalParamSet() = 0;

    virtual sigc::signal<void, std::string> signal_GlobalParamUnset() = 0;

    virtual sigc::signal<void, std::string> signal_GlobalValueChanged() = 0;

    virtual void setEngineRequirements(
        openfluid::machine::ModelInstance& ModelInstance) = 0;

    virtual void update() = 0;

    virtual std::map<std::string, std::string> getGloballyUsed() = 0;

    virtual std::set<std::string> getGloballyNotUsed() = 0;

    virtual std::vector<std::string> getGloballyNoMoreUsed() = 0;

    virtual std::string fromUserGloballyUsedSet(std::string ParamName) = 0;

    virtual void fromUserGloballyUsedUnset(std::string ParamName) = 0;

    virtual void
    setGlobalValue(std::string ParamName, std::string ParamValue) = 0;

    virtual std::string getGlobalValue(std::string ParamName) = 0;

    virtual bool isGloballyDefined(std::string ParamName) = 0;

    virtual std::map<std::string,std::string> getGlobalValues(openfluid::machine::ModelItemInstance* Item) = 0;

};

class ModelGlobalParamsModelImpl: public ModelGlobalParamsModel
{
  private:

    sigc::signal<void> m_signal_FromAppModelChanged;

    sigc::signal<void, std::string> m_signal_GlobalParamSet;

    sigc::signal<void, std::string> m_signal_GlobalParamUnset;

    sigc::signal<void, std::string> m_signal_GlobalValueChanged;

    openfluid::machine::ModelInstance* mp_ModelInstance;

    std::map<std::string, std::string> m_ByParamNameParamUnit;

  protected:

    std::map<std::string, std::string> m_GloballyUsed;

    std::set<std::string> m_GloballyNotUsed;

    std::vector<std::string> m_GloballyNoMoreUsed;

    std::map<std::string, std::string> m_TempNewGloballyUsed;

    void dispatchParam(std::string ParamName);

    void afterDispatch();

  public:

    ModelGlobalParamsModelImpl();

    sigc::signal<void> signal_FromAppModelChanged();

    sigc::signal<void, std::string> signal_GlobalParamSet();

    sigc::signal<void, std::string> signal_GlobalParamUnset();

    sigc::signal<void, std::string> signal_GlobalValueChanged();

    void
    setEngineRequirements(openfluid::machine::ModelInstance& ModelInstance);

    void update();

    std::map<std::string, std::string> getGloballyUsed();

    std::set<std::string> getGloballyNotUsed();

    std::vector<std::string> getGloballyNoMoreUsed();

    std::string fromUserGloballyUsedSet(std::string ParamName);

    void fromUserGloballyUsedUnset(std::string ParamName);

    void setGlobalValue(std::string ParamName, std::string ParamValue);

    std::string getGlobalValue(std::string ParamName);

    bool isGloballyDefined(std::string ParamName);

    std::map<std::string,std::string> getGlobalValues(openfluid::machine::ModelItemInstance* Item);

};

class ModelGlobalParamsModelSub: public ModelGlobalParamsModelImpl
{
  public:

    void setGloballyUsed(std::map<std::string, std::string> Map);

    void setGloballyNotUsed(std::set<std::string> Set);

    void clearGloballyNotUsed();

    void clearTempNewGloballyUsed();

    void clearGloballyNoMoreUsed();

    std::map<std::string, std::string> getTempNewGloballyUsed();

    void dispatchParam(std::string ParamName);

    void afterDispatch();

};
#endif /* __MODELGLOBALPARAMSMODEL_H__ */
