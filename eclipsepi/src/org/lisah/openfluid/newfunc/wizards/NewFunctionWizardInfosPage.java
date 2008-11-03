package org.lisah.openfluid.newfunc.wizards;
import org.eclipse.jface.viewers.ISelection;

import org.eclipse.jface.wizard.WizardPage;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.ModifyEvent;
import org.eclipse.swt.events.ModifyListener;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Text;



public class NewFunctionWizardInfosPage extends WizardPage {
	
	private Text funcDescText;
	private Text funcIDText;
	private Text funcNameText;
	private Text funcDomainText;	
	private Text funcAuthorsText;
	private Text funcEmailsText;


	/**
	 * Constructor for SampleNewWizardPage.
	 * 
	 * @param pageName
	 */
	public NewFunctionWizardInfosPage(ISelection selection) {
		super("wizardPage");
		setTitle("OpenFLUID function informations");
		setDescription("This wizard creates files for an openfluid simulation function.");
		
	}

	/**
	 * @see IDialogPage#createControl(Composite)
	 */
	public void createControl(Composite parent) {
	
		GridData data;
		Label label;
		
		Composite container = new Composite(parent, SWT.NULL);
		GridLayout layout = new GridLayout();		
		container.setLayout(layout);
		layout.numColumns = 3;
		layout.verticalSpacing = 9;
		layout.makeColumnsEqualWidth = false;
		
		
		// function ID		
		label = new Label(container, SWT.RIGHT);
		label.setText("Function ID:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);
		
		
		funcIDText = new Text(container, SWT.BORDER | SWT.SINGLE);
		data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		funcIDText.setLayoutData (data);
		funcIDText.addModifyListener(new ModifyListener() {
			public void modifyText(ModifyEvent e) {
				dialogChanged();
			}
		});

		
		// function name		
		label = new Label(container, SWT.RIGHT);
		label.setText("Function Name:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);

		funcNameText = new Text(container, SWT.BORDER | SWT.SINGLE);
		//data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;		
		data.horizontalSpan = 2;
		funcNameText.setLayoutData (data);

		// function domain		
		label = new Label(container, SWT.RIGHT);
		label.setText("Function Domain:");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);

		funcDomainText = new Text(container, SWT.BORDER | SWT.SINGLE);
		//data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;		
		data.horizontalSpan = 2;
		funcDomainText.setLayoutData (data);
		
		
		
		// function description		
		label = new Label(container, SWT.TOP | SWT.RIGHT);
		label.setText("Description:");
		data = new GridData ();
		data.verticalAlignment = GridData.BEGINNING;		
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);
		
		funcDescText = new Text(container, SWT.BORDER | SWT.MULTI | SWT.WRAP | SWT.V_SCROLL );
		//data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.heightHint = 100;
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.grabExcessHorizontalSpace = true;
		funcDescText.setLayoutData (data);
		
		// function authors		
		label = new Label(container, SWT.RIGHT);
		label.setText("Author(s):");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);
		
		funcAuthorsText = new Text(container, SWT.BORDER | SWT.SINGLE);
		data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.grabExcessHorizontalSpace = true;
		funcAuthorsText.setLayoutData (data);

		// function authors 's email
		label = new Label(container, SWT.RIGHT);
		label.setText("Author(s)'s email(s):");
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		label.setLayoutData (data);
		
		funcEmailsText = new Text(container, SWT.BORDER | SWT.SINGLE);
		data = new GridData(GridData.FILL_HORIZONTAL);
		data = new GridData ();
		data.horizontalAlignment = GridData.FILL;
		data.horizontalSpan = 2;
		data.grabExcessHorizontalSpace = true;
		funcEmailsText.setLayoutData (data);
		
		
		
		
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

	/**
	 * Uses the standard container selection dialog to choose the new value for
	 * the container field.
	 */


	/**
	 * Ensures that both text fields are set.
	 */

	private void dialogChanged() {
		if (getFuncID().length() == 0) {
			updateStatus("Function ID must be specified");
			return;
		}

		updateStatus(null);
	}

	private void updateStatus(String message) {
		setErrorMessage(message);
		setPageComplete(message == null);
	}

	
	public String getFuncID() {
		return funcIDText.getText();
	}	
	
	public String getFuncName() {
		return funcNameText.getText();
	}	

	public String getFuncDomain() {
		return funcDomainText.getText();
	}	
	
	
	public String getFuncDesc() {
		return funcDescText.getText();
	}		
	
	public String getFuncAuthors() {
		return funcAuthorsText.getText();
	}	
	
	public String getFuncEmails() {
		return funcEmailsText.getText();
	}	
	
	
	
	
}