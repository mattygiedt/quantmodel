using System;
using System.IO;
using System.Windows;
using System.Security;
using System.Threading;
using System.Configuration;
using System.Windows.Forms;
using System.ComponentModel;
using System.Security.Permissions;

using quantmodel;
using com.quantmodel.client.controls;

using com.quantmodel.common.data;
using com.quantmodel.common.events;
using com.quantmodel.common.controls;
using com.quantmodel.common.network.message;

namespace com.quantmodel.client
{
    public sealed class Viewer : Form
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Viewer ) );

        public static string BlankIfNull( string input )
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

        private readonly ClientController controller;
        private readonly SplitContainer splitContainer = new SplitContainer();

        private Viewer( ClientController c ) : base( )
        {
            this.controller = c;
            this.IsMdiContainer = false;
            this.SetStyle(  ControlStyles.DoubleBuffer |
                            ControlStyles.UserPaint |
                            ControlStyles.AllPaintingInWmPaint,
                            true );
            this.UpdateStyles();

            //
            //  Create the minimal amount of work to show the login window.
            //

            this.SuspendLayout();
            this.Text = "QuantModel - Account Position Viewer";
            this.Font = new System.Drawing.Font( "Tahoma", 9.25F,
                System.Drawing.FontStyle.Regular,
                    System.Drawing.GraphicsUnit.Point,
                        ((System.Byte)(0)));

            this.FormBorderStyle = FormBorderStyle.Sizable;
            this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.AutoScroll = true;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.WindowState = FormWindowState.Normal;
            this.SizeGripStyle = SizeGripStyle.Show;
            InitializeLogin();
            this.ResumeLayout( false );
            this.PerformLayout();

            this.Load += new EventHandler( FireLoad );
            this.Closing += new CancelEventHandler( FireClosing );
            this.controller.Dispatcher.NavigationPaneChanged +=
                new EventHandler<StringEventArgs>( FireNavigationPaneChanged );
        }

        private void InitializeLogin()
        {
            //
            //  Login window
            //

            LoginControl login = new LoginControl();
            login.Check = false;
			login.Font = new System.Drawing.Font(
			    "Tahoma", 11F, System.Drawing.FontStyle.Regular,
			        System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			login.Dock = System.Windows.Forms.DockStyle.Fill;
			login.Hash = false;
			login.labelButton = "Login";
			login.labelHeadline = "Execution Platform Client Login";
			login.Name = "login";
			login.Password = null;
			login.Size = new System.Drawing.Size( 460, 300 );
			login.TabIndex = 0;
			login.Username = null;
			login.Ready += new System.EventHandler( FireLoginReady );

            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.ClientSize = login.ClientSize;

            //
            //  Controls
            //

            this.Controls.AddRange( new Control [] { login } );
        }

        private void FireLoad( object sender, EventArgs e )
		{

	    }

        private void FireExit( object sender, EventArgs e )
        {
            this.Close();
        }

        protected override void Dispose( bool disposing )
		{
		    if( disposing )
			{
			    foreach( Control c in this.Controls )
                {
                    if( c != null ) { c.Dispose(); }
                }
			}

			base.Dispose( disposing );
    	}

        private void FireClosing( object sender, System.ComponentModel.CancelEventArgs e )
		{
		    controller.Close();
		}

		private void FireNavigationPaneChanged( object sender, StringEventArgs args )
		{
		    Control ctrl = null;

            foreach( Control c in splitContainer.Panel2.Controls )
            {
                if( c.Tag.Equals( args.Data ) )
                {
                    ctrl = c;
                }
                else
                {
                    c.Visible = false;
                }
            }

            if( null != ctrl )
            {
                ctrl.Visible = true;
            }
            else
            {
                System.Windows.Forms.MessageBox.Show(
                    "Unknown window: " + args.Data,
                    "Application Error", MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation );
		    }
		}

		private void FireLoginReady( object sender, System.EventArgs e )
		{
		    LoginControl login = (LoginControl)sender;

			//
			//  Create a new login client request
			//

			ClientAdapterResponse response = controller.SendRpc(
			    new Login( login.Username, login.Password, "VIEWER" ) );

            if( response.Type != ClientAdapterResponse.Types.ResponseType.ACKNOWLEDGEMENT )
            {
                System.Windows.Forms.MessageBox.Show(
                    "Invalid username / password.",
                    "Login Error", MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation );

                login.Reset();
                login.Invalidate();
                return;
            }

            if( !response.HasDatabaseResponse ||
                response.DatabaseResponse.ResultSetCount != 1 ||
                response.DatabaseResponse.GetResultSet( 0 ).RowCount != 1 )
            {
                System.Windows.Forms.MessageBox.Show(
                    "Invalid username / password.",
                    "Login Error", MessageBoxButtons.OK,
                    MessageBoxIcon.Exclamation );

                login.Reset();
                login.Invalidate();
                return;
            }

            User userData = new User(
                response.DatabaseResponse.
                    GetResultSet( 0 ).GetRow( 0 ) );

            //
            //  Set the user
            //

            controller.ApplicationUser = userData;

            //
            //  Load the active set data
            //

            login.labelHeader = "Loading Static Data...";
            Application.DoEvents();

            controller.LoadStaticData();

            login.labelHeader = "Loading Dealing Collection...";
            Application.DoEvents();

            controller.LoadActiveSet();

            //
            //  Create the main GUI
            //

            this.SuspendLayout();

            //
            //  Menu
            //

            MainMenu menu = new MainMenu();
            MenuItem file = new MenuItem( "File" );
            MenuItem edit = new MenuItem( "Edit" );
            MenuItem help = new MenuItem( "Help" );

            menu.MenuItems.Add( file );
            menu.MenuItems.Add( edit );
            menu.MenuItems.Add( help );

            file.MenuItems.Add( new MenuItem( "Exit", new EventHandler( FireExit ) ) );

            //
            //  Status Strip
            //

            ToolStripStatusLabel toolStripStatusLabel = new ToolStripStatusLabel();
            toolStripStatusLabel.Name = "toolStripStatusLabel";
			toolStripStatusLabel.Size = new System.Drawing.Size( 38, 17 );
			toolStripStatusLabel.Text =
			    (string)ConfigurationManager.AppSettings[ "fcm_id" ] + " : " +
			    (string)ConfigurationManager.AppSettings[ "rpc" ];

            System.Windows.Forms.StatusStrip statusStrip = new System.Windows.Forms.StatusStrip();
            statusStrip.Location = new System.Drawing.Point( 0, 248 );
            statusStrip.Name = "statusStrip";
            statusStrip.Size = new System.Drawing.Size( 292, 25 );
            statusStrip.TabIndex = 0;
            statusStrip.Text = "statusStrip";
            statusStrip.Items.AddRange( new ToolStripItem[] { toolStripStatusLabel } );

            //
            //  Split Container
            //

            splitContainer.SuspendLayout();
            splitContainer.BackColor = System.Drawing.Color.LightSteelBlue;
            splitContainer.Dock = System.Windows.Forms.DockStyle.Fill;
            splitContainer.Size = new System.Drawing.Size( 550, 550 );
            splitContainer.Location = new System.Drawing.Point( 0, 0 );
            splitContainer.Name = "splitContainer";
            splitContainer.SplitterIncrement = 10;
            splitContainer.SplitterWidth = 6;
            splitContainer.SplitterDistance = 220;
            splitContainer.Panel1MinSize = 200;
            splitContainer.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            splitContainer.Panel1.Padding = new System.Windows.Forms.Padding( 4, 4, 0, 4 );
            splitContainer.Panel2.Padding = new System.Windows.Forms.Padding( 0, 4, 4, 4 );

            //
            //  Add the account viewer to the split pane. If in the future
            //  we add additional functionality to the viewer, the new
            //  panes would go here.
            //

            splitContainer.Panel2.Controls.AddRange( new Control[]
            {
                new AccountPositionViewer( controller )
            } );

            splitContainer.Panel1.Controls.Add( new ClientNavigation( controller ) );

            //
            //  (this) Form variables
            //

            this.Menu = menu;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Sizable;
			this.MaximizeBox = true;
			this.MinimizeBox = true;
			this.FormBorderStyle = FormBorderStyle.Sizable;
            this.SizeGripStyle = SizeGripStyle.Show;
			this.Size = new System.Drawing.Size( 600, 440 );
			this.CenterToScreen();
			//this.WindowState = FormWindowState.Maximized;

            this.Controls.Clear();
            this.Controls.AddRange( new Control [] { splitContainer, statusStrip } );

			splitContainer.ResumeLayout( false );
            this.ResumeLayout( true );
            this.PerformLayout();
		}

        private static void TimeoutEvent( object state )
        {
            //
            //  Play a sound!!!
            //

            System.Media.SystemSounds.Asterisk.Play();

            //
            //  Show error dialog
            //

            System.Windows.Forms.MessageBox.Show(
              "The viewer cannot connect to the QuantModel Server: " +
                (string)ConfigurationManager.AppSettings[ "rpc" ],
              "Application Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );

            Environment.Exit( 1 );
        }

        [STAThreadAttribute()]
        [SecurityPermission(SecurityAction.Assert, Flags = SecurityPermissionFlag.UnmanagedCode)]
        public static int Main( string[] args )
        {
            Viewer viewer = null;
            ClientController controller = null;

            try
            {
                log4net.Config.XmlConfigurator.Configure();
                log.Info( "Starting QuantModel Account Viewer." );

                controller = new ClientController(
                    (string)ConfigurationManager.AppSettings[ "rpc" ],
                    (string)ConfigurationManager.AppSettings[ "broadcast" ] );

                //
                //  Test for network connectivity
                //

                controller.SendRpc( new Heartbeat() );

                viewer = new Viewer( controller );
                System.Windows.Forms.Application.EnableVisualStyles();
                System.Windows.Forms.Application.DoEvents();
                System.Windows.Forms.Application.Run( viewer );
            }
            catch( Exception e )
            {
                log.Fatal( "Client error:", e );

                //
                //  Show error dialog
                //

                System.Windows.Forms.MessageBox.Show(
                  "A fatal error has occured and the application must be closed: " + e.ToString(),
                  "Application Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
            }

            return 0;
        }
    }
}
