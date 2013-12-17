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


/**
  \file EditMarketplaceDialog.hpp
  \brief Header of ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __EDITMARKETPLACEDIALOG_HPP__
#define __EDITMARKETPLACEDIALOG_HPP__

#include "OpenFLUIDDialog.hpp"

#include <openfluid/guicommon/PreferencesManager.hpp>


namespace Ui
{
  class EditMarketplaceDialog;
}



class EditMarketplaceDialog : public OpenFLUIDDialog
{
  Q_OBJECT;

  private slots:

    void checkGlobally();


  private:

    Ui::EditMarketplaceDialog* ui;

    bool m_IsEditMode;

    QString m_OriginalName;

    const openfluid::guicommon::PreferencesManager::MarketPlaces_t m_MPlaces;

    void setMessage(const QString& Msg = "");



  public:

    EditMarketplaceDialog(QWidget* Parent, const QString& Name, const QString& URL,
                          const openfluid::guicommon::PreferencesManager::MarketPlaces_t& MPlaces);

    virtual ~EditMarketplaceDialog();

    QString getURL() const;

    QString getName() const;

    QString getOriginalName() const { return m_OriginalName; }

};



#endif /* __EDITMARKETPLACEDIALOG_HPP__ */
