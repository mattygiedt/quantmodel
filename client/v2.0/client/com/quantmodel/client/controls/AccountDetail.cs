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

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;
using com.quantmodel.common.network.message;

namespace com.quantmodel.client.controls
{
    public sealed class AccountDetail : UserListControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( AccountDetail ) );

        private Account account;
        private AccountDataPanel accountData;
        private readonly ClientController controller;

        public AccountDetail( ClientController controller )
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Name = "AccountDetail";
			this.BackColor = System.Drawing.SystemColors.Window;
			InitializeComponents();
			this.ResumeLayout( false );

            this.controller.Dispatcher.NavigationPaneChanged +=
                new EventHandler<StringEventArgs>( FireNavigationPaneChanged );

		    this.Enabled = false;
		}

		public void Clear( )
	    {
	        accountData.Clear();
        }

		private void InitializeComponents()
        {
            accountData = new AccountDataPanel( false );
            accountData.Location = new System.Drawing.Point( 10, 40 );

            //
            //  Update / reset button panel
            //

            Panel buttonPanel = new Panel();
            buttonPanel.BackColor = System.Drawing.SystemColors.Control;
            buttonPanel.Size = new System.Drawing.Size( 200, 40 );
            buttonPanel.Padding = new System.Windows.Forms.Padding( 4, 4, 20, 4 );
            buttonPanel.Dock = DockStyle.Bottom;

            Button updateButton = new Button();
            updateButton.Text = "Update";
            updateButton.AutoSize = true;
            updateButton.Location = new System.Drawing.Point( 28, 10 );
            updateButton.Anchor = ( AnchorStyles.Bottom | AnchorStyles.Right );
            updateButton.Click += new EventHandler( FireUpdateButton );

            Button resetButton = new Button();
            resetButton.Text = "Reset";
            resetButton.AutoSize = true;
            resetButton.Location = new System.Drawing.Point( 110, 10 );
            resetButton.Anchor = ( AnchorStyles.Bottom | AnchorStyles.Right );
            resetButton.Click += new EventHandler( FireResetButton );

            buttonPanel.Controls.AddRange( new Control [] { updateButton, resetButton } );

            this.Controls.AddRange( new Control [] { buttonPanel, accountData } );
        }

        private void FireNavigationPaneChanged( object sender, StringEventArgs args )
		{
            if( args.Data.Equals( "AccountComplianceViewer" ) )
            {
                this.controller.Dispatcher.AccountSelected +=
                    new EventHandler<StringEventArgs>( FireAccountSelected );

                Clear();
            }
            else
            {
                this.controller.Dispatcher.AccountSelected -=
                    new EventHandler<StringEventArgs>( FireAccountSelected );

                account = null;
                this.Enabled = false;
            }
		}

        private void FireAccountSelected( object sender, StringEventArgs args )
		{
		    //
            //  Load the selected account details
            //

            account = controller.Cache.GetAccount( args.Data );
            accountData.Update( account );
            this.Enabled = true;
		}

		private void FireResetButton( object sender, EventArgs args )
        {
            if( account != null )
            {
                accountData.Update( account );
            }
        }

        private void FireUpdateButton( object sender, EventArgs args )
        {
            if( account != null )
            {
                ClientAdapterResponse response =
                    controller.SendRpc( new UpdateAccount(
                         account[ DatabaseObject.ACCOUNT_ID ],
                         account[ DatabaseObject.NAME ],
                         accountData.MarketAccountName,
                         accountData.AccountId,
                         accountData.GiveUpAccount,
                         accountData.GiveUpFirm,
                         account[ DatabaseObject.NOTIONAL_VAL ],
                         account[ DatabaseObject.PROFIT_TARGET ],
                         account[ DatabaseObject.STOP_LOSS ],
                         account[ DatabaseObject.SIGNAL_WEIGHT ],
                         account[ DatabaseObject.IS_SUPPRESSED ],
                         accountData.IsActive ) );

                if( response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
                {
                    account.SetAttribute( DatabaseObject.MARKET_ACCOUNT_ID, accountData.MarketAccountName );
                    account.SetAttribute( DatabaseObject.CUSTODY_ACCOUNT_ID, accountData.AccountId );
                    account.SetAttribute( DatabaseObject.GIVEUP_ACCOUNT_ID, accountData.GiveUpAccount );
                    account.SetAttribute( DatabaseObject.GIVEUP_FIRM_ID, accountData.GiveUpFirm );
                    account.SetAttribute( DatabaseObject.IS_ACTIVE, accountData.IsActive );

                    controller.Cache.Update( account );

                    //response = controller.SendRpc( new UpdateAccountLimitSessionIds(
                    //    account[ DatabaseObject.ACCOUNT_ID ], fixSessionIdTextBox.Text ) );

                    response = controller.SendRpc( new ReloadStaticData( ) );

                    MessageBox.Show( "Account: " + account[ DatabaseObject.NAME ] + " updated successfully." );
                }
            }
        }
    }
}
