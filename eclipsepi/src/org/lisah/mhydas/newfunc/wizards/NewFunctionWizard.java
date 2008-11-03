package org.lisah.mhydas.newfunc.wizards;

import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.Wizard;
import org.eclipse.ui.INewWizard;
import org.eclipse.ui.IWorkbench;
import org.eclipse.core.runtime.*;
import org.eclipse.jface.operation.*;
import java.lang.reflect.InvocationTargetException;
import org.eclipse.jface.dialogs.MessageDialog;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.core.resources.*;
import org.eclipse.core.runtime.CoreException;
import java.io.*;
import org.eclipse.ui.*;
import org.eclipse.ui.ide.IDE;



/**
 * This is a sample new wizard. Its role is to create a new file 
 * resource in the provided container. If the container resource
 * (a folder or a project) is selected in the workspace 
 * when the wizard is opened, it will accept it as the target
 * container. The wizard creates one file with the extension
 * "mpe". If a sample multi-page editor (also available
 * as a template) is registered for the same extension, it will
 * be able to open it.
 */

public class NewFunctionWizard extends Wizard implements INewWizard {
	private NewFunctionWizardSourcesPage sourcesPage;
	private NewFunctionWizardInfosPage infosPage;	
	private NewFunctionWizardDataPage dataPage;	
	private ISelection selection;

	/**
	 * Constructor for SampleNewWizard.
	 */
	public NewFunctionWizard() {
		super();
		setNeedsProgressMonitor(true);

	}
	
	/**
	 * Adding the page to the wizard.
	 */

	public void addPages() {
		sourcesPage = new NewFunctionWizardSourcesPage(selection);
		infosPage = new NewFunctionWizardInfosPage(selection);		
		dataPage = new NewFunctionWizardDataPage(selection);		
		addPage(sourcesPage);
		addPage(infosPage);
		addPage(dataPage);				
	}

	/**
	 * This method is called when 'Finish' button is pressed in
	 * the wizard. We will create an operation and run it
	 * using wizard as execution context.
	 */
	public boolean performFinish() {

		final String containerName = sourcesPage.getContainerName();
		final String fileName = sourcesPage.getFileName();
		final String funcClass = sourcesPage.getFuncClass();
		final boolean funcMakefile = sourcesPage.getMakefileCheck();
		final String funcInstallDir = sourcesPage.getInstallDir();
		final String funcBinName = infosPage.getFuncID().toLowerCase();		

		final String funcID = infosPage.getFuncID();
		final String funcName = infosPage.getFuncName();
		final String funcDomain = infosPage.getFuncDomain();		
		final String funcDesc = infosPage.getFuncDesc();
		final String funcAuthors = infosPage.getFuncAuthors();		
		final String funcEmails = infosPage.getFuncEmails();		
		
		final boolean funcRainOnSUs = dataPage.getFuncRainOnSUs();		
		final boolean funcRainOnRSs = dataPage.getFuncRainOnRSs();		
		
		final String[][] funcVars = dataPage.getFuncVars();
		final int funcVarsCount = dataPage.getFuncVarsCount();

		final String[][] funcFiles = dataPage.getFuncFiles();
		final int funcFilesCount = dataPage.getFuncFilesCount();
		
		
		IRunnableWithProgress op = new IRunnableWithProgress() {
			public void run(IProgressMonitor monitor) throws InvocationTargetException {
				try {
					doFinish(containerName, fileName, funcClass, funcMakefile, funcBinName, funcInstallDir,
							 funcID, funcName, funcDomain, funcDesc, funcAuthors, funcEmails,
							 funcRainOnSUs, funcRainOnRSs,
							 funcVars, funcVarsCount,
							 funcFiles, funcFilesCount,							 
							 monitor);
				} catch (CoreException e) {
					throw new InvocationTargetException(e);
				} finally {
					monitor.done();
				}
			}
		};
		try {
			getContainer().run(true, false, op);
		} catch (InterruptedException e) {
			return false;
		} catch (InvocationTargetException e) {
			Throwable realException = e.getTargetException();
			MessageDialog.openError(getShell(), "Error", realException.getMessage());
			return false;
		}
		return true;
	}
	
	/**
	 * The worker method. It will find the container, create the
	 * file if missing or just replace its contents, and open
	 * the editor on the newly created file.
	 */

	private void doFinish(String containerName, String fileName, String funcClass, boolean isMakefile, String funcBinName, String funcInstallDir,
						  String funcID, String funcName, String funcDomain, String funcDesc, String funcAuthors, String funcEmails,
						  boolean funcRainOnSUs, boolean funcRainOnRSs, 
						  String[][] funcVars, int funcVarsCount,
						  String[][] funcFiles, int funcFilesCount,						  
		                  IProgressMonitor monitor)
		throws CoreException {
		// create a sample file
		
		monitor.beginTask("Creating files", 2);
		
		IWorkspaceRoot root = ResourcesPlugin.getWorkspace().getRoot();
		
		IResource resource = root.findMember(new Path(containerName));
		
		if (!resource.exists() || !(resource instanceof IContainer)) {
			throwCoreException("Container \"" + containerName + "\" does not exist.");
		}
		
		IContainer container = (IContainer) resource;
		
		int dotPos = fileName.lastIndexOf(".");
		String rootFileName = fileName.substring(0, dotPos);
		
		final IFile cppFile = container.getFile(new Path(rootFileName+".cpp"));
		final IFile hFile = container.getFile(new Path(rootFileName+".h"));
		final IFile makeFile = container.getFile(new Path("makefile"));		
		
		try {
			InputStream stream = openCPPContentStream(rootFileName, funcClass, 
													  funcID, funcName, funcDomain, funcDesc, funcAuthors, funcEmails,
													  funcRainOnSUs, funcRainOnRSs,
													  funcVars, funcVarsCount,
													  funcFiles, funcFilesCount);
			if (cppFile.exists()) {
				cppFile.setContents(stream, true, true, monitor);
			} else {
				cppFile.create(stream, true, monitor);
			}
			stream.close();
			
			stream = openHContentStream(rootFileName, funcClass);
			if (hFile.exists()) {
				hFile.setContents(stream, true, true, monitor);
			} else {
				hFile.create(stream, true, monitor);
			}
			stream.close();
			
			if (isMakefile)	{
  			  	stream = openMakefileContentStream(rootFileName, funcBinName, funcInstallDir);
  			  	if (makeFile.exists()) {
  			  		makeFile.setContents(stream, true, true, monitor);
  			  	} else {
  			  		makeFile.create(stream, true, monitor);
  			  	}
  			  	stream.close();
			}  
			
			
			
		} catch (IOException e) {
		}
		monitor.worked(1);
		monitor.setTaskName("Opening file for editing...");
		getShell().getDisplay().asyncExec(new Runnable() {
			public void run() {
				IWorkbenchPage page =
					PlatformUI.getWorkbench().getActiveWorkbenchWindow().getActivePage();
				try {
					IDE.openEditor(page, cppFile, true);
					IDE.openEditor(page, hFile, true);

				} catch (PartInitException e) {
				}
			}
		});
		monitor.worked(1);
	}
	
	/**
	 * We will initialize file contents with a sample text.
	 */

	private InputStream openCPPContentStream(String fileName, String funcClass, 
											 String funcID, String funcName, String funcDomain, String funcDesc, String funcAuthors, String funcEmails,
											 boolean funcRainOnSUs, boolean funcRainOnRSs, 
											 String[][] funcVars, int funcVarsCount,
											 String[][] funcFiles, int funcFilesCount
											 ) {	

		String vars = "";
		String pvars = "";
		String rvars = "";
		String dvars = "";
		String uvars = "";
		String inic = "";
		String props = "";
		String gparm = "";

		String files = "";
		String rfiles = "";
		String ufiles = "";
		
		String rain = "";

		
		//"Produced variable", "Required variable", "Used variable", "Updated variable", 
		//"Required property", "Required initial condition", "Global parameter"
		
		
		for (int i=0;i<funcVarsCount;i++) { 
			
			if (funcVars[i][1].equals("Produced variable")) {
				if (pvars.length() == 0) pvars = "\n  // Produced variables\n";
				pvars = pvars + "  DECLARE_" + funcVars[i][2] + "_PRODUCED_VAR(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}

			if (funcVars[i][1].equals("Required variable")) {
				if (rvars.length() == 0) rvars = "\n  // Required variables\n";
				rvars = rvars + "  DECLARE_" + funcVars[i][2] + "_REQUIRED_VAR(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}
			
			if (funcVars[i][1].equals("Used variable")) {
				if (uvars.length() == 0) uvars = "\n  // Used variables\n";
				uvars = uvars + "  DECLARE_" + funcVars[i][2] + "_USED_VAR(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}

			if (funcVars[i][1].equals("Updated variable")) {
				if (dvars.length() == 0) dvars = "\n  // Updated variables\n";
				dvars = dvars + "  DECLARE_" + funcVars[i][2] + "_UPDATED_VAR(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}						

			if (funcVars[i][1].equals("Required property")) {
				if (props.length() == 0) props = "\n  // Required properties\n";
				props = props + "  DECLARE_" + funcVars[i][2] + "_REQUIRED_PROPERTY(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}

			if (funcVars[i][1].equals("Required initial condition")) {
				if (inic.length() == 0) inic = "\n  // Required initial conditions\n";
				inic = inic + "  DECLARE_" + funcVars[i][2] + "_REQUIRED_INICOND(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}

			if (funcVars[i][1].equals("Used property")) {
				if (props.length() == 0) props = "\n  // Used properties\n";
				props = props + "  DECLARE_" + funcVars[i][2] + "_USED_PROPERTY(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}

			if (funcVars[i][1].equals("Used initial condition")) {
				if (inic.length() == 0) inic = "\n  // Used initial conditions\n";
				inic = inic + "  DECLARE_" + funcVars[i][2] + "_USED_INICOND(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}
			
			
			if (funcVars[i][1].equals("Function parameter")) {
				if (gparm.length() == 0) gparm = "\n  // Function parameters\n";
				gparm = gparm + "  DECLARE_FUNCTION_PARAM(\""+funcVars[i][0]+"\",wxT(\""+funcVars[i][4]+"\"),wxT(\""+funcVars[i][3]+"\"));\n";
 			}
			
			
		}
		
		vars = pvars + rvars + dvars + uvars + inic + props + gparm; 

		
		// own files
		for (int i=0;i<funcFilesCount;i++) { 
			
			if (funcFiles[i][1].equals("Required")) {
				if (rfiles.length() == 0) rfiles = "\n  // Required extra files\n";
				rfiles = rfiles + "  DECLARE_REQUIRED_EXTRAFILE(wxT(\""+funcFiles[i][0]+"\"));\n";
 			}

			if (funcFiles[i][1].equals("Used")) {
				if (ufiles.length() == 0) ufiles = "\n  // Used extra files\n";
				ufiles = ufiles + "  DECLARE_USED_EXTRAFILE(wxT(\""+funcFiles[i][0]+"\"));\n";
 			}					
		}
		
		files = rfiles + ufiles;
		

		if (funcRainOnSUs) rain = rain + "  DECLARE_REQUIRED_SU_RAIN;\n";
		if (funcRainOnRSs) rain = rain + "  DECLARE_REQUIRED_RS_RAIN;\n";
			
		if (rain.length() > 0) rain = "\n  // Rain\n" + rain;
		
		String contents =
			"/**\n" +
			"  \\file " + fileName + ".cpp\n" +
			"  \\brief Implements ...\n" +
			"*/\n" +
			"\n\n" +
			"#include \"" + fileName + ".h\"\n"+
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			funcClass + "::" + funcClass + "()\n" +
			"                : PluggableFunction()\n" +
			"{\n" +
			"\n" +
			"  // Signature\n" +
			"  DECLARE_SIGNATURE_ID(wxT(\""+ funcID +"\"));\n" +			
			"  DECLARE_SIGNATURE_NAME(wxT(\""+ funcName + "\"));\n" +
			"  DECLARE_SIGNATURE_DESCRIPTION(wxT(\""+ funcDesc + "\"));\n" +
			"\n" +			
			"  DECLARE_SIGNATURE_VERSION(wxT(\"1.0\"));\n" +
			"  DECLARE_SIGNATURE_SDKVERSION;\n" +
			"  DECLARE_SIGNATURE_STATUS(mhydasdk::base::EXPERIMENTAL);\n" +			
			"\n" +			
			"  DECLARE_SIGNATURE_DOMAIN(wxT(\""+ funcDomain + "\"));\n" +
			"  DECLARE_SIGNATURE_PROCESS(wxT(\"\"));\n" +			
			"  DECLARE_SIGNATURE_METHOD(wxT(\"\"));\n" +
			"  DECLARE_SIGNATURE_AUTHORNAME(wxT(\""+ funcAuthors + "\"));\n" +
			"  DECLARE_SIGNATURE_AUTHOREMAIL(wxT(\""+ funcEmails + "\"));\n" +
			rain +
			vars +
			files +
			"\n" +			
			"}\n" +
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			funcClass + "::~" + funcClass +"()\n" +
			"{\n" +
			"\n" +
			"\n" +
			"}\n" +
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"bool " + funcClass + "::initParams(mhydasdk::core::ParamsMap Params)\n" +
			"{\n" +
			"\n" +
			"\n" +
			"  return true;\n" +
			"}"+
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"bool " + funcClass + "::prepareData()\n" +
			"{\n" +
			"\n" +
			"\n" +
			"  return true;\n" +
			"}\n" +
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"bool " + funcClass + "::checkConsistency()\n" +
			"{\n" +
			"\n" +
			"\n" +
			"  return true;\n" +
			"}\n" +
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"bool " + funcClass + "::initializeRun(mhydasdk::base::SimulationInfo* SimInfo)\n" +
			"{\n" +
			"\n" +
			"\n" +
			"  return true;\n" +
			"}"+
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"bool " + funcClass + "::runStep(mhydasdk::base::SimulationStatus* SimStatus)\n" +
			"{\n" +
			"\n" +
			"\n" +
			"  return true;\n" +
			"}"+
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"bool " + funcClass + "::finalizeRun(mhydasdk::base::SimulationInfo* SimInfo)\n" +
			"{\n" +
			"\n" +
			"\n" +
			"  return true;\n" +
			"}"+
			"\n" +
			"\n" +
			"// =====================================================================\n" +
			"// =====================================================================\n" +
			"\n" +
			"\n" +
			"mhydasdk::base::PluggableFunction* GetMHYDASPluggableFunction()\n" +
			"{\n" +
			"  return new " + funcClass + "();\n" +
			"}\n" +
			"\n\n";

		return new ByteArrayInputStream(contents.getBytes());
	}

	private InputStream openHContentStream(String fileName, String funcClass) {
		fileName = fileName + ".h";
		String hFileID = fileName.toUpperCase();
        hFileID = hFileID.replace(".","_");
        hFileID = "__" + hFileID + "__"; 
		
		String contents =
			"/**\n" +
			"  \\file " + fileName + "\n" +
			"  \\brief Header of ...\n" +			
			"*/\n" +
			"\n" +
			"#ifndef " + hFileID + "\n" +
			"#define " + hFileID + "\n" +		
			"\n" +
			"#include \"mhydasdk-base.h\"\n" +
			"#include \"mhydasdk-core.h\"\n" +
		    "\n" +
		    "\n" +		    
		    "// =====================================================================\n" +
		    "// =====================================================================\n" +			
			"\n" +
			"\n" +
			"/**\n" +
			"\n" +
			"*/\n" +		
			"class "+funcClass+ " : public mhydasdk::base::PluggableFunction\n" +
			"{\n" + 
			"  private:\n" +
			"\n" +
			"  public:\n" +
		    "    /**\n" +
		    "      Constructor\n" +
		    "    */\n" +
		    "    "+funcClass+"();\n" +
		    "\n" +
		    "    /**\n" +
		    "      Destructor\n" +
		    "    */\n" +
		    "    ~"+funcClass+"();\n" +
		    "\n" +
		    "    bool initParams(mhydasdk::core::ParamsMap Params);\n" +
		    "\n" +
		    "    bool prepareData();\n" +
		    "\n" +
		    "    bool checkConsistency();\n" +
		    "\n" +		 
		    "    bool initializeRun(mhydasdk::base::SimulationInfo* SimInfo);\n" +
		    "\n" +
		    "    bool runStep(mhydasdk::base::SimulationStatus* SimStatus);\n" +
		    "\n" +
		    "    bool finalizeRun(mhydasdk::base::SimulationInfo* SimInfo);\n" +
		    "\n" +
		    "};\n" +
		    "\n" +
		    "\n" +		    
		    "// =====================================================================\n" +
		    "// =====================================================================\n" +
		    "\n" +
		    "\n" +		    
		    "extern \"C\"\n" +
		    "{\n" +
		    "  DLLIMPORT mhydasdk::base::PluggableFunction* GetMHYDASPluggableFunction();\n" +
		    "};\n" +	
			"\n" +
			"\n" +
			"#endif  // " + hFileID +
			"\n";

		return new ByteArrayInputStream(contents.getBytes());
	}

	
	private InputStream openMakefileContentStream(String srcName, String binName, String installDir) {		
		
		String contents =
			"# makefile for mhydas function\n" +
			"\n" +
			"CPP = g++\n" +
			"WXLIBS = $(shell wx-config --libs base)\n" +
			"WXFLAGS = $(shell wx-config --cxxflags base)\n" +
			"MHYDASDKLIBS = $(shell mhydasdk-config --libs)\n"+
			"MHYDASDKFLAGS = $(shell mhydasdk-config --cflags)\n"+
			"BINFILE = " + binName +"\n" +
			"SRCFILESROOT = " + srcName +"\n" +
			"\n" +
			"INSTALLPATH = "+ installDir + "\n" +			
			"OBJPATH = .\n" +
			"BINPATH = .\n" +
			"\n" +			
			"LDFLAGS =\n" +
			"PLUGEXT = sompi\n" +
			"ifeq ($(OSTYPE),msys)\n" +
			"  LDFLAGS=-Wl,--enable-runtime-pseudo-reloc\n" +
			"  PLUGEXT = dllmpi\n" +			
			"endif\n" +
			"\n" +
			"ifdef FORCEINSTALLPATH\n" +
			"  INSTALLPATH = $(FORCEINSTALLPATH)\n" +			
			"endif\n" +			
			"\n" +
			"ifdef FORCEOBJPATH\n" +
			"  OBJPATH = $(FORCEOBJPATH)\n" +			
			"endif\n" +			
			"\n" +			
			"ifdef FORCEBINPATH\n" +
			"  BINPATH = $(FORCEBINPATH)\n" +			
			"endif\n" +			
			"\n" +						
			"\n" +			
			"all:\n" +
			"\t$(CPP) -c $(SRCFILESROOT).cpp -o $(OBJPATH)/$(SRCFILESROOT).o -fPIC $(WXFLAGS) $(MHYDASDKFLAGS)\n" +
			"\t$(CPP) $(OBJPATH)/$(SRCFILESROOT).o $(WXLIBS) $(MHYDASDKLIBS) -o $(BINPATH)/$(BINFILE).$(PLUGEXT) -shared $(LDFLAGS)\n" +			
			"\n" +
			"clean:\n" +
			"\trm -f $(BINPATH)/$(BINFILE).$(PLUGEXT)\n" +
			"\trm -f $(OBJPATH)/$(SRCFILESROOT).o\n" +			
			"\n" +			
			"install: all\n" +			
			"\t @mkdir -p $(INSTALLPATH)\n" +
			"\t @cp ./$(BINFILE).$(PLUGEXT) $(INSTALLPATH)\n" +			
			"\n";

			

		return new ByteArrayInputStream(contents.getBytes());
	}
	
	
	
	
	private void throwCoreException(String message) throws CoreException {
		IStatus status =
			new Status(IStatus.ERROR, "org.lisah.mhydas.newfunc", IStatus.OK, message, null);
		throw new CoreException(status);
	}

	/**
	 * We will accept the selection in the workbench to see if
	 * we can initialize from it.
	 * @see IWorkbenchWizard#init(IWorkbench, IStructuredSelection)
	 */
	public void init(IWorkbench workbench, IStructuredSelection selection) {
		this.selection = selection;
		
	}
}