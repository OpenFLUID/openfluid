#!/usr/bin/env python

#
# ofeconvert_13-14
#
# Tool for converting input files for OpenFLUID-Engine, from version 1.3.x to version 1.4.x
#
# Written by Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
#



from optparse import OptionParser
import csv
import os
import shutil

# ====================================================
# ==================================================== 

GUSurfaces = {}



# ====================================================
# ==================================================== 

def convertSUdefs(Options):
  
  InputFilePath = Options.inputdir + "/SUdefs.dat"
  OutputFilePath = Options.outputdir + "/SU.ddef.xml"  
  DataOutputFilePath = Options.outputdir + "/SUgeometry.ddata.xml"     

  OutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <definition>\n"

  DataOutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <inputdata unitclass=\"SU\">\n      <columns order=\"area;slope;flowdist\" />\n      <data>\n\n"


  if os.path.exists(InputFilePath):
   
    print "Converting " + InputFilePath + " to " + OutputFilePath + " and " + DataOutputFilePath + " ... ",
    
    SUdefFile = csv.reader(open(InputFilePath), delimiter='\t')
    for row in SUdefFile:
    
      if (len(row) > 0):
        if (row[0].strip().startswith('%') != True):
          OutputFileContent = OutputFileContent + "      <unit class=\"SU\" ID=\""+ row[0].strip() +"\" pcsorder=\""+ row[7].strip() +"\">\n" 

          # processing "to RS" or "to SU" topology 
          if (int(row[4].strip()) >= 0):
            if ((row[3].strip() == "R") | (row[3].strip() == "r")):
              OutputFileContent = OutputFileContent + "        <to class=\"RS\" ID=\""+ row[4].strip() +"\" />\n"
            else:                          
              OutputFileContent = OutputFileContent + "        <to class=\"SU\" ID=\""+ row[4].strip() +"\" />\n"

          # processing "to GU" topology
          if (int(row[6].strip()) >= 0):
            CurrentSurface = float(row[1].strip())
            CurrentGU = int(row[6].strip())
            if GUSurfaces.has_key(CurrentGU):
              GUSurfaces[CurrentGU] = GUSurfaces[CurrentGU] + CurrentSurface 
            else:
              GUSurfaces[CurrentGU] = CurrentSurface
              
            OutputFileContent = OutputFileContent + "        <to class=\"GU\" ID=\""+ row[6].strip() +"\" />\n"


          OutputFileContent = OutputFileContent + "      </unit>\n"

          DataOutputFileContent = DataOutputFileContent + row[0].strip() + "\t" + row[1].strip() + "\t" + row[2].strip() + "\t" + row[5].strip() + "\n";


    OutputFileContent = OutputFileContent + "    </definition>\n  </domain>\n</openfluid>\n\n\n"
  
    DataOutputFileContent = DataOutputFileContent + "\n      </data>\n    </inputdata>\n  </domain>\n</openfluid>\n\n\n"
  
    #print OutputFileContent
    outfile = open(OutputFilePath, "w")
    outfile.write(OutputFileContent)
    outfile.close()

    #print DataOutputFileContent
    outfile = open(DataOutputFilePath, "w")
    outfile.write(DataOutputFileContent)
    outfile.close()
  
    print "[Done]"
  

  return

# ====================================================
# ==================================================== 


def convertRSdefs(Options):
  
  InputFilePath = Options.inputdir + "/RSdefs.dat"
  OutputFilePath = Options.outputdir + "/RS.ddef.xml"
  DataOutputFilePath = Options.outputdir + "/RSgeometry.ddata.xml"     

  OutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <definition>\n"

  DataOutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <inputdata unitclass=\"RS\">\n      <columns order=\"length;width;height;slope\" />\n      <data>\n\n"

  if os.path.exists(InputFilePath):

    print "Converting " + InputFilePath + " to " + OutputFilePath + " and " + DataOutputFilePath + " ... ",    
    
    SUdefFile = csv.reader(open(InputFilePath), delimiter='\t')
    for row in SUdefFile:
    
      if (len(row) > 0):
        if (row[0].strip().startswith('%') != True):
          OutputFileContent = OutputFileContent + "      <unit class=\"RS\" ID=\""+ row[0].strip() +"\" pcsorder=\""+ row[9].strip() +"\">\n" 

          # processing "to RS" topology 
          if (int(row[3].strip()) >= 0):
            OutputFileContent = OutputFileContent + "        <to class=\"RS\" ID=\""+ row[3].strip() +"\" />\n"

          # processing "to GU" topology
          if (int(row[8].strip()) >= 0):
            OutputFileContent = OutputFileContent + "        <to class=\"GU\" ID=\""+ row[8].strip() +"\" />\n"


          OutputFileContent = OutputFileContent + "      </unit>\n"

          DataOutputFileContent = DataOutputFileContent + row[0].strip() + "\t" + row[4].strip() + "\t" + row[5].strip() + "\t" + row[6].strip() + "\t" + row[7].strip() + "\n";


    OutputFileContent = OutputFileContent + "    </definition>\n  </domain>\n</openfluid>\n\n\n"
  
    DataOutputFileContent = DataOutputFileContent + "\n      </data>\n    </inputdata>\n  </domain>\n</openfluid>\n\n\n"
  
    #print OutputFileContent
    outfile = open(OutputFilePath, "w")
    outfile.write(OutputFileContent)
    outfile.close()

    #print DataOutputFileContent
    outfile = open(DataOutputFilePath, "w")
    outfile.write(DataOutputFileContent)
    outfile.close()

  
    print "[Done]"
  

  return

# ====================================================
# ==================================================== 


def convertGUdefs(Options):
  
  InputFilePath = Options.inputdir + "/GUdefs.dat"
  OutputFilePath = Options.outputdir + "/GU.ddef.xml"
  DataOutputFilePath = Options.outputdir + "/GUgeometry.ddata.xml"   

  OutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <definition>\n"

  DataOutputFileContent = "<?xml version=\"1.0\" standalone=\"yes\"?>\n<openfluid>\n  <domain>\n    <inputdata unitclass=\"GU\">\n      <columns order=\"area;substlevel\" />\n      <data>\n\n"


  if os.path.exists(InputFilePath):
    
    print "Converting " + InputFilePath + " to " + OutputFilePath + " and " + DataOutputFilePath + " ... ",
    
    SUdefFile = csv.reader(open(InputFilePath), delimiter='\t')
    for row in SUdefFile:
    
      if (len(row) > 0):
        if (row[0].strip().startswith('%') != True):
          OutputFileContent = OutputFileContent + "      <unit class=\"GU\" ID=\""+ row[0].strip() +"\" pcsorder=\""+ row[3].strip() +"\">\n" 
          
          CurrentGUsurf = 0;
          if GUSurfaces.has_key(int(row[0])) == True:
            CurrentGUsurf = GUSurfaces[int(row[0])]
          
          DataOutputFileContent = DataOutputFileContent + row[0].strip() + "\t" + str(CurrentGUsurf) + "\t" + row[1].strip() + "\n";

          # processing "to GU" topology
          if (int(row[2].strip()) >= 0):
            OutputFileContent = OutputFileContent + "        <to class=\"GU\" ID=\""+ row[2].strip() +"\" />\n"


          OutputFileContent = OutputFileContent + "      </unit>\n"

    OutputFileContent = OutputFileContent + "    </definition>\n  </domain>\n</openfluid>\n\n\n"
  
    DataOutputFileContent = DataOutputFileContent + "\n      </data>\n    </inputdata>\n  </domain>\n</openfluid>\n\n\n"
  
    #print OutputFileContent
    outfile = open(OutputFilePath, "w")
    outfile.write(OutputFileContent)
    outfile.close()

    #print DataOutputFileContent
    outfile = open(DataOutputFilePath, "w")
    outfile.write(DataOutputFileContent)
    outfile.close()

  
    print "[Done]"
  

  return

# ====================================================
# ==================================================== 

def copyFiles(Options):

  CurrentFile = "/model.xml"
  if os.path.exists(Options.inputdir + CurrentFile):
    print "Copying " + Options.inputdir + CurrentFile + " to " + Options.outputdir + CurrentFile + " ... ",
    shutil.copyfile(Options.inputdir + CurrentFile,Options.outputdir + CurrentFile)
    print "[Done]"


  CurrentFile = "/run.xml"
  if os.path.exists(Options.inputdir + CurrentFile):
    print "Copying " + Options.inputdir + CurrentFile + " to " + Options.outputdir + CurrentFile + " ... ",
    shutil.copyfile(Options.inputdir + CurrentFile,Options.outputdir + CurrentFile)
    print "[Done]"


  AllFiles = os.listdir(Options.inputdir)
  
  for CurrentFile in AllFiles:
    if (CurrentFile.endswith(".events.xml") == True):
      CurrentFile = "/" + CurrentFile
      print "Copying " + Options.inputdir + CurrentFile + " to " + Options.outputdir + CurrentFile + " ... ",
      shutil.copyfile(Options.inputdir + CurrentFile,Options.outputdir + CurrentFile)
      print "[Done]"
  
  return

# ====================================================
# ==================================================== 

def convertOutputConfig(Options):

  CurrentFile = "/output.xml"
  if os.path.exists(Options.inputdir + CurrentFile):
    print "Converting " + Options.inputdir + CurrentFile + " to " + Options.outputdir + CurrentFile + " ... ",
    InFileHandle = open(Options.inputdir + CurrentFile, 'r')      
    CurrentContent = InFileHandle.read()
    InFileHandle.close()

    CurrentContent = CurrentContent.replace(r"<autooutfiles","<files");
    CurrentContent = CurrentContent.replace(r"</autooutfiles","</files");

    CurrentContent = CurrentContent.replace(r"<SUout","<set unitsclass=\"SU\"");      
    CurrentContent = CurrentContent.replace(r"</SUout","</set");      

    CurrentContent = CurrentContent.replace(r"<RSout","<set unitsclass=\"RS\"");      
    CurrentContent = CurrentContent.replace(r"</RSout","</set");      

    CurrentContent = CurrentContent.replace(r"<GUout","<set unitsclass=\"GU\"");      
    CurrentContent = CurrentContent.replace(r"</GUout","</set");      

    CurrentContent = CurrentContent.replace(r"filesuffix=","name=");          
    CurrentContent = CurrentContent.replace(r"selection=","unitsIDs=");      
      
      
    #print CurrentContent
 
    OutFileHandle = open(Options.outputdir + CurrentFile, 'w')      
    OutFileHandle.write(CurrentContent)
    OutFileHandle.close()
    print "[Done]"

  return

# ====================================================
# ==================================================== 

def convertInputData(Options):
  AllFiles = os.listdir(Options.inputdir)
  
  for CurrentFile in AllFiles:
    if (CurrentFile.endswith(".ddata.xml") == True):
      CurrentFile = "/" + CurrentFile
      print "Converting " + Options.inputdir + CurrentFile + " to " + Options.outputdir + CurrentFile + " ... ",
      InFileHandle = open(Options.inputdir + CurrentFile, 'r')      
      CurrentContent = InFileHandle.read()
      InFileHandle.close()

      CurrentContent = CurrentContent.replace(r"<openfluid>","<openfluid>\n  <domain>");
      CurrentContent = CurrentContent.replace(r"</openfluid>","  </domain>\n</openfluid>");
      
      CurrentContent = CurrentContent.replace(r"<distridata","  <inputdata");
      CurrentContent = CurrentContent.replace(r"</distridata","  </inputdata");
      
      CurrentContent = CurrentContent.replace("datacat=\"ini\"","");
      CurrentContent = CurrentContent.replace("datacat=\"param\"","");

      CurrentContent = CurrentContent.replace(r"<columns","  <columns");
      CurrentContent = CurrentContent.replace(r"</columns","  </columns");

      CurrentContent = CurrentContent.replace(r"<data","  <data");
      CurrentContent = CurrentContent.replace(r"</data","  </data");
      
      #print CurrentContent
 
      OutFileHandle = open(Options.outputdir + CurrentFile, 'w')      
      OutFileHandle.write(CurrentContent)
      OutFileHandle.close()
      print "[Done]"

  return

# ====================================================
# ==================================================== 


def doConversion(Options):

  # create output dir if non existent

  if (os.path.isdir(Options.outputdir) != True):
    os.makedirs(Options.outputdir)

  convertSUdefs(Options)
  convertRSdefs(Options)
  convertGUdefs(Options)
  convertInputData(Options)
  convertOutputConfig(Options)
  copyFiles(Options)
  
  return

# ====================================================
# ==================================================== 



parser = OptionParser()

parser.add_option("-i", "--input-dir", action="store", type="string", dest="inputdir",  help="input directory", default=".")
parser.add_option("-o", "--output-dir", action="store", type="string", dest="outputdir",  help="output directory", default=".")


(options, args) = parser.parse_args()

# remove trailing slashes

if options.inputdir[-1:] == "/":
  options.inputdir = options.inputdir[0:-1]

if options.outputdir[-1:] == "/":
  options.outputdir = options.outputdir[0:-1]


if (options.inputdir == options.outputdir):
  print "Input and output directories must be different. Aborting."
else: 
  doConversion(options)



