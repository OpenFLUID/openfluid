#!/usr/bin/env python3
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
############################################################################
#
#  This script extracts and processes icons for OpenFLUID UI. It uses icons from Google material design
#  icons library (https://material.io/icons/) and from https://materialdesignicons.com/
#
#  Usage: oficons-extract /path/to/work
#         oficons-extract --help
#
#  This script requires the convert tool from ImageMagick (https://www.imagemagick.org/)
#
#  For each icon, it generates a dark and/or light and/or grayed icon for OpenFLUID UI, according to the Configuration
#  set in the BaseSet and ExtSet variables in the following lines of code
#
############################################################################


from argparse import ArgumentParser
import os
import sys
import shutil
import subprocess


assert sys.version_info >= (3, 5)


############################################################################
############################################################################


BaseSet = dict() #['subdir','rootname',light,dark,grayed]
ExtSet = dict() #['filename without ext',light,dark,grayed]


BaseSet['project-new'] = ['file','create_new_folder',True,True,True]
BaseSet['project-open-example'] = ['notification','folder_special',True,True,True]
BaseSet['project-save'] = ['content','save',True,False,True]
ExtSet['project-open'] = ['folder-upload',True,True,True]
ExtSet['project-save-as'] = ['content-save-settings',True,False,True]

ExtSet['file-open'] = ['file-export',True,True,True]
BaseSet['file-save'] = ['content','save',True,True,True]
ExtSet['file-save-as'] = ['content-save-settings',True,True,True]
ExtSet['file-new'] = ['file-plus',True,True,True]

BaseSet['close'] = ['navigation','close',True,False,True]
BaseSet['reload'] = ['navigation','refresh',True,False,True]
BaseSet['run'] = ['av','play_circle_outline',True,False,True]

BaseSet['start'] = ['av','play_arrow',False,True,False]
BaseSet['pause'] = ['av','pause',False,True,False]
BaseSet['stop'] = ['av','stop',False,True,False]

BaseSet['add'] = ['content','add',False,True,False]
BaseSet['remove'] = ['content','remove',False,True,False]
BaseSet['delete'] = ['action','delete',False,True,False]
BaseSet['refresh'] = ['action','cached',False,True,False]
BaseSet['search'] = ['action','search',True,True,True]
BaseSet['go-up'] = ['hardware','keyboard_arrow_up',False,True,False]
BaseSet['go-down'] = ['hardware','keyboard_arrow_down',False,True,False]
BaseSet['modify'] = ['image','edit',False,True,False]
BaseSet['settings'] = ['action','settings',True,True,True]
ExtSet['rename-column'] = ['page-layout-header',False,True,False]

BaseSet['configure'] = ['image','tune',True,True,True]
BaseSet['build'] = ['action','build',True,True,True]

BaseSet['magic'] = ['image','flash_on',False,True,False]
BaseSet['menu'] = ['navigation','menu',False,True,False]
BaseSet['market'] = ['action','shopping_cart',False,True,False]
BaseSet['link'] = ['content','link',False,True,False]
BaseSet['open'] = ['action','launch',True,True,False]
BaseSet['dashboard'] = ['action','dashboard',True,True,False]
BaseSet['doc'] = ['action','book',True,True,False]
BaseSet['import'] = ['action','system_update_alt',True,True,False]

ExtSet['file-explorer'] = ['file-tree',False,True,False]
ExtSet['dev-wares'] = ['code-braces',False,True,False]
ExtSet['edit-plugin'] = ['puzzle-edit',True,True,True]
ExtSet['ghost2sim'] = ['code-json',False,True,False]
ExtSet['log-view'] = ['message-text-outline',False,True,False]
ExtSet['export'] = ['export',False,True,False]
ExtSet['linked'] = ['link-variant',False,True,False]
ExtSet['unlinked'] = ['link-variant-off',False,True,False]
ExtSet['generate-doc'] = ['book-open-page-variant',True,True,True]
ExtSet['view-doc'] = ['book-open-variant',True,True,True]
ExtSet['color-marker'] = ['format-color-highlight',False,True,False]
ExtSet['color-marker-cancel'] = ['format-color-marker-cancel',False,True,False]
ExtSet['application'] = ['application',False,True,False]
ExtSet['folder-pound'] = ['folder-pound',False,True,False]


BaseSetGitURL = 'https://github.com/OpenFLUID/material-design-icons.git'
BaseSetDir = 'base'
BaseSetVersion = os.path.join('svg','production')
BaseSetPrefix = 'ic'
BaseSetSize = '48px'

ExtSetGitURL = 'https://github.com/OpenFLUID/MaterialDesign-SVG.git'
ExtSetDir = 'ext'
ExtSetVersion = 'svg'

SrcColor = 'black'
LightColor = '#F0F0F0'
DarkColor = '#555555'
GrayedColor = '#888888'

HiDPIEnabled = True

StandardSize = '32'
HiDPISize = '64'

GitProgram = 'git'
ConvertProgram = 'convert'
InkscapeProgram = 'inkscape'

ErroredFiles = list()


############################################################################
############################################################################


def addErroredFile(FilePath):
  ErroredFiles.append(FilePath)


############################################################################


def isCommandExec(Cmd):
  Status,Output = subprocess.getstatusoutput(Cmd)
  return (Status == 0)

############################################################################


def getSourcesPath(WorkPath):
  return os.path.join(WorkPath,'sources')


############################################################################


def getBuildPath(WorkPath):
  return os.path.join(WorkPath,'build')


############################################################################


def getTmpPath(WorkPath):
  return os.path.join(WorkPath,'tmp')


############################################################################


def cloneToDir(Name,URL,WorkPath,Dir):
  print('--> Cloning',Name,'set from',URL)
  
  ClonePath = os.path.join(getSourcesPath(WorkPath),Dir)
  
  if os.path.isdir(ClonePath):
    print('Destination directory',ClonePath,'already exists. Ignoring.')
    return

  Cmd = [GitProgram,'clone',URL,ClonePath]
  subprocess.call(Cmd)
 

 ############################################################################


def updateDir(Name,WorkPath,Dir):
  print('--> Updating',Name,'set')
  
  UpdatePath = os.path.join(getSourcesPath(WorkPath),Dir)
  
  if not os.path.isdir(UpdatePath):
    print('Sources directory',UpdatePath,'does not exists. Ignoring.')
    return

  Cmd = [GitProgram,'pull']
  subprocess.call(Cmd,cwd=UpdatePath)


############################################################################


def getDestFileName(IconName,Flavour,isHiDPI=False):
  DestFile = IconName+'_'+Flavour+'.png'
  if isHiDPI:
    DestFile = IconName+'_'+Flavour+'@2x.png'
  
  return DestFile


############################################################################


def getConvertCmd(SrcFilePath,DestFileName,DestPath,SrcColor,DestColor):
 
  DestFilePath = os.path.join(DestPath,DestFileName)  

  return [ConvertProgram,
#          '-density','2400',
#          '-background','none',
#          '-resize',Resize,          
          SrcFilePath,
          '-fuzz','75%',
          '-fill',DestColor,'-opaque',SrcColor,
#          '-filter','Lanczos',
          '+antialias',
          DestFilePath]


############################################################################


def convert(SrcFileNoExt,Path,IconName,Light,Dark,Grayed):

  SrcFile = SrcFileNoExt+'.svg'
  DestPath = getBuildPath(Path)
  
  print(SrcFile)

  TmpFile = os.path.join(getTmpPath(Path),IconName)+'.png'
  InkscapeCmd = [InkscapeProgram,'--export-filename',TmpFile,'-h',StandardSize,'-w',StandardSize,SrcFile]
  subprocess.call(InkscapeCmd)

  if HiDPIEnabled:
    TmpFileHiDPI = os.path.join(getTmpPath(Path),IconName)+'@2x.png'
    InkscapeCmd = [InkscapeProgram,'--export-filename',TmpFileHiDPI,'-h',HiDPISize,'-w',HiDPISize,SrcFile]
    subprocess.call(InkscapeCmd)


  if Light:
    #create light icon
    DestFile = getDestFileName(IconName,'light')
    print('  => Standard :',DestFile)
    ConvertCmd = getConvertCmd(TmpFile,DestFile,DestPath,SrcColor,LightColor)
    if subprocess.call(ConvertCmd) != 0:
      addErroredFile(DestFile)
    if HiDPIEnabled:
      DestFileHiDPI = getDestFileName(IconName,'light',True)
      print('  => HiDPI :',DestFileHiDPI)
      ConvertCmdHiDPI = getConvertCmd(TmpFileHiDPI,DestFileHiDPI,DestPath,SrcColor,LightColor)
      if subprocess.call(ConvertCmdHiDPI) != 0:
        addErroredFile(DestFileHiDPI)

  if Dark:
    #create dark icon
    DestFile = getDestFileName(IconName,'dark')
    print('  => Standard :',DestFile)
    ConvertCmd = getConvertCmd(TmpFile,DestFile,DestPath,SrcColor,DarkColor)    
    if subprocess.call(ConvertCmd) != 0:
      addErroredFile(DestFile)
    if HiDPIEnabled:
      DestFileHiDPI = getDestFileName(IconName,'dark',True)
      print('  => HiDPI :',DestFileHiDPI)
      ConvertCmdHiDPI = getConvertCmd(TmpFileHiDPI,DestFileHiDPI,DestPath,SrcColor,DarkColor)
      if subprocess.call(ConvertCmdHiDPI) != 0:
        addErroredFile(DestFileHiDPI)

  if Grayed:
    #create grayed icons
    DestFile = getDestFileName(IconName,'grayed')
    print('  => Standard :',DestFile)
    ConvertCmd = getConvertCmd(TmpFile,DestFile,DestPath,SrcColor,GrayedColor)
    if subprocess.call(ConvertCmd) != 0:
      addErroredFile(DestFile)
    if HiDPIEnabled:
      DestFileHiDPI = getDestFileName(IconName,'grayed',True)
      print('  => HiDPI :',DestFileHiDPI)
      ConvertCmdHiDPI = getConvertCmd(TmpFileHiDPI,DestFileHiDPI,DestPath,SrcColor,GrayedColor)
      if subprocess.call(ConvertCmdHiDPI) != 0:
        addErroredFile(DestFileHiDPI)



############################################################################
############################################################################


def checkExternalTools():
  print('=== Checking external tools')
  
  StatusStr = lambda s : 'OK' if s is True else 'Not found'

  GitStatus = isCommandExec('git --version')
  print('git :',StatusStr(GitStatus))

  ConvStatus = isCommandExec('convert --version')
  print('convert :',StatusStr(ConvStatus))

  InkStatus = isCommandExec('inkscape --version')
  print('inkscape :',StatusStr(InkStatus))

  return GitStatus and ConvStatus and InkStatus


############################################################################


def prepareWorkDirectory(Path):
  print('=== Preparing work directory')

  if not os.path.isdir(Path):
    os.makedirs(Path)

  if not os.path.isdir(getSourcesPath(Path)):
    os.makedirs(getSourcesPath(Path))

  shutil.rmtree(getBuildPath(Path), ignore_errors=True)
  if not os.path.isdir(getBuildPath(Path)):
    os.makedirs(getBuildPath(Path))

  shutil.rmtree(getTmpPath(Path), ignore_errors=True)
  if not os.path.isdir(getTmpPath(Path)):
    os.makedirs(getTmpPath(Path))


############################################################################


def retreiveSources(Path):
  print('=== Retreiving icons')
  cloneToDir("base",BaseSetGitURL,Path,BaseSetDir)
  cloneToDir("ext",ExtSetGitURL,Path,ExtSetDir)


############################################################################


def updateSources(Path):
  print('=== Updating icons')
  updateDir("base",Path,BaseSetDir)
  updateDir("ext",Path,ExtSetDir)


############################################################################


def buildIcons(Path):
  print('=== Converting icons')  

  for Key, Value in BaseSet.items():
    SrcFile = os.path.join(getSourcesPath(WorkPath),BaseSetDir,Value[0],BaseSetVersion,BaseSetPrefix+'_'+Value[1]+'_'+BaseSetSize)
    convert(SrcFile,Path,Key,Value[2],Value[3],Value[4])  

  for Key, Value in ExtSet.items():
    SrcFile = os.path.join(getSourcesPath(WorkPath),ExtSetDir,ExtSetVersion,Value[0])    
    convert(SrcFile,Path,Key,Value[1],Value[2],Value[3])


############################################################################
############################################################################


try:

  Parser = ArgumentParser(prog='oficons-extract')
  Parser.add_argument('WORKPATH', type=str, nargs=1,
                       help='Path where icons will be retreived and processed')
  Parser.add_argument('--with-update',required=False,action='store_true',
                      help='Update icons sources from repositories')
  
  Args = Parser.parse_args()
  Args = vars(Args)


  WorkPath = os.path.abspath(Args['WORKPATH'][0])
  print("Working path: ",WorkPath)

  if not checkExternalTools():
    sys.exit(1) 

  prepareWorkDirectory(WorkPath)

  retreiveSources(WorkPath)

  if Args['with_update'] :
    updateSources(WorkPath)

  buildIcons(WorkPath)

  print()

  if len(ErroredFiles) :
    print('Errors building files:')
    for F in ErroredFiles:
      print('-',F)
  else:
    print('No error detected')

except Exception as e:
  print("Error: ",e)
