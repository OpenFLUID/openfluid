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
*/


#ifndef __OPENFLUID_UTILS_COMMANDLINEPARSER_HPP__
#define __OPENFLUID_UTILS_COMMANDLINEPARSER_HPP__


#include <map>
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


    CommandLineOption(const std::string& LongName, const std::string& ShortName,
                      const std::string& HelpText, bool ValueRequired = false):
      m_LongName(LongName),m_ShortName(ShortName), m_ValueRequired(ValueRequired), m_HelpText(HelpText),
      m_Active(false)
    { }


    // =====================================================================
    // =====================================================================


    std::string getHelpText() const
    { return m_HelpText; }


    // =====================================================================
    // =====================================================================


    std::string getLongName() const
    { return m_LongName; }


    // =====================================================================
    // =====================================================================


    std::string getShortName() const
    { return m_ShortName; }


    // =====================================================================
    // =====================================================================


    std::string getValue() const
    { return m_Value; }


    // =====================================================================
    // =====================================================================


    bool isActive() const
    { return m_Active; }


    // =====================================================================
    // =====================================================================


    bool isValueRequired() const
    { return m_ValueRequired; }


    // =====================================================================
    // =====================================================================


    void activate(const std::string& Value = "")
    {
      m_Value = Value;

      if ((!m_ValueRequired) || (m_ValueRequired && !m_Value.empty()))
        m_Active = true;
      else
        m_Active = false;
    }


    // =====================================================================
    // =====================================================================


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

    std::map<std::string,CommandLineOption> m_Options;

    std::map<std::string,CommandLineOption*> m_ShortOptions;


  public:

    CommandLineCommand()
    { }


    // =====================================================================
    // =====================================================================


    CommandLineCommand(const std::string& Name, const std::string& HelpText):
      m_Name(Name), m_HelpText(HelpText)
    { }


    // =====================================================================
    // =====================================================================


    std::string getHelpText() const
    { return m_HelpText; }


    // =====================================================================
    // =====================================================================


    std::string getName() const
    { return m_Name; }


    // =====================================================================
    // =====================================================================


    const std::map<std::string,CommandLineOption>& options()
    { return m_Options; }


    // =====================================================================
    // =====================================================================


    void addOption(const CommandLineOption& Option)
    {
      m_Options[Option.getLongName()] = Option;

      if (!Option.getShortName().empty())
        m_ShortOptions[Option.getShortName()] = &m_Options[Option.getLongName()];
    }


    // =====================================================================
    // =====================================================================


    bool activateOption(const std::string& LongName, const std::string& Value = "")
    {
      auto it = m_Options.find(LongName);

      if (it == m_Options.end())
        return false;

      (*it).second.activate(Value);
      return (*it).second.isActive();
    }


    // =====================================================================
    // =====================================================================


    std::string getOptionNameFromShortName(const std::string& ShortName) const
    {
      auto it = m_ShortOptions.find(ShortName);

      if (it == m_ShortOptions.end())
        return "";

      return (*it).second->getLongName();
    }


    // =====================================================================
    // =====================================================================


    bool isOptionExists(const std::string& LongName) const
    {
      return (m_Options.find(LongName) != m_Options.end());
    }


    // =====================================================================
    // =====================================================================


    bool isOptionRequiresValue(const std::string& LongName) const
    {
      auto it = m_Options.find(LongName);

      if (it == m_Options.end())
        return false;

      return (*it).second.isValueRequired();

    }


    // =====================================================================
    // =====================================================================


    bool isOptionActive(const std::string& LongName) const
    {
      auto it = m_Options.find(LongName);

      return (it != m_Options.end() && (*it).second.isActive());
    }


    // =====================================================================
    // =====================================================================


    std::string getOptionValue(const std::string& LongName) const
    {
      auto it = m_Options.find(LongName);

      if (it == m_Options.end())
        return "";

      return ((*it).second.getValue());
    }


    // =====================================================================
    // =====================================================================


    void reset()
    {
      for (auto& Opt : m_Options)
        Opt.second.reset();
    }

};


// =====================================================================
// =====================================================================


class CommandLineParser
{
  private:

    std::string m_ProgramName;

    std::string m_HelpText;

    std::map<std::string,CommandLineCommand> m_Commands;

    std::string m_ActiveCommand;

    std::vector<std::string> m_ExtraArgs;

    std::string m_ParsingMessage;

    bool m_HelpAsked;


  public:

    CommandLineParser():
      m_HelpAsked(false)
    {
      addCommand(CommandLineCommand("",""));
    }


    // =====================================================================
    // =====================================================================


    CommandLineParser(const std::string& ProgramName, const std::string& HelpText) :
      m_ProgramName(ProgramName), m_HelpText(HelpText), m_HelpAsked(false)
    {
      addCommand(CommandLineCommand("",""));
    }


    // =====================================================================
    // =====================================================================


    const std::vector<std::string>& extraArgs() const
    { return m_ExtraArgs; }


    // =====================================================================
    // =====================================================================


    std::string getHelpText() const
    { return m_HelpText; }


    // =====================================================================
    // =====================================================================


    std::string getProgramName() const
    { return m_ProgramName; }


    // =====================================================================
    // =====================================================================


    std::string getParsingMessage() const
    { return m_ParsingMessage; }


    // =====================================================================
    // =====================================================================


    std::string getActiveCommand() const
    { return m_ActiveCommand; }


    // =====================================================================
    // =====================================================================


    const CommandLineCommand& command(const std::string& Name) const
    {
      return m_Commands.at(Name);
    }


    // =====================================================================
    // =====================================================================


    void addCommand(const CommandLineCommand& Command)
    {
      m_Commands[Command.getName()] = Command;
    }

    // =====================================================================
    // =====================================================================


    void addOption(const CommandLineOption& Option)
    {
      m_Commands[""].addOption(Option);
    }


    // =====================================================================
    // =====================================================================


    bool parse(std::list<std::string> ArgValues)
    {
      reset();

      while (!ArgValues.empty())
      {
        std::string Arg = ArgValues.front();
        ArgValues.pop_front();

        if (Arg[0] != '-')
        {
          // arg not an option
          if (m_ActiveCommand.empty())
            m_ActiveCommand = Arg;
          else
            m_ExtraArgs.push_back(Arg);
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
            m_HelpAsked = true;
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
                LongOptName = TmpName;
            }

            if (!m_Commands[m_ActiveCommand].isOptionExists(LongOptName))
            {
              // unknown option
              m_ParsingMessage = "unknown option \"" + Arg + "\"";
              if (!m_ActiveCommand.empty())
                m_ParsingMessage += " for command \"" + m_ActiveCommand + "\"";
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


    bool parse(int ArgC, char **ArgV)
    {
      std::list<std::string> ArgValues;

      for (int i=1; i< ArgC; i++)
        ArgValues.push_back(std::string(ArgV[i]));

      return parse(ArgValues);
    }


    // =====================================================================
    // =====================================================================


    void reset()
    {
      m_ActiveCommand.clear();
      m_ParsingMessage.clear();
      m_ExtraArgs.clear();

      for (auto& Cmd : m_Commands)
        Cmd.second.reset();
    }


    // =====================================================================
    // =====================================================================


    bool isHelpAsked()
    { return m_HelpAsked; }


    // =====================================================================
    // =====================================================================


    void printHelp(std::ostream& OutStm)
    {
      std::string m_CmdName = m_ActiveCommand;
      if (m_CmdName.empty())
        m_CmdName = "[<command>]";

      OutStm << "Usage : " << m_ProgramName << " " << m_CmdName << " [<options>] [<args>]\n";

      if (m_ActiveCommand.empty())
      {
        OutStm << "\nAvailable commands:\n";

        for (auto& Cmd : m_Commands)
          if (!Cmd.first.empty())
            OutStm << "  " << Cmd.first << " : " << Cmd.second.getHelpText() << "\n";
      }

      if (!m_Commands[m_ActiveCommand].getHelpText().empty())
        OutStm << "\n" << m_Commands[m_ActiveCommand].getHelpText() << "\n";

      OutStm << "\nAvailable options:\n";
      OutStm << "  --help,-h : display this help message\n";

      for (auto& Opt : m_Commands[m_ActiveCommand].options())
      {
        OutStm << "  --" << Opt.second.getLongName();
        if (Opt.second.isValueRequired())
          OutStm << "=<arg>";

        if (!Opt.second.getShortName().empty())
        {
          OutStm << ", -" << Opt.second.getShortName();
          if (Opt.second.isValueRequired())
            OutStm << " <arg>";
        }

        OutStm << " : " << Opt.second.getHelpText() << "\n";
      }
    }


    // =====================================================================
    // =====================================================================


    void printState(std::ostream& OutStm)
    {
      for (auto& Cmd : m_Commands)
      {
        if (!Cmd.first.empty())
        {
          if (Cmd.first == m_ActiveCommand)
            OutStm << "+";
          else OutStm << "-";

          OutStm << " " << Cmd.first << " : " << Cmd.second.getHelpText() << "\n";

          for (auto& Opt : Cmd.second.options())
          {
            OutStm << "  ";

            if (Opt.second.isActive())
              OutStm << "+";
            else
              OutStm << "-";
            OutStm << " " << Opt.first;

            if (!Opt.second.getShortName().empty())
              OutStm << "," << Opt.second.getShortName();

            if (Opt.second.isValueRequired())
              OutStm << "[" << Opt.second.getValue() << "]";

            OutStm << " : " << Opt.second.getHelpText() << "\n";
          }
        }
      }


      if (!m_ExtraArgs.empty())
      {
        OutStm << "Extra arguments :";
        for (auto& Arg : m_ExtraArgs)
          OutStm << " " << Arg;
        OutStm << "\n";
      }

    }

};



} }


#endif /* __OPENFLUID_UTILS_COMMANDLINEPARSER_HPP__ */
