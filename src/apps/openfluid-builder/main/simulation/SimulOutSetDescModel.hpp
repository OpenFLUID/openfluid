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

#ifndef SIMULOUTSETDESCMODEL_HPP_
#define SIMULOUTSETDESCMODEL_HPP_

#include <sigc++/sigc++.h>

#include <openfluid/base.hpp>
#include <openfluid/machine.hpp>
#include <openfluid/core.hpp>

class SimulOutSetDescModel
{
  public:
    virtual sigc::signal<void> signal_InitForCreationAsked() = 0;
    virtual sigc::signal<void> signal_InitForEditionAsked() = 0;
    virtual sigc::signal<void> signal_ShowDialogAsked() = 0;
    virtual sigc::signal<void> signal_CloseDialogAsked() = 0;
    virtual sigc::signal<void, std::string> signal_ErrorDialogAsked() = 0;
    virtual void setEngineRequirements(
        openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance) = 0;
    virtual void
    initialize(openfluid::base::OutputSetDescriptor* SetDesc,
        std::string FileFormatName, std::vector<std::pair<std::string,
            openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect,
        std::map<std::string, std::pair<std::string,
            openfluid::base::OutputSetDescriptor> > SetsByName) = 0;
    virtual void showDialog() = 0;
    virtual openfluid::base::OutputSetDescriptor* getSetDesc() = 0;
    virtual std::vector<std::string> getClasses() = 0;
    virtual std::map<std::string, std::vector<unsigned int> >
    getByClassIDs() = 0;
    virtual std::map<std::string, std::vector<std::string> >
    getByClassVars() = 0;
    virtual std::vector<std::string> getFormatNames() = 0;
    virtual void setName(std::string SetName) = 0;
    virtual void setClass(std::string Class) = 0;
    virtual void setFormatName(std::string FormatName) = 0;
    virtual void setPrecision(int Precision) = 0;
    virtual void setIDs(std::vector<unsigned int> IDs) = 0;
    virtual void setVars(std::vector<std::string> Vars) = 0;
    virtual std::string getName() = 0;
    virtual std::string getClass() = 0;
    virtual std::string getFormatName() = 0;
    virtual int getFormatIndex() = 0;
    virtual int getPrecision() = 0;
    virtual std::vector<unsigned int> getIDs() = 0;
    virtual std::vector<std::string> getVars() = 0;
    virtual void save() = 0;
    virtual void cancel() = 0;
};

class SimulOutSetDescModelImpl: public SimulOutSetDescModel
{
  private:
    sigc::signal<void> m_signal_InitForCreationAsked;
    sigc::signal<void> m_signal_InitForEditionAsked;
    sigc::signal<void> m_signal_ShowDialogAsked;
    sigc::signal<void> m_signal_CloseDialogAsked;
    sigc::signal<void, std::string> m_signal_ErrorDialogAsked;
    openfluid::base::OutputDescriptor* mp_OutDesc;
    openfluid::core::CoreRepository* mp_CoreRepos;
    openfluid::machine::ModelInstance* mp_ModelInstance;
    std::string m_FormatName;
    std::vector<std::string> m_Classes;
    std::map<std::string, std::vector<unsigned int> > m_ByClassIDs;
    std::map<std::string, std::vector<std::string> > m_ByClassVars;
    std::vector<std::string> m_FormatNames;
    std::map<std::string, std::pair<std::string,
        openfluid::base::OutputSetDescriptor> > m_SetsByName;
    bool checkSetNameFormat();
  protected:
    openfluid::base::OutputSetDescriptor* mp_SetDesc;
    std::map<std::string, std::vector<std::string> > m_ByClassScalarVars;
    std::map<std::string, std::vector<std::string> > m_ByClassVectorVars;
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> >
        m_FilesFormatsByNameVect;
    void extractClassesAndIDs();
    void extractVars();
    void extractFormatNames();
  public:
    SimulOutSetDescModelImpl();
    sigc::signal<void> signal_InitForCreationAsked();
    sigc::signal<void> signal_InitForEditionAsked();
    sigc::signal<void> signal_ShowDialogAsked();
    sigc::signal<void> signal_CloseDialogAsked();
    sigc::signal<void, std::string> signal_ErrorDialogAsked();
    void setEngineRequirements(openfluid::base::OutputDescriptor& OutDesc,
        openfluid::core::CoreRepository& CoreRepos,
        openfluid::machine::ModelInstance& ModelInstance);
    void initialize(openfluid::base::OutputSetDescriptor* SetDesc,
        std::string FileFormatName, std::vector<std::pair<std::string,
            openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect,
        std::map<std::string, std::pair<std::string,
            openfluid::base::OutputSetDescriptor> > SetsByName);
    void showDialog();
    openfluid::base::OutputSetDescriptor* getSetDesc();
    std::vector<std::string> getClasses();
    std::map<std::string, std::vector<unsigned int> > getByClassIDs();
    std::map<std::string, std::vector<std::string> > getByClassVars();
    std::vector<std::string> getFormatNames();
    void setName(std::string SetName);
    void setClass(std::string Class);
    void setFormatName(std::string FormatName);
    void setPrecision(int Precision);
    void setIDs(std::vector<unsigned int> IDs);
    void setVars(std::vector<std::string> Vars);
    std::string getName();
    std::string getClass();
    std::string getFormatName();
    int getFormatIndex();
    int getPrecision();
    std::vector<unsigned int> getIDs();
    std::vector<std::string> getVars();
    void save();
    void cancel();
};

class SimulOutSetDescModelSub: public SimulOutSetDescModelImpl
{
  public:
    void extractClassesAndIDs();
    void extractVars();
    void extractFormatNames();
    void setFilesFormatsByNameVect(std::vector<std::pair<std::string,
        openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect);
    std::map<std::string, std::vector<std::string> > getByClassScalarVars();
    std::map<std::string, std::vector<std::string> > getByClassVectorVars();
};

#endif /* SIMULOUTSETDESCMODEL_HPP_ */
