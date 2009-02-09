#!/usr/bin/env python

#
# ofefunc2doc
#
# Tool for extracting information from OpenFLUID-Engine simulation,
# and generating documentation as PDF and/or HTML 
#
# Written by Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#   with the help of Xavier LOUCHART  <louchart@supagro.inra.fr>
#


from optparse import OptionParser
from string import Template
import re
import subprocess
import os
import sys


class OFEFunc2Latex:

  BEGIN_SIGNATURE_TAG = "BEGIN_SIGNATURE_HOOK"
  END_SIGNATURE_TAG = "END_SIGNATURE_HOOK"

  BEGIN_FUNCDOC_TAG = "<func2doc>"
  END_FUNCDOC_TAG = "</func2doc>"

  PDFBUILDER = '/usr/bin/pdflatex'
  CPREPROCESSOR = '/usr/bin/gcc'
  BIBTEX = '/usr/bin/bibtex'
  HTMLBUILDER = '/usr/bin/latex2html'

  THE_TITLE = ""

  THE_AUTHOR = "NO\_AUTHOR"

  THE_DESCRIPTION = "NO\_DESC"

  THE_NAME = "NO\_NAME"

  THE_DOMAIN = "NO\_DOMAIN"  

  THE_FUNCTIONDOC = "NO\_FUNCDOC" 

  THE_HANDLED_DATA = ""

  THE_LATEX_NEWCOMMANDS = dict()

  THE_INVARS = []
  THE_OUTVARS = []
  THE_MODVARS = []
  THE_PROPS = []
  THE_FPARAMS = []
  THE_EVENTS = []
  THE_EXTRAFILES = []


  LATEX_TEMPLATE_FILE = "/usr/@OFEFUNC2DOC_TPL_INSTALL_PATH@/ofefunc2doc_tpl.tex"
  LATEX_TEMPLATE = ""

  OUTPUT_DIR="./"

  LATEXFILE = ""
  
  SOURCEFILE = "" 
  
  TMPFILE = "/tmp/ofefunc2doc.tmp"
  
# ====================================================  
  
  def __init__(self,options,args):
    self.options = options
    self.args = args
    self.OKLatex = 0

# ====================================================
  
  #
  # backslashes LateX special chars
  #
  def toLateXFriendly(self,InputStr):
      TmpStr = InputStr
      TmpStr = TmpStr.replace(r'$',r'\$')
      TmpStr = TmpStr.replace(r'_',r'\_')
      TmpStr = TmpStr.replace(r'&',r'\&')
      TmpStr = TmpStr.replace(r'%',r'\%')
      TmpStr = TmpStr.replace(r'{',r'\{')
      TmpStr = TmpStr.replace(r'}',r'\}')
      TmpStr = TmpStr.replace(r'~',r'$\sim$')
      TmpStr = TmpStr.replace(r'\\',r'$\backslash$')
#      print TmpStr
      return TmpStr
  

# ====================================================

  #
  # extract signature (located between BEGIN_SIGNATURE_HOOK and END_SIGNATURE_HOOK)
  # and split it into declacrations (beginning with DECLARE_)  
  #
  def extractSignatureLines(self,Content):
    TmpPart = Content.partition(self.BEGIN_SIGNATURE_TAG)
    TmpPart = TmpPart[2].partition(self.END_SIGNATURE_TAG)
    
    SignatureRest = TmpPart[0]
    SignatureLines = []
          
    while len(TmpPart[2]) != 0 :
      TmpPart = SignatureRest.partition("DECLARE_")
      SignatureLines.append(TmpPart[0])
      SignatureRest = TmpPart[2]
    
    return SignatureLines


# ====================================================

  #
  # extract function documentation (located between <func2doc> and </func2doc>)  
  #
  def extractFunctionDocTxt(self,Content):
    TmpPart = Content.partition(self.BEGIN_FUNCDOC_TAG)
    TmpPart = TmpPart[2].partition(self.END_FUNCDOC_TAG)
    return TmpPart[0]


# ====================================================

  #
  # processes all lines of the signature and extract definition strings 
  #
  def processSignatureLines(self,Lines) :

    for Line in Lines : 
      Line = Line.replace("\n","");
      Line = Line.strip(); 
      p = re.compile('wxT([*]*)', re.VERBOSE)
      Line = p.sub('\1',Line)
      #print Line
      
      if (Line.startswith("SIGNATURE_ID") == True) :
        p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
        m = p.search(Line)        
        self.THE_TITLE = self.toLateXFriendly(str(m.group()[1:len(m.group())-1]))
        self.THE_LATEX_NEWCOMMANDS['funcID'] = self.THE_TITLE

      elif (Line.startswith("SIGNATURE_DESCRIPTION") == True) :
        p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
        m = p.search(Line)        
        self.THE_DESCRIPTION = self.toLateXFriendly(str(m.group()[1:len(m.group())-1]))

      elif (Line.startswith("SIGNATURE_NAME") == True) :
        p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
        m = p.search(Line)        
        self.THE_NAME = self.toLateXFriendly(str(m.group()[1:len(m.group())-1]))
        self.THE_LATEX_NEWCOMMANDS['funcNAME'] = self.THE_NAME


      elif (Line.startswith("SIGNATURE_DOMAIN") == True) :
        p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
        m = p.search(Line)        
        self.THE_DOMAIN = self.toLateXFriendly(str(m.group()[1:len(m.group())-1]))

      elif (Line.startswith("SIGNATURE_AUTHORNAME") == True) :
        p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
        m = p.search(Line)        
        self.THE_AUTHOR = m.group()[1:len(m.group())-1]

      elif (Line.startswith("FUNCTION_PARAM") == True) :
        p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
        iterator = p.finditer(Line)
        TmpList = []
        for match in iterator:
          TmpList.append(self.toLateXFriendly(str(match.group()[1:len(match.group())-1])))
        self.THE_FPARAMS.append(TmpList)
      
      else :        
        for VarType in ['SU_REQUIRED_VAR','SU_USED_VAR','SU_REQUIRED_PREVVAR','SU_USED_PREVVAR',
                        'RS_REQUIRED_VAR','RS_USED_VAR','RS_REQUIRED_PREVVAR','RS_USED_PREVVAR',
                        'GU_REQUIRED_VAR','GU_USED_VAR','GU_REQUIRED_PREVVAR','GU_USED_PREVVAR'] :
          if (Line.startswith(VarType) == True) :
            p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
            iterator = p.finditer(Line)
            TmpSplit = VarType.split('_')
            TmpList = [TmpSplit[0],TmpSplit[1]]
            for match in iterator:
              TmpList.append(self.toLateXFriendly(str(match.group()[1:len(match.group())-1])))
            self.THE_INVARS.append(TmpList)

        for VarType in ['SU_PRODUCED_VAR','SU_UPDATED_VAR',
                        'RS_PRODUCED_VAR','RS_UPDATED_VAR',
                        'GU_PRODUCED_VAR','GU_UPDATED_VAR']:
          if (Line.startswith(VarType) == True) :
            p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
            iterator = p.finditer(Line)
            TmpSplit = VarType.split('_')
            TmpList = [TmpSplit[0],TmpSplit[1]]
            for match in iterator:
              TmpList.append(self.toLateXFriendly(str(match.group()[1:len(match.group())-1])))
            self.THE_OUTVARS.append(TmpList)

        for VarType in ['SU_REQUIRED_PROPERTY','SU_USED_PROPERTY','SU_REQUIRED_INICOND','SU_USED_INICOND',
                        'RS_REQUIRED_PROPERTY','RS_USED_PROPERTY','RS_REQUIRED_INICOND','RS_USED_INICOND',
                        'GU_REQUIRED_PROPERTY','GU_USED_PROPERTY','GU_REQUIRED_INICOND','GU_USED_INICOND'] :
          if (Line.startswith(VarType) == True) :
            p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
            iterator = p.finditer(Line)
            TmpSplit = VarType.split('_')
            TmpList = [TmpSplit[0],TmpSplit[1]]
            for match in iterator:
              TmpList.append(self.toLateXFriendly(str(match.group()[1:len(match.group())-1])))
            self.THE_PROPS.append(TmpList)

        for VarType in ['USED_SU_EVENTS','USED_RS_EVENTS','USED_GU_EVENTS'] :
          if (Line.startswith(VarType) == True) :
            TmpSplit = VarType.split('_')
            self.THE_EVENTS.append(TmpSplit[1])

        for VarType in ['REQUIRED_EXTRAFILE','USED_EXTRAFILE']:
          if (Line.startswith(VarType) == True) :
            p = re.compile(r'"([^"\\]|\\.)*"', re.VERBOSE)        
            iterator = p.finditer(Line)
            TmpSplit = VarType.split('_')
            TmpList = [TmpSplit[0]]
            for match in iterator:
              TmpList.append(self.toLateXFriendly(str(match.group()[1:len(match.group())-1])))
            self.THE_EXTRAFILES.append(TmpList)

# ====================================================

  def addBeginHandledDataZone(self,Title,ColsFormat) :
    self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r"\begin{center}\begin{small}"+"\n"+r"\begin{tabularx}{\linewidth}{"+ColsFormat+r"}"+"\n"
    self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r"\multicolumn{"+str(len(ColsFormat))+r"}{l}{\begin{large}\textbf{"+Title+r"}\end{large}}\\"+"\n"+r"\hline"+"\n"
  
  def addEndHandledDataZone(self) :
    self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r"\hline"+"\n"+r"\end{tabularx}"+"\n"+r"\end{small}\end{center}"+"\n"+r"\smallskip"+"\n\n"


# ====================================================

  def runCPreprocessor(self):
    print "Preprocessing input file ...",
    sys.stdout.flush()
    os.system(self.CPREPROCESSOR + ' -E -fdirectives-only -nostdinc -nostdinc++ -undef -fpreprocessed '+ self.SOURCEFILE + ' > ' + self.TMPFILE + ' 2>/dev/null')
    print "done"

# ====================================================


  def sortBy(self,nlist, n):
    nlist.sort(lambda x,y:cmp(str(x[n]).lower(), str(y[n]).lower()))


# ====================================================

  #
  # generate latex file with collexted information from the source file
  # uses signature and documentation extractions  
  #
  def generateLatex(self, FunctionDoc, SignatureLines):

    print "Generating LateX documentation from input file ...",

    self.processSignatureLines(SignatureLines)

    self.THE_FUNCTIONDOC = FunctionDoc

    self.sortBy(self.THE_INVARS,2)
    self.sortBy(self.THE_OUTVARS,2)
    self.sortBy(self.THE_PROPS,2)
    self.sortBy(self.THE_FPARAMS,0)
    self.sortBy(self.THE_EXTRAFILES,0)


    
    if len(self.THE_INVARS) != 0 :
      self.addBeginHandledDataZone("Required or used variable","lllXr")
      for TmpVars in self.THE_INVARS :
        self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r'\texttt{' + TmpVars[2] + '}&' + TmpVars[1] + '&' + TmpVars[0] + '&' + TmpVars[3] + '&$' + TmpVars[4] + r'$\\' + '\n'
      self.addEndHandledDataZone()
             
    if len(self.THE_OUTVARS) != 0 :
      self.addBeginHandledDataZone("Produced or modified variable","lllXr")      
      for TmpVars in self.THE_OUTVARS :
        self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r'\texttt{' + TmpVars[2] + '}&' + TmpVars[1] + '&' + TmpVars[0] + '&' + TmpVars[3] + '&$' + TmpVars[4] + r'$\\' + '\n'
      self.addEndHandledDataZone()

    if len(self.THE_PROPS) != 0 :
      self.addBeginHandledDataZone("Properties and initial conditions","lllXr")      
      for TmpVars in self.THE_PROPS :
        self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r'\texttt{' + TmpVars[2] + '}&' + TmpVars[1] + '&' + TmpVars[0] + '&' + TmpVars[3] + '&$' + TmpVars[4] + r'$\\' + '\n'
      self.addEndHandledDataZone()

    if len(self.THE_FPARAMS) != 0 :
      self.addBeginHandledDataZone("Function parameters","lXr")      
      for TmpVars in self.THE_FPARAMS :
        self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + r'\texttt{' + TmpVars[0] + '}&' + TmpVars[1] + '&$' + TmpVars[2] + r'$\\' + '\n'
      self.addEndHandledDataZone()

    if len(self.THE_EVENTS) != 0 :
      self.addBeginHandledDataZone("Distributed events","l")      
      for TmpVars in self.THE_EVENTS :
        self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + TmpVars + r'\\' + '\n'
      self.addEndHandledDataZone()

    if len(self.THE_EXTRAFILES) != 0 :
      self.addBeginHandledDataZone("Extra files","lX")      
      for TmpVars in self.THE_EXTRAFILES :
        self.THE_HANDLED_DATA = self.THE_HANDLED_DATA + TmpVars[0] + '&' + TmpVars[1] + r'\\' + '\n'
      self.addEndHandledDataZone()


    NewCommands = ''
    
    for Cmd, Val in self.THE_LATEX_NEWCOMMANDS.iteritems() : 
      NewCommands = NewCommands + '\\newcommand{\\' + str(Cmd) + r'}{'+ str(Val) +r'}' + '\n' 


    FileHandle = open(self.LATEX_TEMPLATE_FILE,"r")
    self.LATEX_TEMPLATE = FileHandle.read();
    FileHandle.close()


    Tpl = Template(self.LATEX_TEMPLATE)
    LatexContent = Tpl.safe_substitute(newcommands=NewCommands,title=self.THE_TITLE, author=self.THE_AUTHOR,
                                       description=self.THE_DESCRIPTION, domain=self.THE_DOMAIN,
                                       name=self.THE_NAME, funcdoc=self.THE_FUNCTIONDOC, data=self.THE_HANDLED_DATA)

    self.LATEXFILE = str(self.OUTPUT_DIR)+'/'+str(self.THE_TITLE)+".tex"
    FileHandle = open(self.LATEXFILE,"w")
    FileHandle.write(LatexContent);
    FileHandle.close()


    self.OKLatex = 1
    print "done"

# ====================================================

  def buildPDF(self):
    print "Building PDF... ",
    print "first pass... ",
    sys.stdout.flush()        
    os.system(self.PDFBUILDER + ' -interaction=batchmode -output-directory='+self.OUTPUT_DIR+' '+ self.LATEXFILE + ' > /dev/null')
    print "bibliography and references... ",
    sys.stdout.flush()        
    os.system(self.BIBTEX + ' '+ str(self.OUTPUT_DIR)+'/'+str(self.THE_TITLE)  + ' > /dev/null')    
    print "second pass... ",
    sys.stdout.flush()        
    os.system(self.PDFBUILDER + ' -interaction=batchmode -output-directory='+self.OUTPUT_DIR+' '+ self.LATEXFILE + ' > /dev/null')    
    print "done"

# ====================================================

  def buildHTML(self):
    print "Building HTML ...",
    sys.stdout.flush()    
    os.system(self.HTMLBUILDER + ' -dir='+self.OUTPUT_DIR+' '+ self.LATEXFILE)        
    print "done"


# ====================================================
  
  def isToolExists(selfself,ToolFullPath):
    return os.path.isfile(ToolFullPath)

# ====================================================

  def run(self):

    if len(args) != 1 :
      print "Missing input file"
      return

    self.SOURCEFILE = self.args[0]  

    if os.path.isfile(self.SOURCEFILE) != True : 
      print "Input file " + self.SOURCEFILE + " not found"
      return

    if self.options.outputdir :
      self.OUTPUT_DIR = self.options.outputdir 

    if self.options.tplfilename :
      self.LATEX_TEMPLATE_FILE = self.options.tplfilename 

#    if self.options.Cpreproc :
#      if self.isToolExists(self.CPREPROCESSOR) : 
#        self.runCPreprocessor()
#      else :
#        print "Preprocessing skipped (could not find " + self.CPREPROCESSOR + ")"   


    FileHandle = open(self.SOURCEFILE,"r")
    FileContent = FileHandle.read();
    FileHandle.close()

    FunctionDoc = self.extractFunctionDocTxt(FileContent)

    self.runCPreprocessor()

    FileHandle = open(self.TMPFILE,"r")
    FileContent = FileHandle.read();
    FileHandle.close()

    SignatureLines =  self.extractSignatureLines(FileContent)

    
    self.generateLatex(FunctionDoc,SignatureLines)

    
    if  self.OKLatex == 1 :
    
      if self.options.pdf : 
        if self.isToolExists(self.PDFBUILDER) and self.isToolExists(self.BIBTEX):
          self.buildPDF()
        else: 
          print "PDF build skipped (could not find " + self.PDFBUILDER + " or " + self.BIBTEX + ")"          

      if self.options.html : 
        if self.isToolExists(self.HTMLBUILDER):
          self.buildHTML()
        else: 
          print "HTML build skipped (could not find " + self.HTMLBUILDER + ")"          



# ====================================================
# ==================================================== 




parser = OptionParser()

#parser.add_option("-c", "--C-preproc", action="store_true", dest="Cpreproc", help="preprocess file using C preprocessor")
parser.add_option("-m", "--build-html", action="store_true", dest="html", help="build HTML output")
parser.add_option("-o", "--output-dir", action="store", type="string", dest="outputdir",  help="output directory")
parser.add_option("-p", "--build-pdf", action="store_true", dest="pdf", help="build pdf file")
parser.add_option("-t", "--template-file", action="store", type="string", dest="tplfilename",  help="template file")

(options, args) = parser.parse_args()

generator = OFEFunc2Latex(options,args)

generator.run()



