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
using GlacialComponents.Controls.Common;
using GlacialComponents.Controls.GTLCommon;

using quantmodel;
using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;
using com.quantmodel.common.network.message;

using com.quantmodel.client.controls.form;

namespace com.quantmodel.client.controls
{
    public sealed class AccountCompliancePane : TD.Eyefinder.NavigationPane
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( AccountCompliancePane ) );

        private string selectedAccount = null;
        private GlacialTreeList accounts = null;
        private readonly ClientController controller;

        public AccountCompliancePane( ClientController controller ) : base()
        {
		    this.controller = controller;

		    this.SuspendLayout();
		    this.AutoScroll = true;
		    this.Padding = new Padding( 6 );
            this.Text = "Account Compliance";
            this.Name = "AccountCompliancePane";
            this.Tag = "AccountComplianceViewer";
            this.Dock = DockStyle.Fill;
            this.BackColor = System.Drawing.SystemColors.Control;
            InitializeComponents();
            this.ResumeLayout( false );

            this.VisibleChanged += new EventHandler( FireVisibleChanged );
        }

        private void InitializeComponents()
        {
            //
            //  Account List
            //

            accounts = ControlHelper.NewGlacialTreeList( "AccountLimitPaneData" );

            accounts.SelectedIndexChanged +=
                new GTSelectionChangedEventHandler(
                    FireSelectedIndexChanged );

            //
            //  account list columns
            //

            GTLColumn account_name = new GTLColumn();
            account_name.BackColor = Color.White;
            account_name.CheckBoxes = false;
            account_name.ImageIndex = -1;
            account_name.Name = "account_name";
            account_name.Text = "Account";
            account_name.Width = 132;
            account_name.SortType = SortTypes.InsertionSort;

            accounts.Columns.AddRange( new GTLColumn [] { account_name } );

            //
			//  Context Menu
			//

			ContextMenu ctxMenu = new ContextMenu();

            //
			//  First, build the actions that you can do
			//

			ctxMenu.MenuItems.AddRange( new MenuItem []
			{
			    new MenuItem( "CREATE ACCOUNT", new EventHandler( FireCreateAccount )  ),
			    new MenuItem( "-" ),
			    new MenuItem( "CLONE ACCOUNT", new EventHandler( FireCloneAccount )  )
			} );

            ctxMenu.MenuItems[ 0 ].Enabled = false;
			ctxMenu.MenuItems[ 2 ].Enabled = false;

            accounts.ContextMenu = ctxMenu;

            this.Controls.AddRange( new Control[] { accounts } );
        }

        private void FireCloneAccount( object o, EventArgs e )
        {
            if( selectedAccount != null )
            {
                Account account = controller.Cache.GetAccount( selectedAccount );

                if( account != null )
                {
                    CreateAccountForm createAccountForm = new CreateAccountForm( account );
                    createAccountForm.Size = new Size( 545, 425 );
                    createAccountForm.FormBorderStyle = FormBorderStyle.Sizable;
                    createAccountForm.StartPosition = FormStartPosition.CenterScreen;

                    DialogResult result = createAccountForm.ShowDialog();

        			if( result == System.Windows.Forms.DialogResult.OK )
        			{
                        CreateAccount( createAccountForm.AccountData );
        			}
                }
            }
        }

        private void FireCreateAccount( object o, EventArgs e )
        {
            CreateAccountForm createAccountForm = new CreateAccountForm();
            createAccountForm.Size = new Size( 545, 425 );
            createAccountForm.FormBorderStyle = FormBorderStyle.Sizable;
            createAccountForm.StartPosition = FormStartPosition.CenterScreen;

            DialogResult result = createAccountForm.ShowDialog();

			if( result == System.Windows.Forms.DialogResult.OK )
			{
                CreateAccount( createAccountForm.AccountData );
			}
        }

        private void CreateAccount( AccountDataPanel accountData )
        {
            //
		    //  Create the account
		    //

		    ClientAdapterResponse response = controller.SendRpc(
                new CreateAccount(
                    accountData.AccountName,
                    accountData.MarketAccountName,
                    accountData.AccountId,
                    "0",
                    "0",
                    "0",
                    "0",
                    accountData.IsActive ) );


		    if( response.Type == ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
            {
                MessageBox.Show( "Account: " + accountData.AccountName + " created successfully." );


            }
        }

        private void FireVisibleChanged( object o, EventArgs e )
        {
            if( this.Visible )
            {
                accounts.BeginUpdate();
                accounts.BeginNodeListChange();
                accounts.Nodes.Clear();

                selectedAccount = null;
                //accounts.ContextMenu.MenuItems[ 2 ].Enabled = false;

                foreach( Account account in controller.Cache.Accounts )
                {
                    GTLTreeNode account_node =
                        new GTLTreeNode( account[ DatabaseObject.NAME ] );

                    account_node.Tag = account[ DatabaseObject.ACCOUNT_ID ];
                    accounts.Nodes.Add( account_node );
                }

                accounts.EndNodeListChange();
                accounts.EndUpdate();
            }
        }

        private void FireSelectedIndexChanged( object o, GTSelectionChangedEventArgs args )
        {
            if( args.NewValue )
            {
                //accounts.ContextMenu.MenuItems[ 2 ].Enabled = true;

                selectedAccount = (string)args.TreeNode.Tag;
                controller.Dispatcher.AccountSelectedCallback(
                    new StringEventArgs( selectedAccount ) );
            }
        }
    }
}
