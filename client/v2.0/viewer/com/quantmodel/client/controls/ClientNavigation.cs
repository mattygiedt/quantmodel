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

using com.quantmodel.common.events;

namespace com.quantmodel.client.controls
{
    public sealed class ClientNavigation : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( ClientNavigation ) );

	    private readonly ClientController controller;

	    public ClientNavigation( ClientController controller ) : base()
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Text = "ClientNavigator";
            this.Name = "ClientNavigator";
            this.Dock = DockStyle.Fill;

		    //
            //  Navigation Bar
            //

            NavigationBar navBar = new NavigationBar();
            navBar.SuspendLayout();
            navBar.Dock = DockStyle.Fill;
            navBar.Name = "navBar";
            navBar.Cursor = System.Windows.Forms.Cursors.Default;
			navBar.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			navBar.PaneFont = new System.Drawing.Font( "Tahoma", 8.25F, System.Drawing.FontStyle.Bold );
			navBar.Size = new System.Drawing.Size( 200, 510 );
			navBar.DrawGripper = false;
			navBar.DrawActionsButton = false;

            navBar.Controls.AddRange( new Control[]
            {
                new AccountPositionPane( controller )
            } );

            navBar.SelectedPaneChanged += new EventHandler( FireSelectedPaneChanged );

            navBar.ShowPanes = navBar.Controls.Count;
            navBar.SelectedPane = (TD.Eyefinder.NavigationPane)
                navBar.Controls[ navBar.Controls.Count - 1 ];
            this.Controls.AddRange( new Control[] { navBar } );

            navBar.ResumeLayout( false );
            this.ResumeLayout( false );
	    }

	    private void FireSelectedPaneChanged( object sender, EventArgs e )
		{
            Control selectedPane = ((NavigationBar)sender).SelectedPane;
            controller.Dispatcher.NavigationPaneChangedCallback(
                new StringEventArgs( (string)selectedPane.Tag ) );
		}
    }
}
