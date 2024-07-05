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
  @file StructuredCommandLineParser.hpp

  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_UTILS_STRUCTUREDCOMMANDLINEPARSER_HPP__
#define __OPENFLUID_UTILS_STRUCTUREDCOMMANDLINEPARSER_HPP__


#include <list>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include <openfluid/utils/CommandLineParser.hpp>


namespace openfluid { namespace utils {

class CommandLineSection 
{
  private:

    std::string m_Name;

    std::vector<CommandLineCommand> m_Commands;


  public :

    /**
      Instanciates a command section with the given parameters
      @param[in] Name The long name of the command section
    */
    CommandLineSection(const std::string& Name):
      m_Name(Name)
    { }


    // =====================================================================
    // =====================================================================


    /**
      Returns the name of the command
      @return The name (e.g. "search")
    */
    std::string getName() const
    { 
      return m_Name; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the commands
      @return The list of commands
    */
    std::vector<CommandLineCommand> getCommands() const
    { 
      return m_Commands; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds a command to the section
      @param[in] Command The command to add
    */
    void addCommand(const CommandLineCommand& Command)
    {
      m_Commands.push_back(Command);
    }
};


// =====================================================================
// =====================================================================


class StructuredCommandLineParser : public CommandLineParser
{
  private:

    std::vector<CommandLineSection*> m_CommandSections;

    std::vector<CommandLineCommand> m_NoSectionCommands;


  public : 

    /**
      Instanciates a command line parser with the given parameters
      @param[in] ProgramName The name of the programn
      @param[in] HelpText The help text associated to the option
    */
    StructuredCommandLineParser(const std::string& ProgramName, const std::string& HelpText,
                               const std::vector<CommandLineSection*>& CommandSections) :
      CommandLineParser(ProgramName, HelpText, true)
    {
      addCommandSections(CommandSections);
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds a command section to the parser
      @param[in] CommandSection The command section to add
    */
    void addCommandSection(CommandLineSection* CommandSection)
    {
      m_CommandSections.push_back(CommandSection);
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds a command section to the parser
      @param[in] CommandSections The command section to add
    */
    void addCommandSections(const std::vector<CommandLineSection*>& CommandSections)
    {
      for (const auto& CommandSection : CommandSections)
      {
        addCommandSection(CommandSection);
      }
    }


    // =====================================================================
    // =====================================================================


     /**
      Adds a command. Can add a command to a command section
      @param[in] Command The command to add
      @param[in] CommandSection The section to add the command. Can be null
    */
    void addCommand(const CommandLineCommand& Command, CommandLineSection* CommandSection = nullptr)
    {
      if(CommandSection)
      {
        CommandSection->addCommand(Command);
      }
      else 
      {
        m_NoSectionCommands.push_back(Command);
      }

      CommandLineParser::addCommand(Command);
    }


    // =====================================================================
    // =====================================================================


    /**
      Prints the help text
      @param[in] OutStm The stream where the help text is printed (e.g. std::cout)
    */
    void printHelp(std::ostream& OutStm)
    {
      displayUsageMessage(OutStm);

      int LargestCommandTextLength = getLargestCommandLength();
      int LargestOptionTextLength = getLargestOptionLength();

      if (m_ActiveCommand.empty())
      {
        OutStm << "\n" << m_AvailableCommandsText << "\n\n";

        size_t CmdSectionIndex = 0;
        for (const auto& CmdSection : m_CommandSections)
        {
          OutStm << CmdSection->getName() << ": \n";
          for (auto& Cmd : CmdSection->getCommands())
          {
            displayFormattedData(OutStm, Cmd.getName(), Cmd.getHelpText(), LargestCommandTextLength);
          }
          if(CmdSectionIndex < m_CommandSections.size() - 1)
          {
            OutStm << "\n";
          }
          CmdSectionIndex++;
        }

        if(m_NoSectionCommands.size() > 0)
        {
          OutStm << "\nOthers: \n";
          for (const auto& Cmd : m_NoSectionCommands)
          {
            displayFormattedData(OutStm, Cmd.getName(), Cmd.getHelpText(), LargestCommandTextLength);
          }
        }
      }

      if (!m_Commands[m_ActiveCommand].getLongHelpText().empty())
      {
        OutStm << "\n" << m_Commands[m_ActiveCommand].getLongHelpText() << "\n";
      }

      displayOptions(OutStm, getHelpOption(), LargestOptionTextLength);
    }
};

} }


#endif /* __OPENFLUID_UTILS_STRUCTUREDCOMMANDLINEPARSER_HPP__ */
