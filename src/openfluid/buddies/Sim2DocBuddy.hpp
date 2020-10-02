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
  @file Sim2DocBuddy.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_BUDDIES_SIM2DOCBUDDY_HPP__
#define __OPENFLUID_BUDDIES_SIM2DOCBUDDY_HPP__


#if OPENFLUID_SIM2DOC_ENABLED

// Disabled for compilation errors due to boost.spirit usage under MacOSX
// TODO Should be re-enabled later


#include <map>
#include <vector>

#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/bind.hpp>

#include <openfluid/config.hpp>
#include <openfluid/dllexport.hpp>
#include <openfluid/buddies/OpenFLUIDBuddy.hpp>
#include <openfluid/ware/SimulatorSignature.hpp>
#include <openfluid/utils/ExternalProgram.hpp>


using namespace BOOST_SPIRIT_CLASSIC_NS;


namespace openfluid { namespace buddies {

class BuddiesListener;


class OPENFLUID_API Sim2DocBuddy : public OpenFLUIDBuddy
{
  private:
    typedef std::map< std::string,std::vector<std::string> > SignatureData_t;

    std::string m_CPPFile;
    std::string m_LatexOutFile;

    std::string m_InputDirPath;
    std::string m_OutputDirPath;
    std::string m_InputFilePath;
    std::string m_OutputLatexFilePath;
    std::string m_TplFilePath;
    std::string m_CProcessedFilePath;

    openfluid::utils::ExternalProgram m_PDFLatexProgram;
    openfluid::utils::ExternalProgram m_BibtexProgram;
    openfluid::utils::ExternalProgram m_Latex2HTMLProgram;
    openfluid::utils::ExternalProgram m_GCCProgram;

    std::string m_ExtractedLatexDoc;

    std::string m_Title;
    std::string m_NewCommands;
    std::string m_SimID;
    std::string m_SimName;
    std::string m_SimVersion;
    std::string m_SimStatus;
    std::vector<std::string> m_SimAuthorsNames;
    std::vector<std::string> m_SimAuthorsEmails;
    std::string m_SimDomain;
    std::string m_SimDescription;
    std::string m_SimData;

    std::string m_BeginSignatureTag;
    std::string m_EndSignatureTag;
    std::string m_BeginSim2DocTag;
    std::string m_EndSim2DocTag;

    std::string m_CurrentKeyValue;
    std::string m_CurrentBuiltParam;

    std::string m_HTMLPackageLatexCommand;

    SignatureData_t m_InVars;
    SignatureData_t m_OutVars;
    SignatureData_t m_ModVars;
    SignatureData_t m_InAttrs;
    SignatureData_t m_OutAttrs;
    SignatureData_t m_ParamsData;
    SignatureData_t m_Events;
    SignatureData_t m_ExtraFiles;

    openfluid::ware::SignatureUnitsGraph m_UnitsGraph;
    openfluid::ware::SignatureTimeScheduling m_TimeScheduling;

    static std::string extractBetweenTags(std::string Content, const std::string& BeginTag, const std::string& EndTag);

    static std::string toLatexFriendly(std::string Content);

    static void addLatexDataCatBegin(std::string& Content, const std::string& Title, const std::string& ColsFormat);

    static void addLatexDataCatEnd(std::string& Content);

    std::string extractSignatureLines();

    void copyDocDirectory();

    void extractLatexDocFromCPP();

    void cpreprocessCPP();

    /** Methods called by simulator signature parser **/

    /**
      Builds a signature parameter with parsed elements
      @param[in] First, Last Pointers to the beginning and the end of the parsed element
    */
    void buildParsedParam(const char* First, const char* Last);

    /**
      Clears the string containing the parameter built with parsing
    */
    void clearParsedParam();

    /**
      Stores data parsed in the string parameter
      @param[out] Str Pointer to the string receiving the value
    */
    void storeDataIntoString(std::string *Str);

    /**
      Adds data parsed in the list parameter
      @param[out] List Pointer to the vector receiving the value
     */
    void storeDataIntoVector(std::vector<std::string> *List);

    /**
      Stores the new key value parsed into SignatureData parameter
      @param[out] SignatureData Pointer to the map receiving the key value
      @param[in] State of the attribute or variable stored into the map
              (required, used, produced, updated)
    */
    void storeDataIntoKey(SignatureData_t *SignatureData, const std::string& State);

    /**
      Adds data parsed in the SignatureData attribute received as parameter
      @param[out] SignatureData Pointer to the map receiving the value
    */
    void storeDataIntoSignatureData(SignatureData_t *SignatureData);

    /**
      Checks data parsed and stores it into status attribute
    */
    void storeDataIntoStatus();

    /**
      Stores data parsed in a new UnitsClass item
      @param[out] UpdatedUnitsClass Vector of Units class
      @param[in] Attr Attribute of the new UnitsClass receiving the value
    */
    void storeDataIntoUnitsClass(std::vector<openfluid::ware::SignatureUnitsClassItem> *UpdatedUnitsClass,
            int Attr);

    /**
      Sets fixed scheduling of TimeScheduling attribute
      @param[in] Val Value of scheduling
    */
    void setSchedulingFixed(double Val);

    /**
      Sets range scheduling of TimeScheduling attribute
      @param[in] Max Maximum value of the range
    */
    void setSchedulingRange(double Max);

    /** ********************************** **/

    /**
      Applies latex syntax to the attributes
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
     * Grammar class used to parse simulator signature
    */
    struct SimSignatureGrammar : public grammar<SimSignatureGrammar>
    {
      Sim2DocBuddy *mp_Owner;       // Object of the class containing methods to be called
      SimSignatureGrammar(Sim2DocBuddy *Owner) : mp_Owner(Owner) {}


      template <typename ScannerT>
      struct definition
      {
        /** List of parsing rules **/
        rule<ScannerT> blank, linemarker, endLine, escapedQuote, string, varName, element, parameters, signature,
                        IDRule, NameRule, DescriptionRule, VersionRule, StatusRule, DomainRule, AuthorRule,
                        UsedParamRule, RequiredParamRule,
                        ProducedVarRule, UpdatedVarRule, RequiredVarRule, UsedVarRule,
                        ProducedAttributeRule, RequiredAttributeRule, UsedAttributeRule,
                        UsedEventsRule,
                        UsedExtraFilesRule, RequiredExtraFilesRule,
                        UpdatedUnitsGraphRule, UpdatedUnitsClassRule;

        /**
         * Defines the different rules of content to parse or ignore
         *
         * boost::bind is used to execute a method after parsing of an element or a rule.
         * Its parameters are the method to call, the object with which the method is executed
         * and the different parameters of this method (_1 and _2 are used to send the parsed element).
         *
         * The different parsers which can be used :
         * http://www.boost.org/doc/libs/1_39_0/libs/spirit/classic/doc/quickref.html
        */
        definition(SimSignatureGrammar const& self)
        {
          /** Initial rule **/

          signature = str_p("BEGIN_SIMULATOR_SIGNATURE") >> *blank >> '(' >> IDRule >> ')' >> endLine
                >> *(str_p("DECLARE_NAME") >> *blank >> '(' >> NameRule >> ')' >> endLine
                    | str_p("DECLARE_DESCRIPTION") >> *blank >> '(' >> DescriptionRule >> ')' >> endLine
                    | str_p("DECLARE_VERSION") >> *blank >> '(' >> VersionRule >> ')' >> endLine
                    | str_p("DECLARE_SDKVERSION") >> endLine
                    | str_p("DECLARE_STATUS") >> *blank >> '(' >> StatusRule >> ')' >> endLine
                    | str_p("DECLARE_DOMAIN") >> *blank >> '(' >> DomainRule >> ')' >> endLine
                    | str_p("DECLARE_PROCESS") >> *blank >> '(' >> parameters >> ')' >> endLine
                    | str_p("DECLARE_METHOD") >> *blank >> '(' >> parameters >> ')' >> endLine
                    | str_p("DECLARE_AUTHOR") >> *blank >> '(' >> AuthorRule >> ')' >> endLine

                    | str_p("DECLARE_REQUIRED_PARAMETER") >> *blank >> '(' >> RequiredParamRule >> ')' >> endLine
                    | str_p("DECLARE_USED_PARAMETER") >> *blank >> '(' >> UsedParamRule >> ')' >> endLine

                    // for compatibility with deprecated macros
                    | str_p("DECLARE_SIMULATOR_PARAM") >> *blank >> '(' >> UsedParamRule >> ')' >> endLine

                    | str_p("DECLARE_PRODUCED_VARIABLE") >> *blank >> '(' >> ProducedVarRule >> ')' >> endLine
                    | str_p("DECLARE_UPDATED_VARIABLE") >> *blank >> '(' >> UpdatedVarRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_VARIABLE") >> *blank >> '(' >> RequiredVarRule >> ')' >> endLine
                    | str_p("DECLARE_USED_VARIABLE") >> *blank >> '(' >> UsedVarRule >> ')' >> endLine

                    // for compatibility with deprecated macros
                    | str_p("DECLARE_PRODUCED_VAR") >> *blank >> '(' >> ProducedVarRule >> ')' >> endLine
                    | str_p("DECLARE_UPDATED_VAR") >> *blank >> '(' >> UpdatedVarRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_VAR") >> *blank >> '(' >> RequiredVarRule >> ')' >> endLine
                    | str_p("DECLARE_USED_VAR") >> *blank >> '(' >> UsedVarRule >> ')' >> endLine

                    | str_p("DECLARE_PRODUCED_ATTRIBUTE") >> *blank >> '(' >> ProducedAttributeRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_ATTRIBUTE") >> *blank >> '(' >> RequiredAttributeRule >> ')' >> endLine
                    | str_p("DECLARE_USED_ATTRIBUTE") >> *blank >> '(' >> UsedAttributeRule >> ')' >> endLine

                    | str_p("DECLARE_USED_EVENTS") >> *blank >> '(' >> UsedEventsRule >> ')' >> endLine
                    | str_p("DECLARE_USED_EXTRAFILE") >> *blank >> '(' >> UsedExtraFilesRule >> ')' >> endLine
                    | str_p("DECLARE_REQUIRED_EXTRAFILE") >> *blank >> '(' >> RequiredExtraFilesRule >> ')' >> endLine

                    | str_p("DECLARE_UPDATED_UNITSGRAPH") >> *blank >> '(' >> UpdatedUnitsGraphRule >> ')' >> endLine
                    | str_p("DECLARE_UPDATED_UNITSCLASS") >> *blank >> '(' >> UpdatedUnitsClassRule >> ')' >> endLine

                    | str_p("DECLARE_SCHEDULING_UNDEFINED")[boost::bind(&ware::SignatureTimeScheduling::setAsUndefined,
                        &self.mp_Owner->m_TimeScheduling)] >> endLine
                    | str_p("DECLARE_SCHEDULING_DEFAULT")[boost::bind(
                        &ware::SignatureTimeScheduling::setAsDefaultDeltaT,
                        &self.mp_Owner->m_TimeScheduling)] >> endLine
                    | str_p("DECLARE_SCHEDULING_FIXED") >> *blank >> '(' >> *blank
                            >> real_p[boost::bind(&Sim2DocBuddy::setSchedulingFixed, self.mp_Owner, _1)]
                            >> *blank >> ')' >> endLine
                    | str_p("DECLARE_SCHEDULING_RANGE") >> *blank >> '('
                            >> *blank >> real_p[boost::bind(&Sim2DocBuddy::setSchedulingFixed, self.mp_Owner, _1)]
                            >> *blank >> ','
                            >> *blank >> real_p[boost::bind(&Sim2DocBuddy::setSchedulingRange, self.mp_Owner, _1)]
                            >> *blank >>  ')' >> endLine

                    | linemarker >> endLine
                  )
              >> str_p("END_SIMULATOR_SIGNATURE") >> endLine
          ;

          /** List of common rules **/


          /* Ignoring of inserted lines by the preprocessor which mean
           * the original position of lines in source file.
          */
          linemarker =  '#' >> *(anychar_p - eol_p) >> eol_p;

          blank = blank_p | eol_p;                        // Parse all spaces, tabs and newlines
          endLine = *(blank | ';');
          escapedQuote = str_p("\\\"");

          string = *(escapedQuote | (anychar_p - '"'));   // String content composed to all characters

          varName = *(print_p - ')');

          // Parameter of a method surrounded by quotes (string) or not (var name)
          element = (+(*blank >> '"' >> string[boost::bind(&Sim2DocBuddy::buildParsedParam, self.mp_Owner, _1, _2)]
                                     >> '"' >> *blank))
                      | (*blank >> varName[boost::bind(&Sim2DocBuddy::buildParsedParam, self.mp_Owner, _1, _2)]
                                >> *blank);

          // List of parameters of a declaration (here not stored)
          parameters = element[boost::bind(&Sim2DocBuddy::clearParsedParam, self.mp_Owner)]
           >> *(',' >> element[boost::bind(&Sim2DocBuddy::clearParsedParam, self.mp_Owner)]);


          /** List of rules for the different lines of signature **/

          IDRule =
              element[boost::bind(&Sim2DocBuddy::storeDataIntoString, self.mp_Owner, &self.mp_Owner->m_SimID)];
          NameRule =
              element[boost::bind(&Sim2DocBuddy::storeDataIntoString, self.mp_Owner, &self.mp_Owner->m_SimName)];
          DescriptionRule =
              element[boost::bind(&Sim2DocBuddy::storeDataIntoString, self.mp_Owner, &self.mp_Owner->m_SimDescription)];
          VersionRule =
              element[boost::bind(&Sim2DocBuddy::storeDataIntoString, self.mp_Owner, &self.mp_Owner->m_SimVersion)];

          StatusRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoStatus, self.mp_Owner)];

          DomainRule =
              element[boost::bind(&Sim2DocBuddy::storeDataIntoString, self.mp_Owner, &self.mp_Owner->m_SimDomain)];
          AuthorRule =
              element[boost::bind(&Sim2DocBuddy::storeDataIntoVector, self.mp_Owner, &self.mp_Owner->m_SimAuthorsNames)]
             >> ',' >>
             element[boost::bind(&Sim2DocBuddy::storeDataIntoVector, self.mp_Owner,
                                 &self.mp_Owner->m_SimAuthorsEmails)];

          UsedParamRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey,
                                              self.mp_Owner, &self.mp_Owner->m_ParamsData, "used")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData,
                                                   self.mp_Owner, &self.mp_Owner->m_ParamsData)]);
          RequiredParamRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey,
                                                   self.mp_Owner, &self.mp_Owner->m_ParamsData, "required")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData,
                                                   self.mp_Owner, &self.mp_Owner->m_ParamsData)]);

          ProducedVarRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                &self.mp_Owner->m_OutVars, "produced")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData,
                                                   self.mp_Owner, &self.mp_Owner->m_OutVars)]);
          UpdatedVarRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                               &self.mp_Owner->m_OutVars, "updated")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData,
                                                   self.mp_Owner, &self.mp_Owner->m_OutVars)]);
          RequiredVarRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                &self.mp_Owner->m_InVars, "required")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData,
                                                   self.mp_Owner, &self.mp_Owner->m_InVars)]);
          UsedVarRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                            &self.mp_Owner->m_InVars, "used")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData,
                                                   self.mp_Owner, &self.mp_Owner->m_InVars)]);

          ProducedAttributeRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                      &self.mp_Owner->m_OutAttrs, "produced")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData, self.mp_Owner,
                                                   &self.mp_Owner->m_OutAttrs)]);
          RequiredAttributeRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                      &self.mp_Owner->m_InAttrs, "required")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData, self.mp_Owner,
                                                   &self.mp_Owner->m_InAttrs)]);
          UsedAttributeRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                  &self.mp_Owner->m_InAttrs, "used")]
                   >> *(',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoSignatureData, self.mp_Owner,
                                                   &self.mp_Owner->m_InAttrs)]);

          UsedEventsRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                               &self.mp_Owner->m_Events, "")];
          UsedExtraFilesRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                   &self.mp_Owner->m_ExtraFiles, "used")];
          RequiredExtraFilesRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoKey, self.mp_Owner,
                                                       &self.mp_Owner->m_ExtraFiles, "required")];

          UpdatedUnitsGraphRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoString, self.mp_Owner,
                                                      &self.mp_Owner->m_UnitsGraph.UpdatedUnitsGraph)];
          UpdatedUnitsClassRule = element[boost::bind(&Sim2DocBuddy::storeDataIntoUnitsClass, self.mp_Owner,
                                                      &self.mp_Owner->m_UnitsGraph.UpdatedUnitsClass, 1)]
                  >> ',' >> element[boost::bind(&Sim2DocBuddy::storeDataIntoUnitsClass, self.mp_Owner,
                                                &self.mp_Owner->m_UnitsGraph.UpdatedUnitsClass, 2)];
        }

        /**
          @return Main rule of parsing content
        */
        rule<ScannerT> const& start() const { return signature; }
      };
    };

};


} } //namespaces


#endif /* OPENFLUID_SIM2DOC_ENABLED */

#endif /* __OPENFLUID_BUDDIES_SIM2DOCBUDDY_HPP__ */
