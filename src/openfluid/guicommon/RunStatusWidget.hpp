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
  \file RunStatusWidget.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __RUNSTATUSWIDGET_HPP__
#define __RUNSTATUSWIDGET_HPP__

#include <gtkmm.h>

#include <openfluid/dllexport.hpp>


namespace openfluid { namespace guicommon {


// =====================================================================
// =====================================================================


class DLLEXPORT RunStatusWidget : public Gtk::EventBox
{
  private:
    Gtk::VBox m_MainBox;
    Gtk::HBox m_InfosBox;

    Gtk::ProgressBar m_RunProgressBar;
    Gtk::Label m_PresimLabel;
    Gtk::Label m_InitLabel;
    Gtk::Label m_RunstepLabel;
    Gtk::Label m_FinalLabel;

    std::string m_PresimText;
    std::string m_InitText;
    std::string m_RunstepText;
    std::string m_CurrentStepText;
    std::string m_FinalText;

    std::string m_LastStepNbrStr;

  public:

    RunStatusWidget();

    ~RunStatusWidget();

    void setProgressFraction(double Fraction);

    void setPresimDefault() { m_PresimLabel.set_markup("<span foreground='#777777'>"+m_PresimText+"</span>"); };

    void setPresimRunning() { m_PresimLabel.set_markup("<b>"+m_PresimText+"</b>"); };

    void setPresimDone() { m_PresimLabel.set_markup(m_PresimText); };

    void setInitDefault() { m_InitLabel.set_markup("<span foreground='#777777'>"+m_InitText+"</span>"); };

    void setInitRunning() { m_InitLabel.set_markup("<b>"+m_InitText+"</b>"); };

    void setInitDone() { m_InitLabel.set_markup(m_InitText); };

    void setRunstepDefault() { m_RunstepLabel.set_markup("<span foreground='#777777'>"+m_RunstepText+" "+m_CurrentStepText+"</span>"); };

    void setRunstepRunning() { m_RunstepLabel.set_markup("<b>"+m_RunstepText+" "+m_CurrentStepText+"</b>");  };

    void setRunstepDone() { m_RunstepLabel.set_markup(m_RunstepText+" "+m_CurrentStepText);};

    void setFinalDefault() { m_FinalLabel.set_markup("<span foreground='#777777'>"+m_FinalText+"</span>"); };

    void setFinalRunning() { m_FinalLabel.set_markup("<b>"+m_FinalText+"</b>"); };

    void setFinalDone() { m_FinalLabel.set_markup(m_FinalText); };

    void updateCurrentStep(std::string StepNbrStr) { m_CurrentStepText = StepNbrStr+"/"+m_LastStepNbrStr; };

    void setLastStepStr(std::string StepNbrStr) { m_LastStepNbrStr = StepNbrStr; };

};


} } //namespaces


#endif /* __RUNSTATUSWIDGET_HPP__ */
