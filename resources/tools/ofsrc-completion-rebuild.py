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
#  This script extracts informations from API comments to generate
#  c++ code used for automatic completion in wares development
#
#  Usage: ofsrc-completion-rebuild.py /path/to/openfluid/sources
#         ofsrc-completion-rebuild.py --help
#
#  This script will read source files, searching for json-formatted blocks
#  located between '@cond  OpenFLUID:completion' and '@endcond' Doxygen tags
#  
#  examples:
#  @cond OpenFLUID:completion
#  {
#    "contexts" : ["SIMULATOR"],
#    "menupath" : ["Compute code", "Parameters"],
#    "title" : "Get simulator parameter",
#    "text" : "OPENFLUID_GetSimulatorParameter(Params,%%SEL_START%%ParamName%%SEL_END%%,Val)"
#  }
#  @endcond
#
#  @cond OpenFLUID:completion
#  {
#    "contexts" : ["ANYWARE"],
#    "menupath" : ["Compute code", "Loops"],
#    "title" : "Loop on an events collection",
#    "text" : ["OPENFLUID_EVENT_COLLECTION_LOOP(%%SEL_START%%EvList%%SEL_END%%,Event)",
#              "\n%%INDENT%%{\n%%INDENT%%\n%%INDENT%%}"]
#  }
#  @endcond
#
#  JSON fields are:
#    - contexts : giving one or more contexts where the completion is available. 
#      Possible values are 'SIMULATOR','OBSERVER','BUILDEREXT', and 'ANYWARE' for all contexts
#    - menupath : list of menu entries giving the path where the completion item is accessible
#    - title : title of the completion item
#    - text : the text inserted when the completion is activated. In case of long text, 
#      it can be splitted into a list of texts 
#      (which is automatically joined at runtime without adding any caracter)
#
############################################################################


from argparse import ArgumentParser
import os
import re
import sys
import json


assert sys.version_info >= (3, 5)


############################################################################
############################################################################


class CompletionBuilder:

    def __init__(self, src_rootpath):
        self._SRC_OPENFLUID_DIR = 'src/openfluid'
        self._SRC_OPENFLUID_ROOTPATH = os.path.join(
            src_rootpath, self._SRC_OPENFLUID_DIR)
        self._GENERATED_FILE_NAME = "CompletionProviderAPIData.cpp"
        self._GENERATED_FILE_PATH = os.path.join(
            self._SRC_OPENFLUID_ROOTPATH, "ui", "waresdev", self._GENERATED_FILE_NAME)
        self._GENERATED_RULE_PATTERN = """\
// extracted from {}
addRule(
  Rule(
    "cpp",Origin::OPENFLUID,{},
    {{
{}
    }},
    CompletionProvider::tr("{}"),
    "{}"
  )
);

"""

        self._GENERATED_HEADER_PATTERN = """\
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
  @file {}

  @author OpenFLUID team <contact@openfluid-project.org>

  Automatically generated file, to be included in CompletionProvider class at build time
*/


// OpenFLUID:stylecheck:!llen


"""

        self._founddocs = list()
        self._filelist = list()

        for dirname, dirnames, filenames in os.walk(self._SRC_OPENFLUID_ROOTPATH):
            for filename in filenames:
                relfilename = os.path.relpath(os.path.join(
                    dirname, filename), self._SRC_OPENFLUID_ROOTPATH)
                if not relfilename.startswith('.') and not relfilename.startswith('_'):
                    if (relfilename.endswith('.cpp') or relfilename.endswith('.hpp') or
                            relfilename.endswith('.hpp.in')):
                        self._filelist.append(relfilename)

        self._filelist.sort()


############################################################################


    @staticmethod
    def _parseAPIDoc(filename, text):
        parsed_doc = dict()

        print('{} => '.format(filename), end='')

        try:
            parsed_doc = json.loads(text)
            print('{} {}'.format(parsed_doc["title"], parsed_doc["contexts"]))
        except:
            print("parsing error {}".format(text))
            sys.exit(127)

        return parsed_doc


############################################################################


    def _parseFiles(self):
        for filename in self._filelist:
            filecontent = open(os.path.join(
                self._SRC_OPENFLUID_ROOTPATH, filename), 'r').read()

            apidocs = re.findall(
                r'@cond\s*OpenFLUID:completion(.*?)@endcond', filecontent, re.DOTALL)

            for doc in apidocs:
                parsed_doc = CompletionBuilder._parseAPIDoc(filename, doc)
                if len(parsed_doc):
                    parsed_doc["file"] = filename
                    self._founddocs.append(parsed_doc)


############################################################################


    def _writeCompletionData(self):
        genfile = open(self._GENERATED_FILE_PATH, "w")

        genfile.write(self._GENERATED_HEADER_PATTERN.format(
            self._GENERATED_FILE_NAME))

        for doc in self._founddocs:

            menupathlist = list()
            for item in doc["menupath"]:
                itemstr = item.encode(
                    'unicode-escape').decode().replace('"', '\\"')
                menupathlist.append(
                    """      CompletionProvider::tr("{}")""".format(itemstr))

            title = doc["title"].encode(
                'unicode-escape').decode().replace('"', '\\"')

            text = doc["text"]
            if type(text) == list:
                text = "".join(text)
            text = text.encode(
                'unicode-escape').decode().replace('"', '\\"')
            text = text.replace(
                "%%CMT_START%%", "/*").replace("%%CMT_END%%", "*/")

            genfile.write(self._GENERATED_RULE_PATTERN.format(doc["file"],
                                                              " | ".join(
                                                                  ["ContextFlags::"+ctxt for ctxt in doc["contexts"]]),
                                                              ",\n".join(menupathlist),title,text))
        genfile.close()


############################################################################


    def run(self):

        self._parseFiles()
        self._writeCompletionData()


############################################################################
############################################################################


try:

    Parser = ArgumentParser(prog='ofsrc-rebuild-completion')
    Parser.add_argument('path', default='')

    Args = Parser.parse_args()
    Args = vars(Args)

    Builder = CompletionBuilder(Args['path'])
    Builder.run()

except Exception as e:
    print("Error:", e)
    sys.exit(127)
