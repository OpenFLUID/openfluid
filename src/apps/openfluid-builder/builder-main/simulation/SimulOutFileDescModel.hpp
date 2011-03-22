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
 \file SimulOutFileDescModel.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __SIMULOUTFILEDESCMODEL_HPP__
#define __SIMULOUTFILEDESCMODEL_HPP__

#include <sigc++/sigc++.h>

#include <openfluid/base.hpp>

class SimulOutFileDescModel
{
  public:
    virtual sigc::signal<void> signal_InitForCreationAsked() = 0;
    virtual sigc::signal<void> signal_InitForEditionAsked() = 0;
    virtual sigc::signal<void> signal_ShowDialogAsked() = 0;
    virtual sigc::signal<void> signal_CloseDialogAsked() = 0;
    virtual sigc::signal<void, std::string> signal_ErrorDialogAsked() = 0;
    virtual void setEngineRequirements(
        openfluid::base::OutputDescriptor& OutDesc) = 0;
    virtual void
        initialize(
            openfluid::base::OutputFilesDescriptor* FileDesc,
            std::string FileFormatName,
            std::vector<std::pair<std::string,
                openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect) = 0;
    virtual void showDialog() = 0;
    virtual openfluid::base::OutputFilesDescriptor* getFileDesc() = 0;
    virtual std::string getFormatName() = 0;
    virtual std::vector<std::string> getChars() = 0;
    virtual std::vector<std::string> getDateFormats() = 0;
    virtual std::string getColSeparator() = 0;
    virtual std::string getDateFormat() = 0;
    virtual std::string getCommentChar() = 0;
    virtual void setColSeparator(std::string Value) = 0;
    virtual void setDateFormat(std::string Value) = 0;
    virtual void setCommentChar(std::string Value) = 0;
    virtual void setFormatName(std::string Value) = 0;
    virtual void save() = 0;
    virtual void cancel() = 0;
};

class SimulOutFileDescModelImpl: public SimulOutFileDescModel
{
  private:
    sigc::signal<void> m_signal_InitForCreationAsked;
    sigc::signal<void> m_signal_InitForEditionAsked;
    sigc::signal<void> m_signal_ShowDialogAsked;
    sigc::signal<void> m_signal_CloseDialogAsked;
    sigc::signal<void, std::string> m_signal_ErrorDialogAsked;
    openfluid::base::OutputDescriptor* mp_OutDesc;
    std::vector<std::string> m_Chars;
    std::vector<std::string> m_DateFormats;
    std::string m_FormatName;
    std::vector<std::pair<std::string, openfluid::base::OutputFilesDescriptor> >
        m_FilesFormatsByNameVect;
    bool checkFileFormatName();
  protected:
    openfluid::base::OutputFilesDescriptor* mp_FileDesc;
  public:
    SimulOutFileDescModelImpl();
    sigc::signal<void> signal_InitForCreationAsked();
    sigc::signal<void> signal_InitForEditionAsked();
    sigc::signal<void> signal_ShowDialogAsked();
    sigc::signal<void> signal_CloseDialogAsked();
    sigc::signal<void, std::string> signal_ErrorDialogAsked();
    void setEngineRequirements(openfluid::base::OutputDescriptor& OutDesc);
    void initialize(openfluid::base::OutputFilesDescriptor* FileDesc,
        std::string FileFormatName, std::vector<std::pair<std::string,
            openfluid::base::OutputFilesDescriptor> > FilesFormatsByNameVect);
    void showDialog();
    openfluid::base::OutputFilesDescriptor* getFileDesc();
    std::string getFormatName();
    std::vector<std::string> getChars();
    std::vector<std::string> getDateFormats();
    std::string getColSeparator();
    std::string getDateFormat();
    std::string getCommentChar();
    void setColSeparator(std::string Value);
    void setDateFormat(std::string Value);
    void setCommentChar(std::string Value);
    void setFormatName(std::string Value);
    void save();
    void cancel();
};

class SimulOutFileDescModelSub: public SimulOutFileDescModelImpl
{
  public:
};

#endif /* __SIMULOUTFILEDESCMODEL_HPP__ */
