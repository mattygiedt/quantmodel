using System;
using System.IO;
using System.Windows;
using System.Security;
using System.Threading;
using System.Configuration;
using System.Windows.Forms;
using System.Security.Permissions;

//
//  Not entirely sure what this does, but it gets rid of the compiler warnings.
//

[assembly:CLSCompliant(false)]

namespace com.quantmodel.investment_system
{
    public sealed class Client : Form
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( Client ) );

        public static QuantmodelConfiguration configSection;

        private readonly ClientController controller;
        private readonly SplitContainer splitContainer = new SplitContainer();

        private Client( ClientController c ) : base( )
        {
            this.controller = c;
            this.IsMdiContainer = false;
            this.SetStyle(  ControlStyles.DoubleBuffer |
                            ControlStyles.UserPaint |
                            ControlStyles.AllPaintingInWmPaint,
                            true );
            this.UpdateStyles();

            this.SuspendLayout();
            this.Text = "Hades Investment System";
            this.Font = new System.Drawing.Font( "Tahoma", 9.25F,
                System.Drawing.FontStyle.Regular,
                    System.Drawing.GraphicsUnit.Point,
                        ((System.Byte)(0)));

            this.FormBorderStyle = FormBorderStyle.Sizable;
            this.MaximizeBox = true;
			this.MinimizeBox = true;
			this.AutoScroll = true;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.WindowState = FormWindowState.Normal;
            this.SizeGripStyle = SizeGripStyle.Show;
            this.Size = new System.Drawing.Size( 820, 500 );

            this.Controls.AddRange( new Control [] { new MainWindow( controller ) } );

            this.ResumeLayout( false );
            this.PerformLayout();
        }

        static void ChangeLogFileName( string appenderName, string newFilename )
        {
            log4net.Repository.ILoggerRepository RootRep;
            RootRep = log4net.LogManager.GetRepository();

            foreach( log4net.Appender.IAppender iApp in RootRep.GetAppenders() )
            {
                if( iApp.Name.CompareTo(appenderName) == 0 && iApp is log4net.Appender.FileAppender )
                {
                    log4net.Appender.FileAppender fApp = (log4net.Appender.FileAppender)iApp;

                    fApp.File = newFilename;
                    fApp.ActivateOptions();

                    return; // Appender found and name changed to newFilename
                }
            }
        }

        [STAThreadAttribute()]
        [SecurityPermission(SecurityAction.Assert, Flags = SecurityPermissionFlag.UnmanagedCode)]
        public static int Main( string[] args )
        {
            Client client = null;
            ClientController controller = null;

            try
            {
                System.Windows.Forms.Application.EnableVisualStyles();

                log4net.Config.XmlConfigurator.Configure();

                configSection = ConfigurationManager.GetSection( "quantmodel" ) as QuantmodelConfiguration;

                SelectConnectionForm scf = new SelectConnectionForm( configSection );

                if( scf.ShowDialog() == DialogResult.OK )
                {
                    ChangeLogFileName( "LogFileAppender", scf.ConnectionElement.Name + ".log" );
                    controller = new ClientController( scf.ConnectionElement );
                    client = new Client( controller );

                    System.Windows.Forms.Application.DoEvents();
                    System.Windows.Forms.Application.Run( client );
                }
            }
            catch( Exception e )
            {
                log.Fatal( "Client error:", e );

                //
                //  Show error dialog
                //

                System.Windows.Forms.MessageBox.Show(
                  "The client has encountered a fatal error and must be closed: " + e.ToString(),
                  "Application Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
            }

            //try
            //{
            //    log4net.Config.XmlConfigurator.Configure();
            //    QuantmodelConfiguration section =
            //        ConfigurationManager.GetSection( "quantmodel" )
            //            as QuantmodelConfiguration;
            //
            //    System.Windows.Forms.MessageBox.Show( "DateTime Format: " +
            //        section.DatetimeFormat + ", FCM: " + section.FcmId );
            //}
            //catch( Exception e )
            //{
            //    System.Windows.Forms.MessageBox.Show(
            //        "The client has encountered a fatal error and must be closed: " + e.ToString(),
            //        "Application Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation );
            //}

            return 0;
        }
	}
}
