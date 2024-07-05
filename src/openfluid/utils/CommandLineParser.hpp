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
  @file CommandLineParser.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#ifndef __OPENFLUID_UTILS_COMMANDLINEPARSER_HPP__
#define __OPENFLUID_UTILS_COMMANDLINEPARSER_HPP__


#include <map>
#include <vector>
#include <limits>
#include <list>
#include <string>
#include <ostream>


namespace openfluid { namespace utils {


class CommandLineOption
{
  private:

    std::string m_LongName;

    std::string m_ShortName;

    bool m_ValueRequired;

    std::string m_Value;

    std::string m_HelpText;

    bool m_Active;


  public:

    CommandLineOption() :
      m_ValueRequired(false),m_Active(false)
    { }


    // =====================================================================
    // =====================================================================

    
    /**
      Instanciates an option with the given parameters
      @param[in] LongName The long name of the option
      @param[in] ShortName The short name of the option
      @param[in] HelpText The help text associated to the option
      @param[in] ValueRequired Set to true to require a value with this option (default is false)
    */
    CommandLineOption(const std::string& LongName, const std::string& ShortName,
                      const std::string& HelpText, bool ValueRequired = false):
      m_LongName(LongName),m_ShortName(ShortName), m_ValueRequired(ValueRequired), m_HelpText(HelpText),
      m_Active(false)
    { }


    // =====================================================================
    // =====================================================================


    /**
      Returns the help text of the option
      @return The help text
    */
    std::string getHelpText() const
    { 
      return m_HelpText; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the long name of the option (e.g. 'output-dir')
      @return The long name
    */
    std::string getLongName() const
    { 
      return m_LongName; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the short name of the option (e.g. 'o')
      @return The short name
    */
    std::string getShortName() const
    { 
      return m_ShortName; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of the option
      @return The value
    */
    std::string getValue() const
    { 
      return m_Value; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value of the option
      @return The value
    */
    std::string getDetails() const
    { 
      std::string OptionDetail = "--" + getLongName();
      if (isValueRequired())
      {
        OptionDetail += "=<arg>";
      }

      if (!getShortName().empty())
      {
        OptionDetail += ", -" + getShortName();
        if (isValueRequired())
        {
          OptionDetail += " <arg>";
        }
      }
      return OptionDetail;
    }


    // =====================================================================
    // =====================================================================


    /**
      Tests if the option is active
      @return true if the option is active
    */
    bool isActive() const
    { 
      return m_Active; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Tests if the option requires a value
      @return true if the option requires a value
    */
    bool isValueRequired() const
    { 
      return m_ValueRequired; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Activates the option with the given value
      @param[in] Value The value for activation (default is empty)
    */
    void activate(const std::string& Value = "")
    {
      m_Value = Value;

      if ((!m_ValueRequired) || (m_ValueRequired && !m_Value.empty()))
      {
        m_Active = true;
      }
      else
      {
        m_Active = false;
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Resets the option to default (inactive with empty value)
    */
    void reset()
    {
      m_Active = false;
      m_Value = "";
    }

};


// =====================================================================
// =====================================================================


class CommandLineCommand
{
  private:

    std::string m_Name;

    std::string m_HelpText;

    std::string m_LongHelpText;

    std::map<std::string,CommandLineOption> m_Options;

    std::map<std::string,CommandLineOption*> m_ShortOptions;

    std::vector<CommandLineOption> m_OptionsOrdered;


  public:

    CommandLineCommand()
    { }


    // =====================================================================
    // =====================================================================


    /**
      Instanciates a command with the given parameters
      @param[in] Name The long name of the command
      @param[in] HelpText The help text associated to the command
    */
    CommandLineCommand(const std::string& Name, const std::string& HelpText, const std::string& LongHelpText = ""):
      m_Name(Name), m_HelpText(HelpText)
    { 
      m_LongHelpText = LongHelpText.empty() ? HelpText : LongHelpText;
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the help text of the command
      @return The help text
    */
    std::string getHelpText() const
    { 
      return m_HelpText; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the help text of the command
      @return The help text
    */
    std::string getLongHelpText() const
    { 
      return m_LongHelpText; 
    }


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
      Returns the options registered for the command
      @return The options
    */
    const std::map<std::string,CommandLineOption>& options()
    { 
      return m_Options; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the options (ordered by addition) registered for the command
      @return The options
    */
    const std::vector<CommandLineOption>& optionsOrdered()
    { 
      return m_OptionsOrdered; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds an option to the command
      @param[in] Option The option to add
    */
    void addOption(const CommandLineOption& Option)
    {
      m_Options[Option.getLongName()] = Option;

      m_OptionsOrdered.push_back(Option);

      if (!Option.getShortName().empty())
      {
        m_ShortOptions[Option.getShortName()] = &m_Options[Option.getLongName()];
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds many options to the command
      @param[in] Options The option list to add
    */
    void addOptions(const std::vector<CommandLineOption>& Options)
    {
      for (const auto& Opt : Options)
      {
        addOption(Opt);
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Activates the given option with the given value
      @param[in] LongName The long name of the option
      @param[in] Value The value for activation (default is empty)
    */
    bool activateOption(const std::string& LongName, const std::string& Value = "")
    {
      auto it = m_Options.find(LongName);

      if (it == m_Options.end())
      {
        return false;
      }

      (*it).second.activate(Value);
      return (*it).second.isActive();
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the long name of an option from its short name
      @param[in] ShortName The requested short name for the option
      @return The long name
    */
    std::string getOptionNameFromShortName(const std::string& ShortName) const
    {
      auto it = m_ShortOptions.find(ShortName);

      if (it == m_ShortOptions.end())
      {
        return "";
      }

      return (*it).second->getLongName();
    }


    // =====================================================================
    // =====================================================================


    /**
      Tests if an option exists for the command
      @param[in] LongName The long name for the searched option
      @return true if the option exists
    */
    bool isOptionExists(const std::string& LongName) const
    {
      return (m_Options.find(LongName) != m_Options.end());
    }


    // =====================================================================
    // =====================================================================


    /**
      Tests if an option requires a value
      @param[in] LongName The long name for the searched option
      @return true if the option requires a value
    */
    bool isOptionRequiresValue(const std::string& LongName) const
    {
      auto it = m_Options.find(LongName);

      if (it == m_Options.end())
      {
        return false;
      }

      return (*it).second.isValueRequired();
    }


    // =====================================================================
    // =====================================================================


    /**
      Tests if an option is active
      @param[in] LongName The long name for the searched option
      @return true if the option is active
    */
    bool isOptionActive(const std::string& LongName) const
    {
      auto it = m_Options.find(LongName);

      return (it != m_Options.end() && (*it).second.isActive());
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the value given for an option
      @param[in] LongName The long name for the option
      @return the value for the option
    */
    std::string getOptionValue(const std::string& LongName) const
    {
      auto it = m_Options.find(LongName);

      if (it == m_Options.end())
      {
        return "";
      }

      return ((*it).second.getValue());
    }


    // =====================================================================
    // =====================================================================


    /**
      Resets all options to default (inactive with empty value)
    */
    void reset()
    {
      for (auto& Opt : m_Options)
      {
        Opt.second.reset();
      }
    }

};


// =====================================================================
// =====================================================================


// OpenFLUID:stylecheck:!incs

/**
  @brief Class for management of command line arguments.

  This class allows to manage the command line arguments as commands, options and extra arguments.

  The example below is for a command line program with two commands ('run' and 'search') and associated options
@snippet misc/main.cpp cmdlineparser
*/
class CommandLineParser
{
  protected:

    std::string m_ProgramName;

    std::string m_HelpText;

    std::map<std::string,CommandLineCommand> m_Commands;

    std::vector<CommandLineCommand> m_CommandsOrdered;

    std::string m_ActiveCommand;

    const std::string m_AvailableCommandsText = "Available commands:";


    // =====================================================================
    // =====================================================================


    int getLargestCommandLength()
    {
      int LargestCommandLength = std::numeric_limits<int>::min();
      for (auto& Cmd : m_Commands)
      {
        LargestCommandLength = std::max(LargestCommandLength, (int)Cmd.first.size());
      }
      
      return LargestCommandLength;
    }


    // =====================================================================
    // =====================================================================


    int getLargestOptionLength()
    {
      std::map<std::string,CommandLineOption> AllOptions(m_Commands[m_ActiveCommand].options());
      CommandLineOption HelpOption = getHelpOption();
      AllOptions.insert({HelpOption.getShortName(), HelpOption});

      int LargestOptionLength = std::numeric_limits<int>::min();
      for (auto& Opt : AllOptions)
      {
        LargestOptionLength = std::max(LargestOptionLength, (int)Opt.second.getDetails().size());
      }

      return LargestOptionLength;
    }


    // =====================================================================
    // =====================================================================


    std::string getCustomIndent(int CommandLength, int LargestCommandLength, std::string minimalSpace = "    ")
    {
      int Delta = LargestCommandLength - CommandLength;
      if (Delta > 0) 
      {
        minimalSpace.append(Delta, ' ');
      }
      return minimalSpace;
    }


    // =====================================================================
    // =====================================================================


    CommandLineOption getHelpOption()
    {
      return CommandLineOption("help","h","display this help message");
    }


    // =====================================================================
    // =====================================================================


    void displayFormattedData(std::ostream& OutStm, std::string Title, std::string HelpText, int LargestTextLength)
    {
      OutStm << "   " << Title << getCustomIndent(Title.size(), LargestTextLength) << HelpText << "\n";
    }


    // =====================================================================
    // =====================================================================


    void displayOptions(std::ostream& OutStm, CommandLineOption HelpOption, int LargestTextLength)
    {
      OutStm << "\nAvailable options:\n";

      displayFormattedData(OutStm, HelpOption.getDetails(), HelpOption.getHelpText(), LargestTextLength);

      if(m_UseCustomOrder)
      {
        for (auto& Opt : m_Commands[m_ActiveCommand].optionsOrdered())
        { 
          displayFormattedData(OutStm, Opt.getDetails(), Opt.getHelpText(), LargestTextLength);
        }
      }
      else
      {
        for (auto& Opt : m_Commands[m_ActiveCommand].options())
        { 
          displayFormattedData(OutStm, Opt.second.getDetails(), Opt.second.getHelpText(), LargestTextLength);
        }
      }
    }


    // =====================================================================
    // =====================================================================


    void displayUsageMessage(std::ostream& OutStm)
    {
      std::string CmdName = m_ActiveCommand;
      
      if (CmdName.empty())
      {
        CmdName = "[<command>]";
      }

      OutStm << "Usage : " << m_ProgramName << " " << CmdName << " [<options>] [<args>]\n";
    }


  private:

    std::vector<std::string> m_ExtraArgs;

    std::vector<std::string> m_ThirdPartyArgs;

    std::string m_ParsingMessage;

    bool m_HelpAsked;

    bool m_UseCustomOrder;

  public:

    CommandLineParser():
      m_HelpAsked(false), m_UseCustomOrder(false)
    {
      addCommand(CommandLineCommand("",""));
    }


    // =====================================================================
    // =====================================================================


    /**
      Instanciates a command line parser with the given parameters
      @param[in] ProgramName The name of the programn
      @param[in] HelpText The help text associated to the option
    */
    CommandLineParser(const std::string& ProgramName, const std::string& HelpText, bool UseCustomOrder = false) :
      m_ProgramName(ProgramName), m_HelpText(HelpText), m_HelpAsked(false), m_UseCustomOrder(UseCustomOrder)
    {
      addCommand(CommandLineCommand("",""));
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the extra arguments given to the command
      @return a vector of arguments
    */
    const std::vector<std::string>& extraArgs() const
    { 
      return m_ExtraArgs; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the third party arguments given to the command,
      mainly used to pass arguments to a third party program
      @return a vector of arguments
    */
    const std::vector<std::string>& thirdPartyArgs() const
    { 
      return m_ThirdPartyArgs; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the help text of the parser
      @return The help text
    */
    std::string getHelpText() const
    { 
      return m_HelpText; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the program name
      @return The program name
    */
    std::string getProgramName() const
    { 
      return m_ProgramName; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the message given by the parsing process in case of error(s)
      @return The message
    */
    std::string getParsingMessage() const
    { 
      return m_ParsingMessage; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the active command name
      @return The active command name
    */
    std::string getActiveCommand() const
    { 
      return m_ActiveCommand; 
    }


    // =====================================================================
    // =====================================================================


    /**
      Returns the command corresponding to the given name
      @param[in] Name the name of the command  
      @return The command object
    */
    const CommandLineCommand& command(const std::string& Name) const
    {
      return m_Commands.at(Name);
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds a command to the parser
      @param[in] Command The command to add
    */
    void addCommand(const CommandLineCommand& Command)
    {
      m_Commands[Command.getName()] = Command;
      m_CommandsOrdered.push_back(Command);
    }


    // =====================================================================
    // =====================================================================


    /**
      Adds a global option to the parser
      @param[in] Option The option to add
    */
    void addOption(const CommandLineOption& Option)
    {
      m_Commands[""].addOption(Option);
    }


    // =====================================================================
    // =====================================================================


    /**
      Parses a list of string arguments
      @param[in] ArgValues The lists of arguments to parse
      @return true if the parsing is succesful, false otherwise
    */
    bool parse(std::list<std::string> ArgValues)
    {
      reset();

      while (!ArgValues.empty())
      {
        std::string Arg = ArgValues.front();
        ArgValues.pop_front();

        if (Arg == "---")
        {
          m_ThirdPartyArgs = { std::make_move_iterator(std::begin(ArgValues)),
                               std::make_move_iterator(std::end(ArgValues)) };
          return true;
        }

        if (Arg[0] != '-')
        {
          // arg not an option
          if (m_ActiveCommand.empty())
          {
            m_ActiveCommand = Arg;
          }
          else
          {
            m_ExtraArgs.push_back(Arg);
          }
        }
        else
        {
          // arg is an option

          // check if the command exists
          auto it = m_Commands.find(m_ActiveCommand);
          if (it == m_Commands.end())
          {
            m_ParsingMessage = "unknown \""+ m_ActiveCommand + "\" command";
            return false;
          }

          if (Arg == "-h" || Arg == "--help")
          {
            m_HelpAsked = true;
          }
          else if (Arg.size() < 2)
          {
            m_ParsingMessage = "wrong format for option \"" + Arg + "\"";
            return false;
          }
          else
          {
            std::string LongOptName;
            std::string OptValue;
            bool IsFromShort = false;

            if (Arg[1] != '-')
            {
              if (Arg.size() != 2)
              {
                m_ParsingMessage = "wrong format for short option \"" + Arg + "\"";
                return false;
              }

              // short option
              LongOptName = m_Commands[m_ActiveCommand].getOptionNameFromShortName(Arg.substr(1,1));
              IsFromShort = true;
            }
            else
            {
              // long option
              std::string TmpName = Arg.substr(2,Arg.size()-2);

              std::string::size_type EqualPos = TmpName.find("=");

              if (EqualPos != std::string::npos)
              {
                LongOptName = TmpName.substr(0,EqualPos);
                OptValue = TmpName.substr(EqualPos+1,TmpName.size()-EqualPos-1);
              }
              else
              {
                LongOptName = TmpName;
              }
            }

            if (!m_Commands[m_ActiveCommand].isOptionExists(LongOptName))
            {
              // unknown option
              m_ParsingMessage = "unknown option \"" + Arg + "\"";
              if (!m_ActiveCommand.empty())
              {
                m_ParsingMessage += " for command \"" + m_ActiveCommand + "\"";
              }
              return false;
            }

            if (m_Commands[m_ActiveCommand].isOptionRequiresValue(LongOptName))
            {
              if (IsFromShort && !ArgValues.empty())
              {
                OptValue = ArgValues.front();
                ArgValues.pop_front();
              }

              if (OptValue.empty())
              {
                m_ParsingMessage = "missing value for option \"" + Arg + "\"";
                return false;
              }
            }

            if (!m_Commands[m_ActiveCommand].activateOption(LongOptName,OptValue))
            {
              // wrong option format
              m_ParsingMessage = "unknown error for option \"" + Arg + "\"";
              return false;
            }
          }
        }
      }
      return true;
    }


    // =====================================================================
    // =====================================================================


    /**
      Parses arguments from the standard parameters `(int argc,char** argv)` given through the main function
      @param[in] ArgC The number of arguments
      @param[in] ArgV The array of arguments
      @return true if the parsing is succesful, false otherwise
    */
    bool parse(int ArgC, char **ArgV)
    {
      std::list<std::string> ArgValues;

      for (int i=1; i< ArgC; i++)
      {
        ArgValues.push_back(std::string(ArgV[i]));
      }

      return parse(ArgValues);
    }


    // =====================================================================
    // =====================================================================


    /**
      Resets the parser to default (no active command, no extra arguments, no option activated)
    */
    void reset()
    {
      m_ActiveCommand.clear();
      m_ParsingMessage.clear();
      m_ExtraArgs.clear();
      m_ThirdPartyArgs.clear();

      for (auto& Cmd : m_Commands)
      {
        Cmd.second.reset();
      }
    }


    // =====================================================================
    // =====================================================================


    /**
      Tests if the help is asked
      @return true if the help is asked
    */
    bool isHelpAsked()
    { 
      return m_HelpAsked; 
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
        OutStm << "\n" << m_AvailableCommandsText << "\n";

        if(m_UseCustomOrder)
        {
          for (const auto& Cmd : m_CommandsOrdered)
          {
            if (!Cmd.getName().empty())
            {
              displayFormattedData(OutStm, Cmd.getName(), Cmd.getHelpText(), LargestCommandTextLength);
            }
          }
        }
        else 
        {
          for (const auto& Cmd : m_Commands)
          {
            if (!Cmd.first.empty())
            {
              displayFormattedData(OutStm, Cmd.first, Cmd.second.getHelpText(), LargestCommandTextLength);
            }
          }
        }
      }

      if (!m_Commands[m_ActiveCommand].getLongHelpText().empty())
      {
        OutStm << "\n" << m_Commands[m_ActiveCommand].getLongHelpText() << "\n";
      }

      displayOptions(OutStm, getHelpOption(), LargestOptionTextLength);
    }


    // =====================================================================
    // =====================================================================


    /**
      Prints the state of the parser 
      @param[in] OutStm The stream where the help text is printed (e.g. std::cout)
    */
    void printState(std::ostream& OutStm)
    {
      for (auto& Cmd : m_Commands)
      {
        if (!Cmd.first.empty())
        {
          if (Cmd.first == m_ActiveCommand)
          {
            OutStm << "+";
          }
          else 
          {
            OutStm << "-";
          }

          OutStm << " " << Cmd.first << " : " << Cmd.second.getHelpText() << "\n";

          for (auto& Opt : Cmd.second.options())
          {
            OutStm << "  ";

            if (Opt.second.isActive())
            {
              OutStm << "+";
            }
            else
            {
              OutStm << "-";
            }
            OutStm << " " << Opt.first;

            if (!Opt.second.getShortName().empty())
            {
              OutStm << "," << Opt.second.getShortName();
            }

            if (Opt.second.isValueRequired())
            {
              OutStm << "[" << Opt.second.getValue() << "]";
            }

            OutStm << " : " << Opt.second.getHelpText() << "\n";
          }
        }
      }


      if (!m_ExtraArgs.empty())
      {
        OutStm << "Extra arguments :";
        
        for (auto& Arg : m_ExtraArgs)
        {
          OutStm << " " << Arg;
        }

        OutStm << "\n";
      }

    }

};


} }


#endif /* __OPENFLUID_UTILS_COMMANDLINEPARSER_HPP__ */
