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
 \file ModelFctParamsView.hpp
 \brief Header of ...

 \author Aline LIBRES <libres@supagro.inra.fr>
 */

#ifndef __MODELFCTPARAMSVIEW_HPP__
#define __MODELFCTPARAMSVIEW_HPP__

#include <sigc++/sigc++.h>

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/table.h>

class ModelFctParamRow;
class ModelFctFileRow;
class BuilderTableRowWidget;

// =====================================================================
// =====================================================================


class ModelFctParamsView
{
  public:

    virtual Gtk::Widget* asWidget() = 0;

    virtual void setParams(std::map<std::string, std::string> ParamsMap) = 0;

    virtual void setParamValues(
        std::map<std::string, std::string> ParamValuesMap) = 0;

    virtual sigc::signal<void, std::string, std::string>
    signal_ParamValueChanged() = 0;

    virtual sigc::signal<void> signal_RequiredFileChanged() = 0;

    virtual void updateFiles(std::vector<std::string> RequiredFiles,
        std::vector<std::string> UsedFiles) = 0;

    virtual void updateGlobalValues(
        std::map<std::string, std::string> GlobalValuesMap) = 0;

    virtual void whenRequiredFileChanged() = 0;

};

// =====================================================================
// =====================================================================


class ModelFctParamsViewImpl: public ModelFctParamsView
{
  private:

    Gtk::Box* mp_MainBox;

    Gtk::ScrolledWindow* mp_MainWin;

    Gtk::Table* mp_ParamsTable;

    Gtk::Table* mp_FilesTable;

    unsigned int m_ParamsTableBottom;

    unsigned int m_FilesTableBottom;

    std::map<std::string, ModelFctParamRow*> m_ByParamNameParamRow;

    std::map<std::string, ModelFctFileRow*> m_ByFileNameFileRow;

    sigc::signal<void, std::string, std::string> m_signal_ParamValueChanged;

    sigc::signal<void> m_signal_RequiredFileChanged;

    void attachTableRow(BuilderTableRowWidget& TableRow, Gtk::Table* Table,
        unsigned int& TableBottom);

    void onParamValueChanged(ModelFctParamRow* FctParamRow);

    void setRequiredFiles(std::vector<std::string> Files);

    void setUsedFiles(std::vector<std::string> Files);

  public:

    ModelFctParamsViewImpl();

    sigc::signal<void, std::string, std::string> signal_ParamValueChanged();

    sigc::signal<void> signal_RequiredFileChanged();

    void setParams(std::map<std::string, std::string> ParamsMap);

    void setParamValues(std::map<std::string, std::string> ParamValuesMap);

    void updateGlobalValues(std::map<std::string, std::string> GlobalValuesMap);

    void updateFiles(std::vector<std::string> RequiredFiles, std::vector<
        std::string> UsedFiles);

    void whenRequiredFileChanged();

    Gtk::Widget* asWidget();
};

#endif /* __MODELFCTPARAMSVIEW_HPP__ */
