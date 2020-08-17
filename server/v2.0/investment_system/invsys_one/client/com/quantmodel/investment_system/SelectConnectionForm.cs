using System;
using System.ComponentModel;
using System.Security.Cryptography;
using System.Text;
using System.Windows.Forms;

namespace com.quantmodel.investment_system
{
	public class SelectConnectionForm : System.Windows.Forms.Form
	{
	    private readonly QuantmodelConfiguration config;
	    private ConnectionElement connectionElement;

	    private Button okButton, cancelButton;

	    public ConnectionElement ConnectionElement
	    {
	        get { return connectionElement; }
	    }

		public SelectConnectionForm( QuantmodelConfiguration configSection )
		{
		    config = configSection;
		    connectionElement = null;
			InitializeComponents();
		}

		private void InitializeComponents()
		{
		    System.Windows.Forms.ComboBox comboBox = new System.Windows.Forms.ComboBox();
		    comboBox.DropDownWidth = 80;
            comboBox.Location = new System.Drawing.Point( 88, 68 );
            comboBox.Size = new System.Drawing.Size( 180, 26 );

            for( int i=0; i<config.ConnectionGroup.Count; i++ )
            {
                comboBox.Items.Add( config.ConnectionGroup[ i ].Name );
            }

            comboBox.SelectedIndexChanged += new System.EventHandler( FireSelectedIndexChanged );

            Label infoLabel = new Label();
            infoLabel.ForeColor = System.Drawing.Color.Gray;
            infoLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            infoLabel.Location = new System.Drawing.Point( 0, 12 );
            infoLabel.Size = new System.Drawing.Size( 320, 40 );
            infoLabel.Text = "Please select a connection name.";
            infoLabel.Font = new System.Drawing.Font( "Arial", 12.25F,
                System.Drawing.FontStyle.Bold,
                    System.Drawing.GraphicsUnit.Point,
                        ((System.Byte)(0)));

		    okButton = new Button();
            okButton.Size = new System.Drawing.Size( 80, 26 );
            okButton.Text = "Select";
            okButton.TabIndex = 1;
            okButton.TabStop = true;
            okButton.DialogResult = DialogResult.OK;
            okButton.Location = new System.Drawing.Point( 134, 24 );
            okButton.Anchor = AnchorStyles.Right;
            okButton.Enabled = false;

            cancelButton = new Button();
            cancelButton.Size = new System.Drawing.Size( 80, 26 );
            cancelButton.Text = "Cancel";
            cancelButton.TabIndex = 1;
            cancelButton.TabStop = true;
            cancelButton.DialogResult = DialogResult.No;
            cancelButton.Location = new System.Drawing.Point( 220, 24 );
            cancelButton.Anchor = AnchorStyles.Right;

            Panel buttonPanel = new Panel();
	        buttonPanel.BackColor = System.Drawing.SystemColors.Control;
			buttonPanel.Dock = System.Windows.Forms.DockStyle.Bottom;
			buttonPanel.Name = "buttonPanel";
			buttonPanel.Size = new System.Drawing.Size( 330, 74 );

			Panel infoPanel = new Panel();
	        infoPanel.BackColor = System.Drawing.SystemColors.Window;
			infoPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			infoPanel.Name = "infoPanel";

			infoPanel.Controls.AddRange( new Control [] { infoLabel, comboBox } );
			buttonPanel.Controls.AddRange( new Control [] { okButton, cancelButton } );

			this.Controls.AddRange( new Control [] { infoPanel, buttonPanel } );

            this.Text = "INVSYS ONE: Select Proxy";
			this.Name = "SelectConnectionControl";
			this.Size = new System.Drawing.Size( 330, 234 );
	    }

        private void FireSelectedIndexChanged( object sender, System.EventArgs e )
	    {
	        okButton.Enabled = true;

	        ComboBox comboBox = (ComboBox) sender;
		    string selectedConnection = (string) comboBox.SelectedItem;

            connectionElement = config.ConnectionGroup[ selectedConnection ];
	    }
	}
}
