/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/*
 * MarketWizardPage.hpp
 *
 *  Created on: 9 août 2013
 *      Author: Manuel CHATAIGNER
 */

#ifndef __MARKETWIZARDPAGE_HPP__
#define __MARKETWIZARDPAGE_HPP__

#include <openfluid/dllexport.hpp>
#include <QWizardPage>


namespace openfluid { namespace ui { namespace market {


class DLLEXPORT MarketWizardPage : public QWizardPage
{
  Q_OBJECT;

  private:

    bool m_PackagesSelected;
    bool m_LicensesRadioAccepted;
    bool m_InstallationFinished;

    bool isSelectionPage() const { return nextId() == 1; };
    bool isLicensesnPage() const { return nextId() == 2; };
    bool isInstallPage() const { return nextId() == -1; };

  public:

    MarketWizardPage(QWidget *Parent = 0);
    virtual bool isComplete() const;
    void setPageComplete(bool Complete);
};


} } } // namespaces


#endif /* __MARKETWIZARDPAGE_HPP__ */
