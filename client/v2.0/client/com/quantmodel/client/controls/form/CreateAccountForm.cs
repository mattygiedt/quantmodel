using System;
using System.ComponentModel;
using System.Text;
using System.Windows.Forms;

using com.quantmodel.common.data;

namespace com.quantmodel.client.controls.form
{
	public class CreateAccountForm : Form
	{
	    private AccountDataPanel accountData;
        Button submit = new Button();
	    public AccountDataPanel AccountData { get { return accountData; } }

		public CreateAccountForm() : base()
		{
		    this.Text = "Create New Account:";
		    this.BackColor = System.Drawing.SystemColors.Window;

		    accountData = new AccountDataPanel( true );
		    InitializeComponents();

		    accountData.AccountDataChanged += new EventHandler( FireDataChanged );
        }

        public CreateAccountForm( Account account ) : base()
		{
		    this.Text = "Create New Account:";
		    this.BackColor = System.Drawing.SystemColors.Window;

		    accountData = new AccountDataPanel( true );
		    accountData.Clone( account );
		    InitializeComponents();

		    accountData.AccountDataChanged += new EventHandler( FireDataChanged );
        }

        private void InitializeComponents()
    	{
            accountData.Location = new System.Drawing.Point( 40, 30 );

            Label headerLabel = new Label();
            headerLabel.BackColor = System.Drawing.SystemColors.Window;
			headerLabel.Location = new System.Drawing.Point( 10, 10 );
			headerLabel.Name = "headerLabel";
			headerLabel.Size = new System.Drawing.Size( 360, 20 );
			headerLabel.TabIndex = 3;
			headerLabel.Text = "Create Account:";
			headerLabel.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			headerLabel.Font = new System.Drawing.Font(
			    "Tahoma", 10.25F, System.Drawing.FontStyle.Bold,
			        System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));

    	    //
    	    //  Button panel
    	    //


			submit.Enabled = false;
            submit.Size = new System.Drawing.Size( 84, 26 );
            submit.Text = "Submit";
            submit.TabIndex = 1;
            submit.TabStop = true;
            submit.DialogResult = DialogResult.OK;
            submit.Location = new System.Drawing.Point( 310, 10 );

            Button cancel = new Button();
            cancel.Text = "Cancel";
            cancel.Size = new System.Drawing.Size( 84, 26 );
            cancel.DialogResult = DialogResult.Cancel;
            cancel.Location = new System.Drawing.Point( 400, 10 );

            Panel buttonPanel = new Panel();
	        buttonPanel.BackColor = System.Drawing.SystemColors.Control;
			buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			buttonPanel.Name = "buttonPanel";
			buttonPanel.Size = new System.Drawing.Size( 460, 50 );
			buttonPanel.Controls.AddRange( new Control[] {
			  submit,
			  cancel
			} );

			this.Controls.AddRange( new Control [] {
			    headerLabel,
                accountData,
                buttonPanel
            } );
	    }

	    private void FireDataChanged( object sender, System.EventArgs e )
    	{
    	    if( accountData.AccountName != "" &&
    	        accountData.AccountId != "" )
    	    {
    	        submit.Enabled = true;
    	    }
    	    else
    	    {
    	        submit.Enabled = false;
    	    }
    	}
	}
}
