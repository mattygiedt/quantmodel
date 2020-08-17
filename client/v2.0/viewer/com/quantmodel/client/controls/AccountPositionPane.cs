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

namespace com.quantmodel.client.controls
{
    public sealed class AccountPositionPane : TD.Eyefinder.NavigationPane
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( AccountPositionPane ) );

        private readonly ClientController controller;

        public AccountPositionPane( ClientController controller ) : base()
        {
		    this.controller = controller;

		    this.SuspendLayout();
		    this.AutoScroll = true;
		    this.Padding = new Padding( 6 );
            this.Text = "Account Position";
            this.Name = "AccountPositionPane";
            this.Tag = "AccountPositionViewer";
            this.Dock = DockStyle.Fill;
            this.BackColor = System.Drawing.SystemColors.Control;
            InitializeComponents();
            this.ResumeLayout( false );

            this.VisibleChanged += new EventHandler( FireVisibleChanged );
        }

        private void InitializeComponents()
        {

        }

        private void FireVisibleChanged( object o, EventArgs e )
        {
            if( this.Visible )
            {

            }
        }

        private void FireSelectedIndexChanged( object o, GTSelectionChangedEventArgs args )
        {
            if( args.NewValue )
            {

            }
        }
    }
}
