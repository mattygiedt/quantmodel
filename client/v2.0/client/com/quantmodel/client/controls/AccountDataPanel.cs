using System;
using System.IO;
using System.Xml;
using System.Text;
using System.Drawing;
using System.Threading;
using System.Xml.XPath;
using System.Collections;
using System.Reflection;
using System.Windows.Forms;
using System.ComponentModel;

using TD.Eyefinder;
using GlacialComponents.Controls.GlacialTreeList;
using GlacialComponents.Controls.GTLCommon;

using com.quantmodel.common.data;

namespace com.quantmodel.client.controls
{
    public sealed class AccountDataPanel : Panel
    {
        public event System.EventHandler AccountDataChanged;

        public string AccountName { get { return accountNameTextBox.Text; } }
        public string MarketAccountName { get { return marketAccountNameTextBox.Text; } }
        public string AccountId { get { return accountIdTextBox.Text; } }
        public string GiveUpAccount { get { return giveUpAccountTextBox.Text; } }
        public string GiveUpFirm { get { return giveUpFirmTextBox.Text; } }
        public string IsActive { get { return isActiveCheckBox.Checked == true ? "1" : "0"; } }

        private readonly TextBox accountNameTextBox = new System.Windows.Forms.TextBox();
        private readonly TextBox marketAccountNameTextBox = new System.Windows.Forms.TextBox();
        private readonly TextBox accountIdTextBox = new System.Windows.Forms.TextBox();
        private readonly TextBox giveUpAccountTextBox = new System.Windows.Forms.TextBox();
        private readonly TextBox giveUpFirmTextBox = new System.Windows.Forms.TextBox();
        private readonly CheckBox isActiveCheckBox = new System.Windows.Forms.CheckBox();

        public AccountDataPanel( bool enableNameFlag ) : base()
		{
		    this.SuspendLayout();
			this.Name = "AccountDataPanel";
			InitializeComponents();

			accountNameTextBox.Enabled = enableNameFlag;

			this.ResumeLayout( false );
		}

		private string BlankIfNull( string input )
	    {
	        if( input == null )
	        {
	            return "";
	        }
	        else if( input.Equals( "NULL" ) )
	        {
	            return "";
	        }
	        else
	        {
	            return input;
	        }
	    }

		public void Clear( )
	    {
	        accountNameTextBox.Text = "";
            marketAccountNameTextBox.Text = "";
            accountIdTextBox.Text = "";
            giveUpAccountTextBox.Text = "";
            giveUpFirmTextBox.Text = "";
    		isActiveCheckBox.Checked = false;
        }

        public void Update( Account account )
        {
            accountNameTextBox.Text = BlankIfNull( account[ DatabaseObject.NAME ] );
            marketAccountNameTextBox.Text = BlankIfNull( account[ DatabaseObject.MARKET_ACCOUNT_ID ] );
            accountIdTextBox.Text = BlankIfNull( account[ DatabaseObject.CUSTODY_ACCOUNT_ID ] );
            giveUpAccountTextBox.Text = BlankIfNull( account[ DatabaseObject.GIVEUP_ACCOUNT_ID ] );
            giveUpFirmTextBox.Text = BlankIfNull( account[ DatabaseObject.GIVEUP_FIRM_ID ] );
    		isActiveCheckBox.Checked = account[ DatabaseObject.IS_ACTIVE ].Equals( "1" );
        }

        public void Clone( Account account )
        {
            marketAccountNameTextBox.Text = BlankIfNull( account[ DatabaseObject.MARKET_ACCOUNT_ID ] );
            accountIdTextBox.Text = "";
            giveUpAccountTextBox.Text = BlankIfNull( account[ DatabaseObject.GIVEUP_ACCOUNT_ID ] );
            giveUpFirmTextBox.Text = BlankIfNull( account[ DatabaseObject.GIVEUP_FIRM_ID ] );
            isActiveCheckBox.Checked = account[ DatabaseObject.IS_ACTIVE ].Equals( "1" );
        }

		private void InitializeComponents()
        {
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Size = new System.Drawing.Size( 500, 300 );
            this.Location = new System.Drawing.Point( 10, 10 );

            ////////////////////////////////////////////////////////////////////
            //  Account Name:

            Label accountNameLabel = new Label();
            accountNameLabel.Size = new System.Drawing.Size( 140, 20 );
            accountNameLabel.Location = new System.Drawing.Point( 20, 20 );
            accountNameLabel.Text = "Account Name:";
            accountNameLabel.Name = "accountNameLabel";
            accountNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            accountNameTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			accountNameTextBox.Name = "accountNameTextBox";
			accountNameTextBox.Size = new System.Drawing.Size( 270, 20 );
			accountNameTextBox.Location = new System.Drawing.Point( 180, 20 );
			accountNameTextBox.TabIndex = 6;
			accountNameTextBox.Text = "";
			accountNameTextBox.TextChanged += new EventHandler( FireTextChanged );

			////////////////////////////////////////////////////////////////////
            //  Account ID:

			Label accountIdLabel = new Label();
            accountIdLabel.Size = new System.Drawing.Size( 140, 20 );
            accountIdLabel.Location = new System.Drawing.Point( 20, 50 );
            accountIdLabel.Name = "accountIdLabel";
            accountIdLabel.Text = "Account ID:";
            accountIdLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            accountIdTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			accountIdTextBox.Name = "accountIdTextBox";
			accountIdTextBox.Size = new System.Drawing.Size( 270, 20 );
			accountIdTextBox.Location = new System.Drawing.Point( 180, 50 );
			accountIdTextBox.TabIndex = 6;
			accountIdTextBox.Text = "";
			accountIdTextBox.TextChanged += new EventHandler( FireTextChanged );

			////////////////////////////////////////////////////////////////////
            //  Market Account ID:

			Label marketAccountNameLabel = new Label();
            marketAccountNameLabel.Size = new System.Drawing.Size( 140, 20 );
            marketAccountNameLabel.Location = new System.Drawing.Point( 20, 80 );
            marketAccountNameLabel.Text = "Market Account ID:";
            marketAccountNameLabel.Name = "marketAccountNameLabel";
            marketAccountNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            marketAccountNameTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			marketAccountNameTextBox.Name = "marketAccountNameTextBox";
			marketAccountNameTextBox.Size = new System.Drawing.Size( 270, 20 );
			marketAccountNameTextBox.Location = new System.Drawing.Point( 180, 80 );
			marketAccountNameTextBox.TabIndex = 6;
			marketAccountNameTextBox.Text = "";

			////////////////////////////////////////////////////////////////////
            //  Give-Up Account ID:

			Label giveUpAccountLabel = new Label();
            giveUpAccountLabel.Size = new System.Drawing.Size( 140, 20 );
            giveUpAccountLabel.Location = new System.Drawing.Point( 20, 110 );
            giveUpAccountLabel.Name = "giveUpAccountLabel";
            giveUpAccountLabel.Text = "Give-Up Account ID:";
            giveUpAccountLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            giveUpAccountTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			giveUpAccountTextBox.Name = "giveUpAccountTextBox";
			giveUpAccountTextBox.Size = new System.Drawing.Size( 170, 20 );
			giveUpAccountTextBox.Location = new System.Drawing.Point( 180, 110 );
			giveUpAccountTextBox.TabIndex = 6;
			giveUpAccountTextBox.Text = "";

			////////////////////////////////////////////////////////////////////
            //  Give-Up Firm ID:

			Label giveUpFirmLabel = new Label();
            giveUpFirmLabel.Size = new System.Drawing.Size( 140, 20 );
            giveUpFirmLabel.Location = new System.Drawing.Point( 20, 140 );
            giveUpFirmLabel.Name = "giveUpFirmLabel";
            giveUpFirmLabel.Text = "Give-Up Firm ID:";
            giveUpFirmLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;

            giveUpFirmTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			giveUpFirmTextBox.Name = "giveUpFirmTextBox";
			giveUpFirmTextBox.Size = new System.Drawing.Size( 170, 20 );
			giveUpFirmTextBox.Location = new System.Drawing.Point( 180, 140 );
			giveUpFirmTextBox.TabIndex = 6;
			giveUpFirmTextBox.Text = "";

			////////////////////////////////////////////////////////////////////
            //  Is Active:

            isActiveCheckBox.Name = "isActiveCheckBox";
			isActiveCheckBox.Location = new System.Drawing.Point( 180, 170 );
			isActiveCheckBox.TabIndex = 6;
			isActiveCheckBox.Text = "Active";
			isActiveCheckBox.Checked = false;

			this.Controls.AddRange( new Control[] {
			    accountNameLabel,
			    accountNameTextBox,
			    accountIdLabel,
			    accountIdTextBox,
			    marketAccountNameLabel,
			    marketAccountNameTextBox,
			    giveUpAccountLabel,
			    giveUpAccountTextBox,
			    giveUpFirmLabel,
			    giveUpFirmTextBox,
			    isActiveCheckBox } );
        }

        private void FireTextChanged( object sender, System.EventArgs e )
		{
		    if( AccountDataChanged != null )
			{
                AccountDataChanged( this, new System.EventArgs() );
			}
		}
    }
}
