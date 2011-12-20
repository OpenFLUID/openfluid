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
 \file ResViewerAdapterModel.cpp
 \brief Implements ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#include "ResViewerAdapterModel.hpp"

#include <fstream>
#include <glibmm/i18n.h>
#include <boost/filesystem/path.hpp>

#include <openfluid/base/RuntimeEnv.hpp>
#include <openfluid/core/Unit.hpp>

#include "BuilderListStore.hpp"
#include "ResViewerColumns.hpp"

// =====================================================================
// =====================================================================


void ResViewerAdapterModelImpl::createColumns(std::vector<std::string> VarNames)
{
  delete mp_Columns;
  mp_Columns = new ResViewerColumns();

  for (unsigned int i = 0; i < VarNames.size(); i++)
  {
    Gtk::TreeModelColumn<std::string> * DataColumn = new Gtk::TreeModelColumn<
        std::string>();
    mp_Columns->addWithTitle(VarNames[i], *DataColumn);
  }
}

// =====================================================================
// =====================================================================


void ResViewerAdapterModelImpl::createFileBuffers(openfluid::core::Unit* Unit,
    std::string SetName)
{
    std::string DirPath =
        openfluid::base::RuntimeEnvironment::getInstance()->getOutputDir();

    std::string FileName;
    std::string AbsoluteFileName;
    std::string FilePath;

      FileName = Glib::ustring::compose("%1%2_%3.%4", Unit->getClass(),
          Unit->getID(), SetName, openfluid::config::OUTFILES_EXT);

    AbsoluteFileName = Glib::ustring::compose("%1/%2", DirPath, FileName);
    FilePath = boost::filesystem::path(AbsoluteFileName).string();

    if (m_FileContentsByName.find(FileName) == m_FileContentsByName.end())
    {
      std::ifstream File(FilePath.c_str());

      if (File)
      {
        std::stringstream StringBuf;
        StringBuf << File.rdbuf();
        File.close();

        Glib::RefPtr<Gtk::TextBuffer> TextBuf = Gtk::TextBuffer::create();
        TextBuf->set_text(StringBuf.str());

        m_FileContentsByName[FileName] = TextBuf;
      }
    }
}

// =====================================================================
// =====================================================================


void ResViewerAdapterModelImpl::setNoVarTitle()
{
  m_Title = "no variables to display at this time";
}

// =====================================================================
// =====================================================================


ResViewerAdapterModelImpl::ResViewerAdapterModelImpl() :
  mp_Columns(NULL)
{
  clear();
}

// =====================================================================
// =====================================================================


void ResViewerAdapterModelImpl::init(openfluid::core::Unit* Unit, std::vector<
    std::string> VarNames, openfluid::base::SimulationStatus* SimStatus,
    unsigned int Precision, std::string SetName, bool ShowFiles)
{
  m_Title = Glib::ustring::compose(_("Variables for Unit ID %1 Class %2"),
      Unit->getID(), Unit->getClass());

  createColumns(VarNames);

  m_FileContentsByName.clear();
  if (ShowFiles)
    createFileBuffers(Unit, SetName);

  m_refListStore = BuilderListStore::create(*mp_Columns);

  // populate ListStore
  do
  {
    Gtk::TreeRow Row = *(m_refListStore->append());

    unsigned int Step = SimStatus->getCurrentStep();

    Row[*mp_Columns->getStepColumn()] = Step;
    Row[*mp_Columns->getDateColumn()]
        = SimStatus->getCurrentTime().getAsISOString();

    for (unsigned int i = 0; i < VarNames.size(); i++)
    {
      std::string VarName = VarNames[i];

      openfluid::core::Value* Val = Unit->getVariables()->getValue(VarName, Step);

      if(Val)
        Row[*mp_Columns->getColumnWithTitle(VarName)] = Val->toString();

    }

  } while (SimStatus->switchToNextStep());

}

// =====================================================================
// =====================================================================


ResViewerColumns* ResViewerAdapterModelImpl::getColumns()
{
  return mp_Columns;
}

// =====================================================================
// =====================================================================


Glib::RefPtr<Gtk::TreeModel> ResViewerAdapterModelImpl::getTreeModel()
{
  return m_refListStore;
}

// =====================================================================
// =====================================================================


std::string ResViewerAdapterModelImpl::getTitle()
{
  return m_Title;
}

// =====================================================================
// =====================================================================


std::map<std::string, Glib::RefPtr<Gtk::TextBuffer> > ResViewerAdapterModelImpl::getFileContentsByName()
{
  return m_FileContentsByName;
}

// =====================================================================
// =====================================================================


void ResViewerAdapterModelImpl::clear()
{
  delete mp_Columns;
  mp_Columns = new ResViewerColumns();
  m_refListStore = BuilderListStore::create(*mp_Columns);
  setNoVarTitle();
  m_FileContentsByName.clear();
}

