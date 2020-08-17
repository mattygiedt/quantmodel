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

using com.quantmodel.common.controls;
using com.quantmodel.client.controls.list;

namespace com.quantmodel.client.controls
{
    public sealed class AccountPositionViewer : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( AccountPositionViewer ) );

        private readonly ClientController controller;
        private HeaderControl positionHeaderControl = null;

        public AccountPositionViewer( ClientController controller ) : base()
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Visible = false;
		    this.AutoScroll = true;
		    this.AutoSize = true;
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Tag = "AccountPositionViewer";
			this.Name = "AccountPositionViewer";
            this.BackColor = System.Drawing.SystemColors.Window;
			InitializeComponents();
			this.ResumeLayout( false );
		}

		private void InitializeComponents()
        {
            this.positionHeaderControl = new HeaderControl();
			this.positionHeaderControl.AutoScroll = true;
			this.positionHeaderControl.SuspendLayout();
			this.positionHeaderControl.HeaderStyle = HeaderStyle.SubHeading;
            this.positionHeaderControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.positionHeaderControl.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			this.positionHeaderControl.Location = new System.Drawing.Point( 167, 4 );
			this.positionHeaderControl.Name = "positionHeaderControl";
			this.positionHeaderControl.Size = new System.Drawing.Size( 160, 120 );
			this.positionHeaderControl.MinimumSize = new System.Drawing.Size( 160, 120 );
			this.positionHeaderControl.TabIndex = 0;
			this.positionHeaderControl.Text = "Account Positions";
			this.positionHeaderControl.Controls.Add( new PositionIntradayList( controller ) );

			this.Controls.AddRange( new Control [] { positionHeaderControl } );

            this.positionHeaderControl.ResumeLayout( false );
        }
    }
}
