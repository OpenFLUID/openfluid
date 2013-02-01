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

#ifndef MODELSTRUCTUREMODEL_HPP_
#define MODELSTRUCTUREMODEL_HPP_

#include <sigc++/sigc++.h>
#include <string>
#include <vector>

namespace openfluid {
namespace guicommon {
class BuilderModel;
}
namespace fluidx {
class ModelItemDescriptor;
}
}

class ModelStructureModel
{
  public:

    virtual sigc::signal<void> signal_FromUserSelectionChanged() = 0;

    virtual sigc::signal<void> signal_FromAppModelChanged() = 0;

    virtual sigc::signal<void> signal_FromAppSelectionRequested() = 0;

    virtual std::vector<std::string> getModelIDs() = 0;

    virtual void appendFunction(
        openfluid::fluidx::ModelItemDescriptor* ItemDesc) = 0;

    /* Move the element at From position before the action to be at To position after the action,
     * positions starting from 0. */
    virtual void moveFunction(unsigned int From, unsigned int To) = 0;

    virtual void moveTowardTheBegin() = 0;

    virtual void moveTowardTheEnd() = 0;

    virtual void removeFunctionAt(int Position) = 0;

    virtual void setCurrentSelectionByUserAt(int Position) = 0;

    virtual int getCurrentSelection() = 0;

    virtual unsigned int getFctCount() = 0;

    virtual void requestSelectionByAppAt(int Position) = 0;

    virtual void requestSelectionByApp(std::string FunctionName) = 0;

    virtual int getAppRequestedSelection() = 0;

    virtual void update() = 0;

    virtual ~ModelStructureModel()
    {
    }
    ;

};

class ModelStructureModelImpl: public ModelStructureModel
{
  private:

    sigc::signal<void> m_signal_FromUserSelectionChanged;

    sigc::signal<void> m_signal_FromAppModelChanged;

    sigc::signal<void> m_signal_FromAppSelectionRequested;

    openfluid::guicommon::BuilderModel* mp_Model;

    int m_CurrentSelection;

    int m_AppRequestedSelection;

    bool isModelEmpty();

    int getLastPosition();

  public:

    ModelStructureModelImpl(openfluid::guicommon::BuilderModel& Model);

    ~ModelStructureModelImpl();

    sigc::signal<void> signal_FromUserSelectionChanged();

    sigc::signal<void> signal_FromAppModelChanged();

    sigc::signal<void> signal_FromAppSelectionRequested();

    std::vector<std::string> getModelIDs();

    void appendFunction(openfluid::fluidx::ModelItemDescriptor* ItemDesc);

    void moveFunction(unsigned int From, unsigned int To);

    void moveTowardTheBegin();

    void moveTowardTheEnd();

    void removeFunctionAt(int Position);

    void setCurrentSelectionByUserAt(int Position);

    int getCurrentSelection();

    unsigned int getFctCount();

    void requestSelectionByAppAt(int Position);

    void requestSelectionByApp(std::string FunctionName);

    int getAppRequestedSelection();

    void update();

};

#endif /* MODELSTRUCTUREMODEL_HPP_ */
