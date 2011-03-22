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
 \file SimulOutFilesModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __SIMULOUTFILESMODEL_HPP__
#define __SIMULOUTFILESMODEL_HPP__

#include <sigc++/sigc++.h>

#include <openfluid/base.hpp>

class SimulOutFilesModel
{
  public:
    virtual sigc::signal<void> signal_FromAppDescriptorChanged() = 0;
    virtual sigc::signal<void> signal_FromUserSelectionChanged() = 0;
    virtual sigc::signal<void>
    signal_FromAppShowDialogConfirmDeletionAsked() = 0;
    virtual void setEngineRequirements(
        openfluid::base::OutputDescriptor& OutDesc) = 0;
    virtual Glib::ustring generateNextFormatName() = 0;
    virtual std::vector<std::pair<std::string,
        openfluid::base::OutputFilesDescriptor> >
    getFilesFormatsByNameVect() = 0;
    virtual void setSelectedFileFormatIndex(int Index) = 0;
    virtual openfluid::base::OutputFilesDescriptor* getSelectedFileFormat() = 0;
    virtual std::string getSelectedFileFormatName() = 0;
    virtual void deleteSelectedFileFormat() = 0;
    virtual void deleteSelectedFileFormatConfirmed() = 0;
    virtual void
    addFileFormat(openfluid::base::OutputFilesDescriptor* FileDesc, std::string FormatName) = 0;
    virtual void updateSelectedFileFormat(std::string FormatName) = 0;
};

class SimulOutFilesModelImpl: public SimulOutFilesModel
{
  private:
    sigc::signal<void> m_signal_FromAppDescriptorChanged;
    sigc::signal<void> m_signal_FromUserSelectionChanged;
    sigc::signal<void> m_signal_FromAppShowDialogConfirmDeletionAsked;
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> >
        m_FilesFormatsByNameVect;
    int m_CurrentFormatNameIndex;
    bool isSelectedFileFormatIndexValid();
  protected:
    int m_SelectedFileFormatIndex;
    openfluid::base::OutputDescriptor* mp_OutDesc;
    //    std::string replacedBlankStr(std::string Str)
    //    {
    //      std::string::size_type Pos;
    //      while ((Pos = Str.find(" ")) != std::string::npos)
    //        Str.replace(Pos, 1, m_BlankSubstitute);
    //      return Str;
    //    }
  public:
    SimulOutFilesModelImpl() ;
    sigc::signal<void> signal_FromAppDescriptorChanged();
    sigc::signal<void> signal_FromUserSelectionChanged();
    sigc::signal<void> signal_FromAppShowDialogConfirmDeletionAsked();
    Glib::ustring generateNextFormatName();
    void setEngineRequirements(openfluid::base::OutputDescriptor& OutDesc);
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> > getFilesFormatsByNameVect();
    void setSelectedFileFormatIndex(int Index);
    openfluid::base::OutputFilesDescriptor* getSelectedFileFormat();
    std::string getSelectedFileFormatName();
    void deleteSelectedFileFormat();
    void deleteSelectedFileFormatConfirmed();
    void addFileFormat(openfluid::base::OutputFilesDescriptor* FileDesc, std::string FormatName);
    void updateSelectedFileFormat(std::string FormatName);
};

class SimulOutFilesModelSub: public SimulOutFilesModelImpl
{
  public:
    int getSelectedFileFormatIndex();
};

#endif /* __SIMULOUTFILESMODEL_HPP__ */
