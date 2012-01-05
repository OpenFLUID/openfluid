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
 \file ModelFctParamsModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __MODELFCTPARAMSMODEL_HPP__
#define __MODELFCTPARAMSMODEL_HPP__

#include <sigc++/sigc++.h>
#include <map>
#include <vector>
#include <string>

namespace openfluid {
namespace machine {
class ModelInstance;
class ModelItemInstance;
}
}

class ModelFctParamsModel
{
  public:

    virtual sigc::signal<void> signal_ItemInit() = 0;

    virtual sigc::signal<void> signal_GlobalValueChanged() = 0;

    virtual sigc::signal<void> signal_RequiredFileChanged() = 0;

    virtual sigc::signal<void> signal_ParamsChanged() = 0;

    virtual sigc::signal<void> signal_ParamsChangedFromApp() = 0;

    virtual sigc::signal<void> signal_RequiredFilesChangedFromApp() = 0;

    virtual void setModelItemInstance(
        openfluid::machine::ModelItemInstance* Item) = 0;

    virtual std::map<std::string, std::string> getParams() = 0;

    virtual std::vector<std::string> getRequiredFiles() = 0;

    virtual std::vector<std::string> getUsedFiles() = 0;

    virtual std::map<std::string, std::string> getParamValues() = 0;

    virtual std::map<std::string, std::string> getGlobalValues() = 0;

    virtual void
        setParamValue(std::string ParamName, std::string ParamValue) = 0;

    virtual void updateGlobalValues() = 0;

    virtual void updateParamsValues() = 0;

    virtual void whenRequiredFileChanged() = 0;

};

class ModelFctParamsModelImpl: public ModelFctParamsModel
{
  private:

    sigc::signal<void> m_signal_ItemInit;

    sigc::signal<void> m_signal_RequiredFileChanged;

    sigc::signal<void> m_signal_ParamsChanged;

    sigc::signal<void> m_signal_ParamsChangedFromApp;

    sigc::signal<void> m_signal_GlobalValueChanged;

    sigc::signal<void> m_signal_RequiredFilesChangedFromApp;

    openfluid::machine::ModelItemInstance* mp_Item;

    openfluid::machine::ModelInstance* mp_ModelInstance;

    void updateInterpGeneratorRequiredExtraFiles();

    bool fileExists(std::string FileName);

  public:

    ModelFctParamsModelImpl(openfluid::machine::ModelInstance* ModelInstance);

    void setModelItemInstance(openfluid::machine::ModelItemInstance* Item);

    sigc::signal<void> signal_ItemInit();

    sigc::signal<void> signal_GlobalValueChanged();

    sigc::signal<void> signal_RequiredFileChanged();

    sigc::signal<void> signal_ParamsChanged();

    sigc::signal<void> signal_ParamsChangedFromApp();

    sigc::signal<void> signal_RequiredFilesChangedFromApp();

    std::map<std::string, std::string> getParams();

    std::vector<std::string> getRequiredFiles();

    std::vector<std::string> getUsedFiles();

    std::map<std::string, std::string> getParamValues();

    std::map<std::string, std::string> getGlobalValues();

    void setParamValue(std::string ParamName, std::string ParamValue);

    void updateGlobalValues();

    void updateParamsValues();

    void whenRequiredFileChanged();

};

#endif /* __MODELFCTPARAMSMODEL_HPP__ */
