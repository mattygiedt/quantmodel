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
    public sealed class DealingViewer : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( DealingViewer ) );

        private readonly ClientController controller;
        private HeaderControl orderHeaderControl = null;
        private HeaderControl releaseHeaderControl = null;
        private HeaderControl positionHeaderControl = null;
        private HeaderControl executionHeaderControl = null;

        public DealingViewer( ClientController controller ) : base()
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Visible = false;
		    this.AutoScroll = true;
		    this.AutoSize = true;
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Tag = "DealingViewer";
			this.Name = "DealingViewer";
            this.BackColor = System.Drawing.SystemColors.Window;
			InitializeComponents();
			this.ResumeLayout( false );

			this.Load += new EventHandler( FireLoad );
		}

		private void InitializeComponents()
        {
            //
            //  Controls
            //

            this.positionHeaderControl = new HeaderControl();
			this.positionHeaderControl.AutoScroll = true;
			this.positionHeaderControl.SuspendLayout();
			this.positionHeaderControl.HeaderStyle = HeaderStyle.SubHeading;
            this.positionHeaderControl.Dock = System.Windows.Forms.DockStyle.Top;
			this.positionHeaderControl.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			this.positionHeaderControl.Location = new System.Drawing.Point( 167, 4 );
			this.positionHeaderControl.Name = "positionHeaderControl";
			this.positionHeaderControl.Size = new System.Drawing.Size( 160, 120 );
			this.positionHeaderControl.MinimumSize = new System.Drawing.Size( 160, 120 );
			this.positionHeaderControl.TabIndex = 0;
			this.positionHeaderControl.Text = "Positions";
			this.positionHeaderControl.Controls.Add( new PositionIntradayList( controller ) );

            this.orderHeaderControl = new HeaderControl();
            this.orderHeaderControl.AutoScroll = true;
            this.orderHeaderControl.SuspendLayout();
            this.orderHeaderControl.HeaderStyle = HeaderStyle.SubHeading;
            this.orderHeaderControl.Dock = System.Windows.Forms.DockStyle.Top;
			this.orderHeaderControl.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			this.orderHeaderControl.Location = new System.Drawing.Point( 167, 4 );
			this.orderHeaderControl.Name = "orderHeaderControl";
			this.orderHeaderControl.Size = new System.Drawing.Size( 160, 120 );
			this.orderHeaderControl.MinimumSize = new System.Drawing.Size( 160, 120 );
			this.orderHeaderControl.TabIndex = 0;
			this.orderHeaderControl.Text = "Orders";
			this.orderHeaderControl.Controls.Add( new OrderList( controller ) );

			this.releaseHeaderControl = new HeaderControl();
			this.releaseHeaderControl.AutoScroll = true;
			this.releaseHeaderControl.SuspendLayout();
			this.releaseHeaderControl.HeaderStyle = HeaderStyle.SubHeading;
            this.releaseHeaderControl.Dock = System.Windows.Forms.DockStyle.Top;
			this.releaseHeaderControl.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			this.releaseHeaderControl.Location = new System.Drawing.Point( 167, 4 );
			this.releaseHeaderControl.Name = "releaseHeaderControl";
			this.releaseHeaderControl.Size = new System.Drawing.Size( 160, 120 );
			this.releaseHeaderControl.MinimumSize = new System.Drawing.Size( 160, 120 );
			this.releaseHeaderControl.TabIndex = 0;
			this.releaseHeaderControl.Text = "Releases";
			this.releaseHeaderControl.Controls.Add( new ReleaseList( controller ) );

			this.executionHeaderControl = new HeaderControl();
			this.executionHeaderControl.AutoScroll = true;
			this.executionHeaderControl.SuspendLayout();
			this.executionHeaderControl.HeaderStyle = HeaderStyle.SubHeading;
            this.executionHeaderControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.executionHeaderControl.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			this.executionHeaderControl.Location = new System.Drawing.Point( 167, 4 );
			this.executionHeaderControl.Name = "executionHeaderControl";
			this.executionHeaderControl.Size = new System.Drawing.Size( 160, 120 );
			this.executionHeaderControl.MinimumSize = new System.Drawing.Size( 160, 120 );
			this.executionHeaderControl.TabIndex = 0;
			this.executionHeaderControl.Text = "Executions";
			this.executionHeaderControl.Controls.Add( new ExecutionList( controller ) );

			Splitter splitter1 = new Splitter();
			Splitter splitter2 = new Splitter();
			Splitter splitter3 = new Splitter();
			Splitter splitter4 = new Splitter();

			splitter1.Dock = DockStyle.Top;
			splitter2.Dock = DockStyle.Top;
			splitter3.Dock = DockStyle.Top;
			splitter4.Dock = DockStyle.Top;
			splitter1.Size = new System.Drawing.Size( 10, 6 );
			splitter2.Size = new System.Drawing.Size( 10, 6 );
			splitter3.Size = new System.Drawing.Size( 10, 6 );
			splitter4.Size = new System.Drawing.Size( 10, 6 );
			splitter1.BackColor = System.Drawing.Color.LightSteelBlue;
			splitter2.BackColor = System.Drawing.Color.LightSteelBlue;
			splitter3.BackColor = System.Drawing.Color.LightSteelBlue;
			splitter4.BackColor = System.Drawing.Color.LightSteelBlue;

            this.Controls.AddRange( new Control []
            {
                executionHeaderControl,
                splitter1,
                releaseHeaderControl,
                splitter2,
                orderHeaderControl,
                splitter3,
                positionHeaderControl
            } );

            this.orderHeaderControl.ResumeLayout( false );
            this.releaseHeaderControl.ResumeLayout( false );
            this.positionHeaderControl.ResumeLayout( false );
            this.executionHeaderControl.ResumeLayout( false );
        }

		private void FireLoad( object o, EventArgs e )
        {
            //
            //  Load the active set and register for updates
            //
        }
    }
}
