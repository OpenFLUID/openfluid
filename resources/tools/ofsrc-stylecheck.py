# 
#   This file is part of OpenFLUID software
#   Copyright(c) 2007, INRA - Montpellier SupAgro
# 
# 
#  == GNU General Public License Usage ==
# 
#   OpenFLUID is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
# 
#   OpenFLUID is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.
# 
# 
#  == Other Usage ==
# 
#   Other Usage means a use of OpenFLUID that is inconsistent with the GPL
#   license, and requires a written agreement between You and INRA.
#   Licensees for Other Usage of OpenFLUID may use this file in accordance
#   with the terms contained in the written agreement between You and INRA.
#


from argparse import ArgumentParser
import os
import re
import sys


############################################################################
############################################################################


class SourceTreeChecker:


  def __init__(self, SrcRootPath, IsVerbose, DisableChecks=""):
    
    self.SrcOpenFLUIDDir = 'src/openfluid'
    self.SrcAppsDir = 'src/apps'
    self.MainCMakeFile = 'CMakeLists.txt'
    self.MainCMakeConfigFile = 'CMake.in.cmake'
    
    self.LicenseHeader = """/*

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
"""
    
    
    self.SrcRootPath = SrcRootPath
    self.IsVerbose = IsVerbose
    
    self.FileList = list()
    
    self.CheckStyles = dict()
    self.CheckStyles["LLEN"] = (self.checkLineLength, "lines")
    self.CheckStyles["PRTY"] = (self.checkPrettyCode, "lines")
    self.CheckStyles["LICH"] = (self.checkLicenseHeader, "content")
    self.CheckStyles["AUTH"] = (self.checkFileAuthor, "content")
    self.CheckStyles["FNAM"] = (self.checkFilename, "content")
    self.CheckStyles["HGRD"] = (self.checkHeaderGuard, "content")
    self.CheckStyles["INCO"] = (self.checkIncludesOrder, "lines")
    self.CheckStyles["INCS"] = (self.checkIncludesSpacing, "lines")
    self.CheckStyles["TMSP"] = (self.checkTooMuchSpacing, "lines")
    self.CheckStyles["SEPS"] = (self.checkSeparators, "lines")
    self.CheckStyles["TABC"] = (self.checkTabs, "lines")
    self.CheckStyles["BRAC"] = (self.checkBrackets, "lines")
    
    self.ActiveStyles = list()
    self.ErrorsCount = dict()
    DisableChecksList = DisableChecks.split(",")
    
    for Style in list(self.CheckStyles.keys()):
      if Style not in DisableChecksList:
        self.ActiveStyles.append(Style)
        self.ErrorsCount[Style] = 0
    
    self.DirectiveBase = "OpenFLUID:stylecheck"
    
    self.MaxLineLength = 120
    
    self.PrettyCodeTag = '#prettycode'
    
    self.OFDependentIncludes = ["unistd.h"]
    
    for Dirname, Dirnames, Filenames in os.walk(self.SrcRootPath):
      for Filename in Filenames:
        RelFilename = os.path.relpath(os.path.join(Dirname, Filename),self.SrcRootPath)
        if not RelFilename.startswith('.') and not RelFilename.startswith('_'):
          if (RelFilename.endswith('.cpp') or RelFilename.endswith('.hpp') or
              RelFilename.endswith('.hpp.in')) :
            self.FileList.append(RelFilename)

    self.FileList.sort()

            
############################################################################            


  def runPreliminaryChecks(self):
    return (os.path.isdir(os.path.join(self.SrcRootPath,self.SrcOpenFLUIDDir)) and
            os.path.isdir(os.path.join(self.SrcRootPath,self.SrcAppsDir)) and
            os.path.isfile(os.path.join(self.SrcRootPath,self.MainCMakeFile)) and
            os.path.isfile(os.path.join(self.SrcRootPath,self.MainCMakeConfigFile)))


############################################################################


  def printVerbose(self,*Args):
    if self.IsVerbose:
      for Arg in Args:
        print Arg,
      print


############################################################################


  def addProblem(self,Code,Filename,Line,*Args):
    self.ErrorsCount[Code] += 1
    ArgsStr = list()
    for Arg in Args :
      ArgsStr.append(str(Arg))   
    Msg = '[{0}]{1}:{2}:{3}'.format(Code,Filename,Line," ".join(ArgsStr))
    print Msg


############################################################################


  def isDirective(self,StrData,Directive):
    FullDirective = self.DirectiveBase + ":" + Directive
    if FullDirective in StrData :
      return True
    return False 


############################################################################


  def checkLineLength(self, Filename, Lines):
     
    i = 1
    for Line in Lines :
      if len(Line) > self.MaxLineLength:
        self.addProblem('LLEN',Filename,i,'line is too long (exceeds',self.MaxLineLength,'characters)')
      i += 1


############################################################################


  def checkPrettyCode(self, Filename, Lines):
     
    i = 1
    for Line in Lines :
      if self.PrettyCodeTag in Line.lower():
        self.addProblem('PRTY',Filename,i,'line contains the',self.PrettyCodeTag,'tag')
      i += 1


############################################################################
    

  def checkLicenseHeader(self, Filename, Content):

    if not Content.startswith(self.LicenseHeader):
      self.addProblem('LICH',Filename,1,'missing or malformed license header')
     
     
############################################################################


  def checkFilename(self, Filename, Content):

    ExpectedFilename = os.path.basename(Filename)
    if ExpectedFilename.endswith('.in'):
      ExpectedFilename = ExpectedFilename[:-3]
        
    Result = re.search( r'\@file '+re.escape(ExpectedFilename), Content)
    if not Result:
      ExpectedFilenameWithParentDir = os.path.basename(os.path.dirname(Filename))+'/'+ExpectedFilename;       
      Result = re.search( r'\@file '+re.escape(ExpectedFilenameWithParentDir), Content)
      if not Result:
        self.addProblem('FNAM',Filename,1,'missing or malformed @file information (expected @file',ExpectedFilename,
                        'or @file',ExpectedFilenameWithParentDir+')')


############################################################################
      
      
  def checkFileAuthor(self, Filename, Content):
    
    Result = re.search( r'\@author \w+', Content)
    if not Result:
      self.addProblem('AUTH',Filename,1,'missing or malformed @author information (expected @author <authorname>)')
      

############################################################################


  def checkHeaderGuard(self, Filename, Content):
  
    ExpectedFilename = Filename
    if ExpectedFilename.endswith('.in'):
      ExpectedFilename = ExpectedFilename[:-3]
      
    ExpectedGuardName = ''
    
    # file is part of the OpenFLUID framework
    if ExpectedFilename.endswith('.hpp'):
      if Filename.startswith('src/openfluid'):
        ExpectedFilename = ExpectedFilename[13:]
        LastPart = os.path.basename(ExpectedFilename).upper().replace('.','_')
        MiddlePart = os.path.dirname(ExpectedFilename).upper().replace('/','')
        if MiddlePart:
          MiddlePart += '_'
        ExpectedGuardName = '__OPENFLUID_'+MiddlePart+LastPart+'__'

      # file is part of an OpenFLUID application
      if Filename.startswith('src/apps'):
        ExpectedFilename = ExpectedFilename[8:]
        LastPart = os.path.basename(ExpectedFilename).upper().replace('.','_')
        Dirname = os.path.dirname(ExpectedFilename).strip('/')

        if not Dirname.find("tests"):
          if Dirname.startswith('openfluid-builder'):
            MiddlePart = 'BUILDERAPP_'
          elif Dirname.startswith('openfluid-devstudio'):
            MiddlePart = 'DEVSTUDIOAPP_'
          elif Dirname.startswith('openfluid'):
            MiddlePart = 'CMDLINEAPP_'
          else:
            raise Exception('Unknown application in source code')
        
          ExpectedGuardName = '__OPENFLUID_'+MiddlePart+LastPart+'__'


    if ExpectedGuardName:
      Result = re.search( r'#ifndef '+re.escape(ExpectedGuardName)+'\s*\n#define '+re.escape(ExpectedGuardName), Content)
      if not Result:
        self.addProblem('HGRD',Filename,1,'missing or malformed header guard (expected',ExpectedGuardName+')')


############################################################################


  def checkIncludesOrder(self, Filename, Lines):

    LocalIncStarted = False
    OFIncStarted = False        
     
    i = 1
    for Line in Lines :
      if self.isDirective(Line, "!inco"):
        return
      NoSpaceLine = Line.replace(" ","")
      if NoSpaceLine.startswith("#include\"") :
        LocalIncStarted = True
      elif NoSpaceLine.startswith("#include<openfluid") :
        OFIncStarted = True
        if LocalIncStarted :
          self.addProblem('INCO',Filename,i,'wrong #include order')
          return      
      elif NoSpaceLine.startswith("#include<") :
        if OFIncStarted :
          if not any(Except in NoSpaceLine for Except in self.OFDependentIncludes):
            self.addProblem('INCO',Filename,i,'wrong #include order')
            return            
        elif LocalIncStarted :
          self.addProblem('INCO',Filename,i,'wrong #include order')
          return
      i += 1


############################################################################


  def checkIncludesSpacing(self, Filename, Lines):
  
    FirstIncLine = 0;
    LastIncLine = 0;
  
    i = 1
    for Line in Lines :
      if self.isDirective(Line, "!incs"):
        return
      NoSpaceLine = Line.replace(" ","")
      if NoSpaceLine.startswith("#include") :
        LastIncLine = i
        if FirstIncLine < 1 :
          FirstIncLine = i 
      i += 1

    if LastIncLine == 0 :
      return
  
    if FirstIncLine < 2 : 
      self.addProblem('INCS',Filename,FirstIncLine,'not enough blank lines before first #include (2 blank lines expected)')
    else : 
      NoSpaceBeforeLines = (Lines[FirstIncLine-2]+Lines[FirstIncLine-3]).replace(" ","")
      if len(NoSpaceBeforeLines) > 0:
        self.addProblem('INCS',Filename,FirstIncLine,'not enough blank lines before first #include (2 blank lines expected)')
    
    if len(Lines) - LastIncLine < 2 :
      self.addProblem('INCS',Filename,LastIncLine,'not enough blank lines after last #include (2 blank lines expected)')
    else : 
      NoSpaceAfterLines = (Lines[LastIncLine]+Lines[LastIncLine+1]).replace(" ","")
      if len(NoSpaceAfterLines) > 0:
        self.addProblem('INCS',Filename,LastIncLine,'not enough blank lines after last #include (2 blank lines expected)')


############################################################################


  def checkTooMuchSpacing(self, Filename, Lines):
    
    RunningTMS = 0
    
    i = 1
    for Line in Lines :
      if len(Line.replace(" ","")) == 0  :
        RunningTMS += 1
      else :
        RunningTMS = 0
      if RunningTMS == 3 :
        self.addProblem('TMSP',Filename,i-2,'too many blank lines starting at line',(i-2))         
      i += 1


############################################################################


  def checkSeparators(self, Filename, Lines):
    
    FirstProcessed = False 
    i = 1
    for Line in Lines :
      if FirstProcessed :
        FirstProcessed = False
      else:  
        NoSpaceLine = Line.replace(" ","")
        if NoSpaceLine.startswith('//================================='):
          if (i < 3) or (i > len(Lines)-3) :
            self.addProblem('SEPS',Filename,i,'misplaced separator')
          else:
            FirstProcessed = True
            if ((len(Lines[i-3].replace(" ","")) != 0) or
                (len(Lines[i-2].replace(" ","")) != 0) or
                (not Lines[i].replace(" ","").startswith('//=================================')) or
                (len(Lines[i+1].replace(" ","")) != 0) or
                (len(Lines[i+2].replace(" ","")) != 0)) :
              self.addProblem('SEPS',Filename,i,'wrong separator')  
      i += 1


############################################################################


  def checkTabs(self, Filename, Lines):
     
    i = 1
    for Line in Lines :
      if "\t" in Line.lower():
        self.addProblem('TABC',Filename,i,'line contains at least one tab character')
      i += 1


############################################################################


  def isInstructionBlock(self, Word):
    
    CPPInstructions = ["if", "else", "for", "while", "do"]
    OFInstructions = ["OPENFLUID_UNITS_ORDERED_LOOP", "OPENFLUID_ALLUNITS_ORDERED_LOOP", 
                      "OPENFLUID_UNITSLIST_LOOP", "OPENFLUID_EVENT_COLLECTION_LOOP"]
    Instructions = CPPInstructions + OFInstructions
    return (Word in Instructions)


############################################################################


  def checkBrackets(self, Filename, Lines):
    
    i = 1
    IsNewBlock = False
    IsPartialInstruction = False
    IsDo = False
    for Line in Lines:
      if len(Line.split()) > 0:
        FirstWord = Line.split()[0].split("(")[0]
        NoCommentLine = Line.split("//")[0]
        NoSpaceLine = NoCommentLine.replace(" ","")
        
        
        if IsNewBlock and (NoSpaceLine[:2] in ["==", "&&", "||"] or NoSpaceLine.startswith(".")):
          # case of previously undetected PartialInstruction
          IsNewBlock = False
          IsPartialInstruction = True
        
        if IsNewBlock:
          if NoSpaceLine.startswith("{"):
            IsNewBlock = False
          else:
            self.addProblem('BRAC',Filename,i,'block must be between brackets')
            IsNewBlock = False
        elif (self.isInstructionBlock(FirstWord) or IsPartialInstruction):
          if FirstWord == "do":
            IsDo = True
          if NoSpaceLine.endswith(";") or NoSpaceLine.endswith("{") or NoSpaceLine.endswith("}"):
            if IsDo and FirstWord == "while":
              IsDo = False
            else:
              if NoSpaceLine.endswith(";") or NoSpaceLine.endswith("}"):
                if FirstWord == "for" and NoSpaceLine.endswith(";"):  # unconclusive
                  pass
                else:
                  self.addProblem('BRAC',Filename,i,'block must not be on one line')
              else: # finish with "{" case
                self.addProblem('BRAC',Filename,i,'bracket must be on a new line')
            IsPartialInstruction = False
          elif NoSpaceLine.endswith(")") or NoSpaceLine in ["else", "do"] or NoSpaceLine.endswith("\\"):
            IsNewBlock = True
            IsPartialInstruction = False
          else:
            IsPartialInstruction = True
        
      i += 1
      
        
############################################################################


  def checkFile(self, Filename): 
    self.printVerbose('== Checking',Filename)
    
    FileContent = open(os.path.join(self.SrcRootPath,Filename), 'r').read()
    FileLines = FileContent.split('\n')
    
    for Style in self.ActiveStyles:
      if self.CheckStyles[Style][1] == "lines":
        self.CheckStyles[Style][0](Filename,FileLines)
      elif self.CheckStyles[Style][1] == "content":
        self.CheckStyles[Style][0](Filename,FileContent)


############################################################################


  @staticmethod
  def stringIfMoreThanOne(s,c):
    if c > 1:
      return s
    else:
      return ""


############################################################################


  def run(self):
    
    if not self.runPreliminaryChecks():
      raise Exception('The given path does not appear to be the OpenFLUID source code')
    
    for File in self.FileList:
      self.checkFile(File)
    
    FileCount = len(self.FileList)

    print FileCount,"file"+self.stringIfMoreThanOne('s',FileCount)+" checked"

    TotalErrorsCount = sum(self.ErrorsCount.values())

    if TotalErrorsCount:
      print TotalErrorsCount,"potential problem"+self.stringIfMoreThanOne('s',TotalErrorsCount)+" detected"
      for Code in self.ErrorsCount:
        print '    - ['+Code+']:',self.ErrorsCount[Code],'problem'+self.stringIfMoreThanOne('s',self.ErrorsCount[Code])
    else:
      print "No potential style problem detected"



############################################################################
############################################################################


try:

  Parser = ArgumentParser(prog='ofsrc-stylecheck')
  Parser.add_argument('-V','--verbose', action='store_true', default=False, help='verbose mode')
  Parser.add_argument('--disable', default="", help='ignore warnings for given categories, splitted by commas. Example "Brackets,PrettyCode"')
  Parser.add_argument('path',default='')
    
  Args = Parser.parse_args()
  Args = vars(Args)

  Checker = SourceTreeChecker(Args['path'],Args['verbose'], Args['disable'])
  Checker.run()
  
except Exception as e:
  print "Error:",
  print e
  sys.exit(127)
