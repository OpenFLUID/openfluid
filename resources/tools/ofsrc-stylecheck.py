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


############################################################################
############################################################################


class SourceTreeChecker:


  def __init__(self,SrcRootPath,IsVerbose):
    
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
    
    self.ErrorsCount = 0;
    
    self.MaxLineLength = 120;
    
    for Dirname, Dirnames, Filenames in os.walk(self.SrcRootPath):
      for Filename in Filenames:
        RelFilename = os.path.relpath(os.path.join(Dirname, Filename),self.SrcRootPath)
        if not RelFilename.startswith('.') and not RelFilename.startswith('_'):
          if (RelFilename.endswith('.cpp') or RelFilename.endswith('.hpp') or
              RelFilename.endswith('.hpp.in')) :
            self.FileList.append(RelFilename)

            
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


  def addProblem(self,Filename,*Args):
      self.ErrorsCount += 1
      print Filename+':',
      for Arg in Args:
        print Arg,
      print


############################################################################


  def checkLineLength(self, Filename, Lines):
     
    i = 1
    for Line in Lines :
      if len(Line) > self.MaxLineLength:
        self.addProblem(Filename,'line',i,'is too long (exceeds',self.MaxLineLength,'characters)')
      i += 1


############################################################################
    

  def checkLicenseHeader(self, Filename, Content):

    if not Content.startswith(self.LicenseHeader):
      self.addProblem(Filename,'missing or malformed license header')
     
     
############################################################################


  def checkFileInfo(self, Filename, Content):

    ExpectedFilename = os.path.basename(Filename)
    if ExpectedFilename.endswith('.in'):
      ExpectedFilename = ExpectedFilename[:-3]
        
    Result = re.search( r'\@file '+re.escape(ExpectedFilename), Content)
    if not Result:
      self.addProblem(Filename,'missing or malformed @file information (expected @file',ExpectedFilename+')')
      
    Result = re.search( r'\@author \w+', Content)
    if not Result:
      self.addProblem(Filename,'missing or malformed @author information (expected @author <authorname>)')
      

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
        self.addProblem(Filename,'missing or malformed header guard (expected',ExpectedGuardName+')')


############################################################################


  def checkFile(self, Filename): 
    self.printVerbose('== Checking',Filename)
    
    FileContent = open(os.path.join(self.SrcRootPath,Filename), 'r').read()
    FileLines = FileContent.split('\n')
    
    self.checkLineLength(Filename,FileLines)
    self.checkLicenseHeader(Filename,FileContent)
    self.checkFileInfo(Filename,FileContent)
    self.checkHeaderGuard(Filename,FileContent)


############################################################################


  def run(self):
    
    if not self.runPreliminaryChecks():
      raise Exception('The given path does not appear to be the OpenFLUID source code')
    
    for File in self.FileList:
      self.checkFile(File)
    
    print len(self.FileList),"files checked,",self.ErrorsCount,"potential style problems detected"



############################################################################
############################################################################



try:

  Parser = ArgumentParser(prog='ofsrc-stylecheck')
  Parser.add_argument('-V','--verbose', action='store_true', default=False, help='verbose mode')
  Parser.add_argument('path',default='')
    
  Args = Parser.parse_args()
  Args = vars(Args)

  Checker = SourceTreeChecker(Args['path'],Args['verbose'])
  Checker.run()
  
except Exception as e:
  print "Error:",
  print e
