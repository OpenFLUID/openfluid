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
#  Usage: oficons-extract-from-material --src-path=/path/to/icons/library --dest-path=/path/to/store/extracted/icons
#
#  This script requires the convert tool from ImageMagick (https://www.imagemagick.org/)
#  The src-path must have the following layout:
#    - "base" subdirectory : clone of the Google icons repository (https://github.com/google/material-design-icons/)
#    - "external" subdirectory : downloaded icons from https://materialdesignicons.com/. The icons must be
#      in PNG format, of a size of 48px and of white color
#  For each icon, it generates a dark and/or light and/or grayed icon for OpenFLUID UI, according to the Configuration
#  set in the BaseSet and ExtSet variables in the following lines of code
#
############################################################################


from argparse import ArgumentParser
import os
import shutil
import subprocess


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
BaseSet['go-up'] = ['hardware','keyboard_arrow_up',False,True,False]
BaseSet['go-down'] = ['hardware','keyboard_arrow_down',False,True,False]
BaseSet['modify'] = ['image','edit',False,True,False]
ExtSet['rename-column'] = ['page-layout-header',False,True,False]

BaseSet['configure'] = ['image','tune',True,True,True]
BaseSet['build'] = ['action','build',True,True,True]

BaseSet['magic'] = ['image','flash_on',False,True,False]
BaseSet['menu'] = ['navigation','menu',False,True,False]
BaseSet['market'] = ['action','shopping_cart',False,True,False]
BaseSet['link'] = ['content','link',False,True,False]

ExtSet['file-explorer'] = ['file-tree',False,True,False]
ExtSet['dev-wares'] = ['code-braces',False,True,False]
ExtSet['ghost2sim'] = ['json',False,True,False]
ExtSet['log-view'] = ['message-text-outline',False,True,False]
ExtSet['export'] = ['export',False,True,False]
ExtSet['linked'] = ['link-variant',False,True,False]
ExtSet['unlinked'] = ['link-variant-off',False,True,False]
ExtSet['generate-doc'] = ['book-open-page-variant',True,True,True]
ExtSet['view-doc'] = ['book-open-variant',True,True,True]


BaseSetVersion = '1x_web'
BaseSetPrefix = 'ic'
BaseSetSize = '48dp'

SrcColor = 'white'
LightColor = '#F0F0F0'
DarkColor = '#555555'
GrayedColor = '#888888'


############################################################################
############################################################################


def convertIcon(SrcFile,DestPath,IconName,Light,Dark,Grayed):

  if Light:
    #create light icon
    DestFile = os.path.join(DestPath,IconName+'_light.png')
    print SrcFile,'=>',DestFile
    ConvertCmd = ['convert',SrcFile,'-fuzz','75%','-fill',LightColor,'-opaque',SrcColor,DestFile]
    subprocess.call(ConvertCmd)

  if Dark:
    #create dark icon
    DestFile = os.path.join(DestPath,IconName+'_dark.png')
    print SrcFile,'=>',DestFile
    ConvertCmd = ['convert',SrcFile,'-fuzz','75%','-fill',DarkColor,'-opaque',SrcColor,DestFile]
    subprocess.call(ConvertCmd)

  if Grayed:
    #create grayed icons
    DestFile = os.path.join(DestPath,IconName+'_grayed.png')
    print SrcFile,'=>',DestFile
    ConvertCmd = ['convert',SrcFile,'-fuzz','75%','-fill',GrayedColor,'-opaque',SrcColor,DestFile]
    subprocess.call(ConvertCmd)


############################################################################
############################################################################


try:

  Parser = ArgumentParser(prog='oficons-extract-from-material')
  Parser.add_argument('--src-path',required=True)
  Parser.add_argument('--dest-path',required=True)

  Args = Parser.parse_args()
  Args = vars(Args)


  if not os.path.isdir(Args['dest_path']):
    os.makedirs(Args['dest_path'])


  BaseDir = os.path.join(Args['src_path'],"base")
  if os.path.isdir(BaseDir):
    for Key, Value in BaseSet.items():
      SrcFile = os.path.join(BaseDir,Value[0],BaseSetVersion,BaseSetPrefix+'_'+Value[1]+'_'+SrcColor+'_'+BaseSetSize+'.png')
      convertIcon(SrcFile,Args['dest_path'],Key,Value[2],Value[3],Value[4])

  ExtDir = os.path.join(Args['src_path'],"external")
  if os.path.isdir(ExtDir):
    for Key, Value in ExtSet.items():
      SrcFile = os.path.join(ExtDir,Value[0]+'.png')
      convertIcon(SrcFile,Args['dest_path'],Key,Value[1],Value[2],Value[3])


except Exception as e:
  print "Error:",
  print e
