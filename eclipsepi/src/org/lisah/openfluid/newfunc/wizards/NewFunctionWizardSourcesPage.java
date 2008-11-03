package org.lisah.openfluid.newfunc.wizards;



import org.eclipse.core.resources.IContainer;
import org.eclipse.core.resources.IResource;
import org.eclipse.core.resources.ResourcesPlugin;
import org.eclipse.core.runtime.Path;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.viewers.IStructuredSelection;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.ui.dialogs.ContainerSelectionDialog;

/**
 * The "New" wizard page allows setting the container for the new file as well
 * as the file name. The page will only accept file name without the extension
 * OR with the extension that matches the expected one (mpe).
 */

public class NewFunctionWizardSourcesPage extends WizardPage {
	private Text containerText;
	private Text fileText;
	private Text installText;	
	private Button makefileCheck;

	private Text funcClassText;
	

	private ISelection selection;
	
	private Label installLabel;

	/**
	 * Constructor for SampleNewWizardPage.
	 * 
	 * @param pageName
	 */
	public NewFunctionWizardSourcesPage(ISelection selection) {
		super("wizardPage");
		setTitle("OpenFLUID function sources files");
		setDescription("This wizard creates files for an openfluid simulation function.");
		this.selection = selection;
		
	}

	/**
	 * @see IDialogPage#createControl(Composite)
	 */
	public void createControl(Composite parent) {
	
		
		
		Composite container = new Composite(parent, SWT.NULL);
		GridLayout layout = new GridLayout();		
		container.setLayout(layout);
		layout.numColumns = 3;
		layout.verticalSpacing = 9;
		layout.makeColumnsEqualWidth = false;
		
		
		
		// Directory
		Label label = new Label(container, SWT.RIGHT);
		label.setText("&Container:");
		GridData data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);
		
		containerText = new Text(container, SWT.BORDER);
		data = new GridData();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;
		containerText.setLayoutData (data);
		containerText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent e) {
				dialogChanged();
			}
		});

		Button browseButton = new Button(container, SWT.PUSH);
		browseButton.setText("Browse...");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		browseButton.setLayoutData (data);
		
		browseButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				handleBrowse();
			}
		});
		
		
		label = new Label(container, SWT.RIGHT);
		label.setText("&CPP file name:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);
		
		
		
		fileText = new Text(container, SWT.BORDER | SWT.SINGLE);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.grabExcessHorizontalSpace = true;	
		fileText.setLayoutData(data);
		fileText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent e) {
				dialogChanged();
			}
		});	


		
		label = new Label(container, SWT.RIGHT);
		label.setText("&Class name:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);

		funcClassText = new Text(container, SWT.BORDER | SWT.SINGLE);
		data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		funcClassText.setLayoutData (data);

        // makefile
		makefileCheck = new Button (container, SWT.CHECK);
		makefileCheck.setText ("Create makefile");
		makefileCheck.setSelection(true);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 3;
		data.grabExcessHorizontalSpace = true;		
		makefileCheck.setLayoutData (data);
		makefileCheck.addSelectionListener(new SelectionListener() {
			public void widgetDefaultSelected(SelectionEvent e) {
				dialogChanged();
			}
			public void widgetSelected(SelectionEvent e) {
				dialogChanged();
			}
			
		});	

		
		installLabel = new Label(container, SWT.RIGHT);
		installLabel.setText("&Function install directory:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		installLabel.setLayoutData (data);
		
		installText = new Text(container, SWT.BORDER);
		
		installText.setText("");
		
		if (System.getProperty("os.name").equals("Linux")) {
	        installText.setText("$(HOME)" + System.getProperty("file.separator") +
		                        ".openfluid" + System.getProperty("file.separator") + 
		                        "engine" + System.getProperty("file.separator") + "functions");
			
		}

		if (System.getProperty("os.name").startsWith("Windows")) {
	        installText.setText(System.getProperty("user.home") + System.getProperty("file.separator") +
	        		            "Application Data" + System.getProperty("file.separator") +
		                        "openfluid" + System.getProperty("file.separator") + 
		                        "engine" + System.getProperty("file.separator") + "plugs");
			
		}

		
		
        
		data = new GridData();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.grabExcessHorizontalSpace = true;
		installText.setLayoutData (data);
		installText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent e) {
				dialogChanged();
			}
		});

		

		
		
		
		container.pack ();
		
		
		initialize();
		dialogChanged();
		setControl(container);
	}

	/**
	 * Tests if the current workbench selection is a suitable container to use.
	 */

	private void initialize() {
		if (selection != null && selection.isEmpty() == false
				&& selection instanceof IStructuredSelection) {
			IStructuredSelection ssel = (IStructuredSelection) selection;
			if (ssel.size() > 1)
				return;
			Object obj = ssel.getFirstElement();
			if (obj instanceof IResource) {
				IContainer container;
				if (obj instanceof IContainer)
					container = (IContainer) obj;
				else
					container = ((IResource) obj).getParent();
				containerText.setText(container.getFullPath().toString());
			}
		}
		fileText.setText("NewFunc.cpp");
		funcClassText.setText("NewFunction");
	}

	/**
	 * Uses the standard container selection dialog to choose the new value for
	 * the container field.
	 */

	private void handleBrowse() {
		ContainerSelectionDialog dialog = new ContainerSelectionDialog(
				getShell(), ResourcesPlugin.getWorkspace().getRoot(), false,
				"Select new file container");
		if (dialog.open() == ContainerSelectionDialog.OK) {
			Object[] result = dialog.getResult();
			if (result.length == 1) {
				containerText.setText(((Path) result[0]).toString());
			}
		}
	}

	/**
	 * Ensures that both text fields are set.
	 */

	private void dialogChanged() {
		IResource container = ResourcesPlugin.getWorkspace().getRoot()
				.findMember(new Path(getContainerName()));
		String fileName = getFileName();

		if (getContainerName().length() == 0) {
			updateStatus("File container must be specified");
			return;
		}
		if (container == null
				|| (container.getType() & (IResource.PROJECT | IResource.FOLDER)) == 0) {
			updateStatus("File container must exist");
			return;
		}
		if (!container.isAccessible()) {
			updateStatus("Project must be writable");
			return;
		}
		if (fileName.length() == 0) {
			updateStatus("File name must be specified");
			return;
		}
		if (fileName.replace('\\', '/').indexOf('/', 1) > 0) {
			updateStatus("File name must be valid");
			return;
		}
		int dotLoc = fileName.lastIndexOf('.');
		if (dotLoc != -1) {
			String ext = fileName.substring(dotLoc + 1);
			if (ext.equalsIgnoreCase("cpp") == false) {
				updateStatus("File extension must be \"cpp\"");
				return;
			}
		}
		installText.setEnabled(makefileCheck.getSelection());
		installLabel.setEnabled(makefileCheck.getSelection());
		
		if (makefileCheck.getSelection() && getInstallDir().length() == 0) {
			updateStatus("Install directory must be specified");
			return;
		}
		
		updateStatus(null);
	}

	private void updateStatus(String message) {
		setErrorMessage(message);
		setPageComplete(message == null);
	}

	public String getContainerName() {
		return containerText.getText();
	}

	public String getFileName() {
		return fileText.getText();
	}

	public String getFuncClass() {
		return funcClassText.getText();
	}
	
	
	public boolean getMakefileCheck() {
		return makefileCheck.getSelection();
	}	

	public String getInstallDir() {
		return installText.getText();
	}	
	
	
	
	
}