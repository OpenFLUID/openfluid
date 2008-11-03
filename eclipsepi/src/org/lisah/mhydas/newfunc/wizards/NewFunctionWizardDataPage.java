package org.lisah.mhydas.newfunc.wizards;
import org.eclipse.jface.viewers.ISelection;
import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.Table;
import org.eclipse.swt.widgets.TableItem;
import org.eclipse.swt.widgets.TableColumn;
import org.eclipse.swt.widgets.Combo;




public class NewFunctionWizardDataPage extends WizardPage {
	

	private Table varsTable;
	private Table filesTable;	
	
	private Button addVarButton;
	private Button remVarButton;

	private Button addFileButton;
	private Button remFileButton;

	
	
	private Button rainOnSUCheck;
	private Button rainOnRSCheck;
	private Button eventsOnSUCheck;
	private Button eventsOnRSCheck;
	private Button eventsOnGUCheck;	
	
	
	/**
	 * Constructor for SampleNewWizardPage.
	 * 
	 * @param pageName
	 */
	public NewFunctionWizardDataPage(ISelection selection) {
		super("wizardPage");
		setTitle("MHYDAS function data");
		setDescription("This wizard creates files for a mhydas simulation function.");
		
	}

	/**
	 * @see IDialogPage#createControl(Composite)
	 */
	public void createControl(Composite parent) {
	
		GridData data;
		Label label;
		TableColumn column;
		
		Composite container = new Composite(parent, SWT.NULL);
		GridLayout layout = new GridLayout();		
		container.setLayout(layout);
		layout.numColumns = 2;
		layout.verticalSpacing = 9;
		layout.makeColumnsEqualWidth = false;
		

        // rain on SUs and RSs
		rainOnSUCheck = new Button (container, SWT.CHECK);
		rainOnSUCheck.setText ("Rain is required on SUs");
		rainOnSUCheck.setSelection(true);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		rainOnSUCheck.setLayoutData (data);
		rainOnSUCheck.setSelection(false);
		
		rainOnRSCheck = new Button (container, SWT.CHECK);
		rainOnRSCheck.setText ("Rain is required on RSs");
		rainOnRSCheck.setSelection(true);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		rainOnRSCheck.setLayoutData (data);
		rainOnRSCheck.setSelection(false);

		// line space
		label = new Label(container, SWT.NULL);
		label.setText("");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		label.setLayoutData (data);
		
		
		//events 
		
		eventsOnSUCheck = new Button (container, SWT.CHECK);
		eventsOnSUCheck.setText ("Events are used on SUs");
		eventsOnSUCheck.setSelection(true);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		eventsOnSUCheck.setLayoutData (data);
		eventsOnSUCheck.setSelection(false);
		
		eventsOnRSCheck = new Button (container, SWT.CHECK);
		eventsOnRSCheck.setText ("Events are used on RSs");
		eventsOnRSCheck.setSelection(true);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		eventsOnRSCheck.setLayoutData (data);
		eventsOnRSCheck.setSelection(false);

		eventsOnGUCheck = new Button (container, SWT.CHECK);
		eventsOnGUCheck.setText ("Events are used on GUs");
		eventsOnGUCheck.setSelection(true);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		eventsOnGUCheck.setLayoutData (data);
		eventsOnGUCheck.setSelection(false);
		
		
		
		
		// line space
		label = new Label(container, SWT.NULL);
		label.setText("");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		label.setLayoutData (data);
		
		
		// function variables and parameters
		
		label = new Label(container, SWT.NULL);
		label.setText("Variables and parameters:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		label.setLayoutData (data);

		varsTable = new Table(container, SWT.MULTI | SWT.BORDER );
		data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		data.horizontalSpan = 2;
		data.heightHint = 200;
		varsTable.setLayoutData (data);
		varsTable.setHeaderVisible(true);
		
		String[] varsTitles = {"Name", "Type", "Distribution", "Unit", "Description"};
		for (int i=0; i<varsTitles.length; i++) {
			column = new TableColumn (varsTable, SWT.NONE);
			column.setText (varsTitles [i]);						
		}			

        for (int i=0; i<varsTitles.length; i++) {
		  varsTable.getColumn(i).pack();
	    }			
		
		
		addVarButton = new Button(container, SWT.PUSH);
		addVarButton.setText("Add...");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;
		addVarButton.setLayoutData (data);
		addVarButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				handleAddVar();
			}
		});
		

		remVarButton = new Button(container, SWT.PUSH);
		remVarButton.setText("Remove");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;
		remVarButton.setLayoutData (data);
		remVarButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				handleRemoveVar();
			}
		});

		
		// line space
		label = new Label(container, SWT.NULL);
		label.setText("");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		label.setLayoutData (data);
		
		
		// function files
		
		label = new Label(container, SWT.NULL);
		label.setText("Extra files:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		label.setLayoutData (data);

		filesTable = new Table(container, SWT.MULTI | SWT.BORDER );
		data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;		
		data.horizontalSpan = 2;
		data.heightHint = 100;
		filesTable.setLayoutData (data);
		filesTable.setHeaderVisible(true);
		
		String[] filesTitles = {"Name", "Type"};
		for (int i=0; i<filesTitles.length; i++) {
			column = new TableColumn (filesTable, SWT.NONE);
			column.setText (filesTitles [i]);						
		}			

        for (int i=0; i<filesTitles.length; i++) {
		  filesTable.getColumn(i).pack();
	    }			
		
		
		addFileButton = new Button(container, SWT.PUSH);
		addFileButton.setText("Add...");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;
		addFileButton.setLayoutData (data);
		addFileButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				handleAddFile();
			}
		});
		

		remFileButton = new Button(container, SWT.PUSH);
		remFileButton.setText("Remove");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.grabExcessHorizontalSpace = true;
		remFileButton.setLayoutData (data);
		remFileButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {
				handleRemoveFile();
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
	}

	private void handleAddVar() {		
		Label label;
		GridData data;
		final Label distriLabel;
		final Combo distriCombo;
		final Combo typeCombo;
		
		
		final Shell shell = new Shell(this.getShell());
		shell.setText("Add variable or parameter declaration");
		GridLayout layout = new GridLayout();		
		shell.setLayout(layout);
		layout.numColumns = 2;
		layout.verticalSpacing = 9;
		layout.makeColumnsEqualWidth = false;

		
		// Name (ID)	
		label = new Label(shell, SWT.RIGHT);
		label.setText("Name (ID):");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData(data);

		final Text nameText = new Text (shell, SWT.BORDER);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		nameText.setLayoutData(data);

		
		
		// type	
		label = new Label(shell, SWT.RIGHT);
		label.setText("Type:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData(data);

		typeCombo = new Combo (shell, SWT.READ_ONLY);
		typeCombo.setItems (new String [] {"Produced variable", "Required variable", "Used variable", "Updated variable", 
									   "Required property", "Used property", "Required initial condition", "Used initial condition","Function parameter"});
		typeCombo.select(0);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		typeCombo.setLayoutData(data);
		
		
		// distribution	
		distriLabel = new Label(shell, SWT.RIGHT);
		distriLabel.setText("Distribution:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		distriLabel.setLayoutData(data);

		distriCombo = new Combo (shell, SWT.READ_ONLY);
		distriCombo.setItems (new String [] {"Surface units (SU)", "Reach segment (RS)", "Groundwater units (GU)"});
		distriCombo.select(0);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		distriCombo.setLayoutData (data);

		
		typeCombo.addSelectionListener(new SelectionAdapter(){
			public void widgetSelected(SelectionEvent e){
				distriLabel.setEnabled(typeCombo.getSelectionIndex() != 8);
				distriCombo.setEnabled(typeCombo.getSelectionIndex() != 8);
		    }
		});
		


		// Description	
		label = new Label(shell, SWT.RIGHT);
		label.setText("Description:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData(data);

		final Text descText = new Text (shell, SWT.BORDER);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		descText.setLayoutData(data);
		
		// Unit	
		label = new Label(shell, SWT.RIGHT);
		label.setText("Unit:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData(data);

		final Text unitText = new Text (shell, SWT.BORDER);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		unitText.setLayoutData(data);

		Composite buttonsContainer = new Composite(shell, SWT.NULL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.verticalIndent = 15;		
		buttonsContainer.setLayoutData(data);

		GridLayout buttonsLayout = new GridLayout();		
		buttonsContainer.setLayout(buttonsLayout);
		buttonsLayout.numColumns = 2;
		buttonsLayout.verticalSpacing = 20;
		buttonsLayout.horizontalSpacing = 20;		
		buttonsLayout.makeColumnsEqualWidth = true;
		
		
		Button cancelButton = new Button(buttonsContainer, SWT.PUSH | SWT.CENTER);
		cancelButton.setText("Cancel");
		data = new GridData ();
		data.widthHint = 100;
		data.horizontalAlignment = GridData.END;
		data.grabExcessHorizontalSpace = true;		
		cancelButton.setLayoutData(data);
		cancelButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {				
				shell.close();
			}
		});

		
		
		final Button OKButton = new Button(buttonsContainer, SWT.PUSH | SWT.CENTER);
		OKButton.setText("OK");
		OKButton.setEnabled(false);
		data = new GridData ();
		data.widthHint = 100;
		OKButton.setLayoutData(data);
		OKButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {				
				
				
				TableItem item = new TableItem (varsTable, SWT.NONE);
				item.setText (0, nameText.getText());
				item.setText (1, typeCombo.getText());
				
				if (typeCombo.getSelectionIndex() == 8) item.setText (2, "--");
				else {
				  item.setText (2, "SU");
				  if (distriCombo.getSelectionIndex() == 1) item.setText (2, "RS"); 
				  if (distriCombo.getSelectionIndex() == 2) item.setText (2, "GU");
				
				  item.setText (4, descText.getText());
				  item.setText (3, unitText.getText());

				}								

				shell.close();
			}
		});
		
		nameText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent e) {
				OKButton.setEnabled(nameText.getText().length() != 0);
			}
		});

		
		
		shell.pack();
		shell.open();

		
	}

	private void handleRemoveVar() {
		varsTable.remove(varsTable.getSelectionIndices());
		
	}


	private void handleAddFile() {

		Label label;
		GridData data;
		final Combo typeCombo;

		
		final Shell shell = new Shell(this.getShell());
		shell.setText("Add own file");
		GridLayout layout = new GridLayout();		
		shell.setLayout(layout);
		layout.numColumns = 2;
		layout.verticalSpacing = 9;
		layout.makeColumnsEqualWidth = false;

		
		// Name (ID)	
		label = new Label(shell, SWT.RIGHT);
		label.setText("Name:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData(data);

		final Text nameText = new Text (shell, SWT.BORDER);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		nameText.setLayoutData(data);

		
		
		// type	
		label = new Label(shell, SWT.RIGHT);
		label.setText("Type:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData(data);

		typeCombo = new Combo (shell, SWT.READ_ONLY);
		typeCombo.setItems (new String [] {"Required", "Used"});
		typeCombo.select(0);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		typeCombo.setLayoutData(data);
		
		
		Composite buttonsContainer = new Composite(shell, SWT.NULL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.verticalIndent = 15;		
		buttonsContainer.setLayoutData(data);

		GridLayout buttonsLayout = new GridLayout();		
		buttonsContainer.setLayout(buttonsLayout);
		buttonsLayout.numColumns = 2;
		buttonsLayout.verticalSpacing = 20;
		buttonsLayout.horizontalSpacing = 20;		
		buttonsLayout.makeColumnsEqualWidth = true;
		
		
		Button cancelButton = new Button(buttonsContainer, SWT.PUSH | SWT.CENTER);
		cancelButton.setText("Cancel");
		data = new GridData ();
		data.widthHint = 100;
		data.horizontalAlignment = GridData.END;
		data.grabExcessHorizontalSpace = true;		
		cancelButton.setLayoutData(data);
		cancelButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {				
				shell.close();
			}
		});

		
		
		final Button OKButton = new Button(buttonsContainer, SWT.PUSH | SWT.CENTER);
		OKButton.setText("OK");
		OKButton.setEnabled(false);
		data = new GridData ();
		data.widthHint = 100;
		OKButton.setLayoutData(data);
		OKButton.addSelectionListener(new SelectionAdapter() {
			public void widgetSelected(SelectionEvent e) {				
							
				TableItem item = new TableItem (filesTable, SWT.NONE);
				item.setText (0, nameText.getText());
				item.setText (1, typeCombo.getText());
				
				shell.close();
			}
		});
		
		nameText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent e) {
				OKButton.setEnabled(nameText.getText().length() != 0);
			}
		});

		
		
		shell.pack();
		shell.open();

		
		
		
		
	}

	
	private void handleRemoveFile() {
		filesTable.remove(filesTable.getSelectionIndices());
		
	}

	
	
	/**
	 * Ensures that both text fields are set.
	 */

	private void dialogChanged() {
		updateStatus(null);
	}

	private void updateStatus(String message) {
		setErrorMessage(message);
		setPageComplete(message == null);
	}

	public String[][] getFuncVars() {
		
		String[][] vars = new String[varsTable.getItemCount()][5];
		
		for (int i=0;i<varsTable.getItemCount();i++) {
			vars[i][0] = varsTable.getItem(i).getText(0);
			vars[i][1] = varsTable.getItem(i).getText(1);
			vars[i][2] = varsTable.getItem(i).getText(2);
			vars[i][3] = varsTable.getItem(i).getText(3);
			vars[i][4] = varsTable.getItem(i).getText(4);			
		}
		
		return vars;
	}
	

	public int getFuncVarsCount() {		
		return varsTable.getItemCount();
	}

	
	public String[][] getFuncFiles() {
		
		String[][] vars = new String[filesTable.getItemCount()][5];
		
		for (int i=0;i<filesTable.getItemCount();i++) {
			vars[i][0] = filesTable.getItem(i).getText(0);
			vars[i][1] = filesTable.getItem(i).getText(1);
		}
		
		return vars;
	}
	

	public int getFuncFilesCount() {		
		return filesTable.getItemCount();
	}
	
	public boolean getFuncRainOnSUs() {
		return rainOnSUCheck.getSelection();
	}

	public boolean getFuncRainOnRSs() {
		return rainOnRSCheck.getSelection();
	}

	public boolean getFuncEventsOnSUs() {
		return eventsOnSUCheck.getSelection();
	}

	public boolean getFuncEventsOnRSs() {
		return eventsOnRSCheck.getSelection();
	}

	public boolean getFuncEventsOnGUs() {
		return eventsOnGUCheck.getSelection();
	}
	
	
	
	
	
}


