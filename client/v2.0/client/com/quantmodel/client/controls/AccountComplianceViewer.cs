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
    public sealed class AccountComplianceViewer : UserControl
    {
        private static readonly log4net.ILog log =
		    log4net.LogManager.GetLogger( typeof( AccountComplianceViewer ) );

        private readonly ClientController controller;
        private HeaderControl accountLimitControl = null;

        public AccountComplianceViewer( ClientController controller ) : base()
		{
		    this.controller = controller;
		    this.SuspendLayout();
		    this.Visible = false;
		    this.AutoScroll = true;
		    this.AutoSize = true;
		    this.Dock = System.Windows.Forms.DockStyle.Fill;
			this.Tag = "AccountComplianceViewer";
			this.Name = "AccountComplianceViewer";
            this.BackColor = System.Drawing.SystemColors.Window;
			InitializeComponents();
			this.ResumeLayout( false );
		}

		private void InitializeComponents()
        {
            TabPage detailPage = new System.Windows.Forms.TabPage();
            TabPage limitPage = new System.Windows.Forms.TabPage();
            TabControl tabControl = new System.Windows.Forms.TabControl();

            detailPage.Dock = System.Windows.Forms.DockStyle.Fill;
            detailPage.BackColor = System.Drawing.SystemColors.Control;
            detailPage.Name = "detailPage";
            detailPage.Text = "Account Information";
            detailPage.Controls.AddRange( new Control [] { new AccountDetail( controller ) } );

            limitPage.Dock = System.Windows.Forms.DockStyle.Fill;
            limitPage.BackColor = System.Drawing.SystemColors.Control;
            limitPage.Name = "limitPage";
            limitPage.Text = "Instrument Limits";
            limitPage.Controls.AddRange( new Control [] { new AccountLimitList( controller ) } );

            tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            tabControl.BackColor = System.Drawing.SystemColors.Control;
            tabControl.Name = "tabControl";

            tabControl.Controls.AddRange( new Control [] { detailPage, limitPage } );

            //
            //  Controls
            //

            this.accountLimitControl = new HeaderControl();
            this.accountLimitControl.AutoScroll = true;
			this.accountLimitControl.SuspendLayout();
			this.accountLimitControl.Padding = new Padding( 6, 24, 6, 6 );
			this.accountLimitControl.HeaderStyle = HeaderStyle.SubHeading;
            this.accountLimitControl.Dock = System.Windows.Forms.DockStyle.Fill;
			this.accountLimitControl.HeaderFont = new System.Drawing.Font( "Tahoma", 12F, System.Drawing.FontStyle.Bold );
			this.accountLimitControl.Location = new System.Drawing.Point( 167, 4 );
			this.accountLimitControl.Name = "accountLimitControl";
			this.accountLimitControl.Size = new System.Drawing.Size( 160, 120 );
			this.accountLimitControl.MinimumSize = new System.Drawing.Size( 160, 120 );
			this.accountLimitControl.TabIndex = 0;
			this.accountLimitControl.Text = "";
			this.accountLimitControl.Controls.Add( tabControl );
			this.Controls.AddRange( new Control [] { accountLimitControl } );
            this.accountLimitControl.ResumeLayout( false );
        }
    }
}
