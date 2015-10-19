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
 @file WareSrcFactory.hpp
 @brief Header of ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */


#ifndef __OPENFLUID_WARESDEV_WARESRCFACTORY_HPP__
#define __OPENFLUID_WARESDEV_WARESRCFACTORY_HPP__


#include <QDir>
#include <QValidator>
#include <QRegExpValidator>

#include <openfluid/dllexport.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/waresdev/WareSrcManager.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>


namespace openfluid { namespace waresdev {


class OPENFLUID_API WareSrcFactory
{
  public:

    struct Replacements
    {
      private:

        QList<openfluid::builderext::ExtensionMode> BextMode_Data;
        QStringList BextCategory_Data;
        QStringList Sim2docMode_Data;

      public:

        QString ClassName;
        QString RootCppFilename;
        QString RootHppFilename;
        QString HppHeaderGuard;
        QString LinkUID;
        bool ParamsUiEnabled;
        QString ParamsUiClassname;
        QString ParamsUiRootCppFilename;
        QString ParamsUiRootHppFilename;
        QString ParamsUiHeaderGuard;
        QString ParamsUiComment;
        QString SignatureInfos;
        int Sim2docModeIndex;
        bool Sim2docInstall;
        QString SimulatorSignatureData;
        QString SimulatorInitCode;
        QString SimulatorRunCode;
        QString SimulatorSchedulingReturn;
        int BuilderExtModeIndex;
        int BuilderExtCategoryIndex;
        QString BuilderExtMenuText;


        Replacements() :
            ParamsUiEnabled(false), ParamsUiClassname("ParamsUiWidget"), ParamsUiRootCppFilename(""),
            ParamsUiRootHppFilename("ParamsUiWidget.hpp"), ParamsUiComment("//"), Sim2docModeIndex(0),
            Sim2docInstall(false), BuilderExtModeIndex(-1), BuilderExtCategoryIndex(0)
        {
          BextMode_Data << openfluid::builderext::MODE_MODAL << openfluid::builderext::MODE_MODELESS
                        << openfluid::builderext::MODE_WORKSPACE;

          BextCategory_Data << "openfluid::builderext::CAT_SPATIAL" << "openfluid::builderext::CAT_MODEL"
                            << "openfluid::builderext::CAT_RESULTS" << "openfluid::builderext::CAT_OTHER";

          Sim2docMode_Data << "ON" << "AUTO" << "OFF";
        }

        static QStringList getBuilderExtTypeTexts()
        {
          QStringList BextMode_Texts;
          BextMode_Texts << QObject::tr("Modal") << QObject::tr("Modeless") << QObject::tr("Workspace");
          return BextMode_Texts;
        }

        static QStringList getBuilderExtCategoryTexts()
        {
          QStringList BextCategory_Texts;
          BextCategory_Texts << QObject::tr("Spatial domain") << QObject::tr("Model") << QObject::tr("Results")
                             << QObject::tr("Other");
          return BextCategory_Texts;
        }

        static QStringList getSim2docModeTexts()
        {
          QStringList Sim2docMode_Texts;
          Sim2docMode_Texts << QObject::tr("On - sim2doc must be run manually")
                            << QObject::tr("Auto - sim2doc is automatically run")
                            << QObject::tr("Off - sim2doc is disabled");
          return Sim2docMode_Texts;
        }

        openfluid::builderext::ExtensionMode getBuilderExtType() const
        {
          return BextMode_Data.value(BuilderExtModeIndex, openfluid::builderext::MODE_UNKNOWN);
        }

        QString getBuilderExtCategory() const
        {
          return BextCategory_Data.value(BuilderExtCategoryIndex, "");
        }

        QString getSim2docMode() const
        {
          return Sim2docMode_Data.value(Sim2docModeIndex, "");
        }

        QString getParamsUiEnabled() const
        {
          return ParamsUiEnabled ? "ON" : "OFF";
        }

        QString getSim2docInstall() const
        {
          return Sim2docInstall ? "OFF" : "ON";
        }

    };

  private:

    QDir m_SharedTemplatesDir;
    QDir m_TypedTemplatesDir;

    QDir m_WareTypeDir;

    QString m_WareId;

    bool copyTemplateToNewFile(const QString& TemplatePath, const QString& NewFileName, QString& NewFilePath,
                               QString& ErrMsg);

    bool replaceInFile(const Replacements& R, const QString& NewFilePath, QString& ErrMsg);

  public:

    WareSrcFactory(openfluid::ware::WareType Type);

    ~WareSrcFactory();

    void setWareId(const QString& Id);

    /**
     * Creates a CMakeLists file in the ware directory, from the CMakeLists file template of the ware type.
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createCMakeListsFile(QString& NewFilePath, QString& ErrMsg);

    /**
     * Creates a wareshub.json file in the ware directory, from the (shared) Json file template.
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createJsonFile(QString& NewFilePath, QString& ErrMsg);

    /**
     * Creates a main cpp file in the ware directory, from the source file template of the ware type,
     * depending on the builder extension type (set to TYPE_UNKNOWN means it is not a Builder extension).
     * @param R Replacements struct containing the replacements to perform
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createCppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg);

    /**
     * Creates a main hpp file in the ware directory, from the header file template of the ware type
     * depending on the builder extension type (set to TYPE_UNKNOWN means it is not a Builder extension).
     * @param R Replacements struct containing the replacements to perform
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createHppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg);

    /**
     * Creates a ui-parameterization cpp file in the ware directory,
     * from the ui-parameterization source file template of the ware type.
     * @param R Replacements struct containing the replacements to perform
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createParamUiCppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg);

    /**
     * Creates a ui-parameterization hpp file in the ware directory,
     * from the ui-parameterization header file template of the ware type.
     * @param R Replacements struct containing the replacements to perform
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createParamUiHppFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg);

    /**
     * Creates a Cmake.in.config file in the ware directory, from the Cmake.in.config file template of the ware type.
     * @param R Replacements struct containing the replacements to perform
     * @param NewFilePath A QString that will be filled with the absolute path of the created file
     * @param ErrMsg A QString that will be filled with error messages if the file creation fails
     * @return bool False if the file creation fails, true otherwise
     */
    bool createCmakeConfigFile(const Replacements& R, QString& NewFilePath, QString& ErrMsg);

    static QRegExp getCppFilenameRegExp(QString& Tooltip, bool IsHpp = false);
    static QRegExp getClassnameRegExp(QString& Tooltip);
    static QRegExp getWareIdRegExp(QString& Tooltip);

    static QString getHeaderGuard(const QString& HppFilename);
    static QString getHppFilename(const QString& CppFilename);

    static QString getSimulatorSignatureInfos(const openfluid::ware::SimulatorSignature& Signature);
    static QString getSimulatorSignatureData(const openfluid::ware::SimulatorSignature& Signature);
    static QString getSimulatorInitCode(const openfluid::ware::SimulatorSignature& Signature);
    static QString getSimulatorRunCode(const openfluid::ware::SimulatorSignature& Signature);
    static QString getSimulatorSchedulingReturn(const openfluid::ware::SimulatorSignature& Signature);

};

} }  // namespaces

#endif /* __OPENFLUID_WARESDEV_WARESRCFACTORY_HPP__ */
