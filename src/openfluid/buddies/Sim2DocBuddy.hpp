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
  @file

  @author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
 */


#ifndef __SIM2DOCBUDDY_H___
#define __SIM2DOCBUDDY_H___

#include <boost/filesystem/path.hpp>
#include <map>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/buddies/OpenFLUIDBuddy.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/bind.hpp>

using namespace BOOST_SPIRIT_CLASSIC_NS;

namespace openfluid { namespace buddies {

class BuddiesListener;


class DLLEXPORT Sim2DocBuddy : public OpenFLUIDBuddy
{
  private:
    typedef std::map< std::string,std::vector<std::string> > SignatureData_t;

    std::string m_CPPFile;
    std::string m_LatexOutFile;

    boost::filesystem::path m_InputDirPath;
    boost::filesystem::path m_OutputDirPath;
    boost::filesystem::path m_InputFilePath;
    boost::filesystem::path m_OutputLatexFilePath;
    boost::filesystem::path m_TplFilePath;
    boost::filesystem::path m_CProcessedFilePath;
    boost::filesystem::path m_PDFLatexPath;
    boost::filesystem::path m_BibtexPath;
    boost::filesystem::path m_Latex2HTMLPath;

    boost::filesystem::path m_CPreProcessorPath;

    std::string m_ExtractedLatexDoc;

    std::string m_Title;
    std::string m_NewCommands;
    std::string m_SimID;
    std::string m_SimName;
    std::string m_SimVersion;
    std::string m_SimAuthorName;
    std::string m_SimAuthorEmail;
    std::string m_SimDomain;
    std::string m_SimDescription;
    std::string m_SimData;

    std::string m_BeginSignatureTag;
    std::string m_EndSignatureTag;
    std::string m_BeginSim2DocTag;
    std::string m_EndSim2DocTag;

    std::string m_KeyValue;
    std::string m_BuiltParam;

    std::string m_HTMLPackageLatexCommand;

    SignatureData_t m_InVars;
    SignatureData_t m_OutVars;
    SignatureData_t m_ModVars;
    SignatureData_t m_InAttrs;
    SignatureData_t m_OutAttrs;
    SignatureData_t m_ParamsData;
    SignatureData_t m_Events;
    SignatureData_t m_ExtraFiles;

    openfluid::ware::SignatureHandledUnitsGraph m_UnitsGraph;
    openfluid::ware::SignatureTimeScheduling m_TimeScheduling;

    std::string extractBetweenTags(std::string Content, const std::string BeginTag, const std::string EndTag);

    std::string toLatexFriendly(std::string Content);

    void addLatexDataCatBegin(std::string& Content, const std::string Title, const std::string ColsFormat);

    void addLatexDataCatEnd(std::string& Content);

    std::string extractSignatureLines();

    std::vector<std::string> searchStringLitterals(std::string StrToParse);

    void copyDocDirectory();

    void extractLatexDocFromCPP();

    void cpreprocessCPP();

    /**
     * @action Create a new member of UnitsClass vector
    */
    void addVectorMember(std::vector<openfluid::ware::SignatureHandledUnitsClassItem> *UpdatedUnitsClass);

    /**
     * @action Store data parsed in correct attribute of UnitsClass
    */
    void storeInUnitsClass(std::vector<openfluid::ware::SignatureHandledUnitsClassItem> *UpdatedUnitsClass, int Att);

    /**
     * @action Store the new key value parsed for SignatureData attribute
    */
    void storeKey(SignatureData_t *SignatureData, std::string State);

    /**
     * @action Add data parsed in SignatureData attribute received as parameter
    */
    void storeIntoSignatureData(SignatureData_t *SignatureData);

    /**
     * @action Build a signature parameter with elements parsed
    */
    void buildParam(const char* First, const char* Last);

    /**
     * @action Store data built with parsing in attribute parameter
    */
    void storeData(std::string *Param);

    /**
     * @action Set fixed scheduling of TimeScheduling attribute
    */
    void setSchedulingFixed(double Val);

    /**
     * @action Set range scheduling of TimeScheduling attribute
    */
    void setSchedulingRange(double Max);

    /**
     * @action Apply latex syntax to the attributes
    */
    void turnIntoLatexSyntax();

    void processSignature();

    void generateLatex();

    bool isErrorInPDFLatexLog();

    void buildPDF();

    void buildHTML();


  public:

    Sim2DocBuddy(openfluid::buddies::BuddiesListener* Listener);

    ~Sim2DocBuddy();

    bool run();


    /**
     * Grammar class for parsing simulator signature
    */
    struct SimSignatureGrammar : public grammar<SimSignatureGrammar>
    {
      Sim2DocBuddy *m_Owner;
      SimSignatureGrammar(Sim2DocBuddy *Owner) : m_Owner(Owner) {}

      template <typename ScannerT>
      struct definition
      {
        // List of parsing rules
        rule<ScannerT> blank, endLine, escapedChar, string, element, parameters, signature,
                        IDRule, NameRule, DescriptionRule, VersionRule, DomainRule, AuthorRule,
                        SimulatorParamRule, ProducedVarRule, UpdatedVarRule, RequiredVarRule, UsedVarRule,
                        ProducedAttributeRule, RequiredAttributeRule, UsedAttributeRule,
                        UsedEventsRule, UsedExtraFilesRule, RequiredExtraFilesRule,
                        UpdatedUnitsGraphRule, UpdatedUnitsClassRule;

        /**
         * @action Define the different rules of content to parse
        */
        definition(SimSignatureGrammar const& self)
        {
          std::string *Tmp = 0;
          signature = str_p("BEGIN_SIMULATOR_SIGNATURE(") >> IDRule >> ')' >> endLine
                  >> *(str_p("DECLARE_NAME(") >> NameRule >> ')' >> endLine
                    | str_p("DECLARE_DESCRIPTION(") >> DescriptionRule >> ')' >> endLine
                    | str_p("DECLARE_VERSION(") >> VersionRule >> ')' >> endLine
                    | str_p("DECLARE_SDKVERSION") >> endLine
                    | str_p("DECLARE_STATUS(") >> string >> ')' >> endLine
                    | str_p("DECLARE_DOMAIN(") >> DomainRule >> ')' >> endLine
                    | str_p("DECLARE_PROCESS(") >> parameters >> ')' >> endLine
                    | str_p("DECLARE_METHOD(") >> parameters >> ')' >> endLine
                    | str_p("DECLARE_AUTHOR(") >> AuthorRule >> ')' >> endLine
                    | str_p("DECLARE_SIMULATOR_PARAM(") >> SimulatorParamRule >> ')' >> endLine
                    | str_p("DECLARE_PRODUCED_VAR(") >> ProducedVarRule >> ')' >> endLine
                    | str_p("DECLARE_UPDATED_VAR(") >> UpdatedVarRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_VAR(") >> RequiredVarRule >> ')' >> endLine
                    | str_p("DECLARE_USED_VAR(") >> UsedVarRule >> ')' >> endLine
                    | str_p("DECLARE_PRODUCED_ATTRIBUTE(") >> ProducedAttributeRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_ATTRIBUTE(") >> RequiredAttributeRule >> ')' >> endLine
                    | str_p("DECLARE_USED_ATTRIBUTE(") >> UsedAttributeRule >> ')' >> endLine
                    | str_p("DECLARE_USED_EVENTS(") >> UsedEventsRule >> ')' >> endLine
                    | str_p("DECLARE_USED_EXTRAFILE(") >> UsedExtraFilesRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_EXTRAFILE(") >> RequiredExtraFilesRule >> ')' >> endLine
                    | str_p("DECLARE_UPDATED_UNITSGRAPH(") >> UpdatedUnitsGraphRule >> ')' >> endLine
                    | str_p("DECLARE_UPDATED_UNITSCLASS(")[boost::bind(&Sim2DocBuddy::addVectorMember,
                        self.m_Owner, &self.m_Owner->m_UnitsGraph.UpdatedUnitsClass)] >> UpdatedUnitsClassRule >> ')' >> endLine

                    | str_p("DECLARE_SCHEDULING_UNDEFINED")[boost::bind(&ware::SignatureTimeScheduling::setAsUndefined,
                        &self.m_Owner->m_TimeScheduling)] >> endLine
                    | str_p("DECLARE_SCHEDULING_DEFAULT")[boost::bind(&ware::SignatureTimeScheduling::setAsDefaultDeltaT,
                        &self.m_Owner->m_TimeScheduling)] >> endLine
                    | str_p("DECLARE_SCHEDULING_FIXED(") >> *blank >> real_p[boost::bind(&Sim2DocBuddy::setSchedulingFixed, self.m_Owner, _1)]
                            >> *blank >> ')' >> endLine
                    | str_p("DECLARE_SCHEDULING_RANGE(") >> *blank >> real_p[boost::bind(&Sim2DocBuddy::setSchedulingFixed, self.m_Owner, _1)]
                            >> *blank >> ',' >> *blank >> real_p[boost::bind(&Sim2DocBuddy::setSchedulingRange, self.m_Owner, _1)] >> *blank >>  ')' >> endLine
                  )
              >> str_p("END_SIMULATOR_SIGNATURE") >> endLine
          ;

          /** List of common rules **/

          blank = blank_p | eol_p;    // Parse all spaces, tabs or end of line
          endLine = *(blank | ';');
          escapedChar = '\\' >> (alnum_p | '"');

          // String content composed to alphanumeric and specified characters
          string = *(alnum_p | '.' | ' ' | '@' | '/' | ':' | '_' | '[' | ']' | '-' | escapedChar | '\\');

          // Parameter of a method surrounded by quotes or not
          element = (+(*blank >> '"' >> string[boost::bind(&Sim2DocBuddy::buildParam, self.m_Owner, _1, _2)] >> '"' >> *blank))
                      | (*blank >> string[boost::bind(&Sim2DocBuddy::buildParam, self.m_Owner, _1, _2)] >> *blank);

          // List of parameters of a method stored in a temp variable
          parameters = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, Tmp)]
           >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, Tmp)]);



          /** List of rules for the different lines of signature **/

          IDRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimID)];
          NameRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimName)];
          DescriptionRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimDescription)];
          VersionRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimVersion)];
          DomainRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimDomain)];
          AuthorRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimAuthorName)]
             >> ',' >> element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_SimAuthorEmail)];

          SimulatorParamRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_ParamsData, "")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_ParamsData)]);
          ProducedVarRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_OutVars, "produced")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_OutVars)]);
          UpdatedVarRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_OutVars, "updated")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_OutVars)]);
          RequiredVarRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_InVars, "required")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_InVars)]);
          UsedVarRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_InVars, "used")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_InVars)]);

          ProducedAttributeRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_OutAttrs, "produced")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_OutAttrs)]);
          RequiredAttributeRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_InAttrs, "required")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_InAttrs)]);
          UsedAttributeRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_InAttrs, "used")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeIntoSignatureData, self.m_Owner, &self.m_Owner->m_InAttrs)]);

          UsedEventsRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_Events, "")];
          UsedExtraFilesRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_ExtraFiles, "used")];
          RequiredExtraFilesRule = element[boost::bind(&Sim2DocBuddy::storeKey, self.m_Owner, &self.m_Owner->m_ExtraFiles, "required")];

          UpdatedUnitsGraphRule = element[boost::bind(&Sim2DocBuddy::storeData, self.m_Owner, &self.m_Owner->m_UnitsGraph.UpdatedUnitsGraph)];
          UpdatedUnitsClassRule = element[boost::bind(&Sim2DocBuddy::storeInUnitsClass, self.m_Owner, &self.m_Owner->m_UnitsGraph.UpdatedUnitsClass, 1)]
                  >> ',' >> element[boost::bind(&Sim2DocBuddy::storeInUnitsClass, self.m_Owner, &self.m_Owner->m_UnitsGraph.UpdatedUnitsClass, 2)];
        }

        /**
         * @return Main rule of parsing content
        */
        rule<ScannerT> const& start() const { return signature; }
      };
    };

};


} } //namespaces

#endif /* __SIM2DOCBUDDY_HPP__ */
